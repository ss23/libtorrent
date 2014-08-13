// libTorrent - BitTorrent library
// Copyright (C) 2005-2011, Jari Sundell
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// In addition, as a special exception, the copyright holders give
// permission to link the code of portions of this program with the
// OpenSSL library under certain conditions as described in each
// individual source file, and distribute linked combinations
// including the two.
//
// You must obey the GNU General Public License in all respects for
// all of the code used other than OpenSSL.  If you modify file(s)
// with this exception, you may extend this exception to your version
// of the file(s), but you are not obligated to do so.  If you do not
// wish to do so, delete this exception statement from your version.
// If you delete this exception statement from all source files in the
// program, then also delete it here.
//
// Contact:  Jari Sundell <jaris@ifi.uio.no>
//
//           Skomakerveien 33
//           3185 Skoppum, NORWAY

#ifndef LIBTORRENT_UTILS_URI_PARSER_H
#define LIBTORRENT_UTILS_URI_PARSER_H

#include <string>
#include <vector>
#include <torrent/common.h>
#include <torrent/exceptions.h>

namespace torrent { namespace utils {

typedef std::vector<std::string> uri_resource_list;
typedef std::vector<std::string> uri_query_list;

struct uri_base_state {
  static const int state_empty = 0;
  static const int state_valid = 1;
  static const int state_invalid = 2;

  uri_base_state() : state(state_empty) {}

  int state;
};

struct uri_state : uri_base_state {
  std::string uri;
  std::string scheme;
  std::string resource;
  std::string query;
  std::string fragment;
};

struct uri_resource_state : public uri_base_state {
  std::string resource;
  uri_resource_list path;
};

struct uri_query_state : public uri_base_state {
  std::string query;
  uri_query_list elements;
};

void uri_parse_str(std::string uri, uri_state& state) LIBTORRENT_EXPORT;

void uri_parse_resource(std::string query, uri_query_state& state) LIBTORRENT_EXPORT;
void uri_parse_resource_authority(std::string query, uri_query_state& state) LIBTORRENT_EXPORT;
void uri_parse_resource_path(std::string query, uri_query_state& state) LIBTORRENT_EXPORT;

void uri_parse_query_str(std::string query, uri_query_state& state) LIBTORRENT_EXPORT;

////
////
////

struct uri_parse_result {
  static const int scheme_invalid = 0;
  static const int scheme_unknown = 1;
  static const int scheme_http = 2;
  static const int scheme_magnet = 3;
  static const int scheme_udp = 4;

  uri_parse_result();

  int         scheme;
};

struct uri_parse_authority_result : public uri_base_state {
  uri_parse_authority_result() : port(0) {}

  std::string userinfo;
  std::string hostname;
  uint16_t    port;
};

const char* uri_parse_scheme(const char* first, const char* last, int& result) LIBTORRENT_EXPORT;
const char* uri_parse_authority(const char* first, const char* last, uri_parse_authority_result& result) LIBTORRENT_EXPORT;

inline int uri_parse_scheme_str(const std::string& str);
inline uri_parse_authority_result uri_parse_authority_str(const std::string& str);

//
// Internal:
//

inline
uri_parse_result::uri_parse_result() :
  scheme(scheme_invalid) {
}

inline int
uri_parse_scheme_str(const std::string& str) {
  int result;
  uri_parse_scheme(str.c_str(), str.c_str() + str.size(), result);
  return result;
}

inline uri_parse_authority_result
uri_parse_authority_str(const std::string& str) {
  uri_parse_authority_result result;
  uri_parse_authority(str.c_str(), str.c_str() + str.size(), result);
  return result;
}

}}

#endif

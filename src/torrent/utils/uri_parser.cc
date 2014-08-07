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

#include "config.h"

#include <torrent/utils/uri_parser.h>

#include <cctype>
#include <cstring>

#include "rak/string_manip.h"

namespace torrent { namespace utils {

// TODO: Move?
// TODO: Add verification of escaped chars.

inline bool
is_uri_alphanum(char c) {
  return
    (c >= 'A' && c <= 'Z') ||
    (c >= 'a' && c <= 'z') ||
    (c >= '0' && c <= '9');
}

inline bool
is_uri_hex(char c) {
  return
    (c >= 'A' && c <= 'F') ||
    (c >= 'a' && c <= 'f') ||
    (c >= '0' && c <= '9');
}

inline bool
is_uri_mark(char c) {
  return
    c == '-' || c == '_' || c == '.' || c == '!' ||
    c == '~' || c == '*' || c == '\'' || c == '(' ||
    c == ')';
}

inline bool
is_uri_gen_delim(char c) {
  return
    c == ':' || c == '/' || c == '?' || c == '#' ||
    c == '[' || c == ']' || c == '@';
}

inline bool
is_uri_sub_delim(char c) {
  return
    c == '!' || c == '$' || c == '&' || c == '\'' ||
    c == '(' || c == ')' || c == '*' || c == '+' ||
    c == ',' || c == ';' || c == '=';
}

inline bool
is_uri_reserved(char c) {
  return is_uri_gen_delim(c) || is_uri_sub_delim(c);
}

inline bool
is_uri_unreserved(char c) {
  return is_uri_alphanum(c) || is_uri_mark(c);
}

inline bool
is_uri_scheme(char c) {
  return is_uri_alphanum(c) ||
    c == '+' || c == '-' || c == '.';
}

inline bool
is_uri_host(char c) {
  return is_uri_unreserved(c) || is_uri_sub_delim(c);
}

inline bool is_not_uri_scheme(char c) { return !is_uri_scheme(c); }
inline bool is_not_uri_host(char c) { return !is_uri_host(c); }

template<typename Ftor>
inline std::string::const_iterator
uri_string_copy_until(std::string::const_iterator first, std::string::const_iterator last,
                      std::string& result, Ftor check) {
  std::string::const_iterator next = std::find_if(first, last, check);

  result = std::string(first, next);
  return next;
}

template<typename Ftor>
inline const char*
uri_copy_until(const char* first, const char* last,
               std::string& result, Ftor check) {
  const char* next = std::find_if(first, last, check);

  result = std::string(first, next);
  return next;
}

template<typename Ftor>
inline const char*
uri_copy_pct_until(const char* first, const char* last,
               std::string& result, Ftor check) {
  result = std::string();

  while (first != last) {
    const char* next = std::find_if(first, last, check);
    
    result += std::string(first, next);

    // if ()... pct
  }
  
  return first;
}

void
uri_parse_throw_error(const char* error_msg, char invalid_char) {
  std::string error_str = std::string(error_msg);
  error_str += rak::value_to_hexchar<1>(invalid_char);
  error_str += rak::value_to_hexchar<0>(invalid_char);

  throw uri_error(error_str);
}

const char*
uri_parse_scheme(const char* first, const char* last, int& result) {
  const char* next = std::find_if(first, last, std::ptr_fun(&is_not_uri_scheme));

  if (next == first || next == last || *next != ':') {
    result = uri_parse_result::scheme_invalid;
    return first;
  }

  size_t scheme_distance = std::distance(first, next);

  if (std::strncmp(first, "http", scheme_distance) == 0) {
    result = uri_parse_result::scheme_http;
  } else if (std::strncmp(first, "magnet", scheme_distance) == 0) {
    result = uri_parse_result::scheme_magnet;
  } else if (std::strncmp(first, "udp", scheme_distance) == 0) {
    result = uri_parse_result::scheme_udp;
  } else {
    result = uri_parse_result::scheme_unknown;
  }

  return next;
}

const char*
uri_parse_authority(const char* first, const char* last, uri_parse_authority_result& result) {
  const char* next;

  // First parse userinfo...
  next = std::find_if(first, last, std::ptr_fun(&is_not_uri_scheme));


  return next;
}














//
//
//

inline bool
is_valid_uri_query_char(char c) {
  return
    (c >= 'A' && c <= 'Z') ||
    (c >= 'a' && c <= 'z') ||
    (c >= '0' && c <= '9') ||
    c == '-' || c == '_' || c == '.' || c == '~' ||
    c == ':' || c == '&' || c == '=' || c == '/' ||
    c == '%';
}

inline bool
is_unreserved_uri_query_char(char c) {
  return
    (c >= 'A' && c <= 'Z') ||
    (c >= 'a' && c <= 'z') ||
    (c >= '0' && c <= '9') ||
    c == '-' || c == '_' || c == '.' || c == '~' ||
    c == ':' || c == '=' || c == '/' || c == '%';
}

inline bool
is_not_unreserved_uri_char(char c) {
  return !is_uri_unreserved(c);
}

inline bool
is_not_valid_uri_query_char(char c) {
  return !is_valid_uri_query_char(c);
}

inline bool
is_not_unreserved_uri_query_char(char c) {
  return !is_unreserved_uri_query_char(c);
}

void
uri_parse_str(std::string uri, uri_state& state) {
  if (state.state != uri_state::state_empty)
    throw uri_error("uri_state.state is not uri_state::state_empty");

  state.uri.swap(uri);
  state.state = uri_state::state_invalid;

  std::string::const_iterator first = state.uri.begin();
  std::string::const_iterator last = state.uri.end();

  // Parse scheme:
  first = uri_string_copy_until(first, last, state.scheme, std::ptr_fun(&is_not_uri_scheme));

  if (first == last)
    goto uri_parse_success;

  if (*first++ != ':')
    uri_parse_throw_error("could not find ':' after scheme, found character 0x", *--first);


 uri_parse_success:
  state.state = uri_state::state_valid;
  return;
}


// * Letters (A-Z and a-z), numbers (0-9) and the characters
//   '.','-','~' and '_' are left as-is
// * SPACE is encoded as '+' or "%20"
// * All other characters are encoded as %HH hex representation with
//   any non-ASCII characters first encoded as UTF-8 (or other
//   specified encoding)
void
uri_parse_query_str(std::string query, uri_query_state& state) {
  if (state.state != uri_query_state::state_empty)
    throw uri_error("uri_query_state.state is not uri_query_state::state_empty");

  state.query.swap(query);
  state.state = uri_state::state_invalid;

  std::string::const_iterator first = state.query.begin();
  std::string::const_iterator last = state.query.end();

  while (first != last) {
    std::string element;
    first = uri_string_copy_until(first, last, element, std::ptr_fun(&is_not_unreserved_uri_query_char));

    if (first != last && *first++ != '&') {
      std::string invalid_hex;
      invalid_hex += rak::value_to_hexchar<1>(*--first);
      invalid_hex += rak::value_to_hexchar<0>(*first);

      throw uri_error("query element contains invalid character 0x" + invalid_hex);
    }

    state.elements.push_back(element);
  }

  state.state = uri_state::state_valid;
}

}}

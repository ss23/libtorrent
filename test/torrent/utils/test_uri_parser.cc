#include "config.h"

#include "test_uri_parser.h"

#include "helpers.h"

#include <inttypes.h>
#include <iostream>
#include <torrent/utils/uri_parser.h>

CPPUNIT_TEST_SUITE_REGISTRATION(UriParserTest);

void
UriParserTest::setUp() {
}

void
UriParserTest::tearDown() {
}

// void
// test_print_uri_state(torrent::utils::uri_state state) {
//   std::cerr << "state.uri: " << state.uri << std::endl;
//   std::cerr << "state.scheme: " << state.scheme << std::endl;
//   std::cerr << "state.resource: " << state.resource << std::endl;
//   std::cerr << "state.query: " << state.query << std::endl;
//   std::cerr << "state.fragment: " << state.fragment << std::endl;
// }


// void
// UriParserTest::test_basic_magnet() {
//   torrent::utils::uri_state state;
//   uri_parse_str(BASIC_MAGNET, state);
//   test_print_uri_state(state);

//   CPPUNIT_ASSERT(state.state == torrent::utils::uri_state::state_valid);

//   CPPUNIT_ASSERT(state.uri == BASIC_MAGNET);
//   CPPUNIT_ASSERT(state.scheme == "magnet");
//   CPPUNIT_ASSERT(state.resource == "");
//   CPPUNIT_ASSERT(state.query == "xt=urn:sha1:YNCKHTQCWBTRNJIV4WNAE52SJUQCZO5C");
//   CPPUNIT_ASSERT(state.fragment == "");
// }


// void
// UriParserTest::test_query_magnet() {
//   torrent::utils::uri_state state;
//   torrent::utils::uri_query_state query_state;

//   uri_parse_str(QUERY_MAGNET, state);

//   test_print_uri_state(state);

//   CPPUNIT_ASSERT(state.state == torrent::utils::uri_state::state_valid);

//   CPPUNIT_ASSERT(state.uri == QUERY_MAGNET);
//   CPPUNIT_ASSERT(state.scheme == "magnet");
//   CPPUNIT_ASSERT(state.resource == "");
//   CPPUNIT_ASSERT(state.query == QUERY_MAGNET_QUERY);
//   CPPUNIT_ASSERT(state.fragment == "");

//   uri_parse_query_str(state.query, query_state);
  
//   for (auto element : query_state.elements)
//     std::cerr << "query_element: " << element << std::endl;

//   CPPUNIT_ASSERT(query_state.state == torrent::utils::uri_query_state::state_valid);

//   CPPUNIT_ASSERT(query_state.elements.size() == 5);
//   CPPUNIT_ASSERT(query_state.elements.at(0) == "xt=urn:ed2k:31D6CFE0D16AE931B73C59D7E0C089C0");
//   CPPUNIT_ASSERT(query_state.elements.at(1) == "xl=0");
//   CPPUNIT_ASSERT(query_state.elements.at(2) == "dn=zero_len.fil");
//   CPPUNIT_ASSERT(query_state.elements.at(3) == "xt=urn:bitprint:3I42H3S6NNFQ2MSVX7XZKYAYSCX5QBYJ.LWPNACQDBZRYXW3VHJVCJ64QBZNGHOHHHZWCLNQ");
//   CPPUNIT_ASSERT(query_state.elements.at(4) == "xt=urn:md5:D41D8CD98F00B204E9800998ECF8427E");
// }


// void
// UriParserTest::test_basic_udp() {
//   // torrent::utils::uri_state state;
//   // uri_parse_str(BASIC_UDP, state);
//   // test_print_uri_state(state);

//   // CPPUNIT_ASSERT(state.state == torrent::utils::uri_state::state_valid);

//   // CPPUNIT_ASSERT(state.uri == BASIC_UDP);
//   // CPPUNIT_ASSERT(state.scheme == "udp");
//   // CPPUNIT_ASSERT(state.resource == "//tracker.openbittorrent.com:80");
//   // CPPUNIT_ASSERT(state.query == "");
//   // CPPUNIT_ASSERT(state.fragment == "");  
// }


///
///

// inline uri_parse_authority_result
// uri_parse_authority_str(const std::string& str) {
//   uri_parse_authority_result result;
//   uri_parse_authority(str.c_str(), str.c_str() + str.size(), result);
//   return result;
// }

#define BASIC_MAGNET_SCHEME "magnet:\xff\xff"
#define BASIC_UDP_SCHEME "udp:\xff\xff"

#define BASIC_MAGNET_AUTHORITY "xt=urn:sha1:YNCKHTQCWBTRNJIV4WNAE52SJUQCZO5C"

#define QUERY_MAGNET_QUERY                              \
  "xt=urn:ed2k:31D6CFE0D16AE931B73C59D7E0C089C0"        \
  "&xl=0&dn=zero_len.fil"                               \
  "&xt=urn:bitprint:3I42H3S6NNFQ2MSVX7XZKYAYSCX5QBYJ"   \
  ".LWPNACQDBZRYXW3VHJVCJ64QBZNGHOHHHZWCLNQ"            \
  "&xt=urn:md5:D41D8CD98F00B204E9800998ECF8427E"

#define QUERY_MAGNET "magnet:?" QUERY_MAGNET_QUERY

void
UriParserTest::test_basic() {
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_invalid == 0);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_invalid < torrent::utils::uri_parse_result::scheme_unknown);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_unknown < torrent::utils::uri_parse_result::scheme_http);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_http < torrent::utils::uri_parse_result::scheme_magnet);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_magnet < torrent::utils::uri_parse_result::scheme_udp);
}

void
UriParserTest::test_scheme() {
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result().scheme == torrent::utils::uri_parse_result::scheme_invalid);

  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("http:\x00\x00") == torrent::utils::uri_parse_result::scheme_http);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("http:\xff\xff") == torrent::utils::uri_parse_result::scheme_http);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("http://") == torrent::utils::uri_parse_result::scheme_http);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("http:a") == torrent::utils::uri_parse_result::scheme_http);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str(BASIC_MAGNET_SCHEME) == torrent::utils::uri_parse_result::scheme_magnet);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str(BASIC_UDP_SCHEME) == torrent::utils::uri_parse_result::scheme_udp);

  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("uf+ufu://example.com") == torrent::utils::uri_parse_result::scheme_unknown);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("uf-ufu://example.com") == torrent::utils::uri_parse_result::scheme_unknown);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("uf.ufu://example.com") == torrent::utils::uri_parse_result::scheme_unknown);

  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("/://example.com") == torrent::utils::uri_parse_result::scheme_invalid);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("http//example.com") == torrent::utils::uri_parse_result::scheme_invalid);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("foo_bar://example.com") == torrent::utils::uri_parse_result::scheme_invalid);
}

inline bool
match_uri_result_with_c_str(const std::string& result, const char* expected) {
  return (expected == NULL && result.empty()) || (expected != NULL && result != userinfo);
}

bool
match_uri_parse_authority(const std::string& input,
                          const char* userinfo,
                          const char* hostname,
                          uint16_t port) {
  torrent::utils::uri_parse_authority_result result;

  if (torrent::utils::uri_parse_authority(str.c_str(), str.c_str() + str.size(), result) != str.c_str() + str.size())
    return false;
  
  if (result.state != torrent::utils::uri_parse_authority_result::state_valid)
    return false;
  
  if (match_uri_result_with_c_str(result.userinfo, userinfo)) return false;
  if (match_uri_result_with_c_str(result.hostname, hostname)) return false;
  if (result.port != port) return false;

  return true
}

#define UDP_AUTHORITY_BASIC "tracker.openbittorrent.com"
#define UDP_AUTHORITY_WITH_PORT "tracker.openbittorrent.com:80"
#define UDP_AUTHORITY_WITH_USERINFO "tracker.openbittorrent.com:80"

void
UriParserTest::test_authority() {
  CPPUNIT_ASSERT(torrent::utils::uri_parse_authority_result().state == torrent::utils::uri_parse_authority_result::state_invalid);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_authority_result().port == 0);

  CPPUNIT_ASSERT(match_uri_parse_authority(UDP_AUTHORITY_BASIC, NULL, "tracker.openbittorrent.com", NULL));
}

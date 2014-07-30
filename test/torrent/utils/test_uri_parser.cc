#include "config.h"

#include "test_uri_parser.h"

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

void
test_print_uri_state(torrent::utils::uri_state state) {
  std::cerr << "state.uri: " << state.uri << std::endl;
  std::cerr << "state.scheme: " << state.scheme << std::endl;
  std::cerr << "state.resource: " << state.resource << std::endl;
  std::cerr << "state.query: " << state.query << std::endl;
  std::cerr << "state.fragment: " << state.fragment << std::endl;
}

void
UriParserTest::test_basic() {
  torrent::utils::uri_state state;

  CPPUNIT_ASSERT(state.state == torrent::utils::uri_state::state_empty);
  CPPUNIT_ASSERT(state.state != torrent::utils::uri_state::state_valid);
  CPPUNIT_ASSERT(state.state != torrent::utils::uri_state::state_invalid);
}

#define BASIC_MAGNET "magnet:?xt=urn:sha1:YNCKHTQCWBTRNJIV4WNAE52SJUQCZO5C"
#define BASIC_UDP "udp://tracker.openbittorrent.com:80"

void
UriParserTest::test_basic_magnet() {
  torrent::utils::uri_state state;
  uri_parse_str(BASIC_MAGNET, state);
  test_print_uri_state(state);

  CPPUNIT_ASSERT(state.state == torrent::utils::uri_state::state_valid);

  CPPUNIT_ASSERT(state.uri == BASIC_MAGNET);
  CPPUNIT_ASSERT(state.scheme == "magnet");
  CPPUNIT_ASSERT(state.resource == "");
  CPPUNIT_ASSERT(state.query == "xt=urn:sha1:YNCKHTQCWBTRNJIV4WNAE52SJUQCZO5C");
  CPPUNIT_ASSERT(state.fragment == "");
}

#define QUERY_MAGNET_QUERY                              \
  "xt=urn:ed2k:31D6CFE0D16AE931B73C59D7E0C089C0"        \
  "&xl=0&dn=zero_len.fil"                               \
  "&xt=urn:bitprint:3I42H3S6NNFQ2MSVX7XZKYAYSCX5QBYJ"   \
  ".LWPNACQDBZRYXW3VHJVCJ64QBZNGHOHHHZWCLNQ"            \
  "&xt=urn:md5:D41D8CD98F00B204E9800998ECF8427E"

#define QUERY_MAGNET "magnet:?" QUERY_MAGNET_QUERY

void
UriParserTest::test_query_magnet() {
  torrent::utils::uri_state state;
  torrent::utils::uri_query_state query_state;

  uri_parse_str(QUERY_MAGNET, state);

  test_print_uri_state(state);

  CPPUNIT_ASSERT(state.state == torrent::utils::uri_state::state_valid);

  CPPUNIT_ASSERT(state.uri == QUERY_MAGNET);
  CPPUNIT_ASSERT(state.scheme == "magnet");
  CPPUNIT_ASSERT(state.resource == "");
  CPPUNIT_ASSERT(state.query == QUERY_MAGNET_QUERY);
  CPPUNIT_ASSERT(state.fragment == "");

  uri_parse_query_str(state.query, query_state);
  
  for (auto element : query_state.elements)
    std::cerr << "query_element: " << element << std::endl;

  CPPUNIT_ASSERT(query_state.state == torrent::utils::uri_query_state::state_valid);

  CPPUNIT_ASSERT(query_state.elements.size() == 5);
  CPPUNIT_ASSERT(query_state.elements.at(0) == "xt=urn:ed2k:31D6CFE0D16AE931B73C59D7E0C089C0");
  CPPUNIT_ASSERT(query_state.elements.at(1) == "xl=0");
  CPPUNIT_ASSERT(query_state.elements.at(2) == "dn=zero_len.fil");
  CPPUNIT_ASSERT(query_state.elements.at(3) == "xt=urn:bitprint:3I42H3S6NNFQ2MSVX7XZKYAYSCX5QBYJ.LWPNACQDBZRYXW3VHJVCJ64QBZNGHOHHHZWCLNQ");
  CPPUNIT_ASSERT(query_state.elements.at(4) == "xt=urn:md5:D41D8CD98F00B204E9800998ECF8427E");
}

///
///

void
UriParserTest::test_scheme() {
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_invalid == 0);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_invalid < torrent::utils::uri_parse_result::scheme_unknown);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_unknown < torrent::utils::uri_parse_result::scheme_http);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_http < torrent::utils::uri_parse_result::scheme_magnet);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_result::scheme_magnet < torrent::utils::uri_parse_result::scheme_udp);

  CPPUNIT_ASSERT(torrent::utils::uri_parse_result().scheme == torrent::utils::uri_parse_result::scheme_invalid);

  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("http://example.com") == torrent::utils::uri_parse_result::scheme_http);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str(BASIC_MAGNET) == torrent::utils::uri_parse_result::scheme_magnet);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str(BASIC_UDP) == torrent::utils::uri_parse_result::scheme_udp);

  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("uf+ufu://example.com") == torrent::utils::uri_parse_result::scheme_unknown);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("uf-ufu://example.com") == torrent::utils::uri_parse_result::scheme_unknown);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("uf.ufu://example.com") == torrent::utils::uri_parse_result::scheme_unknown);

  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("/://example.com") == torrent::utils::uri_parse_result::scheme_invalid);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("http//example.com") == torrent::utils::uri_parse_result::scheme_invalid);
  CPPUNIT_ASSERT(torrent::utils::uri_parse_scheme_str("foo_bar://example.com") == torrent::utils::uri_parse_result::scheme_invalid);
}

void
UriParserTest::test_basic_udp() {
  // torrent::utils::uri_state state;
  // uri_parse_str(BASIC_UDP, state);
  // test_print_uri_state(state);

  // CPPUNIT_ASSERT(state.state == torrent::utils::uri_state::state_valid);

  // CPPUNIT_ASSERT(state.uri == BASIC_UDP);
  // CPPUNIT_ASSERT(state.scheme == "udp");
  // CPPUNIT_ASSERT(state.resource == "//tracker.openbittorrent.com:80");
  // CPPUNIT_ASSERT(state.query == "");
  // CPPUNIT_ASSERT(state.fragment == "");  
}

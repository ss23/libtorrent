#include <cppunit/extensions/HelperMacros.h>

class UriParserTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(UriParserTest);
  CPPUNIT_TEST(test_basic);
  CPPUNIT_TEST(test_scheme);
  CPPUNIT_TEST(test_authority);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void test_basic();

  void test_scheme();
  void test_authority();
};

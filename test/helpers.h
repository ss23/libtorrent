#ifndef LIBTORRENT_TEST_HELPERS
#define LIBTORRENT_TEST_HELPERS

#include lt_tr1_functional
#include <torrent/exceptions.h>

inline bool
verify_did_internal_error(std::function<void ()> func, bool should_throw) {
  bool did_throw = false;

  try {
    func();
  } catch (torrent::internal_error& e) {
    did_throw = true;
  }

  return should_throw == did_throw;
}

inline bool
verify_did_uri_error(std::function<void ()> func, bool should_throw) {
  bool did_throw = false;

  try {
    func();
  } catch (torrent::uri_error& e) {
    did_throw = true;
  }

  return should_throw == did_throw;
}

#endif


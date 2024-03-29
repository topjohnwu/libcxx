//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03

// The string reported on errors changed, which makes those tests fail when run
// against already-released libc++'s.
// XFAIL: use_system_cxx_lib && target={{.+}}-apple-macosx{{10.15|11.0}}

// <filesystem>

// class directory_entry

// uintmax_t hard_link_count() const;
// uintmax_t hard_link_count(error_code const&) const noexcept;

#include "filesystem_include.h"
#include <type_traits>
#include <cassert>

#include "filesystem_test_helper.h"
#include "rapid-cxx-test.h"

#include "test_macros.h"

TEST_SUITE(directory_entry_obs_testsuite)

TEST_CASE(signatures) {
  using namespace fs;
  {
    const directory_entry e = {};
    std::error_code ec;
    static_assert(std::is_same<decltype(e.hard_link_count()), uintmax_t>::value, "");
    static_assert(std::is_same<decltype(e.hard_link_count(ec)), uintmax_t>::value,
                  "");
    static_assert(noexcept(e.hard_link_count()) == false, "");
    static_assert(noexcept(e.hard_link_count(ec)) == true, "");
  }
}

TEST_CASE(basic) {
  using namespace fs;

  scoped_test_env env;
  const path file = env.create_file("file", 42);
  const path dir = env.create_dir("dir");
  const path sym = env.create_symlink("file", "sym");

  {
    directory_entry ent(file);
    uintmax_t expect = hard_link_count(ent);

    // Remove the file to show that the results were already in the cache.
    LIBCPP_ONLY(remove(file));

    std::error_code ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == expect);
    TEST_CHECK(!ec);
  }
  {
    directory_entry ent(dir);
    uintmax_t expect = hard_link_count(ent);

    LIBCPP_ONLY(remove(dir));

    std::error_code ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == expect);
    TEST_CHECK(!ec);
  }
#if !defined(__ANDROID__)
  env.create_file("file", 99);
  env.create_hardlink("file", "hl");
  {
    directory_entry ent(sym);
    std::error_code ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == 2);
    TEST_CHECK(!ec);
  }
#endif
}

TEST_CASE(not_regular_file) {
  using namespace fs;

  scoped_test_env env;
  const path dir = env.create_dir("dir");
  const path dir2 = env.create_dir("dir/dir2");

  const perms old_perms = status(dir).permissions();

  auto test_path = [=](const path &p) {
    std::error_code dummy_ec = GetTestEC();
    directory_entry ent(p, dummy_ec);
    TEST_CHECK(!dummy_ec);

    uintmax_t expect = hard_link_count(p);

    LIBCPP_ONLY(permissions(dir, perms::none));

    std::error_code ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == expect);
    TEST_CHECK(!ec);
    TEST_CHECK_NO_THROW(ent.hard_link_count());
    permissions(dir, old_perms);
  };
  test_path(dir2);
#if !defined(_WIN32) && !defined(__ANDROID__)
  const path fifo = env.create_fifo("dir/fifo");
  const path sym_to_fifo = env.create_symlink("dir/fifo", "dir/sym");
  test_path(fifo);
  test_path(sym_to_fifo);
#endif
}

TEST_CASE(error_reporting) {
  using namespace fs;

  static_test_env static_env;
  scoped_test_env env;

  const path dir = env.create_dir("dir");
  const path file = env.create_file("dir/file", 42);
  const path file_out_of_dir = env.create_file("file2", 101);
  const path sym_out_of_dir = env.create_symlink("dir/file", "sym");
  const path sym_in_dir = env.create_symlink("file2", "dir/sym2");

#ifndef TEST_WIN_NO_FILESYSTEM_PERMS_NONE
  const perms old_perms = status(dir).permissions();
#endif

  // test a file which doesn't exist
  {
    directory_entry ent;

    std::error_code ec = GetTestEC();
    ent.assign(static_env.DNE, ec);
    TEST_CHECK(ec);
    TEST_REQUIRE(ent.path() == static_env.DNE);
    TEST_CHECK(ErrorIs(ec, std::errc::no_such_file_or_directory));

    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == uintmax_t(-1));
    TEST_CHECK(ErrorIs(ec, std::errc::no_such_file_or_directory));

    ExceptionChecker Checker(static_env.DNE,
                             std::errc::no_such_file_or_directory,
                             "directory_entry::hard_link_count");
    TEST_CHECK_THROW_RESULT(filesystem_error, Checker, ent.hard_link_count());
  }
  // test a dead symlink
  {
    directory_entry ent;

    std::error_code ec = GetTestEC();
    uintmax_t expect_bad = hard_link_count(static_env.BadSymlink, ec);
    TEST_CHECK(expect_bad == uintmax_t(-1));
    TEST_CHECK(ErrorIs(ec, std::errc::no_such_file_or_directory));

    ec = GetTestEC();
    ent.assign(static_env.BadSymlink, ec);
    TEST_REQUIRE(ent.path() == static_env.BadSymlink);
    TEST_CHECK(!ec);

    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == expect_bad);
    TEST_CHECK(ErrorIs(ec, std::errc::no_such_file_or_directory));

    ExceptionChecker Checker(static_env.BadSymlink,
                             std::errc::no_such_file_or_directory,
                             "directory_entry::hard_link_count");
    TEST_CHECK_THROW_RESULT(filesystem_error, Checker, ent.hard_link_count());
  }
  // Windows doesn't support setting perms::none to trigger failures
  // reading directories.
#ifndef TEST_WIN_NO_FILESYSTEM_PERMS_NONE
  // test a file w/o appropriate permissions.
  {
    directory_entry ent;
    uintmax_t expect_good = hard_link_count(file);
    permissions(dir, perms::none);

    std::error_code ec = GetTestEC();
    ent.assign(file, ec);
    TEST_REQUIRE(ent.path() == file);
    TEST_CHECK(ErrorIs(ec, std::errc::permission_denied));

    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == uintmax_t(-1));
    TEST_CHECK(ErrorIs(ec, std::errc::permission_denied));

    ExceptionChecker Checker(file, std::errc::permission_denied,
                             "hard_link_count");
    TEST_CHECK_THROW_RESULT(filesystem_error, Checker, ent.hard_link_count());

    permissions(dir, old_perms);
    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == expect_good);
    TEST_CHECK(!ec);
    TEST_CHECK_NO_THROW(ent.hard_link_count());
  }
  permissions(dir, old_perms);
  // test a symlink w/o appropriate permissions.
  {
    directory_entry ent;
    uintmax_t expect_good = hard_link_count(sym_in_dir);
    permissions(dir, perms::none);

    std::error_code ec = GetTestEC();
    ent.assign(sym_in_dir, ec);
    TEST_REQUIRE(ent.path() == sym_in_dir);
    TEST_CHECK(ErrorIs(ec, std::errc::permission_denied));

    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == uintmax_t(-1));
    TEST_CHECK(ErrorIs(ec, std::errc::permission_denied));

    ExceptionChecker Checker(sym_in_dir, std::errc::permission_denied,
                             "hard_link_count");
    TEST_CHECK_THROW_RESULT(filesystem_error, Checker, ent.hard_link_count());

    permissions(dir, old_perms);
    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == expect_good);
    TEST_CHECK(!ec);
    TEST_CHECK_NO_THROW(ent.hard_link_count());
  }
  permissions(dir, old_perms);
  // test a symlink to a file w/o appropriate permissions
  {
    directory_entry ent;
    uintmax_t expect_good = hard_link_count(sym_out_of_dir);
    permissions(dir, perms::none);

    std::error_code ec = GetTestEC();
    ent.assign(sym_out_of_dir, ec);
    TEST_REQUIRE(ent.path() == sym_out_of_dir);
    TEST_CHECK(!ec);

    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == uintmax_t(-1));
    TEST_CHECK(ErrorIs(ec, std::errc::permission_denied));

    ExceptionChecker Checker(sym_out_of_dir, std::errc::permission_denied,
                             "hard_link_count");
    TEST_CHECK_THROW_RESULT(filesystem_error, Checker, ent.hard_link_count());

    permissions(dir, old_perms);
    ec = GetTestEC();
    TEST_CHECK(ent.hard_link_count(ec) == expect_good);
    TEST_CHECK(!ec);
    TEST_CHECK_NO_THROW(ent.hard_link_count());
  }
#endif
}

TEST_SUITE_END()

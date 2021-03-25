"""Test format for the NDK tests."""
import os

from libcxx.ndk.executors import NoopExecutor
from libcxx.test.format import LibcxxTestFormat


def prune_xfails(test):
    """Removes most xfail handling from tests.

    We need to keep some xfail handling because some tests in libc++ that
    really should be using REQUIRES actually used XFAIL (i.e. `XFAIL: c++11`).
    """
    test.xfails = [x for x in test.xfails if x.startswith('c++')]


class TestFormat(LibcxxTestFormat):
    """Build-only test format that disables XFAIL handling."""

    # pylint: disable=too-many-arguments
    def __init__(self,
                 cxx,
                 libcxx_src_root,
                 libcxx_obj_root,
                 build_dir,
                 target_info):
        super(TestFormat, self).__init__(cxx,
                                         use_verify_for_fail=True,
                                         executor=NoopExecutor(target_info),
                                         exec_env={})
        self.libcxx_src_root = libcxx_src_root
        self.libcxx_obj_root = libcxx_obj_root
        self.build_dir = build_dir

    def _evaluate_pass_test(self, test, tmpBase, lit_config, test_cxx, parsers,
                            data_files):
        """Clears the test's xfail list before delegating to the parent."""
        prune_xfails(test)
        tmp_base = os.path.join(self.build_dir, *test.path_in_suite)
        return super(TestFormat,
                     self)._evaluate_pass_test(test, tmp_base, lit_config,
                                               test_cxx, parsers, data_files)

    def _evaluate_fail_test(self, test, test_cxx, parsers):
        """Clears the test's xfail list before delegating to the parent."""
        prune_xfails(test)
        return super(TestFormat, self)._evaluate_fail_test(
            test, test_cxx, parsers)

    def _clean(self, exec_path):
        pass

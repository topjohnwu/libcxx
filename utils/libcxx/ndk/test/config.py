import libcxx.test.config
import libcxx.test.target_info
import libcxx.ndk.test.format


class AndroidTargetInfo(libcxx.test.target_info.DefaultTargetInfo):
    def platform(self):
        return 'android'

    def system(self):
        raise NotImplementedError

    def add_cxx_compile_flags(self, flags):
        flags.extend(['-D__STDC_FORMAT_MACROS'])

    def platform_ver(self):
        raise NotImplementedError

    def platform_name(self):
        raise NotImplementedError

    def supports_locale(self, loc):
        raise NotImplementedError

    def is_windows(self):
        return False


class Configuration(libcxx.test.config.Configuration):
    def __init__(self, lit_config, config):
        super(Configuration, self).__init__(lit_config, config)
        self.cxx_under_test = None
        self.build_cmds_dir = None
        self.cxx_template = None
        self.link_template = None
        self.with_availability = False
        self.target_info = None

    def configure_target_info(self):
        self.target_info = AndroidTargetInfo(self)

    def configure_link_flags(self):
        if self.get_lit_conf('linker') == 'lld':
            self.cxx.link_flags.append('-fuse-ld=lld')

        triple = self.get_lit_conf('target_triple')
        if triple.startswith('armv7-'):
            self.cxx.link_flags.append('-Wl,--exclude-libs,libunwind.a')

        self.cxx.link_flags.append('-Wl,--exclude-libs,libatomic.a')
        self.cxx.link_flags.append('-Wl,--exclude-libs,libgcc.a')

    def configure_features(self):
        self.config.available_features.add('long_tests')
        self.config.available_features.add('c++filesystem-disabled')
        self.config.available_features.add('libcpp-no-concepts')

    def configure_modules(self):
        # Not something we support yet.
        pass

    def get_test_format(self):
        # Note that we require that the caller has cleaned this directory,
        # ensured its existence, and copied libc++_shared.so into it.
        build_dir = self.get_lit_conf('build_dir')

        return libcxx.ndk.test.format.TestFormat(
            self.cxx,
            self.libcxx_src_root,
            self.libcxx_obj_root,
            build_dir,
            self.target_info)

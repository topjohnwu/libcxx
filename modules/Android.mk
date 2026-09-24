LOCAL_PATH := $(call my-dir)

ifndef BUILD_STATIC_LIBRARY_MODULE
$(error Load the project's ndk-modules/init.mk before libcxx/modules/Android.mk)
endif

# Keep the BMI separate from abi.cpp, which uses its own C++ standard and
# visibility flags. This target inherits the application's language mode and
# libcxx's exported headers, exception and RTTI settings, just like its users.
include $(CLEAR_VARS)
LOCAL_MODULE := cxx_std
LOCAL_MODULE_SRC_FILES := std.cppm
LOCAL_CPPFLAGS := -Wno-reserved-module-identifier
LOCAL_STATIC_LIBRARIES := libcxx
LOCAL_EXPORT_STATIC_LIBRARIES := libcxx
LOCAL_EXPORT_CPPFLAGS := -fmodule-file=std=$(TARGET_OBJS)/cxx_std/std.cppm.o.pcm
include $(BUILD_STATIC_LIBRARY_MODULE)

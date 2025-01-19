# This file is dual licensed under the MIT and the University of Illinois Open
# Source Licenses. See LICENSE.TXT for details.

LOCAL_PATH := $(call my-dir)

# libcxx defines
libcxx_includes := $(LOCAL_PATH)/include $(LOCAL_PATH)/src
libcxx_export_includes := $(libcxx_includes)
libcxx_sources := \
    $(wildcard $(LOCAL_PATH)/src/*.cpp) \
    $(wildcard $(LOCAL_PATH)/src/filesystem/*.cpp)

libcxx_sources := $(libcxx_sources:$(LOCAL_PATH)/%=%)

libcxx_export_cxxflags :=

libcxx_cxxflags := \
    -std=c++2c \
    -fvisibility-global-new-delete=force-hidden \
    -fvisibility=hidden -fvisibility-inlines-hidden \
    -DLIBCXX_BUILDING_LIBCXXABI \
    -D_LIBCPP_NO_EXCEPTIONS \
    -D_LIBCPP_NO_RTTI \
    -D_LIBCPP_BUILDING_LIBRARY \
    -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS \
    -D__STDC_FORMAT_MACROS \
    $(libcxx_export_cxxflags) \


libcxx_ldflags :=
libcxx_export_ldflags :=

# libcxxabi defines
libcxxabi_src_files := \
    abort_message.cpp \
    cxa_aux_runtime.cpp \
    cxa_default_handlers.cpp \
    cxa_exception_storage.cpp \
    cxa_guard.cpp \
    cxa_handlers.cpp \
    cxa_noexception.cpp \
    cxa_thread_atexit.cpp \
    cxa_vector.cpp \
    cxa_virtual.cpp \
    stdlib_exception.cpp \
    stdlib_new_delete.cpp \
    stdlib_stdexcept.cpp \
    stdlib_typeinfo.cpp \

libcxxabi_src_files := $(libcxxabi_src_files:%=src/abi/%)

libcxxabi_includes := $(LOCAL_PATH)/include

libcxxabi_cflags := -D__STDC_FORMAT_MACROS
libcxxabi_cppflags := \
    -D_LIBCXXABI_NO_EXCEPTIONS \
    -Wno-macro-redefined \
    -Wno-unknown-attributes \
    -DHAS_THREAD_LOCAL

include $(CLEAR_VARS)
LOCAL_MODULE := libcxx
LOCAL_SRC_FILES := $(libcxx_sources) $(libcxxabi_src_files)
LOCAL_C_INCLUDES := $(libcxx_includes) $(libcxxabi_includes)
LOCAL_CPPFLAGS := $(libcxx_cxxflags) $(libcxxabi_cppflags) -ffunction-sections -fdata-sections
LOCAL_EXPORT_C_INCLUDES := $(libcxx_export_includes)
LOCAL_EXPORT_CPPFLAGS := $(libcxx_export_cxxflags)
LOCAL_EXPORT_LDFLAGS := $(libcxx_export_ldflags)

include $(BUILD_STATIC_LIBRARY)

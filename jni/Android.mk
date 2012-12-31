LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE	:= thumb
LOCAL_MODULE	:= easysqlite_jni

LOCAL_C_INCLUDES := LOCAL_PATH

LOCAL_SRC_FILES := \
	CppSQLite3.cpp \
	sqlite3secure.c \
	easysqlite_jni.cpp \

LOCAL_CPPFLAGS += -fexceptions

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
    
include $(BUILD_SHARED_LIBRARY)

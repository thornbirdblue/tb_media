LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=media_scan_main.c

LOCAL_MODULE:=media_scan

include $(BUILD_EXECUTABLE)


ifeq ($(BOARD_HAVE_SAMSUNG_WIFI),true)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    wifiloader.c

LOCAL_SHARED_LIBRARIES := \
    liblog libutils

ifneq ($(WIFI_DRIVER_MODULE_NAME),)
LOCAL_CFLAGS += -DWIFI_DRIVER_MODULE_NAME=\"$(WIFI_DRIVER_MODULE_NAME)\"
endif

LOCAL_MODULE := wifiloader
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

endif

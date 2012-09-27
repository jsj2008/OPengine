# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# the purpose of this sample is to demonstrate how one can
# generate two distinct shared libraries and have them both
# uploaded in
#

LOCAL_PATH:= $(call my-dir)

PROJECT_PATH:= C:/Repos/OPifex Entertainment/OPifex.Hg/OPifex.Engine/

##############
# 1_CORE
# core lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-core

CORE_LIST_C := $(wildcard $(PROJECT_PATH)/Core/*.c)

LOCAL_SRC_FILES := \
	$(CORE_LIST_C:$(PROJECT_PATH)/%=%)
	
include $(BUILD_STATIC_LIBRARY)



##############
# 2_DATA
# data lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-data

DATA_LIST := $(wildcard $(PROJECT_PATH)/Data/*.cpp)
DATA_LIST_C := $(wildcard $(PROJECT_PATH)/Data/*.c)

LOCAL_SRC_FILES := \
	$(DATA_LIST:$(PROJECT_PATH)/%=%) \
	$(DATA_LIST_C:$(PROJECT_PATH)/%=%)
	
LOCAL_STATIC_LIBRARIES := libsmrf-core
include $(BUILD_STATIC_LIBRARY)



##############
# 3_HUMAN
# human lib, which will be built statically
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libopifex-human
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_CFLAGS    := -Werror

HUMAN_RENDERING_LIST := $(wildcard $(PROJECT_PATH)/Human/Rendering/*.cpp)
HUMAN_RENDERING_OPENGLES_LIST := $(wildcard $(PROJECT_PATH)/Human/Rendering/GL/ES/*.cpp)

LOCAL_SRC_FILES := \
	$(HUMAN_RENDERING_LIST:$(PROJECT_PATH)/%=%) \
	$(HUMAN_RENDERING_OPENGLES_LIST:$(PROJECT_PATH)/%=%) 

LOCAL_STATIC_LIBRARIES := libsmrf-data
include $(BUILD_STATIC_LIBRARY)



##############
# 7_APPLICATION
# second lib, which will depend on and include the first one
##############
include $(CLEAR_VARS)
LOCAL_MODULE    := libsmrf
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_SRC_FILES := \
	$($(PROJECT_PATH)/Android/jni/application.cpp)
LOCAL_STATIC_LIBRARIES := libopifex-human
include $(BUILD_SHARED_LIBRARY)
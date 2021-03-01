/*
 * Copyright (C) 2019-2021 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(EXYNOS)
#define LOG_TAG_VARIANT "samsung-exynos"
#elif defined(QCOM)
#define LOG_TAG_VARIANT "samsung-qcom"
#else
#define LOG_TAG_VARIANT "samsung-generic"
#endif

#ifdef LIVES_IN_SYSTEM
#define LOG_TAG "lineage.livedisplay@2.0-service." LOG_TAG_VARIANT
#else
#define LOG_TAG "vendor.lineage.livedisplay@2.0-service." LOG_TAG_VARIANT
#endif

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include <livedisplay/samsung/AdaptiveBacklight.h>
#include <livedisplay/samsung/DisplayModes.h>
#include <livedisplay/samsung/ReadingEnhancement.h>

#if defined(EXYNOS)
#include <livedisplay/samsung/exynos/DisplayColorCalibration.h>
#include <livedisplay/samsung/exynos/SunlightEnhancement.h>
#elif defined(QCOM)
#include <livedisplay/samsung/qcom/DisplayColorCalibration.h>
#include <livedisplay/samsung/qcom/SunlightEnhancement.h>
#endif

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using android::sp;
using android::status_t;
using android::OK;

using vendor::lineage::livedisplay::V2_0::samsung::AdaptiveBacklight;
using vendor::lineage::livedisplay::V2_0::samsung::DisplayModes;
using vendor::lineage::livedisplay::V2_0::samsung::ReadingEnhancement;

#if defined(EXYNOS) || defined(QCOM)
using vendor::lineage::livedisplay::V2_0::samsung::DisplayColorCalibration;
using vendor::lineage::livedisplay::V2_0::samsung::SunlightEnhancement;
#endif

int main() {
    sp<AdaptiveBacklight> adaptiveBacklight;
    sp<DisplayModes> displayModes;
    sp<ReadingEnhancement> readingEnhancement;
    #if defined(EXYNOS) || defined(QCOM)
    sp<DisplayColorCalibration> displayColorCalibration;
    sp<SunlightEnhancement> sunlightEnhancement;
    #endif
    status_t status;

    LOG(INFO) << "LiveDisplay HAL service is starting.";

    adaptiveBacklight = new AdaptiveBacklight();
    if (adaptiveBacklight == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL AdaptiveBacklight Iface, exiting.";
        goto shutdown;
    }

    displayModes = new DisplayModes();
    if (displayModes == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL DisplayModes Iface, exiting.";
        goto shutdown;
    }

    readingEnhancement = new ReadingEnhancement();
    if (readingEnhancement == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL ReadingEnhancement Iface, exiting.";
        goto shutdown;
    }

    #if defined(EXYNOS) || defined(QCOM)
    displayColorCalibration = new DisplayColorCalibration();
    if (displayColorCalibration == nullptr) {
        LOG(ERROR) << "Can not create an instance of LiveDisplay HAL DisplayColorCalibration "
                      "Iface, exiting.";
        goto shutdown;
    }

    sunlightEnhancement = new SunlightEnhancement();
    if (sunlightEnhancement == nullptr) {
        LOG(ERROR)
            << "Can not create an instance of LiveDisplay HAL SunlightEnhancement Iface, exiting.";
        goto shutdown;
    }
    #endif

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (adaptiveBacklight->isSupported()) {
        status = adaptiveBacklight->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL AdaptiveBacklight Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    if (displayModes->isSupported()) {
        status = displayModes->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL DisplayModes Iface ("
                       << status << ")";
            goto shutdown;
        }
    }

    if (readingEnhancement->isSupported()) {
        status = readingEnhancement->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for LiveDisplay HAL ReadingEnhancement Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    #if defined(EXYNOS) || defined(QCOM)
    if (displayColorCalibration->isSupported()) {
        status = displayColorCalibration->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for LiveDisplay HAL DisplayColorCalibration Iface ("
                << status << ")";
            goto shutdown;
        }
    }

    if (sunlightEnhancement->isSupported()) {
        status = sunlightEnhancement->registerAsService();
        if (status != OK) {
            LOG(ERROR)
                << "Could not register service for LiveDisplay HAL SunlightEnhancement Iface ("
                << status << ")";
            goto shutdown;
        }
    }
    #endif

    LOG(INFO) << "LiveDisplay HAL service is ready.";
    joinRpcThreadpool();
// Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return 1;
}

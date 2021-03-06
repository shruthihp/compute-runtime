/*
 * Copyright (c) 2017 - 2018, Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "unit_tests/helpers/gtest_helpers.h"
#include "unit_tests/os_interface/linux/hw_info_config_tests.h"

using namespace OCLRT;
using namespace std;

struct HwInfoConfigTestLinuxSkl : HwInfoConfigTestLinux {
    void SetUp() override {
        HwInfoConfigTestLinux::SetUp();
        drm->StoredDeviceID = ISKL_GT2_DESK_DEVICE_F0_ID;
        drm->setGtType(GTTYPE_GT2);
    }
};

SKLTEST_F(HwInfoConfigTestLinuxSkl, configureHwInfo) {
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ((unsigned short)drm->StoredDeviceID, outHwInfo.pPlatform->usDeviceID);
    EXPECT_EQ((unsigned short)drm->StoredDeviceRevID, outHwInfo.pPlatform->usRevId);
    EXPECT_EQ((uint32_t)drm->StoredEUVal, outHwInfo.pSysInfo->EUCount);
    EXPECT_EQ((uint32_t)drm->StoredSSVal, outHwInfo.pSysInfo->SubSliceCount);

    EXPECT_EQ(GTTYPE_GT2, outHwInfo.pPlatform->eGTType);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1_5);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrGT2);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT3);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT4);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTA);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTC);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTX);

    //constant sysInfo/ftr flags
    EXPECT_EQ(1u, outHwInfo.pSysInfo->VEBoxInfo.Instances.Bits.VEBox0Enabled);
    EXPECT_EQ(1u, outHwInfo.pSysInfo->VDBoxInfo.Instances.Bits.VDBox0Enabled);
    EXPECT_TRUE(outHwInfo.pSysInfo->VEBoxInfo.IsValid);
    EXPECT_TRUE(outHwInfo.pSysInfo->VDBoxInfo.IsValid);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrGpGpuMidBatchPreempt);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrGpGpuThreadGroupLevelPreempt);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGpGpuMidThreadLevelPreempt);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftr3dMidBatchPreempt);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftr3dObjectLevelPreempt);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrPerCtxtPreemptionGranularityControl);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrPPGTT);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrSVM);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrL3IACoherency);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrIA32eGfxPTEs);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrDisplayYTiling);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrTranslationTable);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrUserModeTranslationTable);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrEnableGuC);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrFbc);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrFbc2AddressTranslation);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrFbcBlitterTracking);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrFbcCpuTracking);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrVEBOX);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT1_DT_DEVICE_F0_ID;
    drm->setGtType(GTTYPE_GT1);
    drm->StoredSSVal = 3;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ((unsigned short)drm->StoredDeviceID, outHwInfo.pPlatform->usDeviceID);
    EXPECT_EQ((unsigned short)drm->StoredDeviceRevID, outHwInfo.pPlatform->usRevId);
    EXPECT_EQ((uint32_t)drm->StoredEUVal, outHwInfo.pSysInfo->EUCount);
    EXPECT_EQ((uint32_t)drm->StoredSSVal, outHwInfo.pSysInfo->SubSliceCount);
    EXPECT_EQ(1u, outHwInfo.pSysInfo->SliceCount);

    EXPECT_EQ(GTTYPE_GT1, outHwInfo.pPlatform->eGTType);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrGT1);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1_5);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT2);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT3);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT4);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTA);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTC);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTX);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT1_5_DT_DEVICE_F0_ID;
    drm->setGtType(GTTYPE_GT1_5);
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ((unsigned short)drm->StoredDeviceID, outHwInfo.pPlatform->usDeviceID);
    EXPECT_EQ((unsigned short)drm->StoredDeviceRevID, outHwInfo.pPlatform->usRevId);
    EXPECT_EQ((uint32_t)drm->StoredEUVal, outHwInfo.pSysInfo->EUCount);
    EXPECT_EQ((uint32_t)drm->StoredSSVal, outHwInfo.pSysInfo->SubSliceCount);

    EXPECT_EQ(GTTYPE_GT1_5, outHwInfo.pPlatform->eGTType);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrGT1_5);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT2);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT3);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT4);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTA);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTC);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTX);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT3_DESK_DEVICE_F0_ID;
    drm->setGtType(GTTYPE_GT3);
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ((unsigned short)drm->StoredDeviceID, outHwInfo.pPlatform->usDeviceID);
    EXPECT_EQ((unsigned short)drm->StoredDeviceRevID, outHwInfo.pPlatform->usRevId);
    EXPECT_EQ((uint32_t)drm->StoredEUVal, outHwInfo.pSysInfo->EUCount);
    EXPECT_EQ((uint32_t)drm->StoredSSVal, outHwInfo.pSysInfo->SubSliceCount);

    EXPECT_EQ(GTTYPE_GT3, outHwInfo.pPlatform->eGTType);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1_5);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT2);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrGT3);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT4);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTA);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTC);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTX);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT4_DESK_DEVICE_F0_ID;
    drm->setGtType(GTTYPE_GT4);
    drm->StoredSSVal = 6;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ((unsigned short)drm->StoredDeviceID, outHwInfo.pPlatform->usDeviceID);
    EXPECT_EQ((unsigned short)drm->StoredDeviceRevID, outHwInfo.pPlatform->usRevId);
    EXPECT_EQ((uint32_t)drm->StoredEUVal, outHwInfo.pSysInfo->EUCount);
    EXPECT_EQ((uint32_t)drm->StoredSSVal, outHwInfo.pSysInfo->SubSliceCount);
    EXPECT_EQ(2u, outHwInfo.pSysInfo->SliceCount);

    EXPECT_EQ(GTTYPE_GT4, outHwInfo.pPlatform->eGTType);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT1_5);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT2);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGT3);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrGT4);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTA);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTC);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrGTX);
}

SKLTEST_F(HwInfoConfigTestLinuxSkl, negativeUnknownDevId) {
    drm->StoredDeviceID = 0;
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(-1, ret);
}

SKLTEST_F(HwInfoConfigTestLinuxSkl, negativeFailedIoctlDevId) {
    drm->StoredRetValForDeviceID = -2;
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(-2, ret);
}

SKLTEST_F(HwInfoConfigTestLinuxSkl, negativeFailedIoctlDevRevId) {
    drm->StoredRetValForDeviceRevID = -3;
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(-3, ret);
}

SKLTEST_F(HwInfoConfigTestLinuxSkl, negativeFailedIoctlEuCount) {
    drm->StoredRetValForEUVal = -4;
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(-4, ret);
}

SKLTEST_F(HwInfoConfigTestLinuxSkl, negativeFailedIoctlSsCount) {
    drm->StoredRetValForSSVal = -5;
    auto hwInfoConfig = HwInfoConfig::get(productFamily);
    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(-5, ret);
}

SKLTEST_F(HwInfoConfigTestLinuxSkl, configureHwInfoWaFlags) {
    auto hwInfoConfig = HwInfoConfig::get(productFamily);

    drm->StoredDeviceRevID = 1;
    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waCompressedResourceRequiresConstVA21);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waModifyVFEStateAfterGPGPUPreemption);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waDisablePerCtxtPreemptionGranularityControl);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waCSRUncachable);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waSamplerCacheFlushBetweenRedescribedSurfaceReads);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceRevID = 0;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waCompressedResourceRequiresConstVA21);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waModifyVFEStateAfterGPGPUPreemption);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waDisablePerCtxtPreemptionGranularityControl);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waCSRUncachable);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waSamplerCacheFlushBetweenRedescribedSurfaceReads);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceRevID = 5;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waCompressedResourceRequiresConstVA21);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waModifyVFEStateAfterGPGPUPreemption);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waDisablePerCtxtPreemptionGranularityControl);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waCSRUncachable);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waSamplerCacheFlushBetweenRedescribedSurfaceReads);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceRevID = 6;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waCompressedResourceRequiresConstVA21);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waModifyVFEStateAfterGPGPUPreemption);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waDisablePerCtxtPreemptionGranularityControl);
    EXPECT_EQ(0u, outHwInfo.pWaTable->waCSRUncachable);
    EXPECT_EQ(1u, outHwInfo.pWaTable->waSamplerCacheFlushBetweenRedescribedSurfaceReads);
}

SKLTEST_F(HwInfoConfigTestLinuxSkl, configureHwInfoEdram) {
    auto hwInfoConfig = HwInfoConfig::get(productFamily);

    int ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ_VAL(0u, outHwInfo.pSysInfo->EdramSizeInKb);
    EXPECT_EQ(0u, outHwInfo.pSkuTable->ftrEDram);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT3e_ULT_DEVICE_F0_ID_540;
    drm->setGtType(GTTYPE_GT3);
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ_VAL((64u * 1024u), outHwInfo.pSysInfo->EdramSizeInKb);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrEDram);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT3e_ULT_DEVICE_F0_ID_550;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ_VAL((64u * 1024u), outHwInfo.pSysInfo->EdramSizeInKb);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrEDram);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT3_MEDIA_SERV_DEVICE_F0_ID;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ_VAL((64u * 1024u), outHwInfo.pSysInfo->EdramSizeInKb);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrEDram);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT4_HALO_MOBL_DEVICE_F0_ID;
    drm->setGtType(GTTYPE_GT4);
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ_VAL((128u * 1024u), outHwInfo.pSysInfo->EdramSizeInKb);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrEDram);

    ReleaseOutHwInfoStructs();

    drm->StoredDeviceID = ISKL_GT4_WRK_DEVICE_F0_ID;
    ret = hwInfoConfig->configureHwInfo(pInHwInfo, &outHwInfo, osInterface);
    EXPECT_EQ(0, ret);
    EXPECT_EQ_VAL((128u * 1024u), outHwInfo.pSysInfo->EdramSizeInKb);
    EXPECT_EQ(1u, outHwInfo.pSkuTable->ftrEDram);
}

template <typename T>
class SklHwInfoTests : public ::testing::Test {
};
typedef ::testing::Types<SKL_1x2x6, SKL_1x3x6, SKL_1x3x8, SKL_2x3x8, SKL_3x3x8> sklTestTypes;
TYPED_TEST_CASE(SklHwInfoTests, sklTestTypes);
TYPED_TEST(SklHwInfoTests, gtSetupIsCorrect) {
    GT_SYSTEM_INFO gtSystemInfo;
    memset(&gtSystemInfo, 0, sizeof(gtSystemInfo));
    TypeParam::setupGtSystemInfo(&gtSystemInfo);
    EXPECT_GT(gtSystemInfo.EUCount, 0u);
    EXPECT_GT(gtSystemInfo.ThreadCount, 0u);
    EXPECT_GT(gtSystemInfo.SliceCount, 0u);
    EXPECT_GT(gtSystemInfo.SubSliceCount, 0u);
    EXPECT_GT_VAL(gtSystemInfo.L3CacheSizeInKb, 0u);
    EXPECT_EQ(gtSystemInfo.CsrSizeInMb, 8u);
    EXPECT_FALSE(gtSystemInfo.IsDynamicallyPopulated);
}

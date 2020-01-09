/*
 * Copyright (C) 2019 The LineageOS Project
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

#include "SunlightEnhancement.h"

#include <android-base/file.h>
#include <android-base/strings.h>

#include <fstream>

using android::base::ReadFileToString;
using android::base::Trim;
using android::base::WriteStringToFile;

namespace vendor {
namespace mokee {
namespace livedisplay {
namespace V2_0 {
namespace sysfs {

bool SunlightEnhancement::isSupported() {
    std::fstream hbm(FILE_HBM, hbm.in | hbm.out);
    std::fstream sre(FILE_SRE, sre.in | sre.out);

    if (hbm.good()) {
        mFile = FILE_HBM;
        mEnabledMode = 1;
    } else if (sre.good()) {
        mFile = FILE_SRE;
        mEnabledMode = 2;
    }

    return !mFile.empty();
}

// Methods from ::vendor::mokee::livedisplay::V2_0::ISunlightEnhancement follow.
Return<bool> SunlightEnhancement::isEnabled() {
    std::string tmp;
    int32_t contents = 0;

    if (ReadFileToString(mFile, &tmp)) {
        contents = std::stoi(Trim(tmp));
    }

    return contents > 0;
}

Return<bool> SunlightEnhancement::setEnabled(bool enabled) {
    return WriteStringToFile(enabled ? std::to_string(mEnabledMode) : "0", mFile, true);
}

}  // namespace sysfs
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace mokee
}  // namespace vendor

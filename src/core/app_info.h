#pragma once

#include <QString>

namespace rclock::core {

inline constexpr int kAppMajorVersion = 1;
inline constexpr int kAppMinorVersion = 0;
extern const int kAppBuildId;
extern const QString kAppPrintableBuildId;
extern const QString kAppFullVersion;
extern const QString kAppBuildCommitHash;
extern const QString kAppReleaseTag;

}  // namespace rclock::core

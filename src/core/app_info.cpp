#include "core/app_info.h"

#define STRINGIFY(x) #x
#define MACRO_TO_STRING(x) STRINGIFY(x)

namespace rclock::core {

int calculateBuildId() {
  const QString string_id = MACRO_TO_STRING(CI_BUILD_ID);
  bool conversion_ok;
  uint version = string_id.toInt(&conversion_ok);
  return conversion_ok ? 0 : version;
}
const int kAppBuildId = calculateBuildId();

QString calculatePrintableBuildId() {
  const QString string_id       = MACRO_TO_STRING(CI_BUILD_ID);
  const QString numeric_version = QString::number(kAppBuildId);
  if (string_id != numeric_version) return string_id + "/" + numeric_version;
  return string_id;
}
const QString kAppPrintableBuildId = calculatePrintableBuildId();

const QString kAppBuildCommitHash  = MACRO_TO_STRING(CI_COMMIT_HASH);
const QString kAppFullVersion = QString::number(kAppMajorVersion) + "." +
                                QString::number(kAppMinorVersion) + "." +
                                kAppPrintableBuildId + " from " +
                                kAppBuildCommitHash.left(8);

}  // namespace rclock::core

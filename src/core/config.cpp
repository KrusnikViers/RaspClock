#include "core/config.h"

namespace rclock::core {

namespace {

const QString kSettingsFileName = "settings.ini";

const QString kGeoDataPrefix = "geo";
const QString kLatitudeName  = "latitude";
const QString kLongitudeName = "longitude";

constexpr bool isDebug() {
#ifndef NDEBUG
  return true;
#else
  return false
#endif
}

QDir storage_directory() {
  if (isDebug()) return QDir::current();
  QDir data_directory(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
  if (!data_directory.exists()) data_directory.mkpath(".");
  return data_directory;
}

}  // namespace

Config::Config()
    : QObject(nullptr),
      settings_(storage_directory().absoluteFilePath(kSettingsFileName),
                QSettings::IniFormat, this) {}

void Config::readData() {
  settings_.beginGroup(kGeoDataPrefix);
  if (settings_.contains(kLatitudeName) && settings_.contains(kLongitudeName)) {
    cached_settings_.geo_data = std::make_unique<GeoCoordinates>();
    cached_settings_.geo_data->latitude =
        settings_.value(kLatitudeName).toDouble();
    cached_settings_.geo_data->latitude =
        settings_.value(kLongitudeName).toDouble();
  }
  settings_.endGroup();
}

}  // namespace rclock::core

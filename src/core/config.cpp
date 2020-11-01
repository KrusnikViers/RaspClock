#include "core/config.h"

namespace rclock::core {

namespace {

const QString kSettingsFileName = "settings.ini";

const QString kGeoDataPrefix     = "geo";
const QString kGeoServiceKeyName = "geoservice_key";
const QString kLatitudeName      = "latitude";
const QString kLongitudeName     = "longitude";

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
                QSettings::IniFormat, this) {
    readData();
}

void Config::set(AppSettings new_settings) {
  cached_settings_ = std::move(new_settings);
  writeData();
}

void Config::readData() {
  qDebug() << "Reading settings from " << storage_directory().path();
  settings_.beginGroup(kGeoDataPrefix);
  cached_settings_.geo_service_key =
      settings_.value(kGeoServiceKeyName, QString()).toString();
  cached_settings_.coordinates.latitude =
      settings_.value(kLatitudeName, 0.0).toDouble();
  cached_settings_.coordinates.longitude =
      settings_.value(kLongitudeName, 0.0).toDouble();
  settings_.endGroup();

  // Migrate settings, if anything changed in binary.
  settings_.clear();
  writeData();
}

void Config::writeData() {
  qDebug() << "Writing settings to " << storage_directory().path();
  settings_.beginGroup(kGeoDataPrefix);
  settings_.setValue(kGeoServiceKeyName, cached_settings_.geo_service_key);
  settings_.setValue(kLatitudeName, cached_settings_.coordinates.latitude);
  settings_.setValue(kLongitudeName, cached_settings_.coordinates.longitude);
  settings_.endGroup();
}

}  // namespace rclock::core

#include "core/config.h"

#include <QDir>

#define STRINGIFY(x) #x
#define MACRO_TO_STRING(x) STRINGIFY(x)

namespace rclock::core {

namespace {

const QString kLatitudeName           = "latitude";
const QString kLongitudeName          = "longitude";
const QString kTimeZoneServiceKeyName = "time_zone_key";

QSettings createSettingsIO() {
#ifndef NDEBUG
  QDir settings_directory = QDir::current();
#else
  QDir settings_directory(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
  if (!settings_directory.exists()) data_directory.mkpath(".");
#endif
  return QSettings(settings_directory.absoluteFilePath("settings.ini"),
                   QSettings::IniFormat);
}

}  // namespace

QString appVersion() {
    return QString("1.0.") + MACRO_TO_STRING(CI_BUILD_ID);
}

Config::Config() { readData(); }

void Config::set(AppSettings new_settings) {
  cached_settings_ = std::move(new_settings);
  writeData();
}

void Config::readData() {
  QSettings settings_io = createSettingsIO();
  qDebug() << "Reading settings from " << settings_io.fileName();
  cached_settings_.coordinates.latitude =
      settings_io.value(kLatitudeName, 0.0).toDouble();
  cached_settings_.coordinates.longitude =
      settings_io.value(kLongitudeName, 0.0).toDouble();
  cached_settings_.time_zone_service_key =
      settings_io.value(kTimeZoneServiceKeyName, QString()).toString();

  // Migrate settings, if anything changed in binary.
  writeData();
}

void Config::writeData() {
  QSettings settings_io = createSettingsIO();
  qDebug() << "Writing settings to " << settings_io.fileName();
  settings_io.setValue(kLatitudeName, cached_settings_.coordinates.latitude);
  settings_io.setValue(kLongitudeName, cached_settings_.coordinates.longitude);
  settings_io.setValue(kTimeZoneServiceKeyName,
                       cached_settings_.time_zone_service_key);
}

}  // namespace rclock::core

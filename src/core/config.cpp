#include "core/config.h"

#include <QDir>

namespace rclock::core {

namespace {

const QString kLatitudeName           = "latitude";
const QString kLongitudeName          = "longitude";
const QString kTimeZoneServiceKeyName = "time_zone_key";
const QString kAutoupdatesKeyName     = "autoupdate";

QSettings createSettingsIO() {
#ifndef NDEBUG
  QDir settings_directory = QDir::current();
#else
  QDir settings_directory(
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
  if (!settings_directory.exists()) settings_directory.mkpath(".");
#endif
  return QSettings(settings_directory.absoluteFilePath("settings.ini"),
                   QSettings::IniFormat);
}

}  // namespace

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
  cached_settings_.is_autoupdates_enabled =
      settings_io.value(kAutoupdatesKeyName, false).toBool();

  // Migrate settings, if anything changed in binary.
  writeData();
}

void Config::writeData() {
  QSettings settings_io = createSettingsIO();
  qDebug() << "Writing settings to " << settings_io.fileName();
  settings_io.clear();
  settings_io.setValue(kLatitudeName, cached_settings_.coordinates.latitude);
  settings_io.setValue(kLongitudeName, cached_settings_.coordinates.longitude);
  settings_io.setValue(kTimeZoneServiceKeyName,
                       cached_settings_.time_zone_service_key);
  settings_io.setValue(kAutoupdatesKeyName,
                       cached_settings_.is_autoupdates_enabled);
}

}  // namespace rclock::core

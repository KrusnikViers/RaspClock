#pragma once

#include <QtCore>

namespace rclock::core {

struct GeoCoordinates {
  double latitude  = 0.0;
  double longitude = 0.0;
};

struct AppSettings {
  QString time_zone_service_key;
  GeoCoordinates coordinates;
  bool is_autoupdates_enabled = false;
};

class Config : public QObject {
  Q_OBJECT
 public:
  Config();

  const AppSettings& get() const { return cached_settings_; }
  void set(AppSettings new_settings);

 private:
  void readData();
  void writeData();

  AppSettings cached_settings_;
};

}  // namespace rclock::core

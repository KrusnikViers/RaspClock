#pragma once

#include <QtCore>

namespace rclock::core {

struct GeoCoordinates {
  double latitude  = 0.0;
  double longitude = 0.0;
};

class Config : public QObject {
  Q_OBJECT
 public:
  Config();

  GeoCoordinates coordinates() const;

 private:
  void readData();

  struct CachedSettings {
    std::unique_ptr<GeoCoordinates> geo_data;
  } cached_settings_;

  QSettings settings_;
};

}  // namespace rclock::core

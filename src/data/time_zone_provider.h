#pragma once

#include <QObject>

#include "core/config.h"

namespace rclock::data {

class TimeZoneProvider : public QObject {
  Q_OBJECT
 public:
  explicit TimeZoneProvider(core::Config* config);

 public slots:
  void update();

 private:
  core::Config* config_;
};

}  // namespace rclock::data

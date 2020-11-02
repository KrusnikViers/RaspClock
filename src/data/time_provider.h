#pragma once

#include <QDateTime>
#include <QObject>

#include "core/config.h"
#include "core/main_timer.h"
#include "data/time_zone_provider.h"

namespace rclock::data {

class TimeProvider : public QObject {
  Q_OBJECT
 public:
  TimeProvider(core::Config* config, core::MainTimer* timer);

  QDateTime getDateTime() const;

private:
  TimeZoneProvider time_zone_provider_;
};

}  // namespace rclock::data

#include "data/time_provider.h"

namespace rclock::data {

TimeProvider::TimeProvider(core::Config* config, core::MainTimer* timer)
    : time_zone_provider_(config) {
  connect(timer, &core::MainTimer::updateTimeZone,  //
          &time_zone_provider_, &TimeZoneProvider::update);
}

QDateTime TimeProvider::getDateTime() const {
  return QDateTime::currentDateTime();
}

}  // namespace rclock::data

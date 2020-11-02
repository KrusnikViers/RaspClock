#include "data/time_zone_provider.h"

namespace rclock::data {

TimeZoneProvider::TimeZoneProvider(core::Config* config) : config_(config) {}

void TimeZoneProvider::update() {}

}  // namespace rclock::data

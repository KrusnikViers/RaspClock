#include "core/main_timer.h"

#include "core/application_updater.h"

namespace rclock::core {

namespace {

const quint64 kSecondMsec    = 1000;
const quint64 kMinuteMsec    = 60 * kSecondMsec;
const quint64 kHourMsec      = 60 * kMinuteMsec;
const quint64 kOneshotPeriod = 0;

}  // namespace

MainTimer::MainTimer() {
  assert(monotonic_timer_.isMonotonic());
  monotonic_timer_.start();

  event_queue_.push({0, kSecondMsec, kUpdateClock});
  event_queue_.push({0, 4 * kHourMsec, kUpdateTimeZone});
  event_queue_.push({10, 24 * kHourMsec, kUpdateApplication});
  event_queue_.push({15 * kMinuteMsec, kOneshotPeriod, kCleanOldVersionDump});

  timer_.setSingleShot(true);
  connect(&timer_, &QTimer::timeout, this, &MainTimer::onTimer);
  onTimer();
}

void MainTimer::onTimer() {
  for (;;) {
    assert(!event_queue_.empty());
    const quint64 current_timestamp = monotonic_timer_.elapsed();
    if (current_timestamp >= event_queue_.top().timestamp_msec) {
      PlannedEvent event = event_queue_.top();
      event_queue_.pop();
      if (event.period_msec != kOneshotPeriod) {
        event.timestamp_msec += event.period_msec;
        event_queue_.push(event);
      }

      processTimerEvent(event.event_type);
    } else {
      timer_.start(event_queue_.top().timestamp_msec - current_timestamp);
      return;
    }
  }
}

void MainTimer::processTimerEvent(TimerEventType type) {
  switch (type) {
    case kUpdateClock:
      emit(updateClock());
      return;
    case kUpdateTimeZone:
      emit(updateTimeZone());
      return;
    case kUpdateApplication:
      emit(updateApp());
      return;
    case kCleanOldVersionDump:
      cleanOldCopy();
      return;
    default:
      assert(false);
  }
}

}  // namespace rclock::core

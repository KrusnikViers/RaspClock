#include "core/main_timer.h"

namespace rclock::core {

namespace {

const quint64 kSecondMsec = 1000;
const quint64 kHourMsec   = 3600 * kSecondMsec;

}  // namespace

MainTimer::MainTimer() {
  assert(monotonic_timer_.isMonotonic());
  monotonic_timer_.start();

  event_queue_.push({0, kSecondMsec, kUpdateClock});
  event_queue_.push({0, 4 * kHourMsec, kUpdateTimeZone});

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
      event.timestamp_msec += event.period_msec;
      event_queue_.push(event);

      emitSignal(event.signal_type);
    } else {
      timer_.start(event_queue_.top().timestamp_msec - current_timestamp);
      return;
    }
  }
}

void MainTimer::emitSignal(SignalType type) {
  switch (type) {
    case kUpdateClock:
      emit(updateClock());
      return;
    case kUpdateTimeZone:
      emit(updateTimeZone());
      return;
    default:
      assert(false);
  }
}

}  // namespace rclock::core

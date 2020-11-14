#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QTimer>
#include <queue>

namespace rclock::core {

class MainTimer : public QObject {
  Q_OBJECT
 public:
  MainTimer();

 signals:
  void updateClock();
  void updateTimeZone();
  void updateApp();

 private slots:
  void onTimer();

 private:
  enum TimerEventType {
      // Periodic signals.
      kUpdateClock = 0,
      kUpdateTimeZone,
      kUpdateApplication,

      // Oneshot events.
      kCleanOldVersionDump,
  };

  struct PlannedEvent {
    quint64 timestamp_msec;
    // If period is 0, event will not be resheduled.
    quint64 period_msec;
    TimerEventType event_type;

    bool operator>(const PlannedEvent& other) const {
      return timestamp_msec > other.timestamp_msec;
    }
  };

  void processTimerEvent(TimerEventType type);

  QTimer timer_;
  QElapsedTimer monotonic_timer_;
  std::priority_queue<PlannedEvent, std::vector<PlannedEvent>,
                      std::greater<PlannedEvent>>
      event_queue_;
};

}  // namespace rclock::core

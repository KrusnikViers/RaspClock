#pragma once

#include <QtCore>

namespace rclock::core {

class LogsDisplayInterface {
 public:
  virtual void addLogsRecord(QString logs_record) = 0;
};

void startLogsRecording();

void displayLogsOn(LogsDisplayInterface* display);
void stopLogsDisplay();

}  // namespace rclock::core

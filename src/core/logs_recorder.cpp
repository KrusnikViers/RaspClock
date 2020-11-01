#include "core/logs_recorder.h"

namespace rclock::core {

namespace {

// Weak reference, must be owned by the main() function.
LogsDisplayInterface* current_display = nullptr;
QStringList startup_logs;

void LogsRedirection(QtMsgType /*type*/, const QMessageLogContext& context,
                     const QString& msg) {
  QString full_message = QString(context.category) + ": " + msg;
  if (current_display) {
    current_display->addLogsRecord(std::move(full_message));
  } else {
    startup_logs.append(std::move(full_message));
  }
}

}  // namespace

void startLogsRecording() { qInstallMessageHandler(LogsRedirection); }

void displayLogsOn(LogsDisplayInterface* display) {
  current_display = display;
  for (auto& record : startup_logs)
    current_display->addLogsRecord(std::move(record));
  startup_logs.clear();
}

void stopLogsDisplay() { current_display = nullptr; }

}  // namespace rclock::core

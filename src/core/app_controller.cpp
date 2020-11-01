#include "app_controller.h"

namespace rclock::core {

namespace {

// Weak reference, must be owned by the main() function.
AppController* current_instance = nullptr;
QStringList startup_logs;

void LogsRedirection(QtMsgType /*type*/, const QMessageLogContext& context,
                     const QString& msg) {
  QString full_message = QString(context.category) + " " +
                         QString(context.file) + ":" +
                         QString::number(context.line) + "   " + msg;
  if (current_instance) {
    current_instance->onLogRecord(std::move(full_message));
  } else {
    startup_logs.append(std::move(full_message));
  }
}

}  // namespace

AppController* AppController::instance() { return current_instance; }

void AppController::InitLogging() { qInstallMessageHandler(LogsRedirection); }

AppController::AppController() : main_window_(&config_) {
  assert(!current_instance);
  current_instance = this;

  for (auto& message : startup_logs) onLogRecord(std::move(message));
  startup_logs.clear();
}

AppController::~AppController() {
  assert(current_instance);
  current_instance = nullptr;
}

void AppController::onLogRecord(QString record) {
  main_window_.onLogRecord(record);
}

}  // namespace rclock::core

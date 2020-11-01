#include "ui/logs.h"

#include <QScrollBar>

namespace rclock::ui {

Logs::Logs() {
  ui_.setupUi(this);
  core::displayLogsOn(this);
}

Logs::~Logs() { core::stopLogsDisplay(); }

void Logs::addLogsRecord(QString record) {
  ui_.logs_display->appendPlainText(std::move(record));
  ui_.logs_display->verticalScrollBar()->setValue(
      ui_.logs_display->verticalScrollBar()->maximum());
}

}  // namespace rclock::ui

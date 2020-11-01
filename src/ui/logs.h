#pragma once

#include <QWidget>

#include "core/logs_recorder.h"
#include "ui/ui_logs.h"

namespace rclock::ui {

class Logs : public QWidget, public core::LogsDisplayInterface {
  Q_OBJECT
 public:
  Logs();
  ~Logs();

  void addLogsRecord(QString record) override;

 private:
  Ui::Logs ui_;
};

}  // namespace rclock::ui

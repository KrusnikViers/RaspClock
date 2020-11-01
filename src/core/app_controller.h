#pragma once

#include <QObject>

#include "core/config.h"
#include "ui/main_window.h"

namespace rclock::core {

class AppController : public QObject {
  Q_OBJECT
 public:
  static AppController* instance();
  static void InitLogging();

  AppController();
  ~AppController();

  void onLogRecord(QString record);

 private:
  Config config_;

  ui::MainWindow main_window_;
};

}  // namespace rclock::core

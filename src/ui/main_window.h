#pragma once

#include <QMainWindow>

#include "core/config.h"
#include "ui/ui_main_window.h"

namespace rclock::ui {

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(core::Config* config);

  void onLogRecord(QString log_line);

 private slots:
  void onSwitchButtonClicked(QWidget* button, QWidget* page);
  void onUpdateSettingsButtonClicked();

 private:
  Ui::MainWindow ui_;
  core::Config* config_; // Weak reference
};  // class MainWindow

}  // namespace rclock::ui

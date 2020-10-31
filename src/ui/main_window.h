#pragma once

#include <QMainWindow>

#include "ui/ui_main_window.h"

namespace rclock::ui {

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  static MainWindow* instance();

  MainWindow();
  ~MainWindow();

  void onNewLogLine(const QString& log_line);

private slots:
  void onSwitchButtonClicked(QWidget* button, QWidget* page);

 private:
  Ui::MainWindow ui_;
};  // class MainWindow

}  // namespace rclock::ui

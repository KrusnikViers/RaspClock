#pragma once

#include <QMainWindow>

#include "core/config.h"
#include "data/time_provider.h"
#include "ui/display.h"
#include "ui/logs.h"
#include "ui/settings.h"
#include "ui/ui_main_window.h"

namespace rclock::ui {

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(core::Config* config, core::MainTimer* main_timer, data::TimeProvider* time_provider);

 private slots:
  void onSwitchButtonClicked(QWidget* button, QWidget* page);
  void onFullscreenSwitch();
  void onExit();

 private:
  Ui::MainWindow ui_;

  // Weak references, owned by stacked widget in |ui_|.
  Display* display_page_   = nullptr;
  Settings* settings_page_ = nullptr;
  Logs* logs_page_         = nullptr;
};

}  // namespace rclock::ui

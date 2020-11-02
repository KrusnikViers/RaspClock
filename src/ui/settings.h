#pragma once

#include <QWidget>

#include "core/config.h"
#include "core/main_timer.h"
#include "ui/ui_settings.h"

namespace rclock::ui {

class Settings : public QWidget {
  Q_OBJECT
 public:
  Settings(core::Config* config, core::MainTimer* main_timer);

 private slots:
  void onUpdateSettingsButtonClicked();

 private:
  Ui::Settings ui_;

  core::Config* config_;
  core::MainTimer* main_timer_;
};

}  // namespace rclock::ui
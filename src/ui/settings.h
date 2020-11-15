#pragma once

#include <QWidget>

#include "core/app_updater.h"
#include "core/config.h"
#include "core/main_timer.h"
#include "data/time_provider.h"
#include "ui/ui_settings.h"

namespace rclock::ui {

class Settings : public QWidget {
  Q_OBJECT
 public:
  Settings(core::Config* config, core::MainTimer* main_timer,
           core::ApplicationUpdater* app_updater,
           data::TimeProvider* time_provider);

 signals:
  void fullscreenSwitchRequested();
  void exitRequested();

 private slots:
  void onUpdateSettingsButtonClicked();
  void onCheckUpdatesButtonClicked();

  void onUpdatesChecked(bool new_version);

 private:
  Ui::Settings ui_;

  core::Config* config_;
  core::MainTimer* main_timer_;
  core::ApplicationUpdater* app_updater_;
  data::TimeProvider* time_provider_;
};

}  // namespace rclock::ui

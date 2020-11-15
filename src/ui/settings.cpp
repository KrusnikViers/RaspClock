#include "ui/settings.h"

#include "core/application_updater.h"

namespace rclock::ui {

Settings::Settings(core::Config* config, core::MainTimer* main_timer,
                   core::ApplicationUpdater* app_updater,
                   data::TimeProvider* time_provider)
    : config_(config),
      main_timer_(main_timer),
      app_updater_(app_updater),
      time_provider_(time_provider) {
  ui_.setupUi(this);

  ui_.time_zone_service_key_edit->setText(config_->get().time_zone_service_key);
  ui_.latitude_edit->setValue(config_->get().coordinates.latitude);
  ui_.longitude_edit->setValue(config_->get().coordinates.longitude);
  connect(ui_.update_settings_button, &QPushButton::clicked,  //
          this, &Settings::onUpdateSettingsButtonClicked);

  connect(ui_.fullscreen_button, &QPushButton::clicked,  //
          this, &Settings::fullscreenSwitchRequested);
  connect(ui_.exit_button, &QPushButton::clicked,  //
          this, &Settings::exitRequested);

  ui_.version_group_box->setTitle("Current version: " + core::appVersion());
  connect(ui_.check_updates, &QPushButton::clicked,  //
          this, &Settings::onCheckUpdatesButtonClicked);
  connect(app_updater, &core::ApplicationUpdater::updatesChecked,  //
          this, &Settings::onUpdatesChecked);
}

void Settings::onUpdateSettingsButtonClicked() {
  core::AppSettings new_settings;
  new_settings.time_zone_service_key = ui_.time_zone_service_key_edit->text();
  new_settings.coordinates.latitude  = ui_.latitude_edit->value();
  new_settings.coordinates.longitude = ui_.longitude_edit->value();
  config_->set(std::move(new_settings));
}

void Settings::onCheckUpdatesButtonClicked() { app_updater_->initiateUpdate(); }

void Settings::onUpdatesChecked() {
  ui_.version_check_time->setText(
      "Last check: " +
      time_provider_->getDateTime().toString("dd.MM hh:mm:ss"));
}

}  // namespace rclock::ui

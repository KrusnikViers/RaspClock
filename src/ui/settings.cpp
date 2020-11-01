#include "ui/settings.h"

namespace rclock::ui {

Settings::Settings(core::Config* config) : config_(config) {
  ui_.setupUi(this);

  ui_.time_zone_service_key_edit->setText(config_->get().time_zone_service_key);
  ui_.latitude_edit->setValue(config_->get().coordinates.latitude);
  ui_.longitude_edit->setValue(config_->get().coordinates.longitude);
  QObject::connect(ui_.update_settings_button, &QPushButton::clicked, this,
                   &Settings::onUpdateSettingsButtonClicked);
}

void Settings::onUpdateSettingsButtonClicked() {
  core::AppSettings new_settings;
  new_settings.time_zone_service_key = ui_.time_zone_service_key_edit->text();
  new_settings.coordinates.latitude  = ui_.latitude_edit->value();
  new_settings.coordinates.longitude = ui_.longitude_edit->value();
  config_->set(std::move(new_settings));
}

}  // namespace rclock::ui

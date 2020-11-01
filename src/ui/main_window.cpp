#include "ui/main_window.h"

#include <QtWidgets>

#include "ui/widget_ratio_filter.h"

namespace rclock::ui {

MainWindow::MainWindow(core::Config* config)
    : config_(config) {
  ui_.setupUi(this);

  for (QObject* child : ui_.sidebar_layout->children()) {
    if (child->metaObject()->className() ==
        QPushButton::staticMetaObject.className()) {
      child->installEventFilter(&WidgetRatioFilter::instance());
    }
  }

  QObject::connect(ui_.clock_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.clock_button, ui_.clock_page);
  });
  QObject::connect(ui_.settings_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.settings_button, ui_.settings_page);
  });
  QObject::connect(ui_.logs_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.logs_button, ui_.logs_page);
  });
  onSwitchButtonClicked(ui_.clock_button, ui_.clock_page);

  ui_.time_zone_service_key_edit->setText(config_->get().geo_service_key);
  ui_.latitude_edit->setValue(config_->get().coordinates.latitude);
  ui_.longitude_edit->setValue(config_->get().coordinates.longitude);
  QObject::connect(ui_.update_settings_button, &QPushButton::clicked, this,
                   &MainWindow::onUpdateSettingsButtonClicked);

  this->show();
}

void MainWindow::onLogRecord(QString log_line) {
  ui_.logs_window->appendPlainText(std::move(log_line));
  ui_.logs_window->verticalScrollBar()->setValue(
      ui_.logs_window->verticalScrollBar()->maximum());
}

void MainWindow::onSwitchButtonClicked(QWidget* button, QWidget* page) {
  ui_.stacked_widget->setCurrentWidget(page);
  ui_.clock_button->setVisible(ui_.clock_button != button);
  ui_.settings_button->setVisible(ui_.settings_button != button);
  ui_.logs_button->setVisible(ui_.logs_button != button);
}

void MainWindow::onUpdateSettingsButtonClicked() {
  core::AppSettings new_settings;
  new_settings.geo_service_key       = ui_.time_zone_service_key_edit->text();
  new_settings.coordinates.latitude  = ui_.latitude_edit->value();
  new_settings.coordinates.longitude = ui_.longitude_edit->value();
  config_->set(std::move(new_settings));
}

}  // namespace rclock::ui

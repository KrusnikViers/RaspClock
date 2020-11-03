#include "ui/main_window.h"

#include <QtWidgets>

#include "ui/util/widget_ratio_filter.h"

namespace rclock::ui {

MainWindow::MainWindow(core::Config* config, core::MainTimer* main_timer,
                       data::TimeProvider* time_provider) {
  ui_.setupUi(this);

  for (QObject* child : ui_.sidebar_layout->children()) {
    if (child->metaObject()->className() ==
        QPushButton::staticMetaObject.className()) {
      child->installEventFilter(&WidgetRatioFilter::instance());
    }
  }

  display_page_ = new Display(main_timer, time_provider);
  ui_.stacked_widget->addWidget(display_page_);
  connect(ui_.clock_button, &QPushButton::clicked,
          [this] { onSwitchButtonClicked(ui_.clock_button, display_page_); });

  settings_page_ = new Settings(config, main_timer);
  ui_.stacked_widget->addWidget(settings_page_);
  connect(ui_.settings_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.settings_button, settings_page_);
  });
  connect(settings_page_, &Settings::fullscreenSwitchRequested,  //
          this, &MainWindow::onFullscreenSwitch);
  connect(settings_page_, &Settings::exitRequested,  //
          this, &MainWindow::onExit);

  logs_page_ = new Logs();
  ui_.stacked_widget->addWidget(logs_page_);
  connect(ui_.logs_button, &QPushButton::clicked,
          [this] { onSwitchButtonClicked(ui_.logs_button, logs_page_); });

  onSwitchButtonClicked(ui_.clock_button, display_page_);
  this->show();
}

void MainWindow::onSwitchButtonClicked(QWidget* button, QWidget* page) {
  ui_.stacked_widget->setCurrentWidget(page);
  ui_.clock_button->setVisible(ui_.clock_button != button);
  ui_.settings_button->setVisible(ui_.settings_button != button);
  ui_.logs_button->setVisible(ui_.logs_button != button);
}

void MainWindow::onFullscreenSwitch() {
  this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
}

void MainWindow::onExit() { QApplication::quit(); }

}  // namespace rclock::ui

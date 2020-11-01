#include "ui/main_window.h"

#include <QtWidgets>

#include "ui/util/widget_ratio_filter.h"

namespace rclock::ui {

MainWindow::MainWindow(core::Config* config) {
  ui_.setupUi(this);

  for (QObject* child : ui_.sidebar_layout->children()) {
    if (child->metaObject()->className() ==
        QPushButton::staticMetaObject.className()) {
      child->installEventFilter(&WidgetRatioFilter::instance());
    }
  }

  display_page_ = new Display();
  ui_.stacked_widget->addWidget(display_page_);
  QObject::connect(ui_.clock_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.clock_button, display_page_);
  });

  settings_page_ = new Settings(config);
  ui_.stacked_widget->addWidget(settings_page_);
  QObject::connect(ui_.settings_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.settings_button, settings_page_);
  });

  logs_page_ = new Logs();
  ui_.stacked_widget->addWidget(logs_page_);
  QObject::connect(ui_.logs_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.logs_button, logs_page_);
  });

  this->show();
}

void MainWindow::onSwitchButtonClicked(QWidget* button, QWidget* page) {
  ui_.stacked_widget->setCurrentWidget(page);
  ui_.clock_button->setVisible(ui_.clock_button != button);
  ui_.settings_button->setVisible(ui_.settings_button != button);
  ui_.logs_button->setVisible(ui_.logs_button != button);
}

}  // namespace rclock::ui

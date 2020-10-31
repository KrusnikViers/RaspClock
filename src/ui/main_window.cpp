#include "ui/main_window.h"

#include <QtWidgets>

#include "ui/square_widget_event_filter.h"

namespace rclock::ui {

namespace {

// Weak reference, must be owned by the main() function.
MainWindow* current_instance = nullptr;

}  // namespace

MainWindow* MainWindow::instance() { return current_instance; }

MainWindow::MainWindow() {
  ui_.setupUi(this);

  QObject::connect(ui_.clock_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.clock_button, ui_.clock_page);
  });
  ui_.clock_button->installEventFilter(&SquareWidgetEventFilter::instance());
  QObject::connect(ui_.settings_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.settings_button, ui_.settings_page);
  });
  ui_.settings_button->installEventFilter(&SquareWidgetEventFilter::instance());
  QObject::connect(ui_.logs_button, &QPushButton::clicked, [this] {
    onSwitchButtonClicked(ui_.logs_button, ui_.logs_page);
  });
  ui_.logs_button->installEventFilter(&SquareWidgetEventFilter::instance());
  onSwitchButtonClicked(ui_.clock_button, ui_.clock_page);

  assert(!current_instance);
  current_instance = this;
}

MainWindow::~MainWindow() {
  assert(current_instance);
  current_instance = nullptr;
}

void MainWindow::onNewLogLine(const QString& log_line) {
  ui_.logs_window->appendPlainText(log_line);
  ui_.logs_window->verticalScrollBar()->setValue(
      ui_.logs_window->verticalScrollBar()->maximum());
}

void MainWindow::onSwitchButtonClicked(QWidget* button, QWidget* page) {
  ui_.stacked_widget->setCurrentWidget(page);
  ui_.clock_button->setVisible(ui_.clock_button != button);
  ui_.settings_button->setVisible(ui_.settings_button != button);
  ui_.logs_button->setVisible(ui_.logs_button != button);
}

}  // namespace rclock::ui

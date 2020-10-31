#include "square_widget_event_filter.h"

#include <QDebug>
#include <QEvent>
#include <QPushButton>
#include <QResizeEvent>

namespace rclock::ui {

namespace {
SquareWidgetEventFilter single_instance;
}

SquareWidgetEventFilter& SquareWidgetEventFilter::instance() {
  return single_instance;
}

bool SquareWidgetEventFilter::eventFilter(QObject* object, QEvent* event) {
  // Resizing window will update only buttons width, so we should adjust
  // height manually to the same value.
  if (event->type() == QEvent::Resize) {
    QResizeEvent* resize_event = static_cast<QResizeEvent*>(event);
    if (resize_event->size().width() != resize_event->size().height()) {
      QWidget* widget = static_cast<QWidget*>(object);
      widget->setFixedHeight(resize_event->size().width());
    }
  }
  return QObject::eventFilter(object, event);
}

}  // namespace rclock::ui

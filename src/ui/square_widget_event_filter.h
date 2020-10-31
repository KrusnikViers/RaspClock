#pragma once

#include <QObject>

namespace rclock::ui {

class SquareWidgetEventFilter : public QObject {
  Q_OBJECT
 public:
  static SquareWidgetEventFilter& instance();

 protected:
  bool eventFilter(QObject* object, QEvent* event) override;
};

}  // namespace rclock::ui

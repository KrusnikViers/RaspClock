#pragma once

#include <QObject>

namespace rclock::ui {

class WidgetRatioFilter : public QObject {
  Q_OBJECT
 public:
  static WidgetRatioFilter& instance();

 protected:
  bool eventFilter(QObject* object, QEvent* event) override;
};

}  // namespace rclock::ui

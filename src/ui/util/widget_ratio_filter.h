#pragma once

#include <QObject>

namespace rclock::ui {

class WidgetRatioFilter : public QObject {
 public:
  static WidgetRatioFilter& instance();

 protected:
  bool eventFilter(QObject* object, QEvent* event) override;
};

}  // namespace rclock::ui

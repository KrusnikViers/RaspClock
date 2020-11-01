#pragma once

#include <QWidget>

#include "ui/ui_display.h"

namespace rclock::ui {

class Display : public QWidget {
  Q_OBJECT
 public:
  Display();

 private:
  Ui::Display ui_;
};

}  // namespace rclock::ui

#pragma once

#include <QWidget>

#include "core/main_timer.h"
#include "data/time_provider.h"
#include "ui/ui_display.h"

namespace rclock::ui {

class Display : public QWidget {
  Q_OBJECT
 public:
  Display(core::MainTimer* main_timer, data::TimeProvider* time_provider);

 private slots:
  void updateClock();

 private:
  Ui::Display ui_;
  QLocale locale_ = QLocale(QLocale::English);
  QRect calculated_geometry_;

  data::TimeProvider* time_provider_;
};

}  // namespace rclock::ui

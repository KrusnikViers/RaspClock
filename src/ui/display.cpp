#include "ui/display.h"

#include "ui/util/text_helpers.h"

namespace rclock::ui {

Display::Display(core::MainTimer* main_timer, data::TimeProvider* time_provider)
    : time_provider_(time_provider) {
  ui_.setupUi(this);
  connect(main_timer, &core::MainTimer::updateClock,  //
          this, &Display::updateClock);

  updateClock();
}

void Display::updateClock() {
  if (this->geometry() != calculated_geometry_) {
    fitTextSize(ui_.time_label, 1.5);
    fitTextSize(ui_.day_of_week_label, 0.5);
    fitTextSize(ui_.date_label, 0.25);
    calculated_geometry_ = this->geometry();
  }

  QDateTime current_datetime = time_provider_->getDateTime();

  const QString time    = locale_.toString(current_datetime, "hh:mm");
  const QString seconds = locale_.toString(current_datetime, "ss");
  const QString dow     = locale_.toString(current_datetime, "dddd");
  const QString date    = locale_.toString(current_datetime, "d MMMM yyyy");

  ui_.time_label->setText(
      time + "<span style='font-size:" +
      QString::number(ui_.time_label->font().pointSize() / 3) + "pt;'>" +
      seconds + "</span>");
  ui_.day_of_week_label->setText(dow);
  ui_.date_label->setText(date);
}

}  // namespace rclock::ui

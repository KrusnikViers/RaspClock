#include "ui/util/text_helpers.h"

#include <QtAlgorithms>

namespace rclock::ui {

namespace {}

void fitTextSize(QLabel* label, double width_part) {
  const QString kTestString = "ABCDEFGHI";
  const int expected_width  = label->size().width() * width_part;
  QFont font                = label->font();

  int min_font_size = 1;
  int max_font_size = 256;

  while (min_font_size + 1 != max_font_size) {
    int middle = (min_font_size + max_font_size) / 2;
    font.setPointSize(middle);
    QFontMetrics metrics(font);
    QRect rect = metrics.boundingRect(kTestString);
    if (rect.width() >= expected_width) {
      max_font_size = middle;
    } else {
      min_font_size = middle;
    }
  }

  font.setPointSize(min_font_size);
  label->setFont(font);
}

}  // namespace rclock::ui

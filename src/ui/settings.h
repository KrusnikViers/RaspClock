#pragma once

#include <QWidget>

#include "core/config.h"
#include "ui/ui_settings.h"

namespace rclock::ui {

class Settings : public QWidget {
  Q_OBJECT
 public:
  explicit Settings(core::Config* config);

 private slots:
  void onUpdateSettingsButtonClicked();

 private:
  Ui::Settings ui_;
  core::Config* config_;  // Weak reference
};

}  // namespace rclock::ui

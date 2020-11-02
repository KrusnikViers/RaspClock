#include <QtWidgets>

#include "core/config.h"
#include "core/logs_recorder.h"
#include "core/main_timer.h"
#include "data/time_provider.h"
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QApplication::addLibraryPath(QApplication::applicationDirPath());

  rclock::core::startLogsRecording();
  rclock::core::Config config;
  rclock::core::MainTimer main_timer;

  rclock::data::TimeProvider time_provider(&config, &main_timer);

  rclock::ui::MainWindow main_window(&config, &main_timer, &time_provider);

  return app.exec();
}

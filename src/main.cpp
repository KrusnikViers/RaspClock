#include <QtWidgets>

#include "core/application_updater.h"
#include "core/config.h"
#include "core/logs_recorder.h"
#include "core/main_timer.h"
#include "core/network.h"
#include "data/time_provider.h"
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QApplication::addLibraryPath(QApplication::applicationDirPath());

  rclock::core::startLogsRecording();
  rclock::core::Config config;
  rclock::core::MainTimer main_timer;
  rclock::core::NetworkRequestor network_requestor;

  rclock::core::ApplicationUpdater app_updater(&main_timer, &network_requestor);

  rclock::data::TimeProvider time_provider(&config, &main_timer);

  rclock::ui::MainWindow main_window(&config, &main_timer, &app_updater, &time_provider);

  return app.exec();
}

#include <QtWidgets>

#include "core/app_controller.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QApplication::addLibraryPath(QApplication::applicationDirPath());

  rclock::core::AppController::InitLogging();
  rclock::core::AppController app_controller;

  return app.exec();
}

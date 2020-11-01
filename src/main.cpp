#include <QtWidgets>

#include "core/config.h"
#include "ui/main_window.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QApplication::addLibraryPath(QApplication::applicationDirPath());

  rclock::core::Config config;
  rclock::ui::MainWindow main_window(&config);

  return app.exec();
}

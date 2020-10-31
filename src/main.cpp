#include <QtWidgets>

#include "ui/main_window.h"

namespace {

void LogsRedirection(QtMsgType /*type*/, const QMessageLogContext& context,
                     const QString& msg) {
  if (rclock::ui::MainWindow::instance()) {
    rclock::ui::MainWindow::instance()->onNewLogLine(
        QString(context.category) + " | " +                    //
        QString(context.file) + ":" + QString(context.line) +  //
        msg);
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QApplication::addLibraryPath(QApplication::applicationDirPath());

  rclock::ui::MainWindow main_window;
  main_window.show();
  qInstallMessageHandler(LogsRedirection);

  return app.exec();
}

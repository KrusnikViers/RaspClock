#include "core/application_updater.h"

#include <QApplication>
#include <QDebug>
#include <QWindow>
#include <QtCore>

namespace rclock::core {

namespace {

const QString kRunningCopyDir = "old_version";
const QString kNewBinaryDir   = "new_version";

}  // namespace

void cleanOldCopy() {
  QDir current_dir = QDir(QCoreApplication::applicationDirPath());
  assert(current_dir.exists());

  QDir running_copy_dir = current_dir;
  running_copy_dir.cd(kRunningCopyDir);
  if (!running_copy_dir.exists()) return;

  current_dir.rmdir(kRunningCopyDir);
  assert(!running_copy_dir.exists());
}

void restartAndUpdateApplication() {
  // Make sure, that existing dump of previous version was removed.
  cleanOldCopy();

  QFile current_binary(QCoreApplication::applicationFilePath());
  assert(current_binary.exists());
  QDir current_dir = QDir(QCoreApplication::applicationDirPath());
  assert(current_dir.exists());
  const QString app_binary_path = current_binary.fileName();

  // We should at least ensure, that new version already downloaded.
  QDir new_binary_dir = current_dir;
  current_dir.cd(kNewBinaryDir);
  assert(new_binary_dir.exists());

  // Create directory to dump current version of the application.
  if (!current_dir.mkdir(kRunningCopyDir))
    qDebug() << "Copy directory creation error in " << current_dir.path();
  QDir running_copy_dir = current_dir;
  running_copy_dir.cd(kRunningCopyDir);
  assert(running_copy_dir.exists());

  // Move current executable file in the dump directory. This action should be
  // available in both Windows and Linux OS.
  current_binary.rename(running_copy_dir.path() + QDir::separator() +
                        current_binary.fileName());

  // Move everything from existing binary dir to the current directory.
  // Expected, that binary name was not changed.
  for (const QString& entry : new_binary_dir.entryList(QDir::Files)) {
    QFile::rename(new_binary_dir.path() + QDir::separator() + entry,
                  current_dir.path() + QDir::separator() + entry);
  }
  current_dir.rmdir(kNewBinaryDir);

  // Start new binary and exit from the current one.
  for (QWindow* window : QApplication::allWindows()) window->hide();
  assert(QProcess::execute(app_binary_path, QStringList()));
  QApplication::quit();
}

}  // namespace rclock::core

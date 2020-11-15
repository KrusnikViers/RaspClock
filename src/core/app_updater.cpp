#include "core/app_updater.h"

#include <QApplication>
#include <QDebug>
#include <QJsonObject>
#include <QWindow>
#include <QtCore>

#include "core/app_info.h"

namespace rclock::core {

namespace {

const QString kOldVersionDir = "old_version";
const QString kNewVersionDir = "new_version";

const QString kReleaseTagPrefix = "ci-";
const QString kLatestReleaseMetadataUrl =
    "https://api.github.com/repos/KrusnikViers/RaspClock/releases/latest";

QString join(const QDir& dir, const QString& contents) {
  return dir.path() + QDir::separator() + contents;
}

void moveFileIfExists(const QDir& from, const QDir& to,
                      const QString& file_name) {
  if (!QFile::exists(join(from, file_name))) return;
  assert(QFile(join(from, file_name)).rename(join(to, file_name)));
}

void cleanOldCopy() {
  QDir old_version_dir(join(QDir::current(), kOldVersionDir));
  if (!old_version_dir.exists()) return;
  assert(old_version_dir.removeRecursively());
}

void restartAndUpdateApplication() {
  // Make sure, that existing dump of previous version was removed.
  cleanOldCopy();

  const QDir current_dir = QDir::current();
  const QString binary_name =
      QFileInfo(QCoreApplication::applicationFilePath()).fileName();

  // Create new directory to dump current version of the application.
  if (!current_dir.mkdir(kOldVersionDir))
    qDebug() << "Copy directory creation error in " << current_dir.path();
  const QDir old_version_dir = QDir(join(current_dir, kOldVersionDir));
  const QDir new_version_dir(join(current_dir, kNewVersionDir));

  // Move all existing files, that are going to be replaced, in the dump
  // directory, replacing them one by one. After that, directory for new version
  // should be empty and could be removed.
  for (const QString& file_name : new_version_dir.entryList(QDir::Files)) {
    moveFileIfExists(current_dir, old_version_dir, file_name);
    moveFileIfExists(new_version_dir, current_dir, file_name);
  }
  assert(current_dir.rmdir(kNewVersionDir));

  // Start new binary and exit from the current one.
  for (QWindow* window : QApplication::allWindows()) window->hide();
  assert(
      QProcess::startDetached(join(current_dir, binary_name), QStringList()));
  QApplication::quit();
}

}  // namespace

ApplicationUpdater::ApplicationUpdater(Config* config, MainTimer* main_timer,
                                       NetworkRequestor* requestor)
    : requestor_(requestor), config_(config) {
  connect(main_timer, &MainTimer::updateApp,  //
          this, &ApplicationUpdater::initiateUpdate);
  connect(requestor, &NetworkRequestor::getFinished,  //
          this, &ApplicationUpdater::onRequestFetched);
}

void ApplicationUpdater::initiateUpdate() {
  cleanOldCopy();
  requestor_->get(kLatestReleaseMetadata, kLatestReleaseMetadataUrl);
}

void ApplicationUpdater::onRequestFetched(RequestType type,
                                          RequestStatus status,
                                          const QString& data) {
  if (type == kLatestReleaseMetadata && status == kDone) {
    QJsonObject releases_metadata =
        QJsonDocument::fromJson(data.toUtf8()).object();
    const bool is_new_version =
        (kAppBuildCommitHash !=
         releases_metadata.value("target_commitish").toString());
    if (is_new_version) {
      qDebug() << "New release found: "
               << releases_metadata.value("target_commitish").toString()
               << " vs current " << kAppFullVersion;
    }
    if (is_new_version && config_->get().is_autoupdates_enabled) {
      // restartAndUpdateApplication();
    }
    emit(updatesChecked(is_new_version));
  }
}

}  // namespace rclock::core

#include "core/application_updater.h"

#include <QApplication>
#include <QDebug>
#include <QJsonObject>
#include <QWindow>
#include <QtCore>

#define STRINGIFY(x) #x
#define MACRO_TO_STRING(x) STRINGIFY(x)

namespace rclock::core {

namespace {

const QString kOldVersionDir = "old_version";
const QString kNewVersionDir = "new_version";

const QString kLatestReleaseMetadataUrl =
    "https://api.github.com/repos/KrusnikViers/RaspClock/releases/latest";
const QString kReleaseTagPrefix = "ci-";

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

int calculateBuildVersion() {
  const QString string_id(MACRO_TO_STRING(CI_BUILD_ID));
  if (string_id == "dev") return 0;
  bool conversion_ok;
  uint version = string_id.toUInt(&conversion_ok);
  if (!conversion_ok) {
    qDebug() << "Weird CI_BUILD_ID value, that could not be parsed: "
             << string_id;
    return 0;
  }
  return version;
}

const int current_version_id = calculateBuildVersion();

}  // namespace

QString ApplicationUpdater::currentVersion() {
  return QString("1.0.") + MACRO_TO_STRING(CI_BUILD_ID);
}

ApplicationUpdater::ApplicationUpdater(MainTimer* main_timer,
                                       NetworkRequestor* requestor)
    : requestor_(requestor) {
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
    emit(updatesChecked());
    QJsonObject releases_metadata =
        QJsonDocument::fromJson(data.toUtf8()).object();
    QString tag_name = releases_metadata.value("tag_name").toString();
    if (!tag_name.startsWith(kReleaseTagPrefix)) {
      qDebug() << "Weird release tag (does not start with prefix "
               << kReleaseTagPrefix << " : " << tag_name;
      return;
    }
    tag_name.remove(0, kReleaseTagPrefix.length());
    int new_version = tag_name.toUInt();
    if (new_version > current_version_id) {
      qDebug() << "Hey, I found a new version! It is " << new_version << " vs "
               << current_version_id;
    }
  }
}

}  // namespace rclock::core

#include "core/app_updater.h"

#include <QApplication>
#include <QDebug>
#include <QJsonObject>
#include <QNetworkReply>
#include <QWindow>
#include <QtCore>

#include "core/app_info.h"

namespace rclock::core {

namespace {

const QString kOldVersionDir = "old_version";
const QString kNewVersionDir = "new_version";

const QString kLatestReleaseMetadataUrl =
    "https://api.github.com/repos/KrusnikViers/RaspClock/releases";

QString join(const QDir& dir, const QString& contents) {
  return dir.path() + QDir::separator() + contents;
}

void moveFileIfExists(const QDir& from, const QDir& to,
                      const QString& file_name) {
  if (!QFile::exists(join(from, file_name))) return;
  assert(QFile(join(from, file_name)).rename(join(to, file_name)));
}

void cleanRemnants() {
  QDir old_version_dir(join(QDir::current(), kOldVersionDir));
  if (old_version_dir.exists()) assert(old_version_dir.removeRecursively());
  QDir new_version_dir(join(QDir::current(), kNewVersionDir));
  if (new_version_dir.exists()) assert(new_version_dir.removeRecursively());
}

void restartAndUpdateApplication() {
  // Make sure, that existing dump of previous version was removed.
  cleanRemnants();

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
  connect(requestor, &NetworkRequestor::downloadFinished,  //
          this, &ApplicationUpdater::onFileDownloaded);
}

void ApplicationUpdater::initiateUpdate() {
  clearState();
  metadata_reply_ = requestor_->get(kLatestReleaseMetadataUrl);
}

void ApplicationUpdater::onRequestFetched(QNetworkReply* reply,
                                          RequestStatus status,
                                          const QString& data) {
  if (reply != metadata_reply_) return;
  if (status != kDone) {
    clearState();
    return;
  }

  QJsonArray all_releases = QJsonDocument::fromJson(data.toUtf8()).array();
  QJsonObject releases_metadata;
  bool release_found = false;
  for (const auto& release : all_releases) {
    if (release.toObject().value("tag_name").toString() == kAppReleaseTag) {
      releases_metadata = release.toObject();
      release_found     = true;
    }
  }

  if (!release_found) {
    qDebug() << "Required release " << kAppReleaseTag << " was not found!";
    return;
  }

  const bool is_new_version =
      (kAppBuildCommitHash !=
       releases_metadata.value("target_commitish").toString());
  emit(updatesChecked(is_new_version));

  if (is_new_version && config_->get().is_autoupdates_enabled) {
    QDir::current().mkdir(kNewVersionDir);
    QDir new_version_dir(join(QDir::current(), kNewVersionDir));

    for (const auto& asset_ref : releases_metadata.value("assets").toArray()) {
      const QString url =
          asset_ref.toObject().value("browser_download_url").toString();
      const QString filepath =
          join(new_version_dir, asset_ref.toObject().value("name").toString());
      qDebug() << "Downloading " << filepath << " from " << url << "...";
      downloading_files_.insert(requestor_->download(url, filepath));
    }
  }
}

void ApplicationUpdater::onFileDownloaded(QNetworkReply* reply,
                                          RequestStatus status) {
  if (!downloading_files_.count(reply)) return;
  if (status != kDone) {
    clearState();
    return;
  }

  downloading_files_.erase(reply);
  if (downloading_files_.empty()) restartAndUpdateApplication();
}

void ApplicationUpdater::clearState() {
  if (metadata_reply_ != nullptr) {
    metadata_reply_->abort();
    metadata_reply_ = nullptr;
  }
  for (auto* reply : downloading_files_) reply->abort();
  downloading_files_.clear();
  cleanRemnants();
}

}  // namespace rclock::core

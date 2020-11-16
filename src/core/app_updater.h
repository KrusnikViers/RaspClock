#pragma once

#include <QObject>
#include <set>

#include "core/config.h"
#include "core/main_timer.h"
#include "core/network.h"

namespace rclock::core {

class ApplicationUpdater : public QObject {
  Q_OBJECT

 public:
  ApplicationUpdater(Config* config, MainTimer* main_timer,
                     NetworkRequestor* requestor);

 public slots:
  void initiateUpdate();

 private slots:
  void onRequestFetched(QNetworkReply* reply, RequestStatus status,
                        const QString& data);
  void onFileDownloaded(QNetworkReply* reply, RequestStatus status);

 signals:
  void updatesChecked(bool new_version);

 private:
  void clearState();

  NetworkRequestor* requestor_ = nullptr;
  Config* config_              = nullptr;

  QNetworkReply* metadata_reply_ = nullptr;
  std::set<QNetworkReply*> downloading_files_;
};

}  // namespace rclock::core

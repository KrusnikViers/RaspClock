#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <QFile>
#include <map>

namespace rclock::core {

enum RequestStatus {
  kScheduled = 0,
  kDeduplicated,
  kDone,
  kError,
};

class NetworkRequestor : public QObject {
  Q_OBJECT
 public:
  NetworkRequestor();

  // NetworkRequestor will handle all the required actions with QNetworkReply
  // object. Clients, however, should use this pointer to distinguish emitted
  // signals for different requests.
  QNetworkReply* get(const QString& url);
  QNetworkReply* download(const QString& url, const QString& filepath);

 signals:
  void getFinished(QNetworkReply* reply, RequestStatus status,
                   const QString& data);
  void downloadFinished(QNetworkReply* reply, RequestStatus status);

 private slots:
  void onGetFinished();
  void onDownloadChunkReady(quint64 received, quint64 expected);
  void onDownloadFinished();

 private:
  QNetworkAccessManager manager_;
  std::map<QNetworkReply*, std::unique_ptr<QFile>> downloading_files_;
};

}  // namespace rclock::core

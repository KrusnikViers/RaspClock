#pragma once

#include <QNetworkAccessManager>
#include <QObject>
#include <set>

namespace rclock::core {

enum RequestStatus {
  kScheduled = 0,
  kDeduplicated,
  kDone,
  kError,
};

enum RequestType {
  kLatestReleaseMetadata = 0,
};

class NetworkRequestor : public QObject {
  Q_OBJECT
 public:
  NetworkRequestor();

  RequestStatus get(RequestType type, const QString& url);

 signals:
  void getFinished(RequestType type, RequestStatus status, const QString& data);

 private slots:
  void onGetFinished(RequestType type);

 private:
  QNetworkAccessManager manager_;
  std::set<RequestType> pending_requests_;
};

}  // namespace rclock::core

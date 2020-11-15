#include "core/network.h"

#include <QSslSocket>
#include <QtNetwork>

namespace rclock::core {

NetworkRequestor::NetworkRequestor() {
  if (!QSslSocket::supportsSsl())
    qDebug() << "SSL not supported! Install libraries for your system (e.g "
                "from here: https://wiki.openssl.org/index.php/Binaries)";
}

RequestStatus NetworkRequestor::get(RequestType type, const QString& url) {
  if (pending_requests_.count(type)) return kDeduplicated;
  pending_requests_.insert(type);

  QNetworkRequest request(url);
  QNetworkReply* reply = manager_.get(request);
  connect(reply, &QNetworkReply::finished,  //
          this, [this, type]() { onGetFinished(type); });
  return kScheduled;
}

void NetworkRequestor::onGetFinished(RequestType type) {
  pending_requests_.erase(type);
  QNetworkReply* reply = static_cast<QNetworkReply*>(sender());

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Error during network request: " << reply->errorString() << "\n"
             << reply->url().toString();
    reply->deleteLater();
    emit(getFinished(type, kError, QString()));
    return;
  }

  QByteArray data = reply->readAll();
  emit(getFinished(type, kDone, data));
}

}  // namespace rclock::core

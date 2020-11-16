#include "core/network.h"

#include <QSslSocket>
#include <QtNetwork>

namespace rclock::core {

NetworkRequestor::NetworkRequestor() {
  if (!QSslSocket::supportsSsl())
    qDebug() << "SSL not supported! Install libraries for your system (e.g "
                "from here: https://wiki.openssl.org/index.php/Binaries)";
  manager_.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

QNetworkReply* NetworkRequestor::get(const QString& url) {
  QNetworkRequest request(url);
  QNetworkReply* reply = manager_.get(request);
  connect(reply, &QNetworkReply::finished,  //
          this, &NetworkRequestor::onGetFinished);
  return reply;
}

QNetworkReply* NetworkRequestor::download(const QString& url,
                                          const QString& filepath) {
  QNetworkRequest request(url);
  QNetworkReply* reply      = manager_.get(request);
  downloading_files_[reply] = std::make_unique<QFile>(filepath);
  assert(downloading_files_[reply]->open(QIODevice::WriteOnly));
  connect(reply, &QNetworkReply::downloadProgress,  //
          this, &NetworkRequestor::onDownloadChunkReady);
  connect(reply, &QNetworkReply::finished,  //
          this, &NetworkRequestor::onDownloadFinished);
  return reply;
}

void NetworkRequestor::onGetFinished() {
  QNetworkReply* reply = static_cast<QNetworkReply*>(sender());

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Error during network request: " << reply->errorString() << "\n"
             << reply->url().toString();
    emit(getFinished(reply, kError, QString()));
    reply->deleteLater();
    return;
  }

  QByteArray data = reply->readAll();
  emit(getFinished(reply, kDone, data));
  reply->deleteLater();
}

// This method serves just to dump received data on disk, so that it won't be
// kept in the memory.
void NetworkRequestor::onDownloadChunkReady(quint64 /*received*/,
                                            quint64 /*expected*/) {
  QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
  downloading_files_[reply]->write(reply->readAll());
}

void NetworkRequestor::onDownloadFinished() {
  QNetworkReply* reply = static_cast<QNetworkReply*>(sender());

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << "Error during network request: " << reply->errorString() << "\n"
             << reply->url().toString();
    downloading_files_[reply]->close();
    downloading_files_[reply]->remove();
    emit(downloadFinished(reply, kError));
    reply->deleteLater();
    return;
  }

  downloading_files_[reply]->write(reply->readAll());
  downloading_files_[reply]->close();
  emit(downloadFinished(reply, kDone));
  reply->deleteLater();
}

}  // namespace rclock::core

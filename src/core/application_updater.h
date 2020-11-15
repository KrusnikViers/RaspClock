#pragma once

#include <QObject>

#include "core/main_timer.h"
#include "core/network.h"

namespace rclock::core {

class ApplicationUpdater : public QObject {
  Q_OBJECT

 public:
  static QString currentVersion();

  ApplicationUpdater(MainTimer* main_timer, NetworkRequestor* requestor);

 public slots:
  void initiateUpdate();

 private slots:
  void onRequestFetched(RequestType type, RequestStatus status,
                        const QString& data);

 signals:
  void updatesChecked();

 private:
  NetworkRequestor* requestor_ = nullptr;
};

}  // namespace rclock::core

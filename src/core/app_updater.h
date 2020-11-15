#pragma once

#include <QObject>

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
  void onRequestFetched(RequestType type, RequestStatus status,
                        const QString& data);

 signals:
  void updatesChecked(bool new_version);

 private:
  NetworkRequestor* requestor_ = nullptr;
  Config* config_              = nullptr;
};

}  // namespace rclock::core

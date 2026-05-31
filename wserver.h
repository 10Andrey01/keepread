#ifndef WSERVER_H
#define WSERVER_H

#include <QObject>

class wserver : public QObject {
  Q_OBJECT
public:
  explicit wserver(QObject *parent = nullptr);

signals:
};

#endif // WSERVER_H

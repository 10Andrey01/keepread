#ifndef WSERVER_H
#define WSERVER_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QWebSocketServer>

class wserver : public QObject {
  Q_OBJECT
  QWebSocketServer *m_pWebSocketServer = nullptr;
  quint16 port = 3000;
  QList<QWebSocket *> m_clients;

public:
  explicit wserver(quint16 port,
                   QObject *parent = nullptr);
public slots:
  void onNewConnection();
  void processTextMessage(QString message);
  void processBinaryMessage(QByteArray message);
  void socketDisconected();
signals:
  void closed();
};

#endif // WSERVER_H

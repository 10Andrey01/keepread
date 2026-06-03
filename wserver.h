#ifndef WSERVER_H
#define WSERVER_H

#include <QByteArray>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QWebSocketServer>

class wserver : public QObject {
  Q_OBJECT
  QWebSocketServer *m_pWebSocketServer = nullptr;
  quint16 m_port = 3000;
  enum class m_ConnectionSate
  {
    PendingAuthentication,
    Authenticated,
    Failed
  };
  struct m_client
  {
    QWebSocket *socket = nullptr;
    quint64 userId = 0;
    QString userName;
    QString sessionToken;
    QDateTime connectionTime;
    QDateTime lastMessaging;
    QTimer *authTimer = nullptr;
  };
  QList<m_client *> m_clients;
  void sendAuthenticationResult (QWebSocket *socket, bool succes,
                                 QString message);
  void generateSessionToken (m_client *);
  bool validateSessionToken (const QString &token, m_client &client);

private slots:
  void processTextMessage (QString message);

public:
  explicit wserver (quint16 port, QObject *parent = nullptr);
public slots:
  void onNewConnection();
  void processAuthenticated (const QString userName,
                             const QString userPassword);
  void processBinaryMessage(QByteArray message);
  void socketDisconected ();
signals:
  void authentication (const QString userName, const QString userPassword);
  void closed();
};

#endif // WSERVER_H

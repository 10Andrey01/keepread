#include "wserver.h"
#include <QWebSocket>

wserver::wserver (quint16 port, QObject *parent)
    : QObject (parent), m_port (port),
      m_pWebSocketServer (
          new QWebSocketServer (QStringLiteral ("Keepread server"),
                                QWebSocketServer::NonSecureMode),
          this)

{
  if (!m_pWebSocketServer->listen (QHostAddress::Any, m_port))
    {
      qFatal () << "ERROR: WebSocket server Keepread not started";
      return;
    }
  m_clients << new m_client ();
  qInfo () << "INFO: WebSocker server Keepread started on " << m_port
           << " port";
  connect (m_pWebSocketServer, &QWebSocketServer::newConnection, this,
           &wserver::onNewConnection);
  connect (m_pWebSocketServer, &QWebSocketServer::closed, this,
           &wserver::closed);
}

void
wserver::onNewConnection ()
{
  m_client *client = new m_client ();
  client->socket = m_pWebSocketServer->maxPendingConnections ();
  client->userId = m_clients.last ()->userId + 1;
  client->state = m_connectionSate::PendingAuthentication;

  client->authTimer = new QTimer (client);
  client->authTimer->setSingleShot (true);
  client->authTimer->setInterval (3000);
  connect (client->authTimer, &QTimer::timeout, this, [this, client] () {
    if (client->state == ConnectionState::PendingAuth)
      {
        sendAuthenticationResult (client->socket, false,
                                  "Authentication timeout");
        client->socket->close ();
        m_clients.removeAll (client);
        delete client;
        emit authenticationFailed ("Authentication timeout");
      }
  });
  client->authTimer->start ();
  connect (client->socket, &QWebSocket::textMessageReceived, this,
           &wserver::processTextMessage);
  connect (client->socket, &QWebSocket::binaryMessageReceived, this,
           &wserver::processBinaryMessage);
  connect (client->socket, &QWebSocket::disconnected, this,
           &wserver::socketDisconected);
  m_clients << client;
}

void
wserver::processTextMessage (QString message)
{
  QWebSocket *pClient = qobject_cast<QWebSocket *> (sender ());
  qDebug () << "Text message: " << pClient->peerName () << message;
}

void
wserver::processBinaryMessage (QByteArray message)
{
  QWebSocket *pClient = qobject_cast<QWebSocket *> (sender ());
  qDebug () << "Binary message: " << pClient->peerName () << message;
}

void
wserver::socketDisconected ()
{
  QWebSocket *pClient = qobject_cast<QWebSocket *> (sender ());
  qInfo () << "INFO: client " << pClient->peerName () << " disconnected";
  if (pClient)
    {
      m_clients.removeAll (pClient);
      pClient->deleteLater ();
    }
}

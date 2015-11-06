/*
  Copyright (c) 2011-2012 - Tőkés Attila
  Copyright (C) 2015 Daniel Nicoletti <dantti12@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  See the LICENSE file for more details.
*/

#ifndef SIMPLEMAIL_SENDER_H
#define SIMPLEMAIL_SENDER_H

#include <QObject>
#include <QtNetwork/QSslSocket>

#include "mimemessage.h"
#include "smtpexports.h"

class SenderPrivate;
class SMTP_EXPORT Sender : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Sender)
public:
    enum AuthMethod
    {
        AuthPlain,
        AuthLogin
    };

    enum SmtpError
    {
        ConnectionTimeoutError,
        ResponseTimeoutError,
        SendDataTimeoutError,
        AuthenticationFailedError,
        ServerError,    // 4xx smtp error
        ClientError     // 5xx smtp error
    };

    enum ConnectionType
    {
        TcpConnection,
        SslConnection,
        TlsConnection       // STARTTLS
    };

    Sender(const QString &host = QLatin1String("localhost"), int port = 25, ConnectionType ct = TcpConnection);

    ~Sender();

    QString getHost() const;
    void setHost(const QString &host);

    int getPort() const;
    void setPort(int port);

    QString getName() const;
    void setName(const QString &name);

    ConnectionType getConnectionType() const;
    void setConnectionType(ConnectionType ct);

    const QString & getUser() const;
    void setUser(const QString &user);

    const QString & getPassword() const;
    void setPassword(const QString &password);

    AuthMethod getAuthMethod() const;
    void setAuthMethod(AuthMethod method);

    QString getResponseText() const;
    int getResponseCode() const;

    int getConnectionTimeout() const;
    void setConnectionTimeout(int msec);

    int getResponseTimeout() const;
    void setResponseTimeout(int msec);
    
    int getSendMessageTimeout() const;
    void setSendMessageTimeout(int msec);

    QTcpSocket* getSocket();

    bool connectToHost();

    bool login();
    bool login(const QString &user, const QString &password, AuthMethod method = AuthLogin);

    bool sendMail(MimeMessage& email);

    void quit();

protected:
    class ResponseTimeoutException {};
    class SendMessageTimeoutException {};

    void waitForResponse();

    void sendMessage(const QString &text);

protected Q_SLOTS:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketError(QAbstractSocket::SocketError error);
    void socketReadyRead();

Q_SIGNALS:
    void smtpError(SmtpError e);

protected:
    SenderPrivate *d_ptr;
};

#endif // SIMPLEMAIL_SENDER_H
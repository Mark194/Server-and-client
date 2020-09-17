#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include <string>
#include <QDir>
#include <QCoreApplication>

class server: public QTcpServer{
    Q_OBJECT
public:
    server();
    ~server();

    QTcpSocket* socket;
    QByteArray Data;

    QJsonDocument doc;
    QJsonParseError docError;

public slots:
    void start();
    void incomingConnection(int socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // SERVER_H

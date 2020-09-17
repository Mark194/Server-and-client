#include "server.h"

server::server(){}

server::~server(){}

void server::start(){
    if (this->listen(QHostAddress::Any,5555))
        qDebug() << "Listening...";
    else
        qDebug() << "Not listening";
}

void server::incomingConnection(int socketDescriptor){
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

    qDebug() << "Client №" << socketDescriptor << "connected";

    socket->write("{\"type\":\"connect\",\"status\":\"true\"}");

    qDebug() << "Send to client status connect: true";


}

void server::sockReady(){
    Data = socket->readAll();
    qDebug() << "Data from client: " << Data;

    doc = QJsonDocument::fromJson(Data, &docError);

    if (docError.errorString().toInt() == QJsonParseError::NoError)
    {
        if (doc.object().value("type").toString() == "select" and doc.object().value("param").toString() == "users")
        {
            QFile file;
            file.setFileName( QCoreApplication::applicationDirPath()+"\\Files\\users.json");

            if (file.open(QIODevice::ReadOnly|QFile::Text))
            {
                QByteArray fromFile = file.readAll();
                QByteArray result = "{\"type\":\"resultSelect\",\"result\":"+fromFile+"}";

                socket->write(result);
                socket->waitForBytesWritten(600);
                qDebug() << "Send to user: " + result;
            }

            file.close();
        }
    }
}

void server::sockDisc(){
    qDebug() << "Client disconected";
    socket -> deleteLater();
}

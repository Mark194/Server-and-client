#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisc()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1",5555);
}

void MainWindow::socketDisc()
{
    socket -> deleteLater();
}

void MainWindow::socketReady()
{
    if (socket -> waitForConnected(600))
    {
        socket->waitForReadyRead(500);
        Data = socket -> readAll();
        document = QJsonDocument::fromJson(Data, &error);

        if (error.errorString().toInt() == QJsonParseError::NoError)
        {
            if (document.object().value("type").toString() == "connect" and document.object().value("status").toString() == "true")
            {
                QMessageBox::information(this, "Информация", "Соединение установлено");
            }
            else if (document.object().value("type").toString() == "resultSelect")
            {
                QStandardItemModel * model = new QStandardItemModel(nullptr);
                model->setHorizontalHeaderLabels(QStringList()<<"name");

                QJsonArray arr = document.object().value("result").toArray();
                for (auto element : arr)
                {
                    QStandardItem *column = new QStandardItem(element.toObject().value("name").toString());
                    model->appendRow(column);
                }

                ui->tableView->setModel(model);

            }
            else
            {
                QMessageBox::information(this, "Информация", "Соединение не установлено");
            }
        }
        else
        {
            QMessageBox::information(this, "Информация", "Ошибка неверный формат передачи данных" + error.errorString());
        }
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(socket->isOpen())
    {
        socket->write("{\"type\":\"select\",\"param\":\"users\"}");
        socket->waitForBytesWritten(500);
    }
   else
   {
        QMessageBox::information(this,"Информация", "Соединение не установлено!");
   }
}

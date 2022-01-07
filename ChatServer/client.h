#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QHostAddress>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QStandardPaths>
#include <QTcpSocket>
#include <QDataStream>
#include <QTcpServer>

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QString usernameL, QString passwordL, QWidget *parent = nullptr);
    ~Client();

signals:
    void newMessage(QString);

private slots:
    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(const QString& str);
    void on_pushButton_sendMessage_clicked();
    void sendmess();

private:
    Ui::Client *ui;
    QTcpSocket* socket;
    QTcpServer* serv;
    QSet<QTcpSocket*> conn;
    QString usernameL;
    QString passwordL;
};

#endif // CLIENT_H

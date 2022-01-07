#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QSet>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class Server;
}

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QString usernameL, QString passwordL, QWidget *parent = nullptr);
    ~Server();

signals:
    void newMessage(QString);

private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket* socket);

    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(const QString& str);
    void sendMessage(QTcpSocket* socket);

    void on_pushButton_sendMessage_clicked();

    void refreshComboBox();
    void sendmess();

private:
    Ui::Server *ui;
    QTcpServer* serv;
    QSet<QTcpSocket*> conn;
    QString usernameL;
    QString passwordL;
};

#endif // SERVER_H

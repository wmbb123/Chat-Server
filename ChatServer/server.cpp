#include "server.h"
#include "ui_server.h"
Server::Server(QString usernameL, QString passwordL, QWidget *parent) : QMainWindow(parent), ui(new Ui::Server)
{
    ui->setupUi(this);
    this->usernameL = usernameL;
    this->passwordL = passwordL;
    serv = new QTcpServer();

    if(serv->listen(QHostAddress::Any, 8080))
    {
       connect(this, &Server::newMessage, this, &Server::displayMessage);
       connect(serv, &QTcpServer::newConnection, this, &Server::newConnection);
       ui->statusBar->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(serv->errorString()));
        exit(EXIT_FAILURE);
    }
}

Server::~Server()
{
    foreach (QTcpSocket* socket, conn)
    {
        socket->close();
        socket->deleteLater();
    }

    serv->close();
    serv->deleteLater();

    delete ui;
}

void Server::newConnection()
{
    while (serv->hasPendingConnections())
        appendToSocketList(serv->nextPendingConnection());
}

void Server::appendToSocketList(QTcpSocket* socket)
{
    conn.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &Server::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &Server::discardSocket);
    //connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);
    ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
    displayMessage(QString("INFO :: Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor())); //sockd:%1
}

void Server::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    QByteArray buffer;

    QDataStream socketStream(socket);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {

        QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
        emit newMessage(message);
        return;
    }

    QString header = buffer.mid(0,128);
    QString fileType = header.split(",")[0].split(":")[1];

    buffer = buffer.mid(128);
    if(fileType=="message"){
        QString message = QString("%1 :: %2").arg(socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
        emit newMessage(message);
    }
}

void Server::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = conn.find(socket);
    if (it != conn.end()){
        displayMessage(QString("INFO :: A client has just left the room").arg(socket->socketDescriptor()));
        conn.remove(*it);
    }
    refreshComboBox();

    socket->deleteLater();
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
            QMessageBox::information(this, "QTCPServer", "The host was not found. Please check the host name and port settings.");
        break;
        case QAbstractSocket::ConnectionRefusedError:
            QMessageBox::information(this, "QTCPServer", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
        default:
            QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
            QMessageBox::information(this, "QTCPServer", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void Server::on_pushButton_sendMessage_clicked()
{
    QString receiver = ui->comboBox_receiver->currentText();

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,conn)
        {
            sendMessage(socket);
        }
    }
    else
    {
        foreach (QTcpSocket* socket,conn)
        {
            if(socket->socketDescriptor() == receiver.toLongLong())
            {
                sendMessage(socket);
                break;
            }
        }
    }
    ui->lineEdit_message->clear();
    //sendmess();
}

void Server::sendmess(){
    QString receiver = ui->comboBox_receiver->currentText();

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,conn)
        {
            sendMessage(socket);
        }
    }
    else
    {
        foreach (QTcpSocket* socket,conn)
        {
            if(socket->socketDescriptor() == receiver.toLongLong())
            {
                sendMessage(socket);
                break;
            }
        }
    }
    ui->lineEdit_message->clear();


}

void Server::sendMessage(QTcpSocket* socket)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            QString str = ui->lineEdit_message->text();
            ui->textBrowser_receivedMessages->append(str);
            QDataStream socketStream(socket);

            QByteArray header;
            header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(str.size()).toUtf8());
            header.resize(128);

            QByteArray byteArray = str.toUtf8();
            byteArray.prepend(header);

            socketStream << byteArray;
        }
        else
            QMessageBox::critical(this,"QTCPServer","Socket doesn't seem to be opened");
    }
    else
        QMessageBox::critical(this,"QTCPServer","Not connected");
}

void Server::displayMessage(const QString& str)
{
    ui->textBrowser_receivedMessages->append(str);
    //ui->lineEdit_message->text();
}

void Server::refreshComboBox(){
    ui->comboBox_receiver->clear();
    ui->comboBox_receiver->addItem("Broadcast");
    foreach(QTcpSocket* socket, conn)
        ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
}

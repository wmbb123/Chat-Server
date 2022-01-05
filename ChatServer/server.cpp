#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) : QMainWindow(parent), ui(new Ui::Server)
{
    ui->setupUi(this);
    m_server = new QTcpServer();

    if(m_server->listen(QHostAddress::Any, 8080))
    {
       connect(this, &Server::newMessage, this, &Server::displayMessage);
       connect(m_server, &QTcpServer::newConnection, this, &Server::newConnection);
       ui->statusBar->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(m_server->errorString()));
        exit(EXIT_FAILURE);
    }
}

Server::~Server()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete ui;
}

void Server::newConnection()
{
    while (m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void Server::appendToSocketList(QTcpSocket* socket)
{
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &Server::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &Server::discardSocket);
    //connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);
    ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
    displayMessage(QString("INFO :: Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor()));
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
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){
        displayMessage(QString("INFO :: A client has just left the room").arg(socket->socketDescriptor()));
        connection_set.remove(*it);
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
    sendmess();
}

void Server::sendmess(){
    QString receiver = ui->comboBox_receiver->currentText();

    if(receiver=="Broadcast")
    {
        foreach (QTcpSocket* socket,connection_set)
        {
            sendMessage(socket);
        }
    }
    else
    {
        foreach (QTcpSocket* socket,connection_set)
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
    foreach(QTcpSocket* socket, connection_set)
        ui->comboBox_receiver->addItem(QString::number(socket->socketDescriptor()));
}

#include "homepage.h"
#include "ui_homepage.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "server.h"
#include "ui_server.h"
#include "client.h"
#include "ui_client.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Homepage::Homepage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::on_pushButton_client_clicked()
{
    client = new Client(this);
    client->show();
}

void Homepage::on_pushButton_clicked()
{
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("127.0.0.1");
    database.setUserName("admin");
    database.setPassword("pvcSGmQ2REna");
    database.setDatabaseName("DatabaseChatServer");
    if(database.open()) {


    database.close();
    }
}

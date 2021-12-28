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

void Homepage::on_pushButton_server_clicked()
{
    server = new Server(this);
    server->show();
}

void Homepage::on_pushButton_client_clicked()
{
    client = new Client(this);
    client->show();
}

void Homepage::on_pushButton_display_clicked()
{
    QString username = ui->label_user->text();
    //QString password = ui->label_pass->text();
    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("127.0.0.1");
    database.setUserName("admin");
    database.setPassword("pvcSGmQ2REna");
    database.setDatabaseName("DatabaseChatServer");

    if(database.open()) {
        querymodel = new QSqlQueryModel;
        querymodel->setQuery("SELECT * FROM users");
        ui->tableView->setModel(querymodel);
        QSqlQueryModel model;
        //model.setQuery("SELECT * FROM users WHERE username = " + username);



          //for (int i = 0; i < model.rowCount(); ++i) {
          //    int id = model.record(i).value("id").toInt();
          //    QString name = model.record(i).value("username").toString();
          //    QString email = model.record(i).value("email").toString();
          //    QString phone = model.record(i).value("phone").toString();
          //    qDebug() << id << name << email << phone;
          //    name = username;
          //}
    database.close();
    }
}

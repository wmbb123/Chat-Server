#include "admin.h"
#include "ui_admin.h"
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

Admin::Admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_pushButton_clicked()
{
    server = new Server(this);
    server->show();
}

void Admin::on_pushButton_2_clicked()
{
    client = new Client(this);
    client->show();
}

void Admin::on_pushButton_display_clicked()
{
    //QString username = ui->label_user->text();
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

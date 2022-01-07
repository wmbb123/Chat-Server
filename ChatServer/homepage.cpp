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
#include <QTextEdit>

Homepage::Homepage(QString usernameL, QString passwordL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    this->usernameL = usernameL;
    this->passwordL = passwordL;

}

Homepage::~Homepage()
{
    delete ui;
}

void Homepage::on_pushButton_client_clicked()
{
    client = new Client(usernameL, passwordL);
    client->show();
}

void Homepage::on_pushButton_clicked()
{
    database = QSqlDatabase::addDatabase("QMYSQL", "MyConnect");
    database.setHostName("127.0.0.1");
    database.setUserName("admin");
    database.setPassword("pvcSGmQ2REna");
    database.setDatabaseName("DatabaseChatServer");

    ui->textEdit->setText(usernameL);
    ui->textEdit_2->setText(passwordL);

    //if(database.open()) {
        /*QSqlQuery query(QSqlDatabase::database("MyConnect"));
        query.prepare(QString("SELECT * FROM users WHERE username = :username AND password = :password"));

        query.bindValue(":username", usernameL);
        query.bindValue(":password", passwordL);

        if(!query.exec()) {
            QMessageBox::information(this, "Failed", "Query Failed To Execute");
        }
        else
        {
            while (query.next()) {
                QString usernameFromDB = query.value(1).toString();
                QString passwordFromDB = query.value(2).toString();
        if(usernameFromDB == usernameL)
        {

        }
    }
    database.close();
    }*/
  //}
}

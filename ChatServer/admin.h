#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "client.h"
#include "server.h"

namespace Ui {
class Admin;
}

class Admin : public QMainWindow
{
    Q_OBJECT

public:
    explicit Admin(QString usernameL, QString passwordL, QWidget *parent = nullptr);
    ~Admin();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_display_clicked();

private:
    Ui::Admin *ui;
    QSqlDatabase database;
    QSqlQueryModel *querymodel;
    Client *client;
    Server *server;
    QString usernameL;
    QString passwordL;
};

#endif // ADMIN_H

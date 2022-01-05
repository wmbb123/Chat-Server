#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "client.h"
#include "server.h"

namespace Ui {
class Homepage;
}

class Homepage : public QDialog
{
    Q_OBJECT

public:
    explicit Homepage(QWidget *parent = nullptr);
    ~Homepage();

private slots:

    void on_pushButton_client_clicked();

    void on_pushButton_clicked();

private:
    Ui::Homepage *ui;
    QSqlDatabase database;
    QSqlQueryModel *querymodel;
    Client *client;
    //Server *server;
};

#endif // HOMEPAGE_H

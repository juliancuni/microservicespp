#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMenuBar>
#include "localdb.h"
#include "microservicesmain.h"

namespace Ui {
class ApiManager;
}

class ApiManager : public QDialog
{
    Q_OBJECT

public:
    explicit ApiManager(QWidget *parent = nullptr);
    ~ApiManager();

private slots:
//    void on_showLoginBTN_clicked();

    void on_ruajApiBtn_clicked();

    void on_tryApiBTN_clicked();

    void replyFinished (QNetworkReply *reply);

    void on_listaApis_doubleClicked(const QModelIndex &index);

    void on_fshiApiBTN_clicked();

//    void on_httpRB_clicked();

//    void on_httpsRB_clicked();

//    void on_groupBox_clicked();

private:
    Ui::ApiManager *ui;
    QNetworkAccessManager *manager;
};

#endif // APIMANAGER_H

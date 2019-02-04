#include "apimanager.h"
#include "ui_apimanager.h"
#include "logindialog.h"

static LocalDb lDB;
static QSqlQueryModel * apis = new QSqlQueryModel();
static QString protocol;

ApiManager::ApiManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApiManager)
{
    ui->setupUi(this);

    //Tento CustomContextMenu
    ui->listaApis->setContextMenuPolicy(Qt::CustomContextMenu);
    QSqlQuery qry = lDB.select("domain", "apis", "");
    apis->setQuery(qry);
    ui->listaApis->setModel(apis);
}

ApiManager::~ApiManager()
{
    delete ui;
}

void ApiManager::on_ruajApiBtn_clicked()
{
    QString domain = ui->domainTB->text();
    int port = ui->portTB->text().toInt();
    bool defaultApi = ui->defaultCB->isChecked();

    if(ui->httpRB->isChecked()) {
        protocol = "http";
    }
    if(ui->httpsRB->isChecked()){
        protocol = "https";
    }

    QSqlQuery qry = lDB.insert("OR REPLACE", "apis", "'protocol', 'domain', 'port', 'uri', 'default'", "'" + protocol  + "','" + domain  + "'," + QString::number(port)  + ",'""'," + QString::number(defaultApi) + "");
    if(qry.lastError().isValid()) {
        ui->errorLB->setText(qry.lastError().text());
        return;
    }
    qry = lDB.select("domain", "apis", "");
    apis->setQuery(qry);
    if(ui->defaultCB->isChecked()){
        qry = lDB.update("apis", "`default`", QString::number(0), "WHERE `domain` !=\"" + domain + "\"");
        if(qry.lastError().isValid()) {
            ui->errorLB->setText(qry.lastError().text());
            return;
        }
    }
    ApiManager::close();
    loginDialog login;
    login.setModal(true);
    login.exec();
}

void ApiManager::on_tryApiBTN_clicked()
{
    if(ui->domainTB->text() == "")
        ui->errorLB->setText("Kujdes! Protokolli dhe/ose Domaini\ns'mund te lihen bosh");
    else {
        ui->errorLB->setText("Prisni...");
        ui->tryApiBTN->setDisabled(true);
        ui->ruajApiBtn->setDisabled(true);

        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));

//        QString proto;
        if(ui->httpRB->isChecked()) {
            protocol = "http";
        }
        if(ui->httpsRB->isChecked()){
            protocol = "https";
        }
        QString domain = ui->domainTB->text();
        QString port = ui->portTB->text();

        QString url = protocol + "://" + domain + ":"+ port;

        manager->get(QNetworkRequest(QUrl(url)));
    }

}

void ApiManager::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        ui->errorLB->setText(reply->errorString());
        ui->tryApiBTN->setDisabled(false);
    }
    else
    {
//        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
//        QJsonArray getjsonq = document.array();
//        foreach (const QJsonValue & v, getjsonq)
//                qDebug() << v.toObject().value("name").toString();
//        qDebug() << getjsonq.count();
//        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
//        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
//        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
//        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        int resCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if(resCode == 200 || resCode == 301)
            ui->errorLB->setText("OK, API eshte up and running.\n Mund te klikoni Ruaj");
        ui->tryApiBTN->setDisabled(false);
        ui->ruajApiBtn->setDisabled(false);
    }

    reply->deleteLater();
}

void ApiManager::on_listaApis_doubleClicked(const QModelIndex &index)
{
    QString domain = index.data().toString();
    QSqlQuery qry = lDB.select("*", "apis", "WHERE domain='"+ domain +"'");
    if(qry.lastError().isValid()) {
        ui->errorLB->setText(qry.lastError().text());
        return;
    }
    while (qry.next()) {
        ui->ruajApiBtn->setEnabled(true);
        if(qry.value(1).toString() == "http") {
            ui->httpRB->setChecked(true);
        } else {
            ui->httpsRB->setChecked(true);
        }
        ui->domainTB->setText(qry.value(2).toString());
        if(qry.value(3).toInt() > 0) {
            ui->portTB->setText(qry.value(3).toString());
        } else {
            ui->portTB->setText("");
        }
        ui->defaultCB->setChecked(qry.value(5).toBool());
        ui->fshiApiBTN->setDisabled(false);
    }
}

void ApiManager::on_fshiApiBTN_clicked()
{
    QString domain = ui->domainTB->text();
    QSqlQuery qry = lDB.del("apis", "domain","'"+ domain +"'");
    if(qry.lastError().isValid()) {
        ui->errorLB->setText(qry.lastError().text());
        return;
    }
}

//void ApiManager::on_httpRB_clicked()
//{
//    protocol = "http";
//}

//void ApiManager::on_httpsRB_clicked()
//{
//    protocol = "https";
//}

//void ApiManager::on_groupBox_clicked()
//{
//    protocol = "https";
//}

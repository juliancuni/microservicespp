#include "microservicesmain.h"
#include "ui_microservicesmain.h"

static LocalDb lDB;
static QString apiLink;
static int apiId;
static QString access_token;
MicroServicesMain::MicroServicesMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MicroServicesMain)

{
    QSqlQuery qryApi = lDB.select("*", "apis", "WHERE `default`=" + QString::number(1));
    qryApi.exec();
    while (qryApi.next()) {
        apiId = qryApi.value(0).toInt();
        if(qryApi.value(3) == 0) {
            apiLink = qryApi.value(1).toString() + "://" + qryApi.value(2).toString();
        } else {
            apiLink = qryApi.value(1).toString() + "://" + qryApi.value(2).toString() + ":" + qryApi.value(3).toString();
        }

        this->setWindowTitle("Login: " + apiLink);
    }
    QSqlQuery qryUser = lDB.select("access_token", "credentials", "WHERE `apiId` = " + QString::number(apiId));
    qryUser.exec();

    while (qryUser.next()) {
        access_token = qryUser.value(0).toString();
    }

    ui->setupUi(this);
}

MicroServicesMain::~MicroServicesMain()
{
    delete ui;   
}

void MicroServicesMain::closeEvent(QCloseEvent *event) {

    event->ignore();

    if (QMessageBox::Yes == QMessageBox::question(this, "Logout?!", "Jeni i sigurte qe doni te dilni?", QMessageBox::Yes | QMessageBox::No)){

//        QString apiUri = "/api/Perdoruesit/logout?access_token=" + access_token;
//        HttpRequestInput input(apiLink + apiUri, "POST");
////        input.add_var("access_token", access_token);
//        qDebug() << apiLink + apiUri;
//        HttpRequestWorker *worker = new HttpRequestWorker(this);
//        connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_api_result(HttpRequestWorker*)));
//        worker->execute(&input);


//        lDB.del("credentials", "remember", " IS NULL");
        event->accept();
    }
}

//void MicroServicesMain::handle_api_result(HttpRequestWorker *worker) {
//    QString msg;
//    if (worker->error_type == QNetworkReply::NoError) {
//        qDebug() << "U krye" + worker->response;

//    } else {
//        msg = "Error: Gabim ne logout!\n" +  worker->error_str;
//        qDebug() << msg;
//        QMessageBox::critical(this, "ERROR!", msg, QMessageBox::Cancel);
//    }
//}

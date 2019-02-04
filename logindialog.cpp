#include "logindialog.h"
#include "ui_logindialog.h"

static LocalDb lDB;
static Crypt crypto(Q_UINT64_C(9835187522778933971));
static QSqlQueryModel * credentialsModel = new QSqlQueryModel();
static QString apiLink;
static int apiId;
static QString access_token;

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);

    ui->listSessions->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listSessions, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuListSessions(QPoint)));

    QSqlQuery qryUser = lDB.select("username, access_token", "credentials", "");
    credentialsModel->setQuery(qryUser);
    ui->listSessions->setModel(credentialsModel);

    qryUser.exec();
    while (qryUser.next()) {
        access_token = qryUser.value(1).toString();
    }

    //Gjej te dhenat per api
    QSqlQuery qryApi = lDB.select("*", "apis", "WHERE `default`=" + QString::number(1));

    qryApi.exec();
    if(qryApi.lastError().isValid()) {
        ui->errLB->setText(qryApi.lastError().text());
        return;
    }
    int numberOfRows = 0;
    if(qryApi.last())
    {
        numberOfRows =  qryApi.at() + 1;
        qryApi.first();
        qryApi.previous();
    }
    if(numberOfRows < 1){
        this->close();
        ApiManager apiMan;
        apiMan.setModal(true);
        apiMan.exec();
    }
    while (qryApi.next()) {
        apiId = qryApi.value(0).toInt();
        if(qryApi.value(3) == 0) {
            apiLink = qryApi.value(1).toString() + "://" + qryApi.value(2).toString();
        } else {
            apiLink = qryApi.value(1).toString() + "://" + qryApi.value(2).toString() + ":" + qryApi.value(3).toString();
        }

        this->setWindowTitle("Login: " + apiLink);
    }


}



loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::customMenuListSessions(QPoint pos){

    QModelIndex index=ui->listSessions->indexAt(pos);

    QString idx = index.data().toString();

    QMenu *menu=new QMenu(this);
    QAction * login = new QAction("Login", this);
    QAction * fshiSession = new QAction("Fshi Sesion", this);

    connect(login,  &QAction::triggered, this, [=](){ this->slotLogin(idx); });
    connect(fshiSession,  &QAction::triggered, this, [=](){ this->slotFshiSession(idx); });

    menu->addAction(login);
    menu->addAction(fshiSession);
    menu->popup(ui->listSessions->viewport()->mapToGlobal(pos));
}

void loginDialog::on_loginBTN_clicked()
{
    //Validate input
    QString user = ui->userTB->text();
    QString pass = ui->passTB->text();

    if(user == "") {
        ui->errLB->setText("Kujdes! Username s'mund te lihet bosh");
        return;
    } else {
        ui->errLB->clear();
    }
    if(pass == "") {
        ui->errLB->setText("Kujdes! Password s'mund te lihet bosh");
        return;
    } else {
        ui->errLB->clear();

    }

    //Login ne API dhe nese pergjigjet me te dhenat qe duhen ruaji te dhenat (tokens/user data) dhe pastaj ruaje ne localDB kete user.

    QString apiUri = "/api/Perdoruesit/login";

    HttpRequestInput input(apiLink + apiUri, "POST");
    input.add_var("username", user);
    input.add_var("password", pass);

    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_api_result(HttpRequestWorker*)));
    worker->execute(&input);
}

void loginDialog::on_fshiBTN_clicked()
{
    ui->userTB->setText("");
    ui->passTB->setText("");
}

void loginDialog::on_listSessions_doubleClicked(const QModelIndex &index)
{

    QString username = index.data().toString();

    QSqlQuery qry = lDB.select("*", "credentials", "WHERE username='"+ username +"'");
    if(qry.lastError().isValid()) {
        ui->errLB->setText(qry.lastError().text());
        return;
    }
    while (qry.next()) {
        QString decryptPass = crypto.decryptToString(qry.value(2).toString());
        ui->userTB->setText(qry.value(1).toString());
        ui->passTB->setText(decryptPass);
        ui->saveLogin->setChecked(true);

        QString user = ui->userTB->text();
        QString pass = ui->passTB->text();

        if(user == "") {
            ui->errLB->setText("Kujdes! Username s'mund te lihet bosh");
            return;
        } else {
            ui->errLB->clear();
        }
        if(pass == "") {
            ui->errLB->setText("Kujdes! Password s'mund te lihet bosh");
            return;
        } else {
            ui->errLB->clear();

        }

        //Login ne API dhe nese pergjigjet me te dhenat qe duhen ruaji te dhenat (tokens/user data) dhe pastaj ruaje ne localDB kete user.

        QString apiUri = "/api/Perdoruesit/login";

        HttpRequestInput input(apiLink + apiUri, "POST");
        input.add_var("username", user);
        input.add_var("password", pass);

        HttpRequestWorker *worker = new HttpRequestWorker(this);
        connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_api_result(HttpRequestWorker*)));
        worker->execute(&input);
    }
}

void loginDialog::slotLogin(QString fshiSess) {
    QString username = fshiSess;

    QSqlQuery qry = lDB.select("*", "credentials", "WHERE username='"+ username +"'");
    if(qry.lastError().isValid()) {
        ui->errLB->setText(qry.lastError().text());
        return;
    }
    while (qry.next()) {
        QString decryptPass = crypto.decryptToString(qry.value(2).toString());
        ui->userTB->setText(qry.value(1).toString());
        ui->passTB->setText(decryptPass);
    }
}

void loginDialog::slotFshiSession(QString fshiSess) {
    QString username = fshiSess;
    QSqlQuery qry = lDB.del("credentials", "username", "'"+ username +"'");

    if(qry.lastError().isValid()) {
        ui->errLB->setText(qry.lastError().text());
        return;
    }
    QSqlQuery qry1 = lDB.select("username", "credentials", "");
    if(qry1.lastError().isValid()) {
//        qDebug() << "ERRRRR: " << qry.lastError().text();
        ui->errLB->setText(qry.lastError().text());
        return;
    }
    credentialsModel->setQuery(qry1);
}

void loginDialog::on_toolButton_clicked()
{
    this->close();
    ApiManager apiMan;
    apiMan.setModal(true);
    apiMan.exec();
}

void loginDialog::handle_api_result(HttpRequestWorker *worker) {
    QString msg;

    if (worker->error_type == QNetworkReply::NoError) {
        // communication was successful
        msg = "Success: " + worker->response;
        QString res = worker->response;
        QJsonDocument tokenJSON = QJsonDocument::fromJson(res.toUtf8());

        QString username = ui->userTB->text();
        QString password = ui->passTB->text();
        QString userIdApi = tokenJSON["userId"].toString();
        QString access_token = tokenJSON["id"].toString();
        QString encryptPass = crypto.encryptToString(password);
        QSqlQuery qry;

        if(ui->saveLogin->isChecked()) {
            qry.prepare("INSERT OR REPLACE INTO credentials(username, password, apiId, userIdApi, access_token, remember) VALUES('"+ username +"', '"+ encryptPass +"', '"+ QString::number(apiId) +"', '"+ userIdApi +"', '"+ access_token +"', '"+ QString::number(1) +"');");

        } else {
            qry.prepare("INSERT OR REPLACE INTO credentials(username, password, apiId, userIdApi, access_token) VALUES('"+ username +"', '"+ encryptPass +"', '"+ QString::number(apiId) +"', '"+ userIdApi +"', '"+ access_token +"');");
        }
        qry.exec();
        if(qry.lastError().isValid()) {
            ui->errLB->setText(qry.lastError().text());
            return;
        }
        QSqlQuery qry1 = lDB.select("username", "credentials", "");
        if(qry1.lastError().isValid()) {
        ui->errLB->setText(qry.lastError().text());
            return;
        }
        credentialsModel->setQuery(qry1);
        this->close();
        MicroServicesMain *ms = new MicroServicesMain();
        ms->showMaximized();
    }
    else {
        // an error occurred
//        msg = "Error: " + worker->error_str;
        if(worker->error_str == "Error: Host requires authentication") {
            msg = "Error: Username ose Password jane gabim!";
        }
        ui->errLB->setText(msg);
//        QMessageBox::information(this, "", msg);
    }

//    qDebug() << msg;
}

void loginDialog::closeEvent(QCloseEvent *event) {
    event->accept();
}

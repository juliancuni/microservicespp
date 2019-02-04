#ifndef LOCALDB_H
#define LOCALDB_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>


class LocalDb {

public:
    QSqlDatabase localDb;

    bool localConnect() {

        QSqlDatabase localDb = QSqlDatabase::addDatabase("QSQLITE");

        QString dir = QCoreApplication::applicationDirPath() + "/" + "conf/";
        QString db_local_file = "microservices.db";
        QFileInfo check_file(dir + db_local_file);
        //kerko per folder conf, nese nuk egziston krijoje folderin
        if(!QDir(dir).exists()) {
            QDir().mkdir(dir);
        }
        //Kerko per sqlite file dhe krijo skemen e DB nese nuk egziston
        if (!check_file.exists()) {

            localDb.setDatabaseName(dir + db_local_file);
            if (!localDb.open()) {
                QMessageBox::critical(nullptr, QObject::tr("MicroServices Error"),
                QObject::tr("Lidhja me databazen deshtoi!"), QMessageBox::Ok);
            }
            //Krijo tabelat me te dhenat e duhura.
            QSqlQuery qry;

            qry.exec("CREATE TABLE `credentials` ("
                     "`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
                     "`username`	varchar ( 20 ) NOT NULL,"
                     "`password`	varchar ( 20 ) NOT NULL,"
                     "`apiId`	INTEGER NOT NULL,"
                     "`userIdApi`	TEXT NOT NULL,"
                     "`access_token`	TEXT NOT NULL,"
                     "`remember`	INTEGER"
                 ");");

            qry.exec("CREATE TABLE `apis` ("
                     "`id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
                     "`protocol`	TEXT NOT NULL,"
                     "`domain`	TEXT NOT NULL UNIQUE,"
                     "`port`	INTEGER,"
                     "`uri`	TEXT,"
                     "`default`	INTEGER"
                 ");");
        } else {
            localDb.setDatabaseName(dir + db_local_file);
        }

        if (!localDb.open()) {
            QMessageBox::critical(nullptr, QObject::tr("MicroServices Error"),
            QObject::tr("Lidhja me databazen deshtoi!"), QMessageBox::Ok);
            return false;
        }
        return true;
    }
    void localDisconnect() {
        localDb.close();
        localDb.removeDatabase(QSqlDatabase::defaultConnection);
    }

    //SQL Queries
    //Select
    QSqlQuery select(QString fushat, QString tabela, QString kushtet)
    {
        QSqlQuery qry;
        QString sql = "SELECT " + fushat + " FROM " + tabela + " " + kushtet + ";";
//        qDebug() << sql;
        qry.prepare(sql);
        qry.exec();
        return qry;
    }
    //Insert
    QSqlQuery insert(QString extraArg, QString tabela, QString fushat, QString vlerat)
    {
        QSqlQuery qry;
        QString sql = "INSERT " + extraArg + " INTO " + tabela + "(" + fushat + ") VALUES(" + vlerat + ");";
//        qDebug() << sql;
        qry.prepare(sql);
        qry.exec();
        return qry;
    }
    //Delete
    QSqlQuery del(QString tabela, QString fushat, QString vlerat)
    {
        QSqlQuery qry;
        QString sql = "DELETE FROM " + tabela + " WHERE " + fushat + vlerat + ";";
        qDebug() << sql;
        qry.prepare(sql);
        qry.exec();
        return qry;
    }
    //Update
    QSqlQuery update(QString tabela, QString fusha, QString vlera, QString kushtet)
    {
        QSqlQuery qry;
        QString sql = "UPDATE " + tabela + " SET " + fusha + " = " + vlera + " " + kushtet +";";
        qDebug() << sql;
        qry.prepare(sql);
        qry.exec();
        return qry;
    }


private:

};

#endif // LOCALDB_H

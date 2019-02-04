#include "logindialog.h"
//#include "microservicesmain.h"
#include <QApplication>
#include "localdb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LocalDb lDB;
    if(!lDB.localConnect()) {
        return 1;
    }

    loginDialog loginDialog;
//    MicroServicesMain microServicesMain;
//    QSqlQuery qryUser = lDB.select("*", "credentials", "");
//    qryUser.exec();
//    int numberOfRows = 0;
//    if(qryUser.last())
//    {
//        numberOfRows =  qryUser.at() + 1;
//        qryUser.first();
//        qryUser.previous();
//    }
//    if(numberOfRows > 0) {
//        while (qryUser.next()) {
//            if(qryUser.value(5).toString().length() != 0) {
//                microServicesMain.showMaximized();
//            } else {
//                loginDialog.show();
//            }
//        }
//    } else {
        loginDialog.show();
//    }



    return a.exec();
}

#include "logindialog.h"
//#include "microservicesmain.h"
#include "apimanager.h"
#include <QApplication>
#include "localdb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LocalDb lDB;
    if(!lDB.localConnect()) {
        return 1;
    }

    ApiManager apiMan;
    loginDialog loginDialog;

    QSqlQuery gjejApis =  lDB.select("count(*)","apis","");
    while (gjejApis.next()) {
        if(gjejApis.value(0).toInt() == 0)
            apiMan.show();
        else {
            loginDialog.show();
        }
    }

    return a.exec();
}

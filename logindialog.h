#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMenuBar>
#include "microservicesmain.h"
#include "localdb.h"
#include "crypt.h"

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = nullptr);
    ~loginDialog();

private slots:

    void on_loginBTN_clicked();

    void on_fshiBTN_clicked();

    void on_listSessions_doubleClicked(const QModelIndex &index);

    void customMenuListSessions(QPoint pos);

    void slotLogin(QString);

    void slotFshiSession(QString);

    void on_toolButton_clicked();

    void dataInDaHouse(QByteArray data);
private:
    Ui::loginDialog *ui;
    MicroServicesMain *microServicesMain;
};

#endif // LOGINDIALOG_H

#ifndef MICROSERVICESMAIN_H
#define MICROSERVICESMAIN_H

#include <QMainWindow>
#include <QCloseEvent>
#include "localdb.h"
#include "sdk/httprequestworker.h"

namespace Ui {
class MicroServicesMain;
}

class MicroServicesMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit MicroServicesMain(QWidget *parent = nullptr);
    ~MicroServicesMain();

private slots:
//    void handle_api_result(HttpRequestWorker *worker);

private:
    Ui::MicroServicesMain *ui;
    void closeEvent(QCloseEvent *e);
};

#endif // MICROSERVICESMAIN_H

#include "microservicesmain.h"
#include "ui_microservicesmain.h"

MicroServicesMain::MicroServicesMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MicroServicesMain)

{
    ui->setupUi(this);
}

MicroServicesMain::~MicroServicesMain()
{
    delete ui;
}

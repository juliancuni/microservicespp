#ifndef MICROSERVICESMAIN_H
#define MICROSERVICESMAIN_H

#include <QMainWindow>

namespace Ui {
class MicroServicesMain;
}

class MicroServicesMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit MicroServicesMain(QWidget *parent = nullptr);
    ~MicroServicesMain();

private:
    Ui::MicroServicesMain *ui;
};

#endif // MICROSERVICESMAIN_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->leLon->inputText(125.362569874);
    ui->leLat->inputText(89.7856983);
    if(ui->lineEdit)
    {
        const double val = 89.7856983;
        ui->lineEdit->setValue(val);
        assert(ui->lineEdit->getValue() == val);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit)
    {
        const double val = ui->lineEdit->getValue();
        ui->doubleSpinBox->setValue(val);
    }
}


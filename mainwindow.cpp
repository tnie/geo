#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->leLon->inputText(125.362569874);
    ui->leLat->inputText(89.7856983);
}

MainWindow::~MainWindow()
{
    delete ui;
}


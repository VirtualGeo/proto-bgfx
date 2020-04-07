#ifdef GUI_QT

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    QCoreApplication::setAttribute(Qt::AA_NativeWindows);
//    setAttribute(Qt::WA_NativeWindow, true);
//    setAttribute(Qt::WA_PaintOnScreen, false); // disables qt double buffering
//    setAttribute(Qt::WA_NoSystemBackground, true);
//    setAutoFillBackground(true);

    ui->setupUi(this);

//    ui->widget->setVisible(true);
//    ui->widget->setEnabled(true);
//    ui->widget->show();
//    ui->centralwidget->setVisible(false);
//    ui->widget->setGeometry(0, 0, 100, 100);
//    ui->centralwidget->setVisible(false);
//    ui->centralwidget->setStyleSheet("background-color: orange");
//    ui->widget->setAutoFillBackground(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

#endif

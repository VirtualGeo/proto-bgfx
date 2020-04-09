#ifdef GUI_QT

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "qwidgetbgfx.h"
#include <QLayout>

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

//    QVBoxLayout * layout = new QVBoxLayout;

    m_splitter = new QSplitter(ui->centralwidget);


    QWidgetBgfx * widgetBgfx = new QWidgetBgfx;
    QWidgetBgfx * widgetBgfx2 = new QWidgetBgfx;
    QWidgetBgfx * widgetBgfx3 = new QWidgetBgfx;

//    QLayout * verticalLayout = new QLayout()
    m_splitter->setOrientation(Qt::Orientation::Vertical);
    m_splitter->addWidget(widgetBgfx);
    m_splitter->addWidget(widgetBgfx2);
    m_splitter->addWidget(widgetBgfx3);

    setCentralWidget(m_splitter);
//    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    setFocus();
//    layout->addWidget(m_splitter);

//    ui->centralwidget->setLayout(layout);
//    ui->centralwidget->show();
//    setCentralWidget(ui->centralwidget);
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

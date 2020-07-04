#ifdef GUI_QT

#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "qwidgetbgfx.h"
#include "qopenglwidgetbgfx.h"
#include <QLayout>
#include <QSurface>

MainWindow::MainWindow(QWidget *parent, int nbWidget)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    QCoreApplication::setAttribute(Qt::AA_NativeWindows);
//    setAttribute(Qt::WA_NativeWindow, true);
//    setAttribute(Qt::WA_PaintOnScreen, false); // disables qt double buffering
//    setAttribute(Qt::WA_NoSystemBackground, true);
//    setAutoFillBackground(true);

//    QSurfaceFormat lDefault = QSurfaceFormat::defaultFormat();
//    lDefault.setSwapInterval(0);
//    QSurfaceFormat::setDefaultFormat(lDefault);

    ui->setupUi(this);

//    QVBoxLayout * layout = new QVBoxLayout;
//    ui->centralwidget->resize(1200, 800);


    m_splitter = new QSplitter(ui->centralwidget);

//    QWidgetBgfx * widgetBgfx3 = new QWidgetBgfx;

//    QLayout * verticalLayout = new QLayout()
    m_splitter->setOrientation(Qt::Orientation::Vertical);

//    QWidgetBgfx * widgetBgfx = new QWidgetBgfx;
//    m_splitter->addWidget(widgetBgfx);
    for (int i =0; i <nbWidget; ++i) {
        QWidgetBgfx * widgetBgfx = new QWidgetBgfx;
//        QOpenGLWidgetBgfx * widgetBgfx = new QOpenGLWidgetBgfx;

        m_splitter->addWidget(widgetBgfx);
    }

//    QWidgetBgfx * widgetBgfx2 = new QWidgetBgfx;
//    m_splitter->addWidget(widgetBgfx2);

//    QWidgetBgfx * widgetBgfx3 = new QWidgetBgfx;
//    m_splitter->addWidget(widgetBgfx3);

//    m_splitter->addWidget(widgetBgfx3);
    setCentralWidget(m_splitter);
//    setFocus();


//    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
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

#endif // GUI_QT

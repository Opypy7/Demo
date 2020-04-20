#include "mainview.h"
#include "ui_mainview.h"

#include <QSurfaceFormat>
#include <scenewidget.h>

MainView::MainView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainView)
{
    ui->setupUi(this);
   // setFormatOnOpenGlWidgets();

}

MainView::~MainView()
{
    delete ui;
}

void MainView::setFormatOnOpenGlWidgets()
{
    /// try it run in scenewidget constructor - t might be ok.

   // ui->MainView::openGLWidget_3DScene->setFormat(format); // must be called before the widget or its parent window gets show
   // setContextOnOpenGLWidgets(format);
}


void MainView::on_doubleSpinBox_X_valueChanged(double arg1)
{
    ui->openGLWidget_3DScene->setXPos(arg1);
}

void MainView::on_doubleSpinBox_Y_valueChanged(double arg1)
{
    ui->openGLWidget_3DScene->setYPos(arg1);
}

void MainView::on_doubleSpinBox_Z_valueChanged(double arg1)
{
    ui->openGLWidget_3DScene->setZPos(arg1);
}

void MainView::WireSpinBoxWithOpenGLScene()
{
    connect(ui->openGLWidget_3DScene, SIGNAL(xPosChanged(x)), SLOT(ui->doubleSpinBox_X->setValue(x);));

}

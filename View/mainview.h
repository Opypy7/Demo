#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QSurfaceFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class MainView; }
QT_END_NAMESPACE

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    MainView(QWidget *parent = nullptr);
    ~MainView();

private slots:
    void on_doubleSpinBox_X_valueChanged(double arg1);
    void on_doubleSpinBox_Y_valueChanged(double arg1);
    void on_doubleSpinBox_Z_valueChanged(double arg1);

private:
    Ui::MainView *ui;

    void WireSpinBoxWithOpenGLScene();
    void setFormatOnOpenGlWidgets();
    void setContextOnOpenGLWidgets(QSurfaceFormat format);
    QOpenGLContext *context;
};
#endif // MAINVIEW_H

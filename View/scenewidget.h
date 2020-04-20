#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "Model/Mesh.h"
#include "Model/axis.h"
#include "Model/trianglegeometry.h"
#include "Model/linesgeometry.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class SceneWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SceneWidget(QWidget *parent = 0);
    ~SceneWidget();

    static bool isTransparent() { return transparent; }
    static void setTransparent(bool t) { transparent = t; }

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setXPos(float x);
    void setYPos(float y);
    void setZPos(float z);
    void cleanup();

signals:
    void xPosChanged(float x);
    void yPosChanged(float y);
    void zPosChanged(float z);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:


    bool m_core;
    float xPos;
    float yPos;
    float zPos;
    QPoint m_lastPos;

    TriangleGeometry triangles;
    QOpenGLShaderProgram *program;
    int projMatrixLoc;
    int mvMatrixLoc;
    int normalMatrixLoc;
    int lightPosLoc;
    QMatrix4x4 proj;
    QMatrix4x4 camera;
    QMatrix4x4 world;
    static bool transparent;
};

#endif

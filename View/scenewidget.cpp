#include "scenewidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

bool SceneWidget::transparent = false;

SceneWidget::SceneWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      xPos(0),
      yPos(0),
      zPos(0),
      program(0)
{
    m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    if (transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }
}

SceneWidget::~SceneWidget()
{
    cleanup();
}

QSize SceneWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize SceneWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void limitPos(float &pos)
{
    while (pos < -10.0)
        pos =-10.0;;
    while (pos > 10.0)
        pos=10.0;
}

void SceneWidget::setXPos(float x)
{
    limitPos(x);
    if (xPos != x) {
        xPos = x;
        emit xPosChanged(xPos);
        update();
    }
}

void SceneWidget::setYPos(float y)
{
    limitPos(y);
    if (yPos != y) {
        yPos = y;
        emit yPosChanged(yPos);
        update();
    }
}

void SceneWidget::setZPos(float z)
{
    limitPos(z);
    if (zPos != z) {
        zPos = z;
        emit zPosChanged(zPos);
        update();
    }
}


void SceneWidget::cleanup()
{
    if (program == nullptr)
        return;
    makeCurrent();

  triangles.Clear();
  //  lines.Clear();
    delete program;
    program = 0;
    doneCurrent();
}


static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "in vec3 normal;\n"
    "out vec3 vert;\n"
    "out vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "in highp vec3 vert;\n"
    "in highp vec3 vertNormal;\n"
    "out highp vec4 fragColor;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 0.1, 0.2);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   fragColor = vec4(col, 1.0);\n"
    "}\n";

static const char *vertexShaderSource =
   "attribute vec4 vertex;\n"
   "attribute vec3 normal;\n"
    "varying vec3 vert;\n"
    "varying vec3 vertNormal;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "void main() {\n"
    "   vert = vertex.xyz;\n"
    "   vertNormal = normalMatrix * normal;\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying highp vec3 vert;\n"
   "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, 1.0);\n"
    "}\n";

void SceneWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &SceneWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, transparent ? 0 : 1);

    program = new QOpenGLShaderProgram;
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("normal", 1);
    program->link();

    program->bind();
    projMatrixLoc = program->uniformLocation("projMatrix");
    mvMatrixLoc = program->uniformLocation("mvMatrix");
    normalMatrixLoc = program->uniformLocation("normalMatrix");
    lightPosLoc = program->uniformLocation("lightPos");
    triangles.Create();
    //lines.Create();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    triangles.setupVertexTrianglesAttribs(f);
    //lines.setupVertexLinesAttribs(f);

    camera.setToIdentity();
    camera.translate(0, 0, -10);

    // Light position is fixed.
    program->setUniformValue(lightPosLoc, QVector3D(0, 0, 70));
    program->release();
}



void SceneWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    world.setToIdentity();
//    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
//    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
//    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    triangles.setBinder();
    //lines.setBinder();
    program->bind();
    QMatrix4x4 model ;
    model.setToIdentity();
    model.translate(QVector3D(xPos,yPos,zPos));
    program->setUniformValue(projMatrixLoc, proj );
    program->setUniformValue(mvMatrixLoc, camera * model);
    QMatrix3x3 normalMatrix = world.normalMatrix();
    program->setUniformValue(normalMatrixLoc, normalMatrix);
    triangles.Rander();
    //lines.Rander();
    program->release();


    glEndList();
}

void SceneWidget::resizeGL(int w, int h)
{
    proj.setToIdentity();
    proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void SceneWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void SceneWidget::mouseMoveEvent(QMouseEvent *event)
{
    float speed= 0.01f;
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXPos(xPos + dx*speed);
        setYPos(yPos - dy*speed);
    } else if (event->buttons() & Qt::RightButton) {
        setXPos(xPos + dy*speed);
        setZPos(zPos + dx*speed);
    }
    m_lastPos = event->pos();
}


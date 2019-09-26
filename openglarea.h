#ifndef OPENGLAREA_H
#define OPENGLAREA_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QDebug>

class OpenGLArea : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    OpenGLArea(QWidget* parent = nullptr);
    ~OpenGLArea();

protected:
    void initializeGL() override;
    //void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program;
};

#endif // OPENGLAREA_H

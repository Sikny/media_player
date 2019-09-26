#ifndef GRAPHICSAREA_H
#define GRAPHICSAREA_H

#include <sstream>
#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

class GraphicsArea : public QWidget {
    Q_OBJECT
public:
    enum Shape { Line, Points, Text, Pixmap };

    GraphicsArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setShape(Shape shape);
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setValues(float* data, int length);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Shape shape;
    QPen pen;
    QBrush brush;
    bool antialiased;
    QPixmap pixmap;

    float* values;
    int dataLength;
};

#endif // GRAPHICSAREA_H

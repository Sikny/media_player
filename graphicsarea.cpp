#include "graphicsarea.h"

GraphicsArea::GraphicsArea(QWidget* parent) : QWidget(parent){
    shape = Line;
    antialiased = true;
    values = nullptr;

    pen.setColor(QColor(0, 255, 0));
    brush.setColor(QColor(0, 255, 0));

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);
}

QSize GraphicsArea::sizeHint() const {
    return QSize(330, 200);
}

QSize GraphicsArea::minimumSizeHint() const{
    return QSize(330, 200);
}

void GraphicsArea::setShape(Shape shape){
    this->shape = shape;
    update();
}

void GraphicsArea::setPen(const QPen &pen){
    this->pen = pen;
    update();
}

void GraphicsArea::setBrush(const QBrush &brush){
    this->brush = brush;
    update();
}

void GraphicsArea::setAntialiased(bool antialiased){
    this->antialiased = antialiased;
    update();
}

void GraphicsArea::setValues(float* data, unsigned int length){
    dataLength = static_cast<int>(length);
    float* tmp = new float[width()];
    delete values;
    values = tmp;
    for(int i = 0; i < width(); i++){
        if(i < dataLength){
            values[i] = data[i];
        }
        else values[i] = 100;
    }
    update();
}

void GraphicsArea::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setRenderHint(QPainter::Antialiasing, antialiased);
    painter.save();

    QVector<QLine> lines;
    switch (shape) {
        case Line:
            if(values != nullptr){
                for(int i = 0; i < width(); i++){
                    lines.append(QLine(i, height()-values[i]*20*height(), i, height()));
                }
                painter.drawLines(lines);}
            break;
        case Points:

            //painter.drawPoints(points, 4);
            break;
        case Text:
            //painter.drawText(rect, Qt::AlignCenter, tr("Qt by\nThe Qt Company"));
            break;
        case Pixmap:
            painter.drawPixmap(10, 10, pixmap);
    }
    painter.restore();
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(Qt::NoBrush);
}

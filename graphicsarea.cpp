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

    buildContextMenu();
}

void GraphicsArea::buildContextMenu(){
    actionLine = new QAction(tr("Lines"), this);
    QVariant lineV = qVariantFromValue(Line);
    actionLine->setData(lineV);
    connect(actionLine, SIGNAL(triggered()), this, SLOT(setShape()));
    actionPoints = new QAction(tr("Points"), this);
    QVariant pointsV = qVariantFromValue(Points);
    connect(actionPoints, SIGNAL(triggered()), this, SLOT(setShape()));
    actionPoints->setData(pointsV);
}

QSize GraphicsArea::sizeHint() const {
    return QSize(330, 200);
}

QSize GraphicsArea::minimumSizeHint() const{
    return QSize(330, 200);
}

void GraphicsArea::setShape(){
    QAction* act = qobject_cast<QAction*>(sender());
    this->shape = static_cast<Shape>(act->data().value<Shape>());
    qDebug() << this->shape;
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

void GraphicsArea::setValues(float* data, int length){
    dataLength = length;
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
    QPoint* points;

    switch (shape) {
        case Line:
            if(values != nullptr){
                for(int i = 0; i < width(); i++){
                    lines.append(QLine(i, static_cast<int>(height()-values[i]*20*height()),
                            i, height()));
                }
                painter.drawLines(lines);
            }
            break;
        case Points:
            if(values != nullptr){
                points = new QPoint[width()];
                for(int i = 0; i < width(); i++){
                    points[i] = QPoint(i, static_cast<int>(height()-values[i]*20*height()));
                }
                painter.drawPoints(points, width());
                delete [] points;
            }
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

void GraphicsArea::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu;
    menu.addAction(actionLine);
    menu.addAction(actionPoints);
    menu.exec(event->globalPos());
}

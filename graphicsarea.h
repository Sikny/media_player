#ifndef GRAPHICSAREA_H
#define GRAPHICSAREA_H

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QMenu>
#include <QDebug>

enum Shape { Line, Points, Text, Pixmap };

class GraphicsArea : public QWidget {
    Q_OBJECT
public:

    GraphicsArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:
    void setShape();
    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setValues(float* data, int length);

protected:
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    void buildContextMenu();

    Shape shape;
    QPen pen;
    QBrush brush;
    QAction *actionLine, *actionPoints;
    bool antialiased;
    QPixmap pixmap;

    float* values;
    int dataLength;
};

Q_DECLARE_METATYPE(Shape);

#endif // GRAPHICSAREA_H

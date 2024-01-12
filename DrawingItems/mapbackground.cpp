#include "mapbackground.h"
#include<QDebug>
#include<QColor>
#include<QBrush>
MapBackground::MapBackground(qreal x, qreal y, qreal width, qreal height,
                             const QPixmap &background,QObject *parent1, QGraphicsItem *parent)
    : QObject(parent1),QGraphicsRectItem(x, y, width, height,parent){

    // 设置背景图像
    if(background.isNull())
        backgroundItem = new QGraphicsPixmapItem(this);
    else
        backgroundItem = new QGraphicsPixmapItem(background, this);;
    backgroundItem->setPos(x, y);
    backgroundItem->setZValue(-1);  // 将背景图像放到底层
    setBrush(Qt::transparent);
    setAcceptHoverEvents(true);
}

void MapBackground::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit itemClicked(event);
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Left mouse button clicked at scene position:" << event->scenePos();
        qDebug() << "Local position:" << event->pos();
    } else if (event->button() == Qt::RightButton) {
        qDebug() << "Right mouse button clicked at scene position:" << event->scenePos();
        qDebug() << "Local position:" << event->pos();
    }

    QGraphicsItem::mousePressEvent(event);
}

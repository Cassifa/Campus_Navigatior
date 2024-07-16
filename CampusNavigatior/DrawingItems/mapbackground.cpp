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
    // 将背景图像放到底层
    backgroundItem->setZValue(-1);
    setBrush(Qt::transparent);
    setAcceptHoverEvents(true);
}

bool MapBackground::setImg(QString imgPath){
    QPixmap newBackground(imgPath);
    if (!newBackground.isNull()) {
        backgroundItem->setPixmap(newBackground);
        backgroundItem->setPos(rect().x(), rect().y());
        // 调整背景图像大小以适应MapBackground的大小
        qreal scaleX = rect().width() / newBackground.width();
        qreal scaleY = rect().height() / newBackground.height();
        backgroundItem->setScale(qMin(scaleX, scaleY));
        return true;
    }
    return false;
}

void MapBackground::setEmptyImg(){
    backgroundItem->setPixmap(QPixmap());
}

void MapBackground::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit itemClicked(event);
//    qDebug() << "点了背景" << event->scenePos();
    QGraphicsItem::mousePressEvent(event);
}

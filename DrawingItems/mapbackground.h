#ifndef CUSTOMRECTITEM_H
#define CUSTOMRECTITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include<QGraphicsView>
#include<QObject>
class MapBackground :public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    MapBackground(qreal x, qreal y, qreal width, qreal height, const QPixmap &background,QObject *parent1, QGraphicsItem *parent = nullptr);

signals:
    //点击事件,触发tryAddPoint(QGraphicsSceneMouseEvent *event)
    void itemClicked(QGraphicsSceneMouseEvent *event);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QGraphicsPixmapItem *backgroundItem;
};

#endif // CUSTOMRECTITEM_H

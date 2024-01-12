#ifndef DRAWINGEDGE_H
#define DRAWINGEDGE_H

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QString>
#include<QObject>
#include"./CampusMap/edge.h"
#include<QPropertyAnimation>
class DrawingEdge :public QGraphicsLineItem{
//    Q_OBJECT
public:
    DrawingEdge(const Edge &edge, int penWidth, const QColor &color,
                bool slowDrawing,QGraphicsItem *parent = nullptr);
    //设置是否有绘制动画
//    void setAnimationEnabled(bool enabled);
    //鼠标点击事件
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    // 声明boundingRect函数
    QRectF boundingRect() const override;
    // 声明paint函数
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    int startX,startY;
    int endX,endY;
    QString dist;
    int getPointXId();
    int getPointYId();
signals:
    void edgeClicked(Edge edge);
private:
    int pointXId,pointYId;
    QGraphicsTextItem *textItem;
    QColor lineColor;
    int penWidth;
    bool slowDrawing;
};

#endif // DRAWINGEDGE_H

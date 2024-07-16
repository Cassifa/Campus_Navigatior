#ifndef DRAWINGEDGE_H
#define DRAWINGEDGE_H

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QString>
#include<QObject>
#include"./CampusMap/edge.h"
#include<QPropertyAnimation>
class DrawingEdge :public QObject,public QGraphicsLineItem{
    Q_OBJECT
public:
    DrawingEdge(const Edge &edge, int penWidth, const QColor &color,
                bool slowDrawing,QGraphicsItem *parent = nullptr);

    //设置是否有绘制动画
    //void setAnimationEnabled(bool enabled);

    //鼠标点击悬浮事件
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    //分配id
    static int idCnts;

    int startX,startY;
    int endX,endY;
    int id;
    QString dist;
    int getPointXId();
    int getPointYId();

signals:
//    //点击边,触发删除边事件 void removeEdge(Edge edge);
    void edgeClicked(int startX,int startY,int endX,int endY);

private:
    int pointXId,pointYId;
    //距离信息
    QGraphicsTextItem *textItem;
    //画笔颜色与宽度
    QColor lineColor;
    int penWidth;
    //是否缓慢显示
    bool slowDrawing;
};

#endif // DRAWINGEDGE_H

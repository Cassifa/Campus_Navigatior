#include "drawingpoint.h"
#include"../CampusMap/point.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include<QObject>
//构造函数
DrawingPoint::DrawingPoint(const Point &point,qreal radius,const QColor &color,
                           QGraphicsItem *parent)
    :QGraphicsObject(parent), pointId(point.id), circleRadius(radius), circleColor(color){
    //设置是否为路口节点
    if(point.isHide)
        this->circleName=QString::number(point.id);
    else
        this->circleName=point.name;

    setPos(point.x, point.y);
    // 启用鼠标悬停事件
    setAcceptHoverEvents(true);
    //绘制底部文字
    textItem = new QGraphicsTextItem(circleName, this);
    textItem->setFont(QFont("Arial", 10));
    textItem->setDefaultTextColor(Qt::black);
    qreal textWidth = textItem->boundingRect().width();
    textItem->setPos(-textWidth/2, circleRadius);
}

QRectF DrawingPoint::boundingRect() const{
    // 计算边界矩形的左上角坐标和宽度、高度
    qreal left = -circleRadius - penWidth;
    qreal top = -circleRadius - penWidth;
    qreal diameter = circleRadius * 2 + 2 * penWidth;
    // 创建并返回边界矩形
    return QRectF(left,top, diameter, diameter);
}
void DrawingPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    // 设置画刷颜色并绘制圆
    painter->setPen(QPen(Qt::black, penWidth));
    painter->setBrush(circleColor);
    painter->drawEllipse(QRectF(-circleRadius, -circleRadius, circleRadius * 2, circleRadius * 2));

    // 设置用于绘制文本的字体
    QFont font;
    font.setPixelSize(12);
    painter->setFont(font);
}

//鼠标点击悬浮事件
void DrawingPoint::mousePressEvent(QGraphicsSceneMouseEvent *event){
    // 处理鼠标点击事件
    qDebug() << "圆被点击: " << circleName;
    event->accept();
    emit pointClicked(this->pointId);
}

void DrawingPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    setOpacity(0.8);
    QGraphicsItem::hoverEnterEvent(event);
    event->accept();
}

void DrawingPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    setOpacity(1.0);
    QGraphicsItem::hoverLeaveEvent(event);
    event->accept();
}

//Getter Setter
int DrawingPoint::getId(){
    return pointId;
}

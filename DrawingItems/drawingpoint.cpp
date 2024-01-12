#include "drawingpoint.h"
#include"../CampusMap/point.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

//构造函数
DrawingPoint::DrawingPoint(const Point &point,qreal radius,const QColor &color,QGraphicsItem *parent)
    :QGraphicsObject(parent), pointId(point.id), circleRadius(radius), circleColor(color){
    if(point.isHide)
        this->circleName=QString::number(point.id);
    else
        this->circleName=point.name;
    setPos(point.x, point.y);
    // 启用鼠标悬停事件
    setAcceptHoverEvents(true);

    textItem = new QGraphicsTextItem(circleName, this);
    textItem->setFont(QFont("Arial", 10));
    textItem->setDefaultTextColor(Qt::black);
    qreal textWidth = textItem->boundingRect().width();
    textItem->setPos(-textWidth/2, circleRadius);
}
//
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

    // 在圆的正下方绘制文本
//    painter->drawText(QRectF(-circleRadius, circleRadius, circleRadius * 2, 20), Qt::AlignCenter, circleName);
}

void DrawingPoint::mousePressEvent(QGraphicsSceneMouseEvent *event){
    // 处理鼠标点击事件
    qDebug() << "圆被点击: " << circleName;
    event->accept();
}

void DrawingPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    // 鼠标悬停时改变透明度
    setOpacity(0.8);
    QGraphicsItem::hoverEnterEvent(event);
    event->accept();
}

void DrawingPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    // 鼠标离开时恢复透明度
    setOpacity(1.0);
    QGraphicsItem::hoverLeaveEvent(event);
    event->accept();
}

int DrawingPoint::getId(){
    return pointId;
}

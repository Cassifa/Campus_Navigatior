#include "drawingedge.h"
#include <QDebug>
#include <QString>
#include<QPainter>
#include <QGraphicsSceneMouseEvent>
int DrawingEdge::idCnts = 0;

DrawingEdge::DrawingEdge(const Edge &edge, int penWidth, const QColor &color,
                         bool slowDrawing,QGraphicsItem *parent)
    :QGraphicsLineItem (parent),startX(edge.x.x), startY(edge.x.y), endX(edge.y.x), endY(edge.y.y),id(idCnts++)
            , dist(edge.dist),pointXId(edge.x.id),pointYId(edge.y.id)
        ,lineColor(color),penWidth(penWidth),slowDrawing(slowDrawing){
    // 设置可接受点击
    setFlag(QGraphicsItem::ItemIsSelectable);
    this->setAcceptedMouseButtons(Qt::LeftButton);
    // 启用鼠标悬停事件
    setAcceptHoverEvents(true);
    // 是否慢慢画
//    setAnimationEnabled(slowDrawing);
    setLine(startX,startY,endX,endY);
}

QRectF DrawingEdge::boundingRect() const{
//    qreal extra =penWidth; // 考虑到线条宽度的一半
//    // 使用QGraphicsLineItem的线条坐标计算边界矩形
//    return QRectF(startX, startY, endX - startX, endY - startY)
//        .normalized()
//        .adjusted(-extra, -extra, extra, extra);
    qreal halfPenWidth = penWidth / 2.0+10;
    qreal minX = qMin(startX, endX);
    qreal minY = qMin(startY, endY);
    qreal maxX = qMax(startX, endX);
    qreal maxY = qMax(startY, endY);

    QRectF rect = QRectF(minX, minY, maxX - minX, maxY - minY).normalized();
    rect.adjust(-halfPenWidth, -halfPenWidth, halfPenWidth, halfPenWidth);
    return rect;
}

void DrawingEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //绘制线段
    QPen pen(this->lineColor, penWidth, Qt::SolidLine);
//    setOpacity(0.8);
    painter->setPen(pen);
    painter->drawLine(QLineF(startX, startY, endX, endY));
}

//鼠标事件
void DrawingEdge::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
    Point a(startX,startY),b(endX,endY);
    Edge temp(a,b);
    edgeClicked(startX, startY, endX, endY);
//    qDebug() << "边 Clicked!";
    event->accept();
}

void DrawingEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    // 鼠标悬停时改变透明度
//    this->setPen(QPen(Qt::green));
//    setScale(1.5);
//    setOpacity(1.0);
    event->accept();
}

void DrawingEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    // 鼠标离开时恢复透明度
//    setOpacity(0.8);
//    setScale(1.0);
    event->accept();

}

//Getter Setter
int DrawingEdge::getPointXId(){
    return pointXId;
}
int DrawingEdge::getPointYId(){
    return pointYId;
}

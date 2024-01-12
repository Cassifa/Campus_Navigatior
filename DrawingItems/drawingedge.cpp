#include "drawingedge.h"
#include <QDebug>
#include <QString>
#include<QPainter>
DrawingEdge::DrawingEdge(const Edge &edge, int penWidth, const QColor &color, bool slowDrawing, QGraphicsItem *parent)
    : QGraphicsLineItem(parent), startX(edge.x.x), startY(edge.x.y), endX(edge.y.x), endY(edge.y.y), dist(edge.dist)
        ,lineColor(color),penWidth(penWidth),slowDrawing(slowDrawing){
    // 设置可接受点击
    setFlag(QGraphicsItem::ItemIsSelectable);
    // 启用鼠标悬停事件
    setAcceptHoverEvents(true);
    qDebug()<<"新建了边"<<edge.dist;
    // 是否慢慢画
//    setAnimationEnabled(slowDrawing);
}

QRectF DrawingEdge::boundingRect() const{
    qreal extra =penWidth; // 考虑到线条宽度的一半
    // 使用QGraphicsLineItem的线条坐标计算边界矩形
    return QRectF(startX, startY, endX - startX, endY - startY)
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void DrawingEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //绘制线段
    QPen pen(this->lineColor, penWidth, Qt::SolidLine);
    painter->setPen(pen);
    painter->drawLine(QLineF(startX, startY, endX, endY));
}

void DrawingEdge::setAnimationEnabled(bool enabled) {
}

void DrawingEdge::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
    // 例如，你可以在这里处理线段被点击后的操作
    qDebug() << "Edge Clicked!";
}

void DrawingEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    // 鼠标悬停时改变透明度
    setOpacity(0.8);
    QGraphicsItem::hoverEnterEvent(event);
}

void DrawingEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    // 鼠标离开时恢复透明度
    setOpacity(1.0);
    QGraphicsItem::hoverLeaveEvent(event);

}


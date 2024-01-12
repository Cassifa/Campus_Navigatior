#ifndef DRAWINGPOINT_H
#define DRAWINGPOINT_H
#include"../CampusMap/point.h"
#include <QGraphicsItem>
#include <QPainter>
#include <QMouseEvent>
#include <QFont>
class DrawingPoint:public QGraphicsObject{
    Q_OBJECT
public:
    //根据一个point来构造
    //    int id;
    //    int x,y;
    //    bool isHide;
    //    QString name;
    DrawingPoint(const Point &point,qreal radius,const QColor &color,
                 QGraphicsItem *parent = nullptr);
    // QGraphicsItem 重写
    //优化图像渲染
    QRectF boundingRect() const override;
    //绘制图像
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // 事件处理函数
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    int getId();

signals:
    void pointClicked(int id);
private:
    //申明为常量表达式
    static constexpr qreal penWidth=1;
    //对应点的标号
    int pointId;
    //半径
    qreal circleRadius;
    //填充颜色
    QColor circleColor;
    //名称
    QString circleName;
    //显示文字
    QGraphicsTextItem *textItem;

};

#endif // DRAWINGPOINT_H

#include "edge.h"
#include"point.h"
#include<QtCore>
#include <QString>
Edge::Edge(const Point x,const Point y)
    :x(x),y(y){
    //控制距离精度为两位
    this->dist.setNum(getdist(), 'f', 2);
}

qreal Edge::getdist(){
    qreal dist1=qAbs(x.x-y.x);
    qreal dist2=qAbs(x.y-y.y);
    return qSqrt(dist1*dist1+dist2*dist2);
}

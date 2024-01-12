#ifndef EDGE_H
#define EDGE_H

#include<QtCore>
#include"point.h"
class Edge{
public:
    Edge(const Point x,const Point y);
    //连接的两点
    Point x,y;
    //距离
    QString dist;
private:
    qreal getdist();
};

#endif // EDGE_H

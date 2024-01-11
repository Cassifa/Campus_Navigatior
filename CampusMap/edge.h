#ifndef EDGE_H
#define EDGE_H

#include<QtCore>
class Edge{
public:
    Edge(int x,int y,int color=1);
    //连接两点的编号
    int x,y;
    int color;
};

#endif // EDGE_H

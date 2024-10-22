#ifndef POINT_H
#define POINT_H
#include<QtCore>
class Point{
public:
    Point(qreal x,qreal y,int id,bool isHide,QString name=nullptr);
    //拷贝构造
    Point(Point *point);
    Point();
    Point(qreal a,qreal b);

    //节点标号，两种点都有
    int id;
    //坐标
    qreal x,y;
    //是否隐藏：路口节点在导航时不显示
    bool isHide;
    //节点名字
    QString name;
    void showInfo();

    //重写比较函数
    bool operator==(const Point &other) const {
        return id == other.id;
    }
    bool operator<(const Point &other)const{
        return id<other.id;
    }

};

#endif // POINT_H

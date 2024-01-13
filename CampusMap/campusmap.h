#ifndef CAMPUSMAP_H
#define CAMPUSMAP_H

#include"edge.h"
#include"point.h"
#include<QtCore>
class CampusMap{
public:
    CampusMap();
    ~CampusMap();
    //添加与保存地图
    static CampusMap* addMap(QString path);
    void saveMap();

    //加入边与点
    void pushPoint(Point *point);
    void pushEdge(Edge *edge);

    //读取信息
    QVector<Point*>* getPointsList();
    QVector<Edge*>* getEdgesList();
    Point* getPointAt(int x);

    QString getName();
    QString getPath();
    void setId(int id);
    int getId();
private:
    //地图编号与名称
    int id=0;
    QString name;
    QString pathName;
    //拥有的所有边与点
    QVector<Point*> *points;
    QVector<Edge*> *edges;

    //创建连接两点的边
    Edge* getEdge(int point1Id,int point2Id);

public:
    //重载哈希函数
    friend uint qHash(const CampusMap &obj) {
        return qHash(obj.id);
    }
    //重载小于号
    bool operator<(const CampusMap &other) const {
        // 比较 id 属性
        return id < other.id;
    }
};

#endif // CAMPUSMAP_H

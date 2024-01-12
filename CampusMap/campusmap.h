#ifndef CAMPUSMAP_H
#define CAMPUSMAP_H

#include"edge.h"
#include"point.h"
#include<QtCore>
class CampusMap{
public:
    CampusMap();
    ~CampusMap();
    //重载哈希函数
    friend uint qHash(const CampusMap &obj) {
        return qHash(obj.id);
    }
    //重载小于号
    bool operator<(const CampusMap &other) const {
        // 比较 id 属性
        return id < other.id;
    }
    static CampusMap* addMap(QString path);
    static void saveMap(QString path);
    QString getName();
    void setId(int id);
    int getId();
    QVector<Point*>* getPointsList();
    QVector<Edge*>* getEdgesList();
    void pushPoint(Point *point);
    void pushEdge(Edge *edge);
    Point* getPointAt(int x);
private:
    //地图编号
    int id=0;
    QString name;
    QVector<Point*> *points;
    QVector<Edge*> *edges;
    Edge* getEdge(int point1Id,int point2Id);
};

#endif // CAMPUSMAP_H

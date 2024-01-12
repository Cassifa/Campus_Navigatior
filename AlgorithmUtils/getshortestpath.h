#ifndef GETSHORTESTPATH_H
#define GETSHORTESTPATH_H

#include<QtCore>
#include <qvector.h>
#include"../CampusMap/edge.h"
#include"../CampusMap/point.h"
#include"../CampusMap/campusmap.h"
#include"./DrawingItems/drawingedge.h"
enum SearchAlgorithm{
    BFS,AStar,Heap,Dijkstra,SPFA,Gene,Floyd,Unchoice
};
class GetShortestPath{
public:
    GetShortestPath();
    //设置参数
    void setSearchAlgorithm(SearchAlgorithm algorithmType);
    void setStart(Point *start);
    void setEnd(Point *end);
    void setNeedShowPath(bool need);
    void setAutoNext(bool need);
    void setMap(CampusMap *map);
    void pushDrawItem(DrawingEdge *edge);

    //获取结果
    bool getAcachieveAble();
    QVector<Edge*> getShorestPath();
    QVector<QVector<Edge*>> getPaths();
    //最短距离,起点,终点
    QString getOutpInfo();
    //最短路走法
    QString getOutpPath();
    QVector<DrawingEdge*> getDrawnEdges();

    void init(CampusMap *map);
    bool tryPushPoint(Point *point);

private:
    //当前使用的算法,枚举
    SearchAlgorithm nowUsingAlgorithm=Unchoice;
    //起点终点
    Point *start,*end;
    //是否展示搜索过程
    bool needShowPath;
    //是否自动下一步
    bool autoNext;

    //使用的地图
    CampusMap *map;

    //是否可达
    bool acachieveAble;
    //最短距离
    qreal minDist;
    //每次搜索新加入的边
    QVector<QVector<Edge*>> paths;
    //最短路
    QVector<Edge*> shorestPath;
    QString outpInfo;
    QString outpPath;
    //已经绘制的边
    QVector<DrawingEdge*> drawnEdges;


    Q_PROPERTY(QVector<Edge *> getShorestPath READ getGetShorestPath WRITE setGetShorestPath NOTIFY getShorestPathChanged)
};

#endif // GETSHORTESTPATH_H

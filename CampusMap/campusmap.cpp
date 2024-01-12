#include "campusmap.h"
#include"edge.h"
#include"point.h"
#include<QtCore>
#include<QFile>
#include<iostream>
#include<QDebug>
CampusMap::CampusMap(){
    this->points=new QVector<Point*>;
    this->edges=new QVector<Edge*>;
}
CampusMap::~CampusMap(){
    for(int i=0;i<points->size();i++)
        delete points->at(i);
    for(int i=0;i<edges->size();i++)
        delete edges->at(i);
    delete points;
    delete edges;
}

/*加载一张地图
 * 一张地图存档格式：
     名字
     x y(点位置) 节点编号 名称 是否隐藏(是否是辅助节点)
     -1 -1
     x y 边
     [end]
*/
CampusMap* CampusMap::addMap(QString path){
    CampusMap *nowMap=new CampusMap();
    QFile file(path);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        //打卡文件并设置编码格式
        QTextStream in(&file);
        in.setCodec("UTF-8");
        in.skipWhiteSpace();

        //读取存档
        //存地图名称
        in>>nowMap->name;
        //读取点
        while(!in.atEnd()){
            int a,b;
            in>>a>>b;
            if(a==-1)break;
            int id;QString name; int hide;
            in>>id>>hide;
            if(hide)
                nowMap->points->push_back(new Point(a,b,id,hide));
            else{
                in>>name;
                nowMap->points->push_back(new Point(a,b,id,hide,name));
            }

        }
        //读取边
        while (!in.atEnd()) {
            int a,b;
            in>>a>>b;
            if(a==-1)break;
            nowMap->edges->append(nowMap->getEdge(a,b));
        }

        //展示存储效果
//        qDebug()<<"地图名"<<nowMap->name<<endl;
//        qDebug()<<"点：";
//        for(int i=0;i<nowMap->points->size();i++)
//               nowMap->points->at(i)->showInfo();
//        for(int i=0;i<nowMap->edges->size();i++)
//            qDebug()<<nowMap->edges->at(i)->x<<nowMap->edges->at(i)->y;

        file.close();
    }
    return nowMap;
}

/*将当前信息保存
 * 一张地图存档格式：
     名字
     x y(点位置) 节点编号 名称 是否隐藏(是否是辅助节点)
     -1 -1
     x y 边
     [end]
*/
void CampusMap::saveMap(QString path){

    Q_UNUSED(path);
}


//根据点编号建立一条边
Edge* CampusMap::getEdge(int point1Id, int point2Id){
    Point *a=new Point(),*b=new Point();
    auto list=this->getPointsList();
    for(int i=0;i<list->size();i++){
        auto nowPoint=list->at(i);
        if(nowPoint->id==point1Id){
            a->id=nowPoint->id;
            a->x=nowPoint->x;
            a->y=nowPoint->y;
            a->name=nowPoint->name;
            a->isHide=nowPoint->isHide;
        }
        else if(nowPoint->id==point2Id){
            b->id=nowPoint->id;
            b->x=nowPoint->x;
            b->y=nowPoint->y;
            b->name=nowPoint->name;
            b->isHide=nowPoint->isHide;
        }
        else if(a->id!=-1&&b->id!=-1)break;
    }
    Edge* result = nullptr;
    if (a!=nullptr&&b!=nullptr) result = new Edge(*a, *b);
    delete a;
    delete b;
    return result;
}


//Getter Setter方法
QVector<Point *> *CampusMap::getPointsList(){
    return this->points;
}
QVector<Edge *> *CampusMap::getEdgesList(){
    return this->edges;
}
Point *CampusMap::getPointAt(int x){
    if(x>=points->size())return nullptr;
    return points->at(x);
}

QString CampusMap::getName(){
    return name;
}
void CampusMap::setId(int id){
    this->id=id;
}
int CampusMap::getId(){
    return id;
}
void CampusMap::pushPoint(Point *point){
    this->points->append(point);
}
void CampusMap::pushEdge(Edge *edge){
    this->edges->append(edge);
}


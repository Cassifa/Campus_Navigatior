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
            nowMap->edges->append(new Edge(a,b));
        }

        //展示存储效果
        qDebug()<<"地图名"<<nowMap->name<<endl;
        qDebug()<<"点：";
        for(int i=0;i<nowMap->points->size();i++)
               nowMap->points->at(i)->showInfo();
        for(int i=0;i<nowMap->edges->size();i++)
            qDebug()<<nowMap->edges->at(i)->x<<nowMap->edges->at(i)->y;

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

QVector<Point *> *CampusMap::getPointsList(){
    return this->points;
}

QVector<Edge *> *CampusMap::getEdgesList(){
    return this->edges;
}

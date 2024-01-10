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
    delete points;
    delete edges;
}

/*一张地图存档格式：
     名字
     x y(点位置) 节点编号 名称 是否隐藏(是否是辅助节点)
     -1 -1
     x y 边
     [end]
*/
CampusMap* CampusMap::addMap(QString path){
    qDebug()<<path<<endl;
    CampusMap *nowMap=new CampusMap();
    QFile file(path);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        //打卡文件并设置编码格式
        QTextStream in(&file);
        in.setCodec("UTF-8");
        in.skipWhiteSpace();
        //读取存档
        in>>nowMap->name;
        //读取点
        while(true){
            int a,b;
            in>>a>>b;
            if(a==-1)break;
            int id;QString name; int hide;
            in>>id>>name>>hide;
            nowMap->points->push_back(new Point(a,b,id,name,hide));

        }
        //读取边
        while (!in.atEnd()) {
            int a,b;
            in>>a>>b;
            nowMap->edges->append(new Edge(a,b));
        }

        //展示存储效果
        qDebug()<<"地图名"<<nowMap->name<<endl;
        qDebug()<<"点：\n";
        for(int i=0;i<nowMap->points->size();i++)
               nowMap->points->at(i)->showInfo();
        for(int i=0;i<nowMap->edges->size();i++)
            qDebug()<<nowMap->edges->at(i)->x<<" "<<nowMap->edges->at(i)->y<<endl;


        file.close();
    }
    return nowMap;
}

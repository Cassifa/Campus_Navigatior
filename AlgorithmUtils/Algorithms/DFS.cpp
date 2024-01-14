#pragma once
#include "../../CampusMap/campusmap.h"
#include "../../CampusMap/edge.h"
#include "../../CampusMap/point.h"
#include<QtCore>
#include <queue>
#include<map>
#include<QDebug>
#define PII pair<qreal,int>
const int N=1e5;
int d[N],st[N];
int e[N],ne[N],h[N],idx;
qreal w[N];
std::map<int,Edge*> mp;
void add(int a,int b,qreal c){
    e[idx]=b,ne[idx]=h[a],h[a]=idx++;
    w[idx-1]=c;
}
void dfs(int now,int aim,qreal nowDist,int fa,
         bool &acachieveAble,qreal &minDist,
         QVector<Edge*> nowPath,QVector<QVector<Edge*>> &paths,QVector<Edge*> &shorestPath){
    if(now==aim){
        acachieveAble=true;
        paths.append(nowPath);
        if(nowDist<minDist){
            minDist=nowDist;
            shorestPath=nowPath;
        }
    }
    //所搜所有边
    for(int i=h[now];~i;i=ne[i]){
        int j=e[i];
        if(j==fa)continue;
        nowPath.append(mp[i]);
        dfs(j,aim,nowDist+w[i],now,acachieveAble,minDist,nowPath,paths,shorestPath);
        nowPath.removeLast();
    }
}
void dfs(Point* start,Point* end,
         QVector<Edge*>* edgesList,
         QVector<Point*>* pointsList,
         bool &acachieveAble,
         qreal &minDist,QVector<QVector<Edge*>> &paths,QVector<Edge*> &shorestPath){
    //初始化点
    for(int i=0;i<pointsList->size();i++) h[i]=-1;
    //初始化链式前向星 双向边
    //建立链式前向星与指针对应关系
    for(int i=0;i<edgesList->size();i++){
        auto t=edgesList->at(i);
        mp[idx]=t;
        add(t->x.id,t->y.id,t->dist.toDouble());
        mp[idx]=t;
        add(t->y.id,t->x.id,t->dist.toDouble());
    }
    //初始化参数
    acachieveAble=false;minDist=1e18;
    QVector<Edge*> utilPath;
    dfs(start->id,end->id, 0.0,-1, acachieveAble,minDist, utilPath,paths,shorestPath);
}

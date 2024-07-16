#pragma once
#include "../../CampusMap/campusmap.h"
#include "../../CampusMap/edge.h"
#include "../../CampusMap/point.h"
#include<QtCore>
#include <queue>
#include<map>
#include<QDebug>
#define PII pair<qreal,int>
#define N 100000
extern int d[N],st[N];
extern int e[N],ne[N],h[N],idx;
extern qreal w[N];
extern std::map<int,Edge*> mp;
extern void add(int a,int b,qreal c);
bool dfs(int now,int aim,int fa,qreal nowDist,qreal finalDist,QVector<Edge*> &shorestPath){
    //搜到目标且距离最小
    if(now==aim&&(abs(nowDist-finalDist)<0.01*finalDist))return true;
    //所搜所有边
    for(int i=h[now];~i;i=ne[i]){
        int j=e[i];
        if(j==fa)continue;
        if(nowDist+w[i]>finalDist*1.05)continue;
        shorestPath.append(mp[i]);
        if(dfs(j,aim,now,nowDist+w[i],finalDist,shorestPath))
            return true;
        shorestPath.removeLast();
    }
    return false;
}
void spfa(Point* start,Point* end,
         QVector<Edge*>* edgesList,
         QVector<Point*>* pointsList,
         bool &acachieveAble,
         qreal &minDist,QVector<QVector<Edge*>> &paths,QVector<Edge*> &shorestPath){

    //初始化点
    for(int i=0;i<pointsList->size();i++){
        d[i]=0x3f3f3f3f;
        st[i]=0;
        h[i]=-1;
    }

    //初始化链式前向星 双向边
    //建立链式前向星与指针对应关系
    for(int i=0;i<edgesList->size();i++){
        auto t=edgesList->at(i);
        mp[idx]=t;
        add(t->x.id,t->y.id,t->dist.toDouble());
        mp[idx]=t;
        add(t->y.id,t->x.id,t->dist.toDouble());
    }
    //存搜索过程
    QVector<Edge*> newVector;

    int q[N],hh=0,tt=0;
    d[start->id]=0;q[0]=start->id;
    while(hh<=tt){
        int t=q[hh++];
        st[t]=0;//出队
        for(int i=h[t];i!=-1;i=ne[i]){
            int j=e[i];
            if(d[j]>d[t]+w[i]){//更新
                d[j]=d[t]+w[i];
                if(!st[j]){//入队
                    newVector.append(mp[i]);
                    q[++tt]=j;
                    st[j]=1;
                }
            }
        }
        if(newVector.size())
            paths.append(newVector);
        newVector.clear();
    }

    if(d[end->id]>0x3f3f3f3f/2)acachieveAble=false;
    else acachieveAble=true;
    minDist=d[end->id];;
    //dfs找路径
    if(acachieveAble)dfs(start->id,end->id,-1,0.0,minDist,shorestPath);
}

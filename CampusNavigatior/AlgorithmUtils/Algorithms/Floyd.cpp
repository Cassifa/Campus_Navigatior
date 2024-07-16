#include "../../CampusMap/campusmap.h"
#include "../../CampusMap/edge.h"
#include "../../CampusMap/point.h"
#include<QtCore>
#include<iostream>
#define N 1000
#define M 100000
extern int e[M],ne[M],h[M],idx;
extern qreal w[M];
qreal tableDist[N][N];
extern  std::map<int,Edge*> mp;
extern bool dfs(int now,int aim,int fa,qreal nowDist,qreal finalDist,QVector<Edge*> &shorestPath);
extern void add(int a,int b,qreal c);
void floyd(Point* start,Point* end,
         QVector<Edge*>* edgesList,
         QVector<Point*>* pointsList,
           bool &acachieveAble,
         qreal &minDist,QVector<QVector<Edge*>> &paths,QVector<Edge*> &shorestPath){

    qreal inf=1e17;
    //初始化dfs
    for(int i=0;i<pointsList->size();i++)h[i]=-1;
    for(int i=0;i<edgesList->size();i++){
        auto t=edgesList->at(i);
        mp[idx]=t;
        add(t->x.id,t->y.id,t->dist.toDouble());
        mp[idx]=t;
        add(t->y.id,t->x.id,t->dist.toDouble());
    }
    //初始化
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            tableDist[i][j]=1e18;
    for(int i=0;i<edgesList->size();i++){
        Edge *now=edgesList->at(i);
        int x=now->x.id,y=now->y.id;
        tableDist[x][y]=qMin(tableDist[x][y],now->dist.toDouble());
        tableDist[y][x]=qMin(tableDist[y][x],now->dist.toDouble());
    }
    int size=pointsList->size();
    //跑Floyd
    for(int k=0;k<size;k++)
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                tableDist[i][j]=qMin(tableDist[i][j],tableDist[i][k]+tableDist[k][j]);

    minDist=tableDist[start->id][end->id];
    if(minDist>inf)acachieveAble=false;
    else acachieveAble=true;

    if(acachieveAble)dfs(start->id,end->id,-1,0.0,minDist,shorestPath);
    paths.append(shorestPath);
}

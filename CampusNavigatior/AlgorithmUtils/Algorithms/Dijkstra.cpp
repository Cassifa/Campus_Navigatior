#include "../../CampusMap/campusmap.h"
#include "../../CampusMap/edge.h"
#include "../../CampusMap/point.h"
#include<QtCore>
#include<iostream>
#define N 1000
#define M 100000
extern int st[M];
extern int e[M],ne[M],h[M],idx;
extern qreal w[M];
extern qreal tableDist[N][N];
qreal pointDist[N];
extern  std::map<int,Edge*> mp;
extern bool dfs(int now,int aim,int fa,qreal nowDist,qreal finalDist,QVector<Edge*> &shorestPath);
extern void add(int a,int b,qreal c);
//返回是哪条边连接了x,y
Edge* ask(int x,int y,QVector<Edge*>* edgesList){
    for(int i=0;i<edgesList->size();i++){
        int a=edgesList->at(i)->x.id,b=edgesList->at(i)->y.id;
        if((a==x&&b==y)||(a==y&&b==x))return edgesList->at(i);
    }
    return nullptr;
}
Edge* get(int x,QVector<Edge*>* edgesList){
    for(int i=0;i<edgesList->size();i++){
        int a=edgesList->at(i)->x.id,b=edgesList->at(i)->y.id;
        if(a!=x&&b!=x)continue;
        if(a==x&&st[b])return edgesList->at(i);
        if(b==x&&st[a])return edgesList->at(i);
    }
    return nullptr;
}
void dijkstra(Point* start,Point* end,
         QVector<Edge*>* edgesList,
         QVector<Point*>* pointsList,
          bool &acachieveAble,
         qreal &minDist,QVector<QVector<Edge*>> &paths,QVector<Edge*> &shorestPath){

//    qreal tableDist[N][N];
//    qreal pointDist[N];

    qreal inf=1e9;
    //初始化dfs
    for(int i=0;i<=pointsList->size();i++){
        h[i]=-1;
        st[i]=0;
    }
    for(int i=0;i<edgesList->size();i++){
        auto t=edgesList->at(i);
        mp[idx]=t;
        add(t->x.id,t->y.id,t->dist.toDouble());
        mp[idx]=t;
        add(t->y.id,t->x.id,t->dist.toDouble());
    }

    //初始化距离
    for(int i=0;i<=pointsList->size();i++){
        pointDist[i]=inf;
        for(int j=0;j<=pointsList->size();j++)
            tableDist[i][j]=tableDist[j][j]=inf;
    }
    for(int i=0;i<edgesList->size();i++){
        Edge *edge=edgesList->at(i);
        tableDist[edge->x.id][edge->y.id]=edge->dist.toDouble();
        tableDist[edge->y.id][edge->x.id]=edge->dist.toDouble();
    }

    QVector<Edge*> newVector;
    //执行dijkstra
    pointDist[start->id]=0;
    for(int i=0;i<pointsList->size();i++){
        int t=-1;
        for(int j=0;j<pointsList->size();j++)
            if(!st[j]&&(t==-1||pointDist[t]>pointDist[j]))
                t=j;
        st[t]=1;
        //标记每次被确认的边
        if(t!=start->id){
            Edge *edge=get(t,edgesList);
            if(edge!=nullptr)
                newVector.append(edge);
        }
        for(int j=0;j<pointsList->size();j++){
            if(pointDist[j]>pointDist[t]+tableDist[t][j]){
                pointDist[j]=pointDist[t]+tableDist[t][j];
            }
        }
        if(newVector.size())paths.append(newVector);
        newVector.clear();
    }

    //处理结果
    minDist=pointDist[end->id];
    if(pointDist[end->id]>inf)acachieveAble=false;
    else{
        acachieveAble=true;
        dfs(start->id,end->id,-1,0.0,minDist,shorestPath);
    };
}

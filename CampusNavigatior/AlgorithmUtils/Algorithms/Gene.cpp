#include "../../CampusMap/campusmap.h"
#include "../../CampusMap/edge.h"
#include "../../CampusMap/point.h"
#include<QtCore>
#include<queue>
#define N 100000
extern int d[N],st[N];
extern int e[N],ne[N],h[N],idx;
extern qreal w[N];
extern  std::map<int,Edge*> mp;
extern void add(int a,int b,qreal c);
extern bool dfs(int now,int aim,int fa,qreal nowDist,qreal finalDist,QVector<Edge*> &shorestPath);
void gene(Point* start,Point* end,
          QVector<Edge*>* edgesList,
          QVector<Point*>* pointsList,
          bool &acachieveAble,
          qreal &minDist,QVector<QVector<Edge*>> &paths,QVector<Edge*> &shorestPath){


    //初始化
    for(int i=0;i<pointsList->size();i++){
        d[i]=0x3f3f3f3f;h[i]=-1;st[i]=0;
    }
    for(int i=0;i<edgesList->size();i++){
        auto t=edgesList->at(i);
        mp[idx]=t;
        add(t->x.id,t->y.id,t->dist.toDouble());
        mp[idx]=t;
        add(t->y.id,t->x.id,t->dist.toDouble());
    }

    //存搜索过程
//    QVector<Edge*> newVector;
//    std::queue<std::pair<int,qreal>> q1,q2;
//    q1.push({start->id,0.0});
//    q2.push({end->id,0.0});
//    st[start->id]=st[end->id]=1;
//    while(q1.size()||q2.size()){
//        if(q1.size()>q2.size()){
//            auto t=q1.top();q1.pop();
//             for(int i=h[t.first];~i;i=ne[i]){
//                 int j=e[i];
//                 if(st[j])
//             }
//        }
//        else {

//        }
//    }
}

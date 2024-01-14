#include "../../CampusMap/campusmap.h"
#include "../../CampusMap/edge.h"
#include "../../CampusMap/point.h"
#include<QtCore>
#include <queue>
#include<set>
#define NN 100000
extern int st[NN];
extern int d[NN];
extern int e[NN],ne[NN],h[NN],idx;
extern qreal w[NN];
extern std::map<int,Edge*> mp;
extern void add(int a,int b,qreal c);
extern bool dfs(int now,int aim,int fa,qreal nowDist,qreal finalDist,QVector<Edge*> &shorestPath);
//获取估值:与目的地的欧几里得距离
qreal f(int pointId,int aimId,QVector<Point*>* &pointList){
    Point *a=nullptr,*b=nullptr;
    for(int i=0;i<pointList->size();i++){
        auto t=pointList->at(i);
        if(t->id==pointId)a=t;
        else if(t->id==aimId)b=t;
        if(a!=nullptr&&b!=nullptr)break;
    }
    if(a==nullptr||b==nullptr)
        return 0;
    qreal valueDist=qSqrt(qAbs(a->x-b->x)*qAbs(a->x-b->x)+qAbs(a->y-b->y)*qAbs(a->y-b->y));
    return valueDist;
}
extern Edge* get(int x,QVector<Edge*>* edgesList);
void aStar(Point* start,Point* end,
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
    QVector<Edge*> newVector;
    //存放数据的小根堆
    std::priority_queue<std::pair<std::pair<qreal,qreal>,int>,
            std::vector<std::pair<std::pair<qreal,qreal>,int>>,
            std::greater<std::pair<std::pair<qreal,qreal>,int>>> q;

    //执行AStar
    q.push({{0.0,0.0},start->id});
    d[start->id]=0;
    st[start->id]=1;
    while(q.size()){
        std::pair<std::pair<qreal,qreal>,int> t=q.top();q.pop();
        int pointId=t.second;
        st[pointId]=1;
        if(pointId==end->id)break;
        //拓展节点
        for(int i=h[pointId];~i;i=ne[i]){
            int j=e[i];
            if(st[j])continue;
            //边距离＋更新点的实际距离+估值
            d[j]=d[pointId]+w[i];
            if(j==end->id)break;
            std::pair<std::pair<qreal,qreal>,int> item={ {d[j]+f(j,end->id,pointsList),d[j]} ,j};
            q.push(item);

            //获取连接被拓展节点的边
            Edge *edge=get(pointId,edgesList);
            if(edge!=nullptr)
                newVector.append(edge);
        }
        if(newVector.size()){
            paths.append(newVector);
            newVector.clear();
        }
    }
    minDist=d[end->id];
    if(minDist>0x3f3f3f3f/2)acachieveAble=false;
    else{
        acachieveAble=true;
        dfs(start->id,end->id,-1,0.0,minDist,shorestPath);
    }
}

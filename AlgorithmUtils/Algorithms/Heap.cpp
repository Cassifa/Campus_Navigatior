#include "../../CampusMap/campusmap.h"
#include "../../CampusMap/edge.h"
#include "../../CampusMap/point.h"
#include<QtCore>
#include <queue>
#include<map>
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
bool dfs(int now,int aim,int fa,qreal nowDist,qreal finalDist,QVector<Edge*> &shorestPath){
    //搜到目标且距离最小
    if(now==aim&&(abs(nowDist-finalDist)<0.1*finalDist))return true;
    //所搜所有边
    for(int i=h[now];~i;i=ne[i]){
        int j=e[i];
        if(j==fa)continue;
        if(nowDist+w[i]>finalDist*1.05)continue;
        shorestPath.append(mp[i]);
        if(dfs(j,aim,now,nowDist+w[i],finalDist,shorestPath))
            return true;
        shorestPath.pop_back();
    }
    return false;
}
void heap(Point* start,Point* end,
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

    d[start->id]=0;
    std::priority_queue<std::PII, std::vector<std::PII>, std::greater<std::PII>> heap;
    QVector<Edge*> newVector;
    heap.push({0,start->id});
    while(heap.size()){
        auto t=heap.top();heap.pop();
        int ver=t.second;
        if(st[ver]) continue;
        st[ver]=1;
        //找到了目标
        if(ver==end->id)break;
        for(int i=h[ver];i!=-1;i=ne[i]){
            int j=e[i];
            if(d[j]>d[ver]+w[i]){
                newVector.append(mp[i]);
                d[j]=d[ver]+w[i];
                heap.push({d[j],j});
            }
        }
        paths.append(newVector);
        newVector.resize(0);
    }

    if(d[end->id]==0x3f3f3f3f)acachieveAble=false;
    else minDist=d[end->id];
    //dfs找路径
    dfs(start->id,end->id,-1,0.0,minDist,shorestPath);
    for(int i=0;i<shorestPath.size();i++)
            shorestPath.at(i)->show();
}
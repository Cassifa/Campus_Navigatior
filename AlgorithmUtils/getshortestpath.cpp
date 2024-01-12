#include "getshortestpath.h"
GetShortestPath::GetShortestPath(){
}

void GetShortestPath::init(CampusMap *map){
    //设置地图
    this->map=map;
    nowUsingAlgorithm=Unchoice;
    //展示搜索路径
    needShowPath=false;
    //是否自动下一步
    autoNext=false;
    //重置起点终点
    this->start=nullptr;
    this->end=nullptr;
    //重置输出
    outpInfo="";
    outpInfo="";
    //重置最短路
    this->minDist=-1.0;
    this->acachieveAble=false;
    this->shorestPath.resize(0);
    this->paths.resize(0);
}

//尝试加入待搜索队列
bool GetShortestPath::tryPushPoint(Point *point){
    if(start==nullptr){
        start=point;
    }
    else if(end==nullptr&&start->id!=point->id) {
        //不允许选相同的点
        end=point;
    }
    else {
        return false;
    }
    return true;
}

//获取每次搜索路径
QVector<QVector<Edge *> > GetShortestPath::getPaths(){
    return this->paths;
}
//获取每次搜索结果
QVector<Edge *> GetShortestPath::getShorestPath(){
    return shorestPath;
}
//获取需要显示的计算信息:起点终点 距离 不可达 空
QString GetShortestPath::getOutpInfo(){
    //还没设置
    if(this->start==nullptr) return "";
    QString ans;
    if(this->minDist>0)ans=QString::number(minDist);
    ans+=" 起点： "+start->name;
    if(this->end!=nullptr){
        ans+=" 终点： "+end->name;
    }
    return ans;
}
//获取最短路信息/空
QString GetShortestPath::getOutpPath(){
    if(this->shorestPath.size()==0) return "";
}
//获取已经绘制的QItems
QVector<DrawingEdge *> GetShortestPath::getDrawnEdges(){
    return this->drawnEdges;
}
bool GetShortestPath::getAcachieveAble(){
    return acachieveAble;
}


//设置参数函数
void GetShortestPath::setSearchAlgorithm(SearchAlgorithm algorithmType){
    nowUsingAlgorithm=algorithmType;
}
void GetShortestPath::setStart(Point *start){
    this->start=start;
}
void GetShortestPath::setEnd(Point *end){
    this->end=end;
}
void GetShortestPath::setNeedShowPath(bool need){
    needShowPath=need;
}
void GetShortestPath::setAutoNext(bool need){
    autoNext=need;
}
void GetShortestPath::setMap(CampusMap *map){
    this->map=map;
}
void GetShortestPath::pushDrawItem(DrawingEdge *edge){
    this->drawnEdges.append(edge);
}

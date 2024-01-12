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
    this->minDist=0.0;
    this->acachieveAble=false;
    this->shorestPath.resize(0);
    this->paths.resize(0);
}

//获取最短路列表
QVector<Edge *> GetShortestPath::getShorestPath(){
    return this->shorestPath;
}
//获取每次搜索路径
QVector<QVector<Edge *> > GetShortestPath::getPaths(){

}

//获取需要显示的计算信息:起点终点 距离 不可达 空
QString GetShortestPath::getOutpInfo(){
    if(this->start==nullptr) return "";
}
//获取最短路信息/空
QString GetShortestPath::getOutpPath(){
    if(this->shorestPath.size()==0) return "";
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

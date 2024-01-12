#include "point.h"
#include<QtCore>
#include<QDebug>
Point::Point(qreal x,qreal y,int id,bool isHide,QString name){
    this->x=x;
    this->y=y;
    this->name=name;
    this->isHide=isHide;
    this->id=id;

}

Point::Point(Point *point){
    this->id=point->id;
    this->x=point->x;
    this->y=point->y;
    this->name=point->name;
    this->isHide=point->isHide;
}

Point::Point(){
    this->id=-1;
}

void Point::showInfo(){
    qDebug()<<"名称："<<name;
    qDebug()<<x<<" "<<y;
    qDebug()<<id<<"是否隐藏："<<isHide;
}

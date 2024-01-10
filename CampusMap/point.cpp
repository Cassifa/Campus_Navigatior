#include "point.h"
#include<QtCore>
#include<QDebug>
Point::Point(int x,int y,int id,QString name,bool isHide){
    this->x=x;
    this->y=y;
    this->name=name;
    this->isHide=isHide;
    this->id=id;

}

void Point::showInfo(){
    qDebug()<<"名称："<<name;
    qDebug()<<x<<" "<<y;
    qDebug()<<id<<"是否隐藏："<<!isHide;
}

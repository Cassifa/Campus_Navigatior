#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"CampusMap/campusmap.h"
#include"./DrawingItems/drawingedge.h"
#include"./DrawingItems/drawingpoint.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //当前使用功能 0:导航 1:编辑
    int nowView=0;
    //当前使用地图的编号
    int usingMap=0;
    //当前地图
    QVector<CampusMap*> *maps;
    //当前需要绘制的边
//    DrawingEdges *drawingEdges=new DrawingEdges();
    //当前需要绘制的一类点:标志建筑
    QVector<DrawingPoint*> points1;
    //需要绘制的二类点：节点
    QVector<DrawingPoint*> points2;

    //三个工具栏目
    QAction *navtionTool;
    QAction *modiTool;
    QAction *addTool;
    QGraphicsScene *mapScene;

    //初始化导航栏
    void initMenu();
    //初始化mapScence
    void initScence();
    //修改当前是导航还是编辑的状态 切换到0:查询 1:修改
    void changeView(int aim);
    //添加地图
    void addMap();
    //切换地图
    void choiceMap(int id);
    //绘制校徽
    void drawBadge();
    //加载一张id地图
    void loadMap(int id);

    //将一类点添加到points1并绘制 传入数据而非指针
    void addPoint1(Point &points);
    //将二类点添加到points2并绘制
    void addPoint2(Point &points);
    //删除一个点并取消绘制
    void cleanPoints(int id);


};
#endif // MAINWINDOW_H

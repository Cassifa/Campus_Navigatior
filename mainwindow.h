#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"CampusMap/campusmap.h"
#include"./DrawingItems/drawingedge.h"
#include"./DrawingItems/drawingpoint.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
enum SearchAlgorithm{
    BFS,AStar,Heap,Dijkstra,SPFA,Gene,Floyd,Unchoice
};
enum ModifyingOptions{
    AddBuilding,AddNode,RemoveNode,AddPath,RemovePath,SaveChange,NotModifying
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_heap_clicked();

    void on_aStar_clicked();

    void on_dijkstra_clicked();

    void on_bfs_clicked();

    void on_spfa_clicked();

    void on_gene_clicked();

    void on_floyd_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_addBuilding_clicked();

    void on_addNode_clicked();

    void on_removeNode_clicked();

    void on_addPath_clicked();

    void on_removePath_clicked();

    void on_saveChange_clicked();

private:
    Ui::MainWindow *ui;
    //当前使用的算法,枚举
    SearchAlgorithm nowUsingAlgorithm=Unchoice;
    //修改地图时选择的功能
    ModifyingOptions modifyingOptions=NotModifying;
    //当前使用功能 0:导航 1:编辑
    int nowView=0;
    //当前使用地图的编号
    int usingMap=0;
    //是否要展示搜索过程
    bool needShowPath=false;
    //是否自动下一步
    bool autoNext=false;
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

    //进入导航页面
    void switchToNav();
    //进入编辑页面
    void switchEdit();


};
#endif // MAINWINDOW_H

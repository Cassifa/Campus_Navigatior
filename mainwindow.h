#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"CampusMap/campusmap.h"
#include"./DrawingItems/drawingedge.h"
#include"./DrawingItems/drawingpoint.h"
#include"./DrawingItems/mapbackground.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
enum SearchAlgorithm{
    BFS,AStar,Heap,Dijkstra,SPFA,Gene,Floyd,Unchoice
};
enum ModifyingOptions{
    AddBuilding,AddNode,RemoveNode,AddPath,RemovePath,SaveChange,NotModifying
};

class MainWindow : public QMainWindow{
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

public slots:
    //尝试加点
    void tryAddPoint(QGraphicsSceneMouseEvent *event);
    //删除点
    void removePoint(int id);
    //删边
    void removeEdge(Edge edge);
    //加边
    void addEdge(int id);

private:
    //加边时上次点到的点
    Point *lastAddPathStartPoint=nullptr;
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
    //当前需要绘制的边(道路)
    QVector<DrawingEdge*> roads;
    //当前需要绘制的一类点:标志建筑
    QVector<DrawingPoint*> points1;
    //需要绘制的二类点：节点
    QVector<DrawingPoint*> points2;

    //三个工具栏目
    QAction *navtionTool;
    QAction *modiTool;
    QAction *addTool;
    QGraphicsScene *mapScene;
    MapBackground *transparentItem;

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

    //以下只操作绘图容器,删除maps的功能在其它地方实现
    //将一类点添加到points1并绘制 传入数据而非指针
    void addPoint1(Point &point);
    //将二类点添加到points2并绘制
    void addPoint2(Point &point);
    //删除一个点并取消绘制
    void cleanPoint(int id);
    //绘制一条边,可以是roads的也可以是搜索结果,如果是roads会先添加到roads 如果不是返回Item指针
    DrawingEdge* addEdge(Edge &edge,bool isRoad,//非road会窜入下面属性，最短路最要求不缓慢展示
                         int penWidth=4,QColor color=QColor(Qt::black),bool slowDrawing=true);
    //取消绘制并删除一条边 只能是road里面的,传入边的指针会去roads里找
    void cleanEdge(DrawingEdge *edge);


    //进入导航页面
    void switchToNav();
    //进入编辑页面
    void switchEdit();


};
#endif // MAINWINDOW_H

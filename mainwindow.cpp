#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QDir>
#include<QGraphicsPixmapItem>
#include"./DrawingItems/drawingpoint.h"
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    //初始化顶部选项:加载存档和功能栏目并绑定点击事件
    initMenu();
    //加载校徽
    drawBadge();
    //初始化mapScene
    initScence();
}

//初始化顶部选项:加载存档和功能栏目并绑定点击事件
void MainWindow::initMenu(){
    //加入三个点击框
    navtionTool=new QAction("导航");
    modiTool=new QAction("修改");
    addTool=new QAction("添加地图");
    menuBar()->addAction(navtionTool);
    menuBar()->addAction(modiTool);
    menuBar()->addAction(addTool);

    //为点击框添加点击事件
    //点击导航功能
    connect(navtionTool,&QAction::triggered,this,[&](){
        qDebug()<<"点击了导航";
        this->nowView=0;
        changeView(nowView);
    });
    //点击修改功能
    connect(modiTool,&QAction::triggered,this,[&](){
        this->nowView=1;
        changeView(nowView);
    });


    //加载所有存档
    this->maps=new QVector<CampusMap*>;
    //获取存档地址
    QString path=QCoreApplication::applicationDirPath()+"/src/maps";
    path="D:/0projects/CampusNavigatior/src/maps";
    QDir dir(path);
    if(!dir.exists()) dir.mkdir(path);
    QStringList list=dir.entryList();
    //将每个存档加载到this.maps
    for(int i=0;i<list.count();i++){
        if(list[i]=="."||list[i]=="..")continue;
        CampusMap obj;
        CampusMap* nowAddingMap=obj.addMap(path+"/"+list[i]);
        nowAddingMap->setId(this->maps->size());
        maps->push_back(nowAddingMap);
    }

    //将存档添加到菜单
    for(int i=0;i<maps->size();i++){
        //加到菜单
        QAction *nowAddingMapAction=new QAction(maps->at(i)->getName(),this);
        ui->menu->addAction(nowAddingMapAction);
        //绑定点击事件
        connect(nowAddingMapAction,&QAction::triggered,this,[=](){
               choiceMap(i);
        });
    }
    qDebug()<<"执行完成"<<endl;
}

//初始化mapScence
void MainWindow::initScence(){
    auto graphicsView=this->ui->mapGraphicsView;
    graphicsView->setGeometry(0, 0, graphicsView->width(), graphicsView->height());
    QSize viewSize = ui->mapGraphicsView->size();
    mapScene=new QGraphicsScene(0,0,viewSize.width(),viewSize.height(),this);
    // 设置视图的中心为场景的中心
    graphicsView->setScene(mapScene);
    graphicsView->setSceneRect(mapScene->sceneRect());
    // 禁用滚动条
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 禁用抗锯齿
//    ui->mapGraphicsView->setRenderHint(QPainter::Antialiasing, false);
    //加载0号地图
    loadMap(0);
    graphicsView->show();
}

//修改当前是导航还是编辑的状态
void MainWindow::changeView(int aim){
    if(aim==0) switchToNav();
    else switchEdit();
}

//添加一张地图
void MainWindow::addMap(){
    qDebug()<<"添加地图";
}

//切换地图
void MainWindow::choiceMap(int id){
    qDebug()<<"点击了存档";
    qDebug()<<"切换到了id为"<<id<<"的存档";
    qDebug()<<"名称为："<<maps->at(id)->getName();
    this->loadMap(id);
}

//画校徽
void MainWindow::drawBadge(){
    ui->graphicsView->setGeometry(0,800,ui->graphicsView->width(),ui->graphicsView->height());
    // 创建场景
    QGraphicsScene *scene = new QGraphicsScene(this);
    // 添加背景图像
    QPixmap backgroundImage(":/ZJUT");
    // 设置图像大小以适应 QGraphicsView
    QSize viewSize = ui->graphicsView->size();
    backgroundImage = backgroundImage.scaled(viewSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *backgroundPixmap = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(backgroundPixmap);
    // 设置场景矩形
    scene->setSceneRect(backgroundPixmap->boundingRect());
    // 设置场景
    ui->graphicsView->setScene(scene);
    // 禁用滚轮滚动
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 显示视图
    ui->graphicsView->show();
}

//加载地图：删除原有内容并展示边与一类点
void MainWindow::loadMap(int id){
    //删除原有点
    for(int i=0;i<points1.size();i++)
        cleanPoints(points1[i]->getId());
    points1.resize(0);
    for(int i=0;i<points2.size();i++)
        cleanPoints(points2[i]->getId());
    points2.resize(0);

    //绘制新点
    QVector<Point*> points=*this->maps->at(id)->getPointsList();
    for(const Point *now:points){
        Point nowPoint=*now;
        if(!nowPoint.isHide)addPoint1(nowPoint);
    }
}

//将一个点添加到points1并绘制
void MainWindow::addPoint1(Point &point){
    DrawingPoint *nowPoint=new DrawingPoint(point,30,QColor(Qt::red).lighter(150));
    this->points1.append(nowPoint);
    this->mapScene->addItem(nowPoint);
}
//将一个点添加到points2并绘制
void MainWindow::addPoint2(Point &point){
    DrawingPoint *nowPoint=new DrawingPoint(point,10,QColor(Qt::blue).lighter(150));
    this->points2.append(nowPoint);
    this->mapScene->addItem(nowPoint);
}

//取消绘制点并从Vector<point>中删除
void MainWindow::cleanPoints(int id){
    for(int i=0;i<points1.size();i++){
        auto now=points1[i];
        if(now->getId()==id){
            this->mapScene->removeItem(now);
            delete now;
            return;
        }
    }
    for(int i=0;i<points2.size();i++){
        auto now=points2[i];
        if(now->getId()==id){
            this->mapScene->removeItem(now);
            delete now;
            return;
        }
    }
}

//进入导航页面 重新加载地图
void MainWindow::switchToNav(){//usingMap
    //切换页面
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    //加载地图
    loadMap(this->usingMap);
    //重置修改栏状态
    this->modifyingOptions=NotModifying;
}

//进入编辑页面 显示二类点
void MainWindow::switchEdit(){
    //切换到修改页面
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(1);
    //重置修改栏状态
    this->modifyingOptions=NotModifying;
    //显示二类点
    QVector<Point*> points=*this->maps->at(usingMap)->getPointsList();
    for(const Point *now:points){
        Point nowPoint=*now;
        if(nowPoint.isHide)addPoint2(nowPoint);
    }
}

MainWindow::~MainWindow(){
    delete ui;
    delete maps;
    for(int i=0;i<points1.size();i++)
         delete points1[i];
    for(int i=0;i<points2.size();i++)
         delete points2[i];
}


//改变选择算法
void MainWindow::on_heap_clicked(){
    this->nowUsingAlgorithm=BFS;
}
void MainWindow::on_aStar_clicked(){
    this->nowUsingAlgorithm=AStar;
}
void MainWindow::on_dijkstra_clicked(){
    this->nowUsingAlgorithm=Dijkstra;
}
void MainWindow::on_bfs_clicked(){
    this->nowUsingAlgorithm=BFS;
}
void MainWindow::on_spfa_clicked(){
    this->nowUsingAlgorithm=SPFA;
}
void MainWindow::on_gene_clicked(){
    this->nowUsingAlgorithm=Gene;
}
void MainWindow::on_floyd_clicked(){
    this->nowUsingAlgorithm=Floyd;
}

//演示时选项参数
void MainWindow::on_checkBox_stateChanged(int arg1){
    if(arg1==Qt::Unchecked)this->needShowPath=false;
    else needShowPath=true;
}
void MainWindow::on_checkBox_2_stateChanged(int arg1){
    if(arg1==Qt::Unchecked)this->autoNext=false;
    else autoNext=true;
}
void MainWindow::on_pushButton_clicked(){
    qDebug()<<"要求下一步";
}
void MainWindow::on_pushButton_2_clicked(){
    qDebug()<<"重置搜索过程";
}

//修改时选项
void MainWindow::on_addBuilding_clicked(){
    this->modifyingOptions=AddBuilding;
}
void MainWindow::on_addNode_clicked(){
    this->modifyingOptions=AddNode;
}
void MainWindow::on_removeNode_clicked(){
    this->modifyingOptions=RemoveNode;
}
void MainWindow::on_addPath_clicked(){
    this->modifyingOptions=AddPath;
}
void MainWindow::on_removePath_clicked(){
    this->modifyingOptions=RemovePath;
}
void MainWindow::on_saveChange_clicked(){
    this->modifyingOptions=SaveChange;
}


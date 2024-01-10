#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QDir>
#include<QGraphicsPixmapItem>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    //初始化顶部选项:加载存档和功能栏目并绑定点击事件
    initMenu();
    drawBadge();

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

//修改当前是导航还是编辑的状态
void MainWindow::changeView(int aim){
    qDebug()<<aim;
    ui->stackedWidget->setCurrentIndex(aim);
}

void MainWindow::addMap(){
    qDebug()<<"添加地图";
}

//切换地图
void MainWindow::choiceMap(int id){
    qDebug()<<"点击了存档";
    qDebug()<<"切换到了id为"<<id<<"的存档";
    qDebug()<<"名称为："<<maps->at(id)->getName();
}

void MainWindow::drawBadge(){
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




MainWindow::~MainWindow(){
    delete ui;
    delete maps;
}


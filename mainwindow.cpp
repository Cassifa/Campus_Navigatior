#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QDir>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    //初始化顶部工具并加载所有存档
    initMenu();

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
}

//初始化顶部选项
void MainWindow::initMenu(){
    //加入三个点击框
    navtionTool=new QAction("导航");
    modiTool=new QAction("修改");
    addTool=new QAction("添加地图");
    menuBar()->addAction(navtionTool);
    menuBar()->addAction(modiTool);
    menuBar()->addAction(addTool);

    //加载所有存档
    this->maps=new QVector<CampusMap*>;
    QString path=QCoreApplication::applicationDirPath()+"/src/maps";
    path="D:/0projects/CampusNavigatior/src/maps";
    QDir dir(path);
    if(!dir.exists()) dir.mkdir(path);
    QStringList list=dir.entryList();
    for(int i=0;i<list.count();i++){
        if(list[i]=="."||list[i]=="..")continue;
        CampusMap obj;
        maps->push_back(obj.addMap(path+"/"+list[i]));
    }

    //将存档添加到菜单

    qDebug()<<"执行完成"<<endl;
}

void MainWindow::changeView(int aim){
    qDebug()<<aim;
    ui->stackedWidget->setCurrentIndex(aim);
}

void MainWindow::addMap(){
    qDebug()<<"添加地图";
}




MainWindow::~MainWindow(){
    delete ui;
    delete maps;
}


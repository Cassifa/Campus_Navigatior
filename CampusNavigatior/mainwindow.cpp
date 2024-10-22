#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QDir>
#include<QGraphicsPixmapItem>
#include"./DrawingItems/drawingpoint.h"
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QInputDialog>
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle("CampusNavigator");
    this->setWindowIcon(QIcon(":/Nav"));
    //初始化顶部选项:加载存档和功能栏目并绑定点击事件
    initMenu();
    //加载校徽
    drawBadge();
    //初始化mapScene,加载0号地图并尝试展示背景
    initScence();
    //初始化算法类
    serachUtil.init(this->maps->at(0));
}


//6个绘制点与线行为:
//将一个点添加到points1并绘制
void MainWindow::addPoint1(Point &point){
    DrawingPoint *nowPoint=new DrawingPoint(point,20,QColor(Qt::red).lighter(150));
    this->points1.append(nowPoint);
    nowPoint->setZValue(3);
    connect(nowPoint, &DrawingPoint::pointClicked,this,[=](int pointId){
        removePoint(pointId);
        addEdge(pointId);
        askSerach(pointId);
    });
    this->mapScene->addItem(nowPoint);
}
//将一个点添加到points2并绘制
void MainWindow::addPoint2(Point &point){
    DrawingPoint *nowPoint=new DrawingPoint(point,8,QColor(Qt::blue).lighter(150));
    this->points2.append(nowPoint);
    nowPoint->setZValue(3);
    connect(nowPoint, &DrawingPoint::pointClicked,this,[=](int pointId){
        removePoint(pointId);
        addEdge(pointId);
    });
    this->mapScene->addItem(nowPoint);
}
//取消绘制点并从Vector<point*> points1/points2中删除
void MainWindow::cleanPoint(int id){
    for(int i=0;i<points1.size();i++){
        auto now=points1[i];
        //跳过已经被删除的点--->这里错误，即使是指针也不能在函数体内重新赋值
        if(now->getId()==id){
            this->mapScene->removeItem(now);
            this->mapScene->update();
            delete now;
            now=nullptr;
            return;
        }
    }
    points1.clear();
    for(int i=0;i<points2.size();i++){
        auto now=points2[i];
        if(now->getId()==id){
            this->mapScene->removeItem(now);
            this->mapScene->update();
            delete now;
            now=nullptr;
            return;
        }
    }
    points2.clear();
}

//取消绘制所有搜索边
void MainWindow::cleanAllSearchEdge(){
    auto edgeList=this->serachUtil.getDrawnEdges();
    for(int i=0;i<edgeList.size();i++){
        auto now=edgeList.at(i);
        //从画布上删除并释放内存
        this->mapScene->removeItem(now);
        delete now;
    }
    //init函数会自己重置DrawnEdges大小
}
//绘制一条边,可以是roads的也可以是搜索结果,如果是roads会先添加到roads
DrawingEdge* MainWindow::addEdge(Edge &edge,bool isRoad,int penWidth,QColor color,bool slowDrawing){
     //int penWidth, const QColor &color, bool slowDrawing
    DrawingEdge *nowEdge;
    if(isRoad){
        //创建普通道路并加入roads;
        nowEdge=new DrawingEdge(edge,penWidth,color,false);
        this->roads.append(nowEdge);
        this->mapScene->addItem(nowEdge);
        nowEdge->setZValue(1);
        connect(nowEdge, &DrawingEdge::edgeClicked,this,[=](int startX,int startY,int endX,int endY){
            removeEdge(startX, startY, endX, endY);
        });
        return nullptr;
    }
    else {
        //创建搜索边
        nowEdge=new DrawingEdge(edge,penWidth+1,color,slowDrawing);
        this->mapScene->addItem(nowEdge);
        nowEdge->setZValue(2);
        connect(nowEdge, &DrawingEdge::edgeClicked,this,[=](int startX,int startY,int endX,int endY){
            removeEdge(startX, startY, endX, endY);
        });
        return nowEdge;
    }
}
//取消绘制并删除一条road
void MainWindow::cleanEdge(DrawingEdge *edge){
    for(int i=0;i<this->roads.size();i++){
        auto now=roads.at(i);
        if(now==edge){
            //移除并释放空间
            this->mapScene->removeItem(now);
            delete now;
            return;
        }
    }
}


//4个编辑视角下的元素点击行为:
//加点
void MainWindow::tryAddPoint(QGraphicsSceneMouseEvent *event){
//    qDebug()<<"加点";
    if(nowView==0||(modifyingOptions!=AddNode&&modifyingOptions!=AddBuilding))return;
    QPointF  pos=event->pos();
    //加节点
    if(modifyingOptions==AddNode){
        //寻找合法id
        QSet<int> st;
        auto pointList=*this->maps->at(usingMap)->getPointsList();
        for(Point* t:pointList)
            st.insert(t->id);
        Point *newPoint;
        for(int i=0;;i++)
            if(!st.contains(i)){
                newPoint=new Point(pos.rx(),pos.ry(),i,true);
                break;
            }
        //加到maps
        this->maps->at(usingMap)->pushPoint(newPoint);
        //绘制图像
        addPoint2(*newPoint);
    }
    //加建筑
    else{
        //设置弹窗
        QInputDialog inputDialog(this);
        inputDialog.setWindowTitle("");
        inputDialog.setLabelText("请输入建筑名称：");
        inputDialog.setInputMode(QInputDialog::TextInput);
        //设置输入长度限制
        inputDialog.setIntRange(1, 20);
        if(inputDialog.exec()==QInputDialog::Accepted){
            //寻找合法id
            QSet<int> st;
            auto pointList=*this->maps->at(usingMap)->getPointsList();
            for(Point* t:pointList)
                st.insert(t->id);
            Point *newPoint;
            for(int i=0;;i++)
                if(!st.contains(i)){
                    newPoint=new Point(pos.rx(),pos.ry(),i,false,inputDialog.textValue());
                    break;
                }
            //加到maps
            this->maps->at(usingMap)->pushPoint(newPoint);
            //绘制图像
            addPoint1(*newPoint);
        }
        else return;
    }
}
//删边(路) 这里的参数没id
void MainWindow::removeEdge(int startX,int startY,int endX,int endY){
//    qDebug()<<"删边";
    if(nowView==0||modifyingOptions!=RemovePath)return;
    auto list=this->maps->at(usingMap)->getEdgesList();
    for(int i=0;i<list->size();i++){
        //找出Maps中对应的边
        if(list->at(i)->x.x==startX&&list->at(i)->y.x==endX
                &&list->at(i)->x.y==startY&&list->at(i)->y.y==endY){
            //从roads中去除
            for(int j=0;j<roads.size();j++){
                //x:map中存的边 y:roads(Item)中存的线段
                auto x=list->at(i);auto y=roads.at(j);
                if((x->dist==y->dist)&&(x->x.x=y->startX)&&(x->y.x==y->endX)){
                    cleanEdge(y);
                    roads.remove(j);
                    break;
                }
            }
            //从maps中去除
            list->remove(i);
        }
    }
}
//删点
void MainWindow::removePoint(int id){
//    qDebug()<<"删点";
    if(nowView==0||modifyingOptions!=RemoveNode)return;

    //1删除相关边
    //1.1删除map中记录
    auto edgeList=this->maps->at(usingMap)->getEdgesList();
    QVector<int> needRemove;
    for(int i=0;i<edgeList->size();i++)
        //找到所有相关边并从maps中删除
       if(edgeList->at(i)->x.id==id||edgeList->at(i)->y.id==id){
           delete edgeList->at(i);
           needRemove.append(i);
       }
    for(int i=0;i<needRemove.size();i++)
        //保证删除的时原来那个位置的
        edgeList->remove(needRemove[i]-i);
    //1.2删除所有渲染边
    needRemove.clear();
    for(int i=0;i<roads.size();i++){
        if(roads.at(i)->getPointXId()==id||roads.at(i)->getPointYId()==id){
            cleanEdge(roads.at(i));
            needRemove.append(i);
        }
    }
    for(int i=0;i<needRemove.size();i++)
        //保证删除的时原来那个位置的
        roads.remove(needRemove[i]-i);

    //2删除点
    //2.1删除maps记录
    auto pointList=this->maps->at(usingMap)->getPointsList();
    for(int i=0;i<pointList->size();i++)
        if(pointList->at(i)->id==id){
            delete pointList->at(i);
            pointList->remove(i);
            break;
        }
    //2.2删除绘制点
    cleanPoint(id);
}
//加边
void MainWindow::addEdge(int id){
//    qDebug()<<"加边";
    if(nowView==0||modifyingOptions!=AddPath)return;
    //第一个选的点
    if(lastAddPathStartPoint==nullptr){
        //找到对应id点
        auto pointLists=this->maps->at(usingMap)->getPointsList();
        for(int i=0;i<pointLists->size();i++){
            if(pointLists->at(i)->id==id){
                lastAddPathStartPoint=new Point(this->maps->at(usingMap)->getPointAt(i));
                return;
            }
        }
    }
    else {
        //点了自己则取消添加边
        if(lastAddPathStartPoint->id==id){
            lastAddPathStartPoint=nullptr;
            return;
        }
        //已经有边
        auto t=this->maps->at(usingMap)->getEdgesList();
        int lastId=lastAddPathStartPoint->id;
        for(int i=0;i<t->size();i++){
            auto nowEdge=t->at(i);
            if((nowEdge->x.id==lastId&&nowEdge->y.id==id)||(nowEdge->x.id==id&&nowEdge->y.id==lastId))
                return;
        }
        //即不是第一个 也不是重复边 也不是自环则加边
        //找到对应id点
        auto pointLists=this->maps->at(usingMap)->getPointsList();
        for(int i=0;i<pointLists->size();i++){
            if(pointLists->at(i)->id==id){
                Edge *newEdge=new Edge(*lastAddPathStartPoint,*pointLists->at(i));
                //road加入队列
                this->maps->at(usingMap)->pushEdge(newEdge);
                //渲染road Edge &edge,bool isRoad,int penWidth,QColor color,bool slowDrawing
                this->addEdge(*newEdge,true);
                //设置新的Point
                delete lastAddPathStartPoint;
                lastAddPathStartPoint=new Point(this->maps->at(usingMap)->getPointAt(i));
                return;
            }
        }


    }

}

//1个导航视角下元素点击行为:
//尝试加入搜索队列
void MainWindow::askSerach(int id){
    if(this->nowView==1)return;
    auto pointList=this->maps->at(usingMap)->getPointsList();
    for(int i=0;i<pointList->size();i++)
        //找到被点击的点
        if(pointList->at(i)->id==id){
            //尝试加入起点/终点
            this->serachUtil.tryPushPoint(pointList->at(i));
            checkAlgorithmUtilStatus();
        }
}


//6个页面路由及刷新信息函数
//进入导航页面 重新加载地图
void MainWindow::switchToNav(){//usingMap
    //切换页面
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(0);
    //加载地图
    loadMap(this->usingMap);
    //重置修改栏状态
    this->modifyingOptions=NotModifying;
    //加边时上次点到的点
    lastAddPathStartPoint=nullptr;
}
//进入编辑页面 显示二类点
void MainWindow::switchEdit(){
    //切换到修改页面
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget_2->setCurrentIndex(1);
    //重置修改栏状态
    this->modifyingOptions=NotModifying;
    //删除导航边
     cleanAllSearchEdge();
    //重置搜索工具
     serachUtil.init(this->maps->at(usingMap));
     this->nowCallUpTo=0;
     refreashOutputArea();
    //显示二类点
    QVector<Point*> points=*this->maps->at(usingMap)->getPointsList();
    for(const Point *now:points){
        Point nowPoint=*now;
        if(nowPoint.isHide)addPoint2(nowPoint);
    }
}
//刷新底部输出信息
void MainWindow::refreashOutputArea(){
    QString beginEndInformation,pathInformation;
    beginEndInformation=this->serachUtil.getOutpInfo();
    pathInformation=this->serachUtil.getOutpPath();
    this->ui->label_7->setText(beginEndInformation);
    this->ui->label_9->setText(pathInformation);
}
//展示搜索过程
void MainWindow::callShowPath(){
    auto ai=this->serachUtil;
    //展示搜索过程:中间边都绘制
    //不需要展示/要展示且自动下一步：展示搜索过程的话隔一秒话下一步
    if(!serachUtil.getIsNeedShowPath()||serachUtil.getIsAutoNext()||serachUtil.getSearchAlgorithm()==Floyd){
        //直接展示结果
        if(!serachUtil.getIsNeedShowPath()||serachUtil.getSearchAlgorithm()==Floyd){
            auto adgeList=this->serachUtil.getShorestPath();
            for(int i=0;i<adgeList.size();i++){
                DrawingEdge *edge=addEdge(*adgeList.at(i),false,5,QColor(Qt::green),false);
                this->serachUtil.pushDrawItem(edge);
            }
        }
        //自动展示
        else {
             int cnts=serachUtil.getPaths().size()+1;
             while (cnts--) {
                 showOncePath();
                 QThread::sleep(1);
             }
        }
    }
    else showOncePath();
}
//绘制一次数据
void MainWindow::showOncePath(){
    //全部展示过了
    if(nowCallUpTo>this->serachUtil.getPaths().size())return;
    //如果是dfs或者遗传要清除上次搜索路径
    if(this->serachUtil.getSearchAlgorithm()==DFS){
        cleanAllSearchEdge();
        this->serachUtil.setDrawItemEmpty();
    }
    //如果已经展示完所有则绘制最短路
    if(nowCallUpTo==this->serachUtil.getPaths().size()){
        auto adgeList=this->serachUtil.getShorestPath();
        for(int i=0;i<adgeList.size();i++){
            DrawingEdge *edge=addEdge(*adgeList.at(i),false,5,QColor(Qt::green),true);
            this->serachUtil.pushDrawItem(edge);
        }
        nowCallUpTo++;
    }
    else{
        auto adgeList=this->serachUtil.getPaths().at(nowCallUpTo++);
        for(int i=0;i<adgeList.size();i++){
            DrawingEdge *edge=addEdge(*adgeList.at(i),false,5,QColor(Qt::cyan),true);
            this->serachUtil.pushDrawItem(edge);
        }
    }
}
//刷新底部输出栏 检测是否可以计算了,可以的话执行展示函数
void MainWindow::checkAlgorithmUtilStatus(){
    //检查是否可以执行算法
    this->serachUtil.canCompute();
    //刷新底部输出栏
    this->refreashOutputArea();
    //如果有去搜 执行展示函数
    if(this->serachUtil.getIsComputed())
        callShowPath();

}
//加载地图：删除原有内容并展示边与一类点
void MainWindow::loadMap(int id){
    //删除原有点
    for(int i=0;i<points1.size();i++)
        cleanPoint(points1[i]->getId());
    points1.clear();
    for(int i=0;i<points2.size();i++)
        cleanPoint(points2[i]->getId());
    points2.clear();
    //删除原有道路
     for(int i=0;i<roads.size();i++)
          cleanEdge(roads.at(i));
    roads.clear();
    //删除搜索边和最短路边
    cleanAllSearchEdge();

    //绘制道路
    QVector<Edge*> edges=*this->maps->at(id)->getEdgesList();
    for(const Edge *now:edges){
        Edge nowEdge=*now;
        this->addEdge(nowEdge,true);
    }
    //绘制新点
    QVector<Point*> points=*this->maps->at(id)->getPointsList();
    for(const Point *now:points){
        Point nowPoint=*now;
        if(!nowPoint.isHide)addPoint1(nowPoint);
    }

    //尝试加载背景图
    QString imgPath=this->maps->at(usingMap)->getPath();
    //替换路径
    int index = imgPath.indexOf("maps");
    imgPath.replace(index,4,"img");
    //替换文件后缀,读取png和jpg的
    index = imgPath.indexOf("txt");
    imgPath.replace(index,3,"jpg");
    if(this->mapBackground->setImg(imgPath))return;
    index = imgPath.indexOf("jpg");
    imgPath.replace(index,3,"png");
    //如果jpg与png都没匹配到则设为空地图
    if(!this->mapBackground->setImg(imgPath))
        this->mapBackground->setEmptyImg();
}

//3个初始化页面函数:
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
        this->nowView=0;
        switchToNav();
    });
    //点击修改功能
    connect(modiTool,&QAction::triggered,this,[&](){
        this->nowView=1;
        switchEdit();
    });
    //点击添加地图
    connect(addTool,&QAction::triggered,this,[&](){
        this->addMap();
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
        QAction *nowAddingMapAction=new QAction(maps->at(i)->getName(),navtionTool);
        ui->menu->addAction(nowAddingMapAction);
        //绑定点击事件
        connect(nowAddingMapAction,&QAction::triggered,this,[=](){
               choiceMap(i);
        });
    }
}
//初始化mapScence
void MainWindow::initScence(){
    //新建Scence
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
    //添加背景图
    mapBackground=new MapBackground(0.0,0.0,qreal(mapScene->width()), qreal(mapScene->height()),QPixmap(),this);
    mapScene->addItem(mapBackground);
    connect(mapBackground, &MapBackground::itemClicked, this,&MainWindow::tryAddPoint);

    //清除默认文字
    this->ui->label_7->setText("");
    this->ui->label_9->setText("");
    //加载0号地图
    loadMap(0);
    graphicsView->show();
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


//2个切换与添加地图函数:
//添加一张地图
void MainWindow::addMap(){
    //设置弹窗
    QDialog *addMapDialog=new QDialog(this);
    addMapDialog->resize(400,300);
    addMapDialog->setWindowTitle("Add Map");

    //用户输入的参数
    mapPic="";
    mapName="";

    //输入名字与上传地图
    QLabel *mapNameLabel=new QLabel("Please Input Your Map Name:",addMapDialog);
    QLineEdit *inputMapName=new QLineEdit(mapName,addMapDialog);
    QPushButton *choiceMapBtn=new QPushButton("Choice Map Background(optional)",addMapDialog);

    //确认/关闭对话框
    QPushButton *ok=new QPushButton("OK",addMapDialog);
    QPushButton *cancel=new QPushButton("CANCEL",addMapDialog);

    //设置布局与属性
    // 水平布局，包含mapNameLabel和inputMapName
    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(mapNameLabel);
    nameLayout->addWidget(inputMapName);
    // 垂直布局，包含水平布局和choiceMapBtn
    QVBoxLayout *mainLayout = new QVBoxLayout(addMapDialog);
    mainLayout->addLayout(nameLayout);
    mainLayout->addWidget(choiceMapBtn, 0, Qt::AlignHCenter);
    //按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    // 确认/关闭对话框按钮布局;// 设置按钮布局的边距，以便留有一些空白
    buttonLayout->setContentsMargins(0, 0, 10, 10);
    // 将按钮布局添加到主布局，并设置对齐方式
    mainLayout->addLayout(buttonLayout,Qt::AlignRight | Qt::AlignBottom);
    // 设置主布局
    addMapDialog->setLayout(mainLayout);


    //绑定对话框
    connect(ok,&QPushButton::clicked,this,[=](){
        mapName=inputMapName->text();
        if(mapName=="")return;
        saveNewMap(mapName,mapPic);
        addMapDialog->close();
    });
    connect(cancel,&QPushButton::clicked,this,[=](){
        addMapDialog->close();
    });
    connect(choiceMapBtn,&QPushButton::clicked,this,[=](){
        // 打开文件选择对话框获取文件路径
        QFileDialog fileDialog(addMapDialog);
        fileDialog.setWindowTitle("选择图片");
        fileDialog.setNameFilter("Images (*.png *.jpg)");
        fileDialog.setFileMode(QFileDialog::ExistingFile);
        if (fileDialog.exec() == QDialog::Accepted) {
            QStringList selectedFiles = fileDialog.selectedFiles();
            if (!selectedFiles.isEmpty())
                mapPic= selectedFiles.first();
        }
    });

    //打开上上传栏目
    addMapDialog->open();
}
//保存用户上传的新地图
void MainWindow::saveNewMap(QString mapName,QString mapPic){
    QString path=QCoreApplication::applicationDirPath()+"/src/";
    path="D:/0projects/CampusNavigatior/src/";
    QDir dir(path+"maps/");
    if(!dir.exists()) dir.mkdir(path+"maps/");
    QStringList list=dir.entryList();
    //找到最小未分配编号
    QSet<int> st;
    for(int i=0;i<list.count();i++){
        if(list[i]=="."||list[i]=="..")continue;
        QString now=list.at(i);
        int t=now.leftRef(now.indexOf('.')).toInt();
        st.insert(t);
    }
    //写入新文件
    int newId;
    for(int i=0;;i++){
        if(st.find(i)==st.end()){
            newId=i;
            QString mapFileName=path+"maps/"+QString::number(i)+".txt";
            QString imgFileName=path+"img/"+QString::number(i)+".jpg";
            //处理新地图
            QFile mapFile(mapFileName);
            if (mapFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&mapFile);
                out.setCodec("UTF-8");
                out << mapName << "\n-1 -1\n-1 -1\n";
                mapFile.close();
            } else {
                qDebug() << "创建地图失败,终止新建：" << mapFileName;
                return;
            }
            // 复制图片文件
            if (!mapPic.isEmpty()) {
                QFile::copy(mapPic,imgFileName);
            }
            // 处理完成，可以退出循环
            break;
        }
    }
    //重新加载一次导航栏
    delete navtionTool;
    delete modiTool;
    delete addTool;
    delete maps;
    initMenu();
    choiceMap(newId);
    switchEdit();
}
//切换地图
void MainWindow::choiceMap(int id){
    usingMap=id;
    switchToNav();
    //重新加载地图
    //重新构建算法类
    serachUtil.init(this->maps->at(usingMap));
    //刷新输出区域
    refreashOutputArea();
    this->nowCallUpTo=0;
    this->modifyingOptions=NotModifying;
}


//4导航视角时选项参数
void MainWindow::on_checkBox_stateChanged(int arg1){
    //已经搜索了不允许设置NeedShowPath
    if(this->serachUtil.getIsComputed()){
        this->ui->checkBox->setChecked(!arg1);
        return;
    }
    if(arg1==Qt::Unchecked)serachUtil.setNeedShowPath(false);
    else serachUtil.setNeedShowPath(true);
}
void MainWindow::on_checkBox_2_stateChanged(int arg1){
    //已经搜索了不允许设置AutoNext
    if(this->serachUtil.getIsComputed()){
        this->ui->checkBox_2->setChecked(!arg1);
        return;
    }
    if(arg1==Qt::Unchecked)serachUtil.setAutoNext(false);
    else serachUtil.setAutoNext(true);
}
void MainWindow::on_pushButton_clicked(){
//    qDebug()<<"要求下一步";
    auto ai=this->serachUtil;
    //还没搜索/自动播放/不需要展示路径 就忽略点击
    if(!ai.getIsComputed()||ai.getIsAutoNext()||!ai.getIsNeedShowPath())return;
    //Floyd没有过程
    if(serachUtil.getSearchAlgorithm()==Floyd)return;
    showOncePath();
}
//重置搜索
void MainWindow::on_pushButton_2_clicked(){
//    qDebug()<<"重置搜索过程";
    this->nowCallUpTo=0;
    //清除输出
//    refreashOutputArea();
    this->ui->label_7->setText("");
    this->ui->label_9->setText("");
    cleanAllSearchEdge();
    //重置地图
    serachUtil.init(this->maps->at(usingMap));
    //取消选择
    this->ui->checkBox->setChecked(false);
    this->ui->checkBox_2->setChecked(false);

    this->ui->heap->setAutoExclusive(false);
    this->ui->heap->setChecked(false);
    this->ui->heap->setAutoExclusive(true);

    this->ui->dfs->setAutoExclusive(false);
    this->ui->dfs->setChecked(false);
    this->ui->dfs->setAutoExclusive(true);

    this->ui->aStar->setAutoExclusive(false);
    this->ui->aStar->setChecked(false);
    this->ui->aStar->setAutoExclusive(true);

    this->ui->dijkstra->setAutoExclusive(false);
    this->ui->dijkstra->setChecked(false);
    this->ui->dijkstra->setAutoExclusive(true);

    this->ui->spfa->setAutoExclusive(false);
    this->ui->spfa->setChecked(false);
    this->ui->spfa->setAutoExclusive(true);

    this->ui->floyd->setAutoExclusive(false);
    this->ui->floyd->setChecked(false);
    this->ui->floyd->setAutoExclusive(true);

    this->ui->gene->setAutoExclusive(false);
    this->ui->gene->setChecked(false);
    this->ui->gene->setAutoExclusive(true);
}


//6修改视角时的选项
void MainWindow::on_addBuilding_clicked(){
    this->modifyingOptions=AddBuilding;
    lastAddPathStartPoint=nullptr;
}
void MainWindow::on_addNode_clicked(){
    this->modifyingOptions=AddNode;
    lastAddPathStartPoint=nullptr;
}
void MainWindow::on_removeNode_clicked(){
    this->modifyingOptions=RemoveNode;
    lastAddPathStartPoint=nullptr;
}
void MainWindow::on_addPath_clicked(){
    this->modifyingOptions=AddPath;
    lastAddPathStartPoint=nullptr;
}
void MainWindow::on_removePath_clicked(){
    this->modifyingOptions=RemovePath;
    lastAddPathStartPoint=nullptr;
}
void MainWindow::on_saveChange_clicked(){
    this->modifyingOptions=SaveChange;
    lastAddPathStartPoint=nullptr;
    this->maps->at(usingMap)->saveMap();
}


//改变选择算法
void MainWindow::on_heap_clicked(){
    serachUtil.setSearchAlgorithm(SearchAlgorithm::Heap);
    checkAlgorithmUtilStatus();
}
void MainWindow::on_aStar_clicked(){
    serachUtil.setSearchAlgorithm(SearchAlgorithm::AStar);
    checkAlgorithmUtilStatus();
}
void MainWindow::on_dijkstra_clicked(){
    serachUtil.setSearchAlgorithm(SearchAlgorithm::Dijkstra);
    checkAlgorithmUtilStatus();
}
void MainWindow::on_dfs_clicked(){
    serachUtil.setSearchAlgorithm(SearchAlgorithm::DFS);
    checkAlgorithmUtilStatus();
}
void MainWindow::on_spfa_clicked(){
    serachUtil.setSearchAlgorithm(SearchAlgorithm::SPFA);
    checkAlgorithmUtilStatus();
}
void MainWindow::on_gene_clicked(){
    serachUtil.setSearchAlgorithm(SearchAlgorithm::Gene);
    checkAlgorithmUtilStatus();
}
void MainWindow::on_floyd_clicked(){
    serachUtil.setSearchAlgorithm(SearchAlgorithm::Floyd);
    checkAlgorithmUtilStatus();
}

MainWindow::~MainWindow(){
    delete ui;
    delete maps;
    for(int i=0;i<points1.size();i++)
         delete points1[i];
    for(int i=0;i<points2.size();i++)
        delete points2[i];
}

void MainWindow::on_bfs_clicked(){

}


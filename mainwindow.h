#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"CampusMap/campusmap.h"
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
    int nowView=0;
    QAction *navtionTool;
    QAction *modiTool;
    QAction *addTool;
    QVector<CampusMap*> *maps;
    void initMenu();
    //修改当前是导航还是编辑的状态 切换到0:查询 1:修改
    void changeView(int aim);
    //添加地图
    void addMap();
    //切换地图
    void choiceMap(int id);
    //绘制校徽
    void drawBadge();
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void initMenu();
    //切换到0:查询 1:修改
    void changeView(int aim);
    //添加地图
    void addMap();
    //使用信息初始化为一个地图类
    void initThisMap(QString path);
};
#endif // MAINWINDOW_H

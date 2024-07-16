# 校园导航系统



## 一、概述

​		**本项目为使用Qt实现的可视化地图导航工具。支持用户使用多种寻路算法并且将寻路算法可视化；为用户提供了操作简单流畅的地图修改、上传地图功能模块；采用两级地图点位，使得地图规划的路线符合现实逻辑，切实可用。**



## 二、页面展示

### 2.1 导航页面

​		在导航页面玩家可以在此选择起点终点，导航算法，若选择展示搜索过程还可查看每个算法的运行过程。目前支持：堆优化Dijkstra, A*, Dijkstra, DFS, SPFA, Floyd, 六种算法。 （注：下图淡蓝色路线为DFS的一条可行路线，并非最短路）

![][guideView]

### 2.2 编辑地图

​		在编辑页面用户可以添加/删除建筑与路口节点。建筑为可作为导航起点终点的点，路口节点用于辅助规范路线，避免出现规划出的道路横穿建筑的离谱情况。用户在添加道路的模式下连续点击节点可快速在节点之间建立道路。同样的，用户在删除节点时会自动删除与之相连的道路。用户也可以点击道路单独删除道路。用户的修改即时生效，但是要用户手动点击保存修改才会将修改记录计入存档，以防止误操作。

![][modifyMapView]

### 2.3 添加地图

​	玩家可以新建地图，并可选择上传图片作为地图背景。上传后会自动跳转到新建的地图并进入编辑状态。

![][addMapView]



## 三、业务逻辑设计与功能实现

### 3.1 主程序设计思路

​		用户启动程序后会开始加载存档，并将第一张存档渲染展示出来。当前展示的页面（导航/编辑）与底部信息栏目以分别置于各自的stackedWidget中，通过控制stackedWidget的index属性达到切换页面效果。用户当前选择的算法/对地图的修改模式以枚举形式存放，用户点击地图时通过判断index与枚举属性来执行不同的操作。

​		用户的地图存放在地图类中，地图类由点与边及相应方法构成。Mainwindow中的maps属性存放所有存档地图的指针，程序地图类内展示的图像元素均根据由当前使用的地图生成，玩家在编辑页面会直接操作当前使用地图类的元素并相应的创建/销毁图像元素。达到动态展示地图的效果。

​		 玩家查询最短路时会将地图传入最短路工具类中，最短路工具类调用玩家选择的算法获取最短路并记录搜索过程，若玩家勾选了展示搜索过程则会依次展示每次搜索的边。

![][process]

​	*添加地图流程图：*

​	*此部分较为独立，是上图中“添加新地图”子进程展开*

![][addMapProcess]

### 3.2 类设计

####  3.2.1 设计概述与类关系图

​		本程序设计的类按功能大致分为主窗口类、地图类、图形类、寻路算法类、添加地图类五种

![][classDiagram]



####  3.2.2 主窗口类

​		存放所有界面元素，监听用户点击事件，由以下几个部分组成。

-  **界面组件**

​		六个界面组件：Ui，navtionTool，modiTool，addTool，mapScene，mapBackground。分别为：界面UI设计，切换导航页面，切换修改页面，添加新地图，地图展示框，当前展示地图。

- **槽函数**

​		五个槽函数：tryAddPoint，removePoint，removeEdge，addEdge，askSerach。分别用于：尝试添加建筑与节点，尝试删除节点，尝试删除边，尝试添加边，导航模式下将节点加入最短路搜索队列。

- **图形元素成员变量**

​		roads，points1，points2分别用于存放当前展示的图形。用户每次对于边点操作会修改maps[usingMap]的对应成员，并去添加/删除对应图形元素，保证展示的图形一定与maps[usingMap]内属性一一对应。

- **用户点击主界面事件**

​		地图展示框会监听用户点击事件，并根据当前不同模式执行对应操作。①在导航模式下：用户点击点,算法等会为搜索工具设定搜索参数。当参数设定完成整后会调用搜索工具类并根据解析出来的结果展示给用户，用户每次重置会打断展示过程并重置参数；在编辑模式下。②在编辑模式下：用户单击功能会设定当前编辑模式参数，地图展示框在监听到点击事件会根据设定的参数执行不同的效果。用户确认存档会将当前maps[usingMap]的数据写入存档文件，以持久化存档。

- **用户点击导航栏事件**

​	    ①点击导航页面：会切换到导航页面并清除所有路口节点。重新加载一次地图；②点击编辑页面：会清除所有的搜索结果并刷新底部输出信息栏，再将所有二类节点显示出来

- **用户添加新存档**

​	    执行添加地图逻辑(见3.3.5 添加地图类)

*此模块相关类图如下：*

![][mainWindow]

####  3.2.3 地图类

- 每张地图拥有 ①地图名称 ②存档路径(.txt的路径) ③存档名称(.txt文档的不含后缀文件名) ④若干边 ⑤若干点;

- 地图类还有saveMap和addMap两个函数分别用于将地图写入存档和根据存档路径加载地图

- 点类记录了点的坐标，名称，以及是否为路口节点。边类拥有两个点和两者之间的距离，getdist方法用于计算两点欧几里得距离

*此模块相关类图如下：*

![][mapClass]

####  3.2.4 图形类

- 图形类是点类，边类在地图上的可视化体现，每个类会监听一定的操作并向mainwindow发出信号，mainwindow监听到图形类发出的请求会根据当前状态执行对应函数。

- 图形类是maps[usingMap]的实时体现，通过函调用规范实现对于map[usingMap]的操作同时作用在roads，points1，points2三个图形类容器上，而图形类会渲染在地图上，从而实现了用户修改存档并实时修改地图的效果。

*此模块相关类图如下：*

![][drawingItem]

####  3.2.5 寻路算法类

- **概述**

​       GetShortestPath用于接收用户设定的参数并根据参数执行搜索。搜索参数：①起点，终点 ②当前使用地图 ③是否需要展示搜索路径。用户每设定一次参数都会调用canCompute函数判断参数是否已经完备，如果完备会开始搜索并将搜索结果记录在achiveAble，paths与shortestPath中，分别表示是否可达，搜索过程，最终最短路，mainwindow会根据这些参数来展示本次搜索效果。

   	每个搜索函数都写作一个文件，暴露一个统一格式的接口，在GetShortestPath的compute函数中根据选择算法参数分别调用。通过引用传递的形式将结果记录下来。搜索完成后会统一用一个dfs来获取最短路径，如下图所示。

![][dfs]

- **一个接口示例：**

![][interface]

- **目前实现了SPFA,堆优化Dijkstra,Dijkstra,DFS,Floyd,A* 六个算法。其中Dijkstra和Floyd使用邻接矩阵存图，其余使用链式前向星存图。**

  - SPFA

    DP,期望O(N),最差退化O(NM)。搜索过程会展示每次被松弛的边

  - 堆优化Dijkstra

    贪心，O(MlogN),搜索过程会展示每次确认的边

  - Dijkstra

    贪心，O(N^2),搜索过程会展示每次更新的边

  - A*

    优化BFS,最劣O(M),启发函数为当点与目的地的欧几里得距离，搜索过程展示每次探索的边

  - Floyd

    DP,O(N^3),由于Floyd的特殊性，无法展示搜索过程

  - DFS

    最劣O(2^M),有加剪枝，但是还是不建议在变特别多的图使用。搜索过程展示所有可行路线。

*此模块相关类图如下*：

![][GetShorestPath]

####  3.2.6 添加地图类

​		用户点击添加地图后会弹出对话框，如下图

![][addMapWindow]

​		输入地图名称和背景图(可选)后即可创建新地图，初始存档内容为“地图名称\n -1 -1 \n -1 -1”。文件名称为MEX(当前已有的地图名)。添加完成后自动加载此地图并跳转到编辑页面。

*图片默认存档格式如下：*

![][mapFileFormat]

*注：MEX为找到集合中最小不存在非负整数*



## 四、一次完成创建地图并查询最短路的演示

- **选择地图上传**

  ![][addMapView]

- **上传后跳转页面**

  ![][show2]

- **上传后初始文件格式**

  ![][show3]

- **手动添加节点与道路后效果（当前鼠标悬浮在右下角的体育场，节点变大）**

  ![][show4]

- **使用A*展示的从东门到18号楼的搜索过程**

  ![][show5]

- **A*搜索结果，深蓝色为最短路**

  ![][show6]



## 五、寻路算法运行过程展示

### 5.1 堆优化Dijkstra

​		贪心思想，复杂度：MlogN

​		展示搜索过程会将每次确认最短的点绘制出来，与之相对应后面的朴素Dijkstra会展示每次更新的边

- 子良到毓秀搜索过程：

  ![][heapDijkstra1]

- 搜索结果：

  ![][heapDijkstra2]



### 5.2 A*

​		加入启发函数的BFS，可以看到，刚开始的路线十分准确

- 文汇到精弘搜索过程：

  ![][AStar1]

- 搜索结果：

  ![][AStar2]



### 5.3 Dijkstra

​		贪心思想，复杂度N^2

​		搜素过程展示了每次被更新的边

- 子良到毓秀搜索过程：

  ![][dijkstra1]

- 搜索结果：

  ![][dijkstra2]



### 5.4 DFS

​		搜索过程展示所有可行路线，并最终取最小的

​		UN公社对面门到健B

- 搜索过程1：

  ![][dfs1]

- 搜索过程2：

  ![][dfs2]

- 搜索过程3：

  ![][dfs3]

- 最佳路线：

  ![][dfs4]



### 5.5 SPFA

​		DP思想，bellman-ford的优化，期望复杂度O(M),.可能退化至O(NM)

- 搜索过程：

  ![][spfa1]

- 搜索结果：

  ![][spfa2]



### 5.6 Floyd

​		dp思想，复杂度N^3

​		由于floyd的特殊性，这里无法展示搜索过程

- 搜索结果：

  ![][floyd]



## 六、开发工具

- 项目开发工具：Qt Creator 5.12.12

- Qt版本：5.12.12

- C++编译器：mingw730_64



[addMapProcess]: ./desgin/img/addMapProcess.png
[addMapView]: ./desgin/img/addMapView.png
[addMapWindow]: ./desgin/img/addMapWindow.png
[choiceMap]: ./desgin/img/choiceMap.png
[classDiagram]: ./desgin/img/classDiagram.jpg
[dfs]: ./desgin/img/dfs.png
[drawingItem]: ./desgin/img/drawingItem.png
[GetShorestPath]: ./desgin/img/GetShorestPath.png
[guideView]: ./desgin/img/guideView.png
[interface]: ./desgin/img/interface.png
[mainWindow]: ./desgin/img/mainWindow.png
[mapClass]: ./desgin/img/mapClass.png
[mapFileFormat]: ./desgin/img/mapFileFormat.png
[modifyMapView]: ./desgin/img/modifyMapView.png
[process]: ./desgin/img/process.png
[show2]: ./desgin/img/show2.png
[show3]: ./desgin/img/show3.png
[show4]: ./desgin/img/show4.png
[show5]: ./desgin/img/show5.png
[show6]: ./desgin/img/show6.png
[heapDijkstra1]: ./desgin/img/shorestPathAlgorithm/heapDijkstra1.png
[heapDijkstra2]: ./desgin/img/shorestPathAlgorithm/heapDijkstra2.png
[AStar1]: ./desgin/img/shorestPathAlgorithm/AStar1.png
[AStar2]: ./desgin/img/shorestPathAlgorithm/AStar2.png
[dijkstra1]: ./desgin/img/shorestPathAlgorithm/dijkstra1.png
[dijkstra2]: ./desgin/img/shorestPathAlgorithm/dijkstra2.png
[dfs1]: ./desgin/img/shorestPathAlgorithm/dfs1.png
[dfs2]: ./desgin/img/shorestPathAlgorithm/dfs2.png
[dfs3]: ./desgin/img/shorestPathAlgorithm/dfs3.png
[dfs4]: ./desgin/img/shorestPathAlgorithm/dfs4.png
[spfa1]: ./desgin/img/shorestPathAlgorithm/spfa1.png
[spfa2]: ./desgin/img/shorestPathAlgorithm/spfa2.png
[floyd]: ./desgin/img/shorestPathAlgorithm/floyd.png
#include "./myVector.cpp"

template<typename T>
class myQueue{
private:
    Vector<T> queue;
    //大数下浮
    void down(int u){
        int t = u;
        if (u * 2 <= queue.size()-1 && queue[u * 2] < queue[t]) t = u * 2;
        if (u * 2 + 1 <= queue.size()-1 && queue[u * 2 + 1] < queue[t]) t = u * 2 + 1;
        if (u != t){
            swap(queue[u], queue[t]);
            down(t);
        }
    }
public:
    myQueue(){};
    ~myQueue(){};

    //弹出
    void pop(){
        queue.pop_front();
        queue.pop_front();
        queue.push_front(queue[0]);
    }
    //插入
    void push(T x){
        //堆的下标从一开始，所以放一个哨兵占据0
        if(queue.empty())
            queue.push_back(x);
        queue.push_back(x);
        down(1);
    }
    //取值
    T top(){
        return queue[1];
    }

    int size(){
        if(!queue.size())return 0;
        return queue.size()-1;
    }

};

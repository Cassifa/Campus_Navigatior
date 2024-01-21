#include<iostream>
#define ll long long
template<typename T>
class Vector{
    private:
        int maxSize=-1;//当前最大容量,即数组容量
        int usingSize=-1;//当前使用大小
        T *Arr=NULL;//数组存贮
        //private函数
        void resizeTo(int x){//数组扩容至x
            x=std::max(x,1);//至少要扩容到1,即初始时有可能对-1扩容
            T *nowarr=new T[x];
            if(nowarr==NULL)
                throw std::underflow_error("不能开这么大空间哦(虽然程序活不到这句话~");
            if(maxSize!=-1)//不是第一次扩容
                for(int i=0;i<maxSize;i++)
                    nowarr[i]=Arr[i];
            if(Arr!=NULL)delete []Arr;//不是第一次扩容,此前已有Arr
            Arr=nowarr;
            maxSize=x;
        }

        bool couldVisit(int x)const{//是否允许访问x位置
            if(x<0)return false;
            if(++x>usingSize) return false;
            return true;
        }
        void checkAccptable(int x)const{//检测是否允许操作x,不允许抛出异常
            if(!couldVisit(x))
                throw std::underflow_error("不可以访问这里哦~");
        }

    public:
        Vector(int x=0){//构造大小为x的Vector
            resizeTo(x);
            usingSize=x;maxSize=x;
        }
        Vector(int x,T val){//构造大小为x的Vector,初始值为val
            resizeTo(x);
            usingSize=x;maxSize=x;
            for(int i=0;i<x;i++)
                Arr[i]=val;
        }
        Vector(Vector<T>& t){//拷贝构造函数
            resizeTo(t.size());
            maxSize=t.max_capacity();
            usingSize=t.size();
            for(int i=0;i<usingSize;i++)
                Arr[i]=t[i];
        }
        ~Vector(){
            if(Arr!=NULL)
                delete []Arr;
        }

        void pop_back(){
            checkAccptable(usingSize-1);
            usingSize--;
        }
        void push_back(T x){
            if(usingSize+1>maxSize)
                resizeTo(maxSize*2);
            Arr[usingSize++]=x;
        }
        void pop_front(){
            if(!usingSize)
                throw std::underflow_error("已经弹无可弹啦~");
            T *newArr=new T[maxSize];
            for(int i=1;i<usingSize;i++)
                newArr[i-1]=Arr[i];
            usingSize--;
            delete []Arr;
            Arr=newArr;
        }
        void push_front(T x){
            if(usingSize+1>maxSize)
                resizeTo(maxSize*2);
            T *newArr=new T[maxSize];
            newArr[0]=x;
            for(int i=0;i<usingSize;i++)
                newArr[i+1]=Arr[i];
            usingSize++;
            delete []Arr;
            Arr=newArr;
        }

        int& front(){
            return Arr[0];
        }
        int& back(){
            return Arr[usingSize-1];
        }
        T& operator[](const int x)const{
            checkAccptable(x);
            return Arr[x];
        }

        int size()const{//返回容器当前使用大小
            return usingSize;
        }
        bool empty(){//判断是否为空
            return usingSize==0;
        }
        int max_capacity()const{//返回最大容量
            return maxSize;
        }

        void resize(int x){//修改容量
            if(x>maxSize){//变大
                resizeTo(x);
            }
            else{//变小
                T *newArr=new T[x];
                for(int i=0;i<x;i++)
                    newArr[i]=Arr[i];
                delete []Arr;
                Arr=newArr;
            }
            usingSize=x,maxSize=x;
        }
        void resize(int x,T val){//修改容量并赋初始值
            resize(x);
            for(int i=0;i<x;i++)
                Arr[i]=val;
        }

        int at(int x){//返回v中下标为x的元素
            checkAccptable(x);
            return Arr[x];
        }


        void swap(Vector<T>& other) {//交换两个Vecor
            std::swap(maxSize, other.maxSize);
            std::swap(usingSize, other.usingSize);
            std::swap(Arr, other.Arr);
        }

        void operator=(Vector<T>& t){// 重载=,深拷贝
            resizeTo(t.size());
            maxSize=t.max_capacity();
            usingSize=t.size();
            for(int i=0;i<usingSize;i++)
                Arr[i]=t[i];
        }

        bool operator<(const Vector<T>& t)const{//重定义<
            if(t.size()!=usingSize)return usingSize<t.size();
            for(int i=0;i<usingSize;i++)
                if(t[i]!=Arr[i])return Arr[i]<t[i];
            return false;
        }

        bool operator==(const Vector& t)const{//重定义==
            if(t.size()!=usingSize)return false;
            for(int i=0;i<usingSize;i++)
                if(t[i]!=Arr[i])return false;
            return true;
        }

        void print(){//打印Vector
            std::cout<<"usingSize: "<<usingSize<<"  maxSize "<<maxSize<<std::endl;
            for(int i=0;i<usingSize;i++)
                std::cout<<Arr[i]<<" ";
            std::cout<<std::endl;
        }
};

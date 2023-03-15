struct Squeue
{
    int *data;
    int front,rear;
};

class SqQueue
{
private:
    Squeue Q;
    int len;
    bool oflg = false;
public:
    SqQueue(const int &num);            //按传入长度构建队列
    bool EnQueue(const int &v);         //将传入的值入队
    bool DeQueue();                     //将队头元素出队
    bool DeQueue(int &v);               //将队头元素出队，并返回队头元素
    int GetHead();                      //获取队头元素
    bool GetHead(int &v);               //获取队头元素（重载版本）
    bool isEmpty();                     //判断队列是否为空
    ~SqQueue();                         //析构队列
};


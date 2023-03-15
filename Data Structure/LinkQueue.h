struct LNode
{
    int data;
    LNode *next;
};

class LinkQueue
{
private:
    LNode *front,*rear;
public:
    LinkQueue();                            //初始化队列
    bool EnQueue(const int &v);             //将传入的值入队
    bool DeQueue(int &v);                   //将队头元素出队，并返回队头元素
    bool DeQueue();                         //将队头元素出队
    bool GetHead(int &v);                   //获取队头元素
    int GetHead();                          //获取队头元素（重载版本）
    bool isEmpty();                         //判断队列是否为空
    ~LinkQueue();                           //析构队列
};

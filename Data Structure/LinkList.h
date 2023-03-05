struct LNode
{
    int data;
    LNode *next;
    LNode(int v): data(v), next(NULL) {};
};

class LList
{
private:
    LNode *head;                                       //记录头结点
    LNode *tail;                                       //记录尾结点
    int len;                                           //记录链表长度
public:
    LList();
    LNode* GetElem(const int &pos);                    //按位查找指定结点
    LNode* LocateElem(const int &v);                   //按值查找指定结点
    int GetValue(const int &pos);                      //获取指定位置的结点内部值
    int Size();                                        //返回链表长度
    bool ModValue(const int &v,const int &pos);        //修改指定位置的结点值
    bool ListInsert(const int &v,const int &pos);      //在指定pos上插入新结点
    bool ListInsert(const int &v);                     //在尾部插入一个新结点
    bool HeadInsert(const int &v);                     //在头部插入一个新结点
    bool InsertNextNode(const int &pos,const int &v);  //在指定结点后插入一个新结点
    bool PopLNode(const int &pos,int &v);              //去除指定位置结点，并保存其值
    bool PopLNode(const int &pos);                     //仅删除指定位置结点
    bool PopLNode();                                   //删除尾结点
    void PrintList();                                  //输出整张表内容
    bool isEmpty();                                    //检查链表是否为空
    ~LList();
};
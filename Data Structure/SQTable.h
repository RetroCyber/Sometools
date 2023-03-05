class SQTable
{
private:
    int *data;
    int Maxsize;
    int length;
public:
    SQTable(int init_size);  //依靠传入长度(init_size)创建顺序表实例
    bool EnlargeSize(int len);   //按传入长度(len)扩大实例大小
    int GetElem(int pos);       //按传入位置(pos)返回对应的元素
    int LocateElem(int v);       //按传入的值(v)返回其所在位置
    int len();               //返回表的长度(length)
    bool InsertElem(int v, int pos); //将数据(value)插入到表的指定位置(pos)
    bool InsertElem(int v);  //向表中插入一个数据
    bool PopElem(int pos);   //弹出指定位置的元素
    void PrintTable();      //输出表的各项值
    bool isEmpty();         //判断列表是否为空
    ~SQTable();         //对表实例进行析构
};


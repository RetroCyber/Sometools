class SqStack
{
private:
    int *stack;
    int top = -1;
    int len = 0;
public:
    SqStack(const int &num);    //栈的构造函数
    bool Push(const int &v);    //将元素压入栈顶
    bool Pop(int &v);           //将栈顶元素弹出
    bool GetTop(int &v);        //获取栈顶元素
    int GetTop();               //上一功能的重载版本
    bool StackEmpty();          //判断是否为空栈
    ~SqStack();                 //析构函数
};

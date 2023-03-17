/****************************************
 数据结构类型：顺序栈
 完成时间：2023-03-18
 ****************************************/
#include "SqStack.h"
#include <stdio.h>

SqStack::SqStack(const int &num)
{
    this->stack = new int[num];         //按传入长度创建栈
    this->len = num;
}

bool SqStack::Push(const int &v){
    bool ret = false;
    if(top != this->len-1){
        this->stack[++this->top] = v;  //若栈未满则将传入值压入栈顶
        ret = true;
    }
    return ret;
}

bool SqStack::Pop(int &v){
    bool ret = false;
    if(!StackEmpty()){
        v = this->stack[this->top--];//若栈非空则将栈顶元素弹出
        ret = true;
    }
    return ret;
}

int SqStack::Pop(){
    int ret = -1;
    if(!Pop(ret))
        printf("Fatal:Can't pop the top element\n");
    return ret;
}

bool SqStack::GetTop(int &v){
    bool ret = false;
    if(!StackEmpty()){
        v = this->stack[this->top];//若栈非空则返回栈顶元素
        ret = true;
    }
    return ret;
}

int SqStack::GetTop(){
    int ret = -1;
    if(GetTop(ret))
        printf("SUCC\n");
    else
        printf("ERROR\n");
        return ret;
}

bool SqStack::StackEmpty(){
    bool ret = true;
    if(this->top)
        ret = false;            //栈顶指针非空时不为空栈
    return ret;
}

SqStack::~SqStack()
{
    delete[] this->stack;
}

int main(){
    SqStack L(10);
    for(int i=1;i<11;i++)
        L.Push(i);
    printf("Stack Top=%d\n",L.GetTop());
    return 0;
}
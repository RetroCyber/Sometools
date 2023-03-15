/****************************************
 数据结构类型：链式队列（带头结点）
 完成时间：2023-03-16
 ****************************************/
#include "LinkQueue.h"
#include <stdio.h>
LinkQueue::LinkQueue()
{
    this->front = new LNode;
    this->front->next = NULL;           //头结点创建后指针域为空
    this->rear = this->front;           //队头队尾指向同一处
}

bool LinkQueue::EnQueue(const int &v){
    bool ret = false;
    LNode *t = new LNode;
    if (t != NULL)                      //避免少量可能发生的内存已满情况（）
    {
        t->data = v;
        t->next = NULL;
        this->rear->next = t;           //将新结点入队
        this->rear = t;                 //队尾指针更新
        ret = true;
    }else
        printf("Failed to enqueue!\n");
    return ret;
}

bool LinkQueue::DeQueue(int &v){
    bool ret = false;
    if(!isEmpty()){
        LNode *t = this->front->next;
        this->front->next = t->next;    //队头元素更新
        v = t->data;
        if(this->rear == t)
            this->rear = this->front;   //最后一个元素删除时将队头队尾指针复位
        delete[] t;
        t = NULL;
        ret = true;
    }
    return ret;
}

bool LinkQueue::DeQueue(){
    int temp = 0;
    return DeQueue(temp);
}

bool LinkQueue::GetHead(int &v){
    bool ret = false;
    if(!isEmpty()){
        v = this->front->next->data;
        ret = true;
    }
    return ret;
}

int LinkQueue::GetHead(){
    int ret = -1;
    if(!isEmpty())
        GetHead(ret);
    else
        printf("Failed to obtain the head data.\n");
    return ret;
}

bool LinkQueue::isEmpty(){
    bool ret = true;
    if(this->front->next != NULL)
        ret = false;
    return ret;
}

LinkQueue::~LinkQueue()
{
    LNode *t = this->front,*p = NULL;
    while (t != NULL)
    {
        p = t;
        t = t->next;
        delete[] t;                         //普通的链表析构程序
    }
    p = NULL;
    this->front = NULL;
    this->rear = NULL;
}

int main()
{
    LinkQueue L;
    for(int i = 0;i<11;i++)
        L.EnQueue(i);
    printf("The head data=%d\n",L.GetHead());
    L.DeQueue();
    printf("The head data=%d\n",L.GetHead());
    return 0;
}

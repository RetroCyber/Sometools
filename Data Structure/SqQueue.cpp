/****************************************
 数据结构类型：顺序队列
 完成时间：2023-03-15
 ****************************************/
#include "SqQueue.h"
#include <stdio.h>
SqQueue::SqQueue(const int &num)
{
    this->Q.data = new int[num];
    this->Q.front = this->Q.rear = 0;       //队头与队尾指针初始时均指向起始位置
    this->len = num;
}

bool SqQueue::EnQueue(const int &v){
    bool ret = false;
    if(!((this->Q.rear% this->len == this->Q.front) && this->oflg == true)){        //判断是否队满
        this->Q.data[this->Q.rear] = v;
        this->Q.rear=(this->Q.rear + 1) % this->len;                                //队尾指针后移，若已到队尾则直接回到队头
        this->oflg = true;                                                          //设置标志位为插入完成状态
        ret = true;
    }else
        printf("Full Queue!\n");
    
    return ret;
}

bool SqQueue::DeQueue(int &v){
    bool ret = false;
    if(!isEmpty()){
        v = this->Q.data[this->Q.front];
        this->Q.front = (this->Q.front+1) % len;                                    //不执行抹除数据操作，仅将队头指针后移一位，之后入队时会覆盖原数据
        this->oflg = false;                                                         //设置标志位为出队完成状态
        ret = true;
    }
    return ret;
}

bool SqQueue::DeQueue(){
    int v = 0;
    return DeQueue(v);
}

bool SqQueue::GetHead(int &v){
    bool ret = false;
    if(!isEmpty()){
        v = this->Q.data[this->Q.front];
        ret = true;
    }
    return ret;
}

int SqQueue::GetHead(){
    int ret = -1;
    if(!GetHead(ret))
        printf("ERROR!\n");
    return ret;
}

bool SqQueue::isEmpty(){
    bool ret = false;
    if((this->Q.front == this->Q.rear) && this->oflg == false){         //若队头队尾指向同一处且位于出队状态时，为空队列
        ret = true;
    }
    return ret;
}

SqQueue::~SqQueue()
{
    delete[] this->Q.data;
}

int main(){
    SqQueue L(10);
    for (int i = 0; i < 11; i++)
        L.EnQueue(i);
    printf("Head data=%d\n",L.GetHead());
    L.DeQueue();
    printf("Head data=%d\n",L.GetHead());
    return 0;
}
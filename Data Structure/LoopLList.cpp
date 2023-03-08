/****************************************
 数据结构类型：循环单链表
 完成时间：2023-03-08
 ****************************************/
#include <stdio.h>
#include "LoopLList.h"

LoopLL::LoopLL(){
    this->head = new LNode(-1);
    this->head->next = this->head;      //让尾结点指向头结点，形成循环
    this->tail = this->head;
    this->len = 0;
}

int LoopLL::GetValue(const int &pos){
    int ret = -1;
    LNode *p = GetElem(pos);
    if(p != NULL)
        ret = p->data;                 //结点位置合法则返回该节点的值
    else
        printf_s("Invalid position!\n");
    return ret;
}

int LoopLL::Size(){
    return this->len;
}

LNode* LoopLL::LocateElem(const int &v){
    LNode* p = NULL;
    if(!isEmpty())
        for (p = this->head->next; p != this->head; p = p->next)
            if(p->data = v)
                break;
    return p;

}

bool LoopLL::ModValue(const int &v,const int &pos){
    bool ret = false;
    LNode *p = GetElem(pos);
    if(p != NULL)
        p->data = v;
    return ret;
}

LNode* LoopLL::GetElem(const int &pos){
    LNode *ret = NULL;
    LNode *p = this->head;
    if(pos < 0 || pos > this->len){
        printf_s("Invalid position!\n");        //结点非法则直接返回
        p = NULL;
    }
    else if (pos == this->len)
        ret = this->tail;                       //若查找尾部则直接返回尾结点
    else
    {
        for (int i = 0; i < pos; i++)
            if(p->next != this->head)
                p = p->next;                        //定位至指定结点
        if(p != NULL)
            ret = p;
    }
    return ret;
}

bool LoopLL::ListInsert(const int &v){
    bool ret =false;
    LNode *p = this->tail;              //获取尾结点指针
    p->next = new LNode(v);
    if(p->next != NULL){
        this->tail = p->next;           //更新尾结点指针
        this->tail->next = this->head;
        ret = true;
        ++this->len;
    }
    return ret;
}

bool LoopLL::ListInsert(const int &v,const int &pos){
    bool ret = false;
    if(pos == this->len)
        ListInsert(v);
    else if (pos == 1)
        HeadInsert(v);
    else{
        LNode *p = GetElem(pos);
        LNode *tp = new LNode(p->data);         
        if(tp != NULL){
        tp->next = p->next;
        p->next = tp;                       //拷贝该结点数据并将其后移一位
        p->data = v;                        //将传入值填入该位
        ret = true;
        ++this->len;
        }
    }
    return ret;
}

bool LoopLL::HeadInsert(const int &v){
    bool ret = false;
    LNode *p = this->head;
    LNode *tp = new LNode(v);
    if(tp != NULL){
        tp->next = p->next;
        p->next = tp;
        ret = true;
        ++this->len;
    }
    return ret;
}

bool LoopLL::InsertPriorNode(const int &pos,const int &v){
    bool ret = ListInsert(v,pos);
    return ret;
}

bool LoopLL::InsertNextNode(const int &pos,const int &v){
    bool ret = ListInsert(v,pos + 1);
    return ret;
}

bool LoopLL::PopLNode(const int &pos,int &v){
    bool ret = false;
    LNode *p = GetElem(pos);
    if(p != NULL){
        LNode *tp = p->next;
        if(tp != this->head){
        p->next = tp->next;
        v = p->data;
        p->data = tp->data;                 //拷贝该结点的下一结点数据并接管之后的链表
        delete[] tp;
        tp = NULL;
        ret = true;
        --this->len;
        }
        else if (!isEmpty())                //处理尾结点，但不清理整个链表对象
        {
            while (tp->next != this->tail)
                tp = tp->next;              //获取倒数第二个结点
            v = p->data;
            tp->next = this->head;          //将前一结点断尾
            this->tail = tp;                //更新尾结点指针
            delete[] p;
            p = NULL;
            ret = true;
            --this->len;
        }
    }
    return ret;
}

bool LoopLL::PopLNode(const int &pos){
    int temp = 0;
    bool ret = PopLNode(pos,temp);
    return ret;
}

bool LoopLL::PopLNode(){
    return PopLNode(len);
}

void LoopLL::PrintList(){
    LNode *p = this->head->next;
    while(p != this->head){
        printf_s("%d ",p->data);
        p = p->next;
    }
    printf_s("\n");
}

bool LoopLL::isEmpty(){
    bool ret = true;
    if(this->head->next != this->head)
        ret = false;
    return ret;
}

LoopLL::~LoopLL()
{
    LNode *tp = NULL;
    for (LNode *p =this->head;p != this->tail;)
    {
        tp = p;
        p = p->next;
        delete[] tp;
        tp = NULL;
    }
    this->tail->next = NULL;
    delete[] this->tail;                //由于尾节点带有头结点指针，单独处理
    this->tail = NULL;
}

int main(){
    LoopLL cl;
    int a[10] = {12,13,14,15,32,12,435,32,45,1243};
    int num = 0;
    for(int i=0;i<10;i++)
        cl.ListInsert(a[i]);
    cl.PrintList();
    printf_s("Test value=%d\n",cl.GetValue(1));
    cl.PrintList();
    return 0;
}
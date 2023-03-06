/****************************************
 数据结构类型：双链表
 完成时间：2023-03-05
 ****************************************/
#include <stdio.h>
#include "DoubleLList.h"

DLList::DLList(){
    this->head = new LNode(-1);
    this->tail = this->head;
}

int DLList::GetValue(const int &pos){
    int ret = -1;
    LNode *p = GetElem(pos);
    if(p != NULL)
        ret = p->data;                 //结点位置合法则返回该节点的值
    else
        printf_s("Invalid position!\n");
    return ret;
}

int DLList::Size(){
    LNode *p = this->head;
    int len = 0;
    for (;p != NULL;len++)
        p = p->next;
    return len;
}

LNode* DLList::LocateElem(const int &v){
    LNode* p = NULL;
    if(!isEmpty())
        for (p = this->head;p != NULL;p = p->next)
            if(p->data = v)
                break;
    return p;

}

bool DLList::ModValue(const int &v,const int &pos){
    bool ret = false;
    LNode *p = GetElem(pos);
    if(p != NULL)
        p->data = v;
    return ret;
}

LNode* DLList::GetElem(const int &pos){
    LNode *ret = NULL;
    LNode *p = this->head;
    if(pos == 0)
        ret = p;
    else if(pos > 0){
            for (int i = 0; i < pos; i++){
                if(p != NULL)
                    p = p->next;                        //定位至指定结点
                else
                    break;                              //位置非法则立刻结束该循环
            }
            if(p != NULL)
                ret = p;
    }
    return ret;
}

bool DLList::ListInsert(const int &v){
    bool ret =false;
    LNode *p = this->tail;              //获取尾结点指针
    p->next = new LNode(v);
    if(p->next != NULL){
        p->next->prior = p;
        this->tail = p->next;           //更新尾结点指针
        ret = true;
    }
    return ret;
}

bool DLList::ListInsert(const int &v,const int &pos){
    bool ret = false;
    if(pos == 0)
        HeadInsert(v);                      //位置为0表示从头部插入结点
    else{
        LNode *p = GetElem(pos);
        if(p != NULL){
            LNode *tp = new LNode(v);         
            tp->next = p;
            tp->prior = p->prior;          //并联至指定结点与前一结点之间
            p->prior->next = tp;
            p->prior = tp;                 //断开原连接，将新结点接入链表
            ret = true;
        }
    }
    return ret;
}

bool DLList::HeadInsert(const int &v){
    bool ret = false;
    LNode *p = this->head;
    LNode *tp = new LNode(v);
    if(tp != NULL){
        tp->next = p->next;
        tp->prior = p;          //将新结点接入链表中
        if(p->next == NULL)
            this->tail = tp;    //若对空表插入则尾节点指针指向新节点
        p->next = tp;
        ret = true;
    }
    return ret;
}

bool DLList::InsertNextNode(const int &pos,const int &v){
    bool ret = ListInsert(v,pos + 1);
    return ret;
}

bool DLList::PopLNode(const int &pos,int &v){
    bool ret = false;
    LNode *p = GetElem(pos);
    if(p != NULL){
        LNode *tp = p->prior;
        if(tp != NULL){             //不操作头指针
        tp->next = p->next;
        if(p->next != NULL)
            p->next->prior = tp;    //断开欲删除结点连接
        else
            this->tail = tp;        //若为尾结点则更新尾结点指针
        p->next = NULL;
        p->prior = NULL;            //清空该结点指针域，防止可能发生的非法访问
        v = p->data;
        delete[] p;
        p = NULL;                   //防止访问到已释放内存，造成内存泄漏
        ret = true;
        }
    }
    return ret;
}

bool DLList::PopLNode(const int &pos){
    int temp = 0;
    bool ret = PopLNode(pos,temp);
    return ret;
}

bool DLList::PopLNode(){
    bool ret = false;
    LNode *p = this->tail;          //获取尾节点
    LNode *tp = p->prior;
    tp->next = NULL;
    p->prior = NULL;                //断开尾节点
    delete[] p;                     //删除该结点
    p = NULL;
    return ret;
}

void DLList::PrintList(){
    LNode *p = this->head->next;
    while(p != NULL){
        printf_s("%d ",p->data);
        p = p->next;
    }
    printf_s("\n");
}

bool DLList::isEmpty(){
    bool ret = false;
    if(this->tail == this->head)
        ret = true;
    return ret;
}

DLList::~DLList()
{
    LNode *tp = NULL;
    for (LNode *p =this->head;p != NULL;)
    {
        tp = p;
        p->prior = NULL;
        p = p->next;
        delete[] tp;
        tp = NULL;
    }
}

// int main(){
//     DLList cl;
//     int a[10] = {12,13,14,15,32,12,435,32,45,1243};
//     int num = 0;
//     for(int i=0;i<10;i++)
//         cl.ListInsert(a[i]);
//     cl.PrintList();
//     printf("The 3rd val is %d\n",cl.GetValue(3));
//     cl.ListInsert(32,3);
//     cl.PrintList();
//     cl.PopLNode(1,num);
//     cl.PrintList();
//     cl.ModValue(11,2);
//     cl.PrintList();
//     cl.HeadInsert(3456);
//     cl.PrintList();
//     cl.PopLNode();
//     cl.PrintList();
//     cl.InsertNextNode(0,12);
//     cl.PrintList();
//     cl.InsertNextNode(1,21);
//     cl.PrintList();
//     printf("List length=%d\n",cl.Size());
//     return 0;
// }
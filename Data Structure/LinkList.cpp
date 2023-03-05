/****************************************
 数据结构类型：单链表
 完成时间：2023-03-05
 ****************************************/
#include <stdio.h>
#include "LinkList.h"

LList::LList(){
    this->head = new LNode(-1);
    this->tail = this->head;
    this->len = 0;
}

int LList::GetValue(const int &pos){
    int ret = -11;
    LNode *p = GetElem(pos);
    if(p != NULL)
        ret = p->data;                 //结点位置合法则返回该节点的值
    return ret;
}

int LList::Size(){
    return this->len;
}

LNode* LList::LocateElem(const int &v){
    LNode* p = NULL;
    if(!isEmpty())
        for (p = this->head; p != NULL; p = p->next)
            if(p->data = v)
                break;
    return p;

}

bool LList::ModValue(const int &v,const int &pos){
    bool ret = false;
    LNode *p = GetElem(pos);
    if(p != NULL)
        p->data = v;
    return ret;
}

LNode* LList::GetElem(const int &pos){
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
        if(p->next != NULL)
            p = p->next;                        //定位至指定结点
        if(p != NULL)
            ret = p;
    }
    return ret;
}

bool LList::ListInsert(const int &v){
    bool ret =false;
    LNode *p = this->tail;              //获取尾结点指针
    p->next = new LNode(v);
    if(p->next != NULL){
        this->tail = p->next;           //更新尾结点指针
        ret = true;
        ++this->len;
    }
    return ret;
}

bool LList::ListInsert(const int &v,const int &pos){
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

bool LList::HeadInsert(const int &v){
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

bool LList::InsertNextNode(const int &pos,const int &v){
    bool ret = ListInsert(v,pos + 1);
    return ret;
}

bool LList::PopLNode(const int &pos,int &v){
    bool ret = false;
    LNode *p = GetElem(pos);
    if(p != NULL){
        LNode *tp = p->next;
        if(tp != NULL){
        p->next = tp->next;
        v = p->data;
        p->data = tp->data;                 //拷贝该结点的下一结点数据并接管之后的链表
        delete[] tp;
        tp = NULL;
        ret = true;
        --this->len;
        }
        else if (!isEmpty())                //确保不删除头结点本身
        {
            tp = GetElem(len-1);            //获取倒数第二个结点
            v = p->data;
            tp->next = NULL;                //将前一结点断尾
            this->tail = tp;
            delete[] p;
            p = NULL;
            ret = true;
            --this->len;
        }
    }
    return ret;
}

bool LList::PopLNode(const int &pos){
    int temp = 0;
    bool ret = PopLNode(pos,temp);
    return ret;
}

bool LList::PopLNode(){
    return PopLNode(len);
}

void LList::PrintList(){
    LNode *p = this->head->next;
    while(p != NULL){
        printf_s("%d ",p->data);
        p = p->next;
    }
    printf_s("\n");
}

bool LList::isEmpty(){
    bool ret = true;
    if(this->head->next != NULL)
        ret = false;
    return ret;
}

LList::~LList()
{
    LNode *tp = NULL;
    for (LNode *p =this->head;p != NULL;)
    {
        tp = p;
        p = p->next;
        delete[] tp;
        tp = NULL;
    }
}

// int main(){
//     LList cl;
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
//     printf("List length=%d\n",cl.Size());
//     cl.PopLNode();
//     cl.PrintList();
//     return 0;
// }
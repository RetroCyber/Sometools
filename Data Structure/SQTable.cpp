/****************************************
 数据结构类型：顺序表
 完成时间：2023-02-25
 ****************************************/
#include "SQTable.h"
#include <stdio.h>
SQTable::SQTable(int init_size){
    data =new int(init_size);
    length = 0;
    Maxsize = init_size;
}

int SQTable::GetElem(int pos){
    if(pos <= length)
        return data[pos-1];                         //判断欲获取数据的位置，若大于表长度则返回假，否则返回对应的数据。
    printf("Invalid position!\n");
    return 0;
}

int SQTable::LocateElem(int v){
    int pos = -1;
    if(!isEmpty())
        for (int i = 0; i < length; i++)            //遍历表进行值对比，找到则返回该位置，表为空或未找到则返回-1
            if (data[i] == v)
                pos = i+1;
return pos;    
}
//直接插入元素
bool SQTable::InsertElem(int v){
    if(length == Maxsize){                          //若表长度已达最大容量则先行扩充10个位置，下同
        EnlargeSize(10);
        Maxsize += 10;
    }
    data[length] = v;                               //在表的最后插入数据
    ++length;
    return true;
}
//在指定位置插入元素
bool SQTable::InsertElem(int v, int pos){
    if(length == Maxsize){                          
        EnlargeSize(10);
        Maxsize += 10;
    }
    for (int i = length; i >= pos; i--)
        data[i] = data[i-1];                        //该位置之后的数据整体后移1位
    data[pos-1] = v;
    ++length;                                       //表因插入一个数据而增长1个长度
    return true;
}
//删除指定位置表元素
bool SQTable::PopElem(int pos){
    if(pos == 0 || pos > length)
        return false;                               //判断传入位置量是否非法
    for (int i = pos-1; i < length; i++)
        data[i] = data[i+1];                        //数据全部前移一位
    --length;
    return true;
}
//扩充表长度
bool SQTable::EnlargeSize(int len){
    int *temp = new int(Maxsize + len);             //申请新表内存
    for (int i = 0; i < length; i++)
        temp[i] = data[i];                          //对旧表的内容进行复制
    Maxsize += len;
    printf("SUCC! Maxsize=%d\n",Maxsize);
    delete[] data;                                  //销毁旧表内存
    data = temp;                                    //取得对新表的访问权
    return true;
}
//返回表长度
int SQTable::len(){
    return length;
}
//输出表的内容
void SQTable::PrintTable(){
    for (int i = 0; i < length; i++)
    printf("%d\t",data[i]);
    printf("\n");
}
//判断表是否为空，为空返回真，否则返回假。
bool SQTable::isEmpty(){
    bool flag = true;
    if(length > 0)
        flag = false;
return flag;
}

SQTable::~SQTable(){
    delete[] data;
}
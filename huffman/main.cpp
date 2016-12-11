//
//  main.cpp
//  huffman
//
//  Created by Jiang Zhenfei on 2016/12/6.
//  Copyright © 2016年 姜震飞. All rights reserved.
//

/*
 第1题 二叉树的存储结构的建立与遍历算法
 （1）已知一棵二叉树的先序序列和中序序列，是否能够唯一确定这棵二叉树？如果能，请证明，并编写实现构造该二叉树的程序；否则，举出反例。
 可以
 （2）已知一棵二叉树的后序序列和中序序列，是否能够唯一确定这棵二叉树？如果能，请证明，并编写实现构造该二叉树的程序；否则，举出反例。
 可以
 （3）已知一棵二叉树的先序序列和后序序列，是否能够唯一确定这棵二叉树？如果能，请证明，并编写实现构造该二叉树的程序；否则，举出反例。
 不可以
 */
#include <fstream>
#include <iostream>

#define max 100
#define ascii 128
using namespace std;
typedef struct {
    char bits[ascii+1];
} code_node;
typedef code_node huffmancode[ascii];
typedef int datatype;// change type here
//binary tree defined below
struct node{
    struct node *lchild;
    struct node *rchild;
    datatype data;
};
typedef struct node * btree;
int indx[max];
void ind(int inorder[],int n)
{
    for (int i = 0;  i < n; i++) {
        indx[inorder[i]] = i;
    }
}
//level order traversal
typedef struct{
    int weight;
    int lchild;
    int rchild;
    int parent;
}huff_node;
typedef huff_node HuffmanT[2*ascii-1];
void level_order(btree root)
{
    btree Q[max],q;//queue
    int front,rear;
    front = rear = 0;//顺序队列
    if (root == nullptr) {
        return;
    }
    Q[++rear] = root;
    while (front != rear) {
        q = Q[++front];
        cout << q->data<<" ";
        if (q->lchild!=nullptr) {
            Q[++rear] = q->lchild;
        }
        if (q->rchild!=nullptr) {
            Q[++rear] = q->rchild;
        }
    }
}
btree inorder_preorder_build(int pre[],int n, int offset){
    if (n == 0) return nullptr;
    int rootval = pre[0];
    int i = indx[rootval] - offset;
    btree root = new struct node;
    root->data = rootval;
    root->lchild = inorder_preorder_build(pre+1, i, offset);
    root->rchild = inorder_preorder_build(pre+i+1, n-i-1, offset+i+1);
    return root;
}
void build_in_pre_test()
{
    int preorder[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    int inorder[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    int n = sizeof(inorder)/sizeof(inorder[0]);//树的节点数
    int offset = 0;//子树开始的位置
    ind(inorder, n);
    btree root = inorder_preorder_build(preorder, n, offset);
    level_order(root);
    cout <<endl;
}
btree inorder_postorder_build(int post[],int n,int offset)
{
    
    if(n == 0) return nullptr;
    int rootval = post[n-1];
    int i = indx[rootval] - offset;
    btree root = new struct node;
    root->data = rootval;
    root->lchild = inorder_postorder_build(post, i, offset);
    root->rchild = inorder_postorder_build(post+i, n-i-1, offset+i+1);
    return root;
}
void build_in_post_test()
{
    int postorder[] = {17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
    int inorder[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    int n = sizeof(inorder)/sizeof(inorder[0]);//树的节点数
    int offset = 0;//子树开始的位置
    ind(inorder, n);
    btree root = inorder_postorder_build(postorder, n, offset);
    level_order(root);
    cout <<endl;
}
/*
 第2题 哈夫曼编码与译码系统
 哈夫曼编码是一种可以哈夫曼树（最优二叉树，带权路径长度最小的二叉树）为基础变长编码方式。它的基本思想是：将使用次数多的代码转换成长度较短的编码，而使用次数少的可以使用较长的编码，并且保持编码的唯一可解性。在计算机信息处理中，经常应用于数据压缩。是一种一致性编码法（又称"熵编码法"），用于数据的无损耗压缩。要求实现一个完整的哈夫曼编码与译码系统。
 要求：
 1.从文件中读入任意一篇英文文本文件，分别统计英文文本文件中各字符（包括标点符号和空格）使用频率；
 2.根据字符的使用频率分别构造哈夫曼树，并给出每个字符的哈夫曼编码；
 3.将文本文件利用哈夫曼树进行编码，存储成压缩文件（哈夫曼编码文件）；
 4.计算哈夫曼编码文件的压缩率；
 5.将哈夫曼编码文件译码为文本文件，并与原文件进行比较。
 */
void readtxt(int frequency[])
{
    ifstream in("test.txt");
    char c;
    
    if (in.fail()) {
        cout << "Could not find file, please check"<<endl;
    }
    while (in.get(c)) {
        frequency[c] ++;
    }
    in.close();
    //print frequency
    /*
     for (int i = 0; i < ascii; i++) {
     printf("%c",i);
     cout <<":"<< count[i]<<endl;
     }
     */
}
void huffcreate(HuffmanT T,int fre[])
{
    int p1,p2,min1,min2;
    for (int i = 0; i < ascii*2-1; i++) {//initialize
        T[i].lchild = -1;
        T[i].rchild = -1;
        T[i].parent = -1;
    }
    for (int i = 0; i < ascii; i++) {
        T[i].weight = fre[i];
        //input weight
    }
    //merge * n-1 times
    for (int i = ascii; i < 2*ascii -1; i++) {
        //select min between 『0』 and 『i-1』
        p1 = 0;
        p2 = 0;
        min1 = 99999999, min2 = 99999999;
        for (int j = 0; j < i; j++) {
            if (T[j].parent == -1) {
                min1 = T[j].weight;
                p1 = j;
                for (int k = j+1; k < i; k++) {
                    if (T[k].parent == -1) {
                        min2 = T[k].weight;
                        p2 = k;
                        break;
                    }
                }
                break;
            }
        }
        for (int j = 0; j < i; j++) {
            if (T[j].parent == -1 && T[j].weight <= min1 && j != p1) {
                min2 = min1;
                p2 = p1;
                min1 = T[j].weight;
                p1 = j;
            }
        }
        T[p1].parent = T[p2].parent = i;
        T[i].lchild = p1;
        T[i].rchild = p2;
        T[i].weight = T[p1].weight + T[p2].weight;
    }
}
void char_set_huff_encode(HuffmanT T,huffmancode H)
{
    int c, p , i;
    char cd[ascii+1];
    int start;
    cd[ascii] = '\0';
    for (i = 0; i < ascii; i++) {
        start = ascii;
        c = i;
        while ((p = T[c].parent) >= 0) {
            cd[--start] = (T[p].lchild == c)?'0':'1';
            c = p;
        }
        strcpy(H[i].bits, &cd[start]);
    }
}
void huff_coding(huffmancode H)
{
    ifstream in("test.txt");
    ofstream out("out.txt");
    char c;
    if (in.fail()) {
        cout << "Could not find test.txt, please check"<<endl;
    }
    if (out.fail()) {
        cout << "Could not find out.txt, please check"<<endl;
    }
    while (in.get(c)) {
        out << H[c].bits;
    }
    in.close();
    out.close();
}
void decode(HuffmanT T, int n)
{
    ifstream de("out.txt");
    ofstream deo("decode.txt");
    char c;
    char temp;
    int x;
    if (de.fail()) {
        cout << "Could not find out.txt, please check"<<endl;
    }
    x = n;
    while (de.get(c)) {
        if (c == '0') {
            x = T[x].lchild;
        }
        else if (c == '1') {
            x = T[x].rchild;
        }
        if (T[x].lchild == -1||T[x].rchild == -1) {
            temp = x;
            deo << temp;
            x = n;
        }
    }
    de.close();
    deo.close();
}
void huffman_test()
{
    
    int frequency[ascii] = {0};
    readtxt(frequency);
    HuffmanT T;
    huffmancode H;
    huffcreate(T,frequency);
    char_set_huff_encode(T, H);
    huff_coding(H);
    decode(T, ascii * 2 -2);
    return;
}
int main(int argc, const char * argv[]) {
    build_in_pre_test();
    build_in_post_test();
    huffman_test();
    return 0;
}

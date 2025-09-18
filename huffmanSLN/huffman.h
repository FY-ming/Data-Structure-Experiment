#pragma once
// huffman.h 头文件
// 定义哈夫曼树及其相关操作
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Huffman 树节点
typedef struct HTNode {
    int weight;  // 权值
    int parent;  // 父亲节点下标
    int lchild;  // 左孩子下标
    int rchild;  // 右孩子下标
} HTNode, * HuffmanTree;

int Select(HuffmanTree pHT, int nSize);
void Select(HuffmanTree HT, int n, int* s1, int* s2);
void CreateHuffmanTree(HuffmanTree* HT, int* weight, int n);
void CreateHuffmanCode(HuffmanTree HT, char*** HC, int n);
void HuffmanDecode(HuffmanTree HT, char* code, int root, int n);
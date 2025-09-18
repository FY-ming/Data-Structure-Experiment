// main.cpp文件
// 程序主入口
#define _CRT_SECURE_NO_WARNINGS
#define BUFFER_SIZE 4096
#include <iostream>
#include <stdlib.h>
#include "huffman.h"
#include "compress.h"
//#include <string>


using namespace std;

void countByteFrequencies(const char* filename, int weight[]) {
    // 打开文件
    FILE* in = fopen(filename, "rb");
    if (in == NULL) {
        // 若文件打开失败，输出错误信息并终止程序
        perror("无法打开文件");
        exit(EXIT_FAILURE);
    }

    unsigned char buffer[BUFFER_SIZE];
    size_t bytesRead;

    // 循环读取文件内容到缓冲区
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, in)) > 0) {
        // 遍历缓冲区中的每个字节
        for (size_t i = 0; i < bytesRead; i++) {
            // 统计每个字节的出现次数
            weight[(unsigned char)buffer[i]]++;
        }
    }

    // 关闭文件
    fclose(in);
}
void TestHufTree(HuffmanTree pHT)
{
    // ..
    cout << "字符" << "\t权重" << "\t父亲节点" << "  左孩子" << "\t右孩子" << endl;
    for (int i = 1; i < 512; i++)
    {
        printf("pHT[%d]\t%d\t%d\t%d\t%d\n", i, pHT[i].weight, pHT[i].parent,
            pHT[i].lchild, pHT[i].rchild);
    }
}
void TestHufTreeN(int root, HuffmanTree pHT, int &n)
{
    cout << pHT[root].weight << " ";
    n++;
    if (pHT[root].lchild != 0)
    {
        TestHufTreeN(pHT[root].lchild, pHT, n);
    }
    if (pHT[root].rchild != 0)
    {
        TestHufTreeN(pHT[root].rchild, pHT, n);
    }
}
void TestHufCode(int root, HuffmanTree pHT, char** pHC)
{
    if (pHT[root].lchild == 0 && pHT[root].rchild == 0)
    {
        printf("0x%02X %s\n", root - 1, pHC[root - 1]);
    }
    if (pHT[root].lchild)//访问左孩子
    {
        TestHufCode(pHT[root].lchild, pHT, pHC);
    }
    if (pHT[root].rchild)//访问右孩子
    {
        TestHufCode(pHT[root].rchild, pHT, pHC);
    }
}

int main()
{
	// 提示用户进行交互操作
	cout << "============= Huffman 文件压缩 ==============" << endl
		<< "请输入文件名" << endl;
	char filename[256];
    // picture.bmp
	cin >> filename;
    cout << "文件 " << filename << " 的大小是 " << getFileSize(filename) << " 字节。" << endl;
	int weight[256] = { 0 }; // 256个字节，下标i从0~255
    // 调用函数统计字节频率
    countByteFrequencies(filename, weight);

    // //统计文件字节频率函数测试
    // //输出每个字节的出现次数
    //for (int i = 0; i < 256; i++) {
    //    printf("字节 0x%02X 出现次数: %d\n", i, weight[i]); // %02X 用于将一个整数以十六进制形式输出，保证输出的十六进制数宽度为 2 位，不足 2 位时在前面补 0。
    //} // 出现次数正确

    int n = sizeof(weight) / sizeof(weight[0]); // 叶子节点数，256个字节，256个叶子节点
    HuffmanTree HT; // 保存哈夫曼树信息
    char** HC; // 保存哈夫曼编码信息
    CreateHuffmanTree(&HT, weight, n); // 初始化哈夫曼树

    ////测试哈夫曼树构建情况
    ////输出哈夫曼树相关节点信息
    TestHufTree(HT); // 正常


    ////测试前序遍历哈夫曼树（中左右）
    ////输出所有哈夫曼树的节点，所以包括了文件的256个数据节点和生成的其他非叶子节点
    //cout << "前序遍历哈夫曼树，依次输出哈夫曼树节点权重" << endl;
    //int test = 0;
    //TestHufTreeN(511, HT, test); // 针对256个字节的哈夫曼树，根节点下标应该为 256 + 256 - 1 = 511
    //cout << "遍历次数为" << test << endl;


    CreateHuffmanCode(HT, &HC, n);// 生成哈夫曼编码


    // //测试哈夫曼编码生成情况
    cout << "输出哈夫曼树节点的哈夫曼编码" << endl;
    for (int i = 1; i <= n; i++) {
        printf("0x%02X 节点的哈夫曼编码: %s\n", i-1, HC[i]); // HC一共创建了256+1个空间，其中HC[0]不存放数据
    }


    // 压缩文件测试代码
    if (Compress(filename, weight)) {
        std::cout << "文件压缩成功" << std::endl;
    }
    else {
        std::cout << "文件压缩失败" << std::endl;
    }


	return 0;
}

#pragma once
// compress.h文件
// 调用huffman.h文件，使用哈夫曼树数据结构
// 声明文件压缩相关操作 
#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;

// 文件头
struct HEAD
{
	char type[4];    // 文件类型
	int length;      // 原文件长度
	int weight[256]; // 权值数值
};
streamsize getFileSize(const string& filename);
int Compress(const char* pFilename, int* weight);
int Encode(const char* pFilename, char** HC, char** pBuffer, const int nSize);
char Str2byte(const char* pBinStr);
int InitHead(const char* pFilename, HEAD& sHead);
int WriteFile(const char* pFilename, const HEAD sHead, char* pBuffer, const int nSize);
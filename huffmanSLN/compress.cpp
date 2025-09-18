// compress.cpp文件
// 调用huffman.h文件，使用哈夫曼树数据结构
// 实现文件压缩相关操作
#define _CRT_SECURE_NO_WARNINGS
#define SIZE 256
#include "compress.h"
#include "huffman.h"


// 获取文件大小的函数
streamsize getFileSize(const string& filename) {
	ifstream file(filename, ios::binary | ios::ate);
	if (!file) {
		cerr << "无法打开文件" << endl;
		return -1;
	}
	// 获取当前文件指针的位置，即文件的大小
	streamsize size = file.tellg();
	// 关闭文件
	file.close();
	return size;
}

int Compress(const char* pFilename, int *weight)
{
	int n = 256; // 叶子节点数，256个字节，256个叶子节点
	HuffmanTree HT; // 保存哈夫曼树信息
	char** HC; // 保存哈夫曼编码信息
	CreateHuffmanTree(&HT, weight, n); // 初始化哈夫曼树 CreateHuffmanCode(HT, &HC, n);// 生成哈夫曼编码
	CreateHuffmanCode(HT, &HC, n);// 生成哈夫曼编码

	//计算编码缓冲区大小
	// 得到 Huffman 编码之后, 需要计算编码完整个文件需要的空间大小
	// 计算的方法为:先计算每个字符的权值乘以该字符编码的长度, 这样即为同一个字符在编码文件中所占的大小, 将256个这样的值相加即为编码整个文件所需的空间大小。除以8即为总字节数。
	int nSize = 0;
	for (int i = 0; i < 256; i++)
	{
		nSize += weight[i] * strlen(HC[ i + 1 ]); // HC数组从下标1开始记录数据
	}
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;
	// 对原文件进行压缩编码
	char* pBuffer = NULL;
	int pos = Encode(pFilename, HC, &pBuffer, nSize);
	if (!pBuffer)
	{
		return 0;
	}

	//// 创建压缩文件测试文件
	//FILE* out = fopen("compressed.huf", "wb");
	//if (!out) {
	//	perror("无法打开压缩文件");
	//	free(pBuffer);
	//	return 0;
	//}
	//fwrite(pBuffer, 1, pos, out);
	//fclose(out);

	HEAD head;
	InitHead(pFilename, head); // 初始化文件头
	WriteFile(pFilename, head, pBuffer, pos);

	//// 测试代码
	//// 以二进制形式输出 pBuffer 的内容
	//std::cout << "压缩后的数据（二进制）: ";
	//for (int i = 0; i < pos; ++i) {
	//	std::cout << std::bitset<8>(static_cast<unsigned char>(pBuffer[i]));
	//}
	//std::cout << std::endl;

	// 释放内存
	for (int i = 1; i <= n; i++) {
		free(HC[i]);
	}
	free(HC);
	free(HT);
	free(pBuffer);
	return 1; // 成功返回true信号
	// ...
}

int Encode(const char* pFilename, char** HC, char** pBuffer, const int nSize)
{
	// 一边读取一边查找 Huffman 编码表,将每个字符用相应的编码替换掉,这样就得到了重新编码后的文件。
	// 此时, 我们会发现得到的新的文件比原来的文件还要大, 所以我们需要对得到的编码进行压缩。
	
	// 打开原文件
	FILE* in = fopen(pFilename, "rb");
	if (!in) {
		perror("无法打开原文件");
		return 0;
	}

	// 开辟缓冲区
	*pBuffer = (char*)malloc(nSize * sizeof(char));
	if (!*pBuffer) {
		perror("开辟缓冲区失败");
		fclose(in);
		return 0;
	}

	char cd[SIZE] = { 0 }; // cd 是一个字符数组，作为工作区，用于临时存储拼接的哈夫曼编码字符串。初始化为全 0。cd大小保证大于哈夫曼编码最大长度即可
	int pos = 0; // pos 是 pBuffer 缓冲区的指针，用于记录当前存储位置，初始化为 0。
	int ch;
	while ((ch = fgetc(in)) != EOF)
	{
		strcat(cd, HC[ch + 1]); // 将当前读取字符 ch 对应的哈夫曼编码 HC[ch+1] 拼接到 cd 字符串的末尾。 cd葱0~255，即00~FF，但HC下标从1开始存放哈夫曼编码
		while (strlen(cd) >= 8) { // 当 cd 字符串的长度达到或超过 8 位时，执行以下操作：
			(*pBuffer)[pos++] = Str2byte(cd); // 调用 Str2byte 函数将 cd 字符串的前 8 位转换为一个字节，并存储到 pBuffer 中，然后 pos 指针向后移动一位。
			memmove(cd, cd + 8, strlen(cd + 8) + 1); // 使用 memmove 函数将 cd 字符串中第 9 位及以后的部分向前移动 8 位，覆盖掉前 8 位，以继续处理后续的编码。strlen(cd + 8) + 1，strlen(cd + 8) 计算的是从 cd 数组中第 9 个字符开始到字符串结束符 '\0' 之前的字符个数，+ 1 是为了把字符串结束符 '\0' 也包含进来，确保移动后的字符串仍然是一个以 '\0' 结尾的有效字符串。
		}
	}
	// 如果文件读取结束后，cd 字符串中还有剩余的编码（长度大于 0），则通过循环在末尾补 0，使其长度达到 8 位。
	// 最后再调用 Str2byte 函数将补 0 后的 cd 字符串转换为一个字节，存储到 pBuffer 中。
	if (strlen(cd) > 0) {
		// 不足 8 位时补 0
		while (strlen(cd) < 8) {
			strcat(cd, "0");
		}
		(*pBuffer)[pos++] = Str2byte(cd);
	}

	fclose(in);

	return pos;
}

char Str2byte(const char* pBinStr)
{
	// 由于 Huffman 编码表是以字符数组的形式保存的,重新编码后的数据将是一个很长的字符串。定义 Str2byte函数, 将形如“01010101”字符串转换成字节, 才能得到最终的编码。
	// Str2byte 函数的主要功能是将一个长度为 8 的二进制字符串（例如 "01010101"）转换为一个字节（8 位）的二进制数据。在这个过程中，使用了位运算来完成具体的转换工作
	char b = 0x00;
	for (int i = 0; i < 8; i++)
	{
		b = b << 1;//左移1位
		if (pBinStr[i] == '1') {
			b = b | 0x01;
		}
	}
	return b;
}

int InitHead(const char* pFilename, HEAD& sHead)
{
	// 初始化文件头
	strcpy(sHead.type, "HUF");//文件类型
	sHead.length = 0;// 原文件长度
	for (int i = 0; i < SIZE; i++)
	{
		sHead.weight[i] = 0;//权值
	}
	// 以二进制流形式打开文件
	FILE* in = fopen(pFilename, "rb");
	if (!in) {
		perror("无法打开原文件");
		return 0;
	}
	int ch;
	//扫描文件,获得权重
	while ((ch = fgetc(in)) != EOF)
	{
		sHead.weight[ch]++;
		sHead.length++;
	}
	// 关闭文件
	fclose(in);
	in = NULL;
	return 1;
}
int WriteFile(const char* pFilename, const HEAD sHead, char* pBuffer, const int nSize)
{
	// 生成文件名
	char filename00[256] = { 0 };
	strcpy(filename00, pFilename);
	char filename[256] = { 0 };
	strcpy(filename, pFilename);
	strcat(filename, ".huf"); // 为压缩文件添加.huf后缀
	// 以二进制流形式打开文件
	FILE* out = fopen(filename, "wb");
	if (!out) {
		perror("无法打开压缩文件");
		free(pBuffer);
		return 0;
	}
	// 写文件头
	 fwrite(&sHead, sizeof(HEAD), 1, out);
	// 写压缩后的编码
	fwrite(pBuffer, sizeof(char), nSize, out);
	//关闭文件,释放文件指针
	fclose(out);

	cout << "文件头大小：" << sizeof(HEAD) << "字节" << endl;

	out = NULL;
	cout << "生成压缩文件:" << filename << endl;


	// 在代码 sizeof(HEAD) + strlen(pFilename) + 1 + nSize 里加 1，通常是为了包含字符串的终止符 '\0'。strlen 函数返回的是字符串的长度，它不包含字符串末尾的 '\0' 字符。当需要考虑整个字符串在内存中占用的字节数时，就需要额外加上 1 来包含这个终止符。
	// 不过在当前的 WriteFile 函数中，pFilename 并没有被写入文件，所以 strlen(pFilename) + 1 这部分不应该参与生成文件大小的计算。
	int len = sizeof(HEAD) + strlen(pFilename) + 1 + nSize;
	// int len = sizeof(HEAD) + nSize;

	streamsize originalSize = getFileSize(filename00);
	streamsize compressedSize = getFileSize(filename);
	

	// 对标指导书数据版
	float compressionRatio = static_cast<float>(compressedSize + strlen(pFilename) + 1) / originalSize;
	cout << "文件 " << filename00 << " 的大小是 " << originalSize << " 字节。" << endl
		<< "压缩文件 " << filename << " 的大小是 " << (compressedSize + strlen(pFilename) + 1) << " 字节。" << endl
		<< "文件压缩比例为：" << compressionRatio * 100 << "%" << endl;

	//// 将其中一个操作数转换为浮点数以进行浮点数除法，完成压缩比例计算
	//float compressionRatio = static_cast<float> (compressedSize) / originalSize;
	//cout << "文件 " << filename00 << " 的大小是 " << originalSize << " 字节。" << endl
	//	<< "压缩文件 " << filename << " 的大小是 " << compressedSize << " 字节。" << endl
	//	<< "文件压缩比例为：" << compressionRatio * 100 << "%" << endl;

	return len;
}









//// 将字节转换为二进制字符串
//std::string byteToBinaryString(unsigned char byte) {
//	return std::bitset<8>(byte).to_string();
//}
//
//// 解压缩函数
//int Decompress(const char* pFilename) {
//	// 生成解压后的文件名
//	char filename[256] = { 0 };
//	strcpy(filename, pFilename);
//	char* dot = strrchr(filename, '.');
//	if (dot != nullptr) {
//		*dot = '\0';
//	}
//
//	// 以二进制模式打开压缩文件
//	FILE* in = fopen(pFilename, "rb");
//	if (!in) {
//		perror("无法打开压缩文件");
//		return 0;
//	}
//
//	// 读取文件头
//	HEAD head;
//	fread(&head, sizeof(HEAD), 1, in);
//
//	// 根据文件头中的权值数组生成哈夫曼树和哈夫曼编码表
//	HuffmanTree HT;
//	char** HC;
//	CreateHuffmanTree(&HT, head.weight, SIZE);
//	CreateHuffmanCode(HT, &HC, SIZE);
//
//	// 以二进制模式打开解压后的文件
//	FILE* out = fopen(filename, "wb");
//	if (!out) {
//		perror("无法打开解压后的文件");
//		fclose(in);
//		return 0;
//	}
//
//	// 找到哈夫曼树的根节点
//	int root = 2 * SIZE - 1;
//	int current = root;
//
//	unsigned char byte;
//	int count = 0;
//	while (fread(&byte, sizeof(unsigned char), 1, in) == 1) {
//		std::string binaryStr = byteToBinaryString(byte);
//		for (char bit : binaryStr) {
//			if (bit == '0') {
//				current = HT[current].lchild;
//			}
//			else {
//				current = HT[current].rchild;
//			}
//			if (HT[current].lchild == 0 && HT[current].rchild == 0) {
//				fputc(current - 1, out);
//				count++;
//				if (count == head.length) {
//					break;
//				}
//				current = root;
//			}
//		}
//		if (count == head.length) {
//			break;
//		}
//	}
//
//	// 关闭文件
//	fclose(in);
//	fclose(out);
//
//	// 释放内存
//	for (int i = 1; i <= SIZE; ++i) {
//		free(HC[i]);
//	}
//	free(HC);
//	free(HT);
//
//	std::cout << "解压文件: " << filename << std::endl;
//	return 1;
//}
// huffman.cpp 文件
// 实现哈夫曼数的相关操作
#define _CRT_SECURE_NO_WARNINGS

#include "huffman.h"


int Select(HuffmanTree pHT, int nSize)
{
	int minValue = 0x7FFFFFFF;// 最小值
	int min = 0;//元素序号
	//找到最小权值的元素序号
	for (int i = 1; i <= nSize; i++)
	{
		if (pHT[i].parent = 0 && pHT[i].weight < minValue)
		{
			minValue = pHT[i].weight;
			min = i;
		}
	}
    // 返回最小值下标
	return min;
}
// 选择两个权值最小且未被使用过的节点
void Select(HuffmanTree HT, int n, int* s1, int* s2) {
    // 用于在哈夫曼树中选择两个权值最小且未被使用过的节点，返回它们的下标。
    // HuffmanTree HT:一个指向 HTNode 结构体数组的指针,代表哈夫曼树。该数组存储了哈夫曼树的所有节点信息，包括权值、父节点、左右孩子等。
    // int n:表示当前可供选择的节点范围是从 1 到 n
    // 使用整型指针s1,s2保存最小权值的下标
    int min1 = 0x7FFFFFFF, min2 = 0x7FFFFFFF; // 使用极大值初始化min1和min2
    *s1 = *s2 = -1;

    for (int i = 1; i <= n; i++) {
        if (HT[i].parent == 0) {
            if (HT[i].weight < min1) {
                min2 = min1;
                *s2 = *s1;
                min1 = HT[i].weight;
                *s1 = i;
            }
            else if (HT[i].weight < min2) {
                min2 = HT[i].weight;
                *s2 = i;
            }
        }
    }
}

// 创建哈夫曼树
void CreateHuffmanTree(HuffmanTree* HT, int* weight, int n) {
    // 创建哈夫曼树,首先初始化所有节点，然后不断选择两个权值最小的节点合并成一个新节点，直到所有节点合并成一棵完整的哈夫曼树。
    // HuffmanTree * HT:是一个指向 HuffmanTree（即指向 HTNode 结构体数组的指针）的指针。使用二级指针的原因是要在函数内部为 HT 分配内存空间并修改其指向，所以需要传递指针的地址。
    // int * weight:是一个整型数组指针，该数组存储了哈夫曼树所有叶子节点的初始权值。
    // int n:表示叶子节点的数量。也就是 weight 数组的元素个数，它决定了哈夫曼树的叶子节点规模。
    if (n <= 1) return;
    int m = 2 * n - 1; // 哈夫曼树的所有节点数：n+(n-1)。n:叶子节点数，（n-1）:非叶子数
    *HT = (HuffmanTree)malloc((m + 1) * sizeof(HTNode)); // +1，用于多创建一个空地址使哈夫曼树下标符号人类思维
    // 检索哈夫曼树下标从1开始
    // 叶子节点
    for (int i = 1; i <= n; i++) {
        (*HT)[i].weight = weight[i - 1];
        (*HT)[i].parent = 0;
        (*HT)[i].lchild = 0;
        (*HT)[i].rchild = 0;
    }
    // 非叶子节点初始化
    for (int i = n + 1; i <= m; i++) {
        (*HT)[i].weight = 0;
        (*HT)[i].parent = 0;
        (*HT)[i].lchild = 0;
        (*HT)[i].rchild = 0;
    }
    // 哈夫曼树构造
    for (int i = n + 1; i <= m; i++) {
        int s1, s2;
        Select(*HT, i - 1, &s1, &s2);
        (*HT)[s1].parent = i;
        (*HT)[s2].parent = i;
        (*HT)[i].lchild = s1;
        (*HT)[i].rchild = s2;
        (*HT)[i].weight = (*HT)[s1].weight + (*HT)[s2].weight;
    }
}
// 生成哈夫曼编码
void CreateHuffmanCode(HuffmanTree HT, char*** HC, int n) {
    // 生成哈夫曼编码，从叶子节点开始，逆向遍历到根节点，根据路径上的左右分支生成编码。
    // HuffmanTree HT：是一个指向 HTNode 结构体数组的指针，代表已经创建好的哈夫曼树。
    // char*** HC：是一个三级字符指针(一级字符指针相当于是一个字符串，即一个哈夫曼编码；二级是储存所有哈夫曼编码的数组；三级是为数组分配空间使用的地址)。它用于存储生成的哈夫曼编码，HC[i] 表示第 i 个叶子节点对应的哈夫曼编码字符串。函数会为每个叶子节点分配存储编码的内存空间，并将编码存储在相应位置。
    // int n：表示叶子节点的数量。它决定了需要为多少个叶子节点生成哈夫曼编码。
    *HC = (char**)malloc((n + 1) * sizeof(char*)); // +1，用于多创建一个空地址使哈夫曼树下标符号人类思维
    char* cd = (char*)malloc(n * sizeof(char)); // 临时存放哈夫曼编码
    cd[n - 1] = '\0'; // 将数组的最后一个元素设置为字符串结束符 '\0'，以便后续使用字符串处理函数。
    // 外层循环，遍历每个叶子节点（从 1 到 n）
    for (int i = 1; i <= n; i++) {
        int start = n - 1; // 初始化为 n - 1，表示从临时数组的末尾开始存储编码。
        int c = i; // 该节点临时下标,表示当前节点
        int f = HT[i].parent; // 该节点父亲节点临时下标,表示当前节点的父节点
        // while循环，从当前叶子节点开始，逆向遍历到根节点。
        while (f != 0) {
            // 如果当前节点是其父节点的左孩子，则在 cd 数组中存储 '0'；否则存储 '1'
            --start;
            if (HT[f].lchild == c) cd[start] = '0';
            else cd[start] = '1';
            // 向上检索父节点
            c = f;
            f = HT[f].parent;
        }
        (*HC)[i] = (char*)malloc((n - start) * sizeof(char)); // (n - start) 表示当前叶子节点的哈夫曼编码的长度。
        strcpy((*HC)[i], &cd[start]);
    }
    free(cd);
}
// 哈夫曼解码
void HuffmanDecode(HuffmanTree HT, char* code, int root, int n) {
    // 进行哈夫曼解码，根据编码从根节点开始遍历哈夫曼树，遇到叶子节点则输出解码结果。
    // HuffmanTree HT：是一个指向 HTNode 结构体数组的指针，代表已经创建好的哈夫曼树。
    // char* code:是一个字符指针，指向要解码的哈夫曼编码字符串。
    // int root:表示哈夫曼树的根节点下标。
    // int n:表示叶子节点的数量。虽然在这个函数中 n 没有直接用于解码逻辑，但它可能在某些扩展功能或边界检查中会用到。
    int p = root;
    for (int i = 0; code[i] != '\0'; i++) {
        if (code[i] == '0') p = HT[p].lchild;
        else p = HT[p].rchild;

        if (HT[p].lchild == 0 && HT[p].rchild == 0) {
            printf("解码字符对应的位置: %d, 权值: %d\n", p, HT[p].weight);
            p = root;
        }
    }
}
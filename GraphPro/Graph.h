#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<vector>

using std::cout, std::endl, std::string, std::cerr, std::ifstream, std::vector;

const int MAX_VERTEX_NUM = 20;

// 顶点结构体
struct Vex
{
	int num; // 景点编号
	char name[20]; // 景点名称
	char desc[1024]; // 景点描述
};
// 边结构体
struct Edge
{
	int vex1; // 边对应的顶点1
	int vex2; // 边对应的顶点2
	int weight; // 边的权值
};

//// DFS路径结构体
//typedef struct Path
//{
//	int vexs[20]; //保存一条路径
//	Path* next; //下一条路径
//}Path, *PathList;

// 图
class CGraph
{
public:
	void InitGraph(); // 初始化
	bool InsertVex(Vex sVex); // 插入顶点信息
	bool InsertEdge(Edge sEdge); // 插入边信息
	void BuildGraphFromFiles(const string& vexFile, const string& edgeFile); // 读取文件构建图
	
	Vex GetVex(int v) const; // 获取图中指定下标的顶点，下标v大小：0~m_nVexNum
	int Get_m_nVexNum() const; // 获取目前图中的顶点个数
	void QueryRelatedEdgesAndVertices(int vertexNum); // 根据顶点编号查询所有与它有关的边和顶点
	void FindAllPaths(int startVertex); // 导航方法，为用户提供所有可行观光路径
	void FindShortestPath(int startVertex, int endVertex); // 查找两景点间最短路径
	void CreateMSTUsingPrim(); // 最小生成树函数

	void PrintGraph(); // 测试函数，输出图的顶点和边信息
private:
	int m_aAdjMatrix[20][20]; // 邻接矩阵，int存放两顶点间的权值
	Vex m_aVexs[20]; // 顶点信息数字，最多存放20个顶点
	int m_nVexNum; // 当前图的顶点个数
	int pathCount;  // 记录导航查找到的所有路径数量
	void DFS(int currentVertex, vector<bool>& visited, vector<int>& path, int& pathLength); // 深度优先搜索
	void printPath(const vector<int>& path, int pathLength); // 打印路径及路径长度
	int findVertexIndex(int vertexNum); // 查找顶点编号对应的索引

};

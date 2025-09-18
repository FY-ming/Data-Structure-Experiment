#include "Graph.h"

// 初始化函数
void CGraph::InitGraph()
{
    // 初始化邻接矩阵，将所有元素设为0
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            m_aAdjMatrix[i][j] = 0;
        }
    }
    // 初始化顶点个数为 0
    m_nVexNum = 0;
    // 初始化导航路径数量为 0
    pathCount = 0;
}
// 插入顶点信息
bool CGraph::InsertVex(Vex sVex)
{
    if (m_nVexNum == MAX_VERTEX_NUM) // 检查是否已经达到图的顶点容量上限
    {
        //顶点已满
        return false;
    }
    m_aVexs[m_nVexNum++] = sVex; //插入顶点信息
    return true;
}
// 插入边信息
bool CGraph::InsertEdge(Edge sEdge)
{
    if (sEdge.vex1 < 0 || sEdge.vex1 >= m_nVexNum || sEdge.vex2 < 0 || sEdge.vex2 >= m_nVexNum)
    {
        // 下标越界
        return false;
    }
     //插入边的信息
    m_aAdjMatrix[sEdge.vex1][sEdge.vex2] = sEdge.weight;
    m_aAdjMatrix[sEdge.vex2][sEdge.vex1] = sEdge.weight;
    return true;
}


// 从文件构建图
void CGraph::BuildGraphFromFiles(const string& vexFile, const string& edgeFile)
{
    InitGraph();

    // 读取顶点信息
    ifstream vexIn(vexFile);
    if (!vexIn)
    {
        cerr << "无法打开文件：" << vexFile << endl;
        return;
    }
    int totalVex;
    vexIn >> totalVex;
    for (int i = 0; i < totalVex; ++i)
    {
        Vex vex;
        vexIn >> vex.num;
        vexIn >> vex.name;
        vexIn.ignore(); // 忽略换行符
        vexIn.getline(vex.desc, sizeof(vex.desc));
        InsertVex(vex);
    }
    vexIn.close();

    // 读取边信息
    ifstream edgeIn(edgeFile);
    if (!edgeIn)
    {
        cerr << "无法打开文件：" << edgeFile << endl;
        return;
    }
    int vex1, vex2, weight;
    while (edgeIn >> vex1 >> vex2 >> weight)
    {
        Edge edge;
        edge.vex1 = vex1;
        edge.vex2 = vex2;
        edge.weight = weight;
        InsertEdge(edge);
    }
    edgeIn.close();
}

// 获取图中指定下标的顶点，下标v大小：0~m_nVexNum
// 为用户提供访问图的顶点的函数
Vex CGraph::GetVex(int v) const
{
    return m_aVexs[v];
}
// 获取目前图中的顶点个数
int CGraph::Get_m_nVexNum() const
{
    return m_nVexNum;
}

// 根据顶点编号查询所有与它有关的边和顶点
void CGraph::QueryRelatedEdgesAndVertices(int vertexNum)
{
    bool found = false;
    for (int i = 0; i < m_nVexNum; ++i)
    {
        if (m_aVexs[i].num == vertexNum) // 遍历查找对于对应的顶点
        {
            found = true;
            cout << "您查找的景区： " << m_aVexs[i].name << " (编号: " << vertexNum << ")" << ", 景点描述: " << m_aVexs[i].desc << "附近景点：" << endl;
            for (int j = 0; j < m_nVexNum; ++j) // 找到对应顶点后，查找相邻的顶点信息
            {
                if (m_aAdjMatrix[i][j] != 0)
                {
                    cout << "景点 " << m_aVexs[i].name << " 前往 " << m_aVexs[j].name << ", 距离: " << m_aAdjMatrix[i][j] << endl;
                }
            }
            break;
        }
    }
    if (!found) // 未找到对应顶点
    {
        cout << "该景点编号：" << vertexNum << " 不存在，查找失败。" << endl;
    }
}


// 查找顶点编号对应的索引
int CGraph::findVertexIndex(int vertexNum)
{
    // 就目前简单的编号系统，数组的下标和景点的编号其实是一一对应的，但是，考虑到后续的编号可能会出现非连续的情况，这个搜索函数成了后续维护的一个优秀函数
    for (int i = 0; i < m_nVexNum; ++i)
    {
        if (m_aVexs[i].num == vertexNum)
        {
            return i;
        }
    }
    return -1;
}

// 打印路径及路径长度
void CGraph::printPath(const vector<int>& path, int pathLength)
{
    pathCount++;  // 路径数量加 1
    cout << "Path " << pathCount << ": ";
    for (int i = 0; i < path.size(); ++i)
    {
        cout << m_aVexs[path[i]].name;
        if (i < path.size() - 1)
        {
            cout << " -> ";
        }
    }
    cout << ", 观光线路总长度: " << pathLength << endl;
}

// 深度优先搜索
void CGraph::DFS(int currentVertex, vector<bool>& visited, vector<int>& path, int& pathLength)
{
    // currentVertex：待查看顶点
    // visited：记录下标是否被访问
    // path：记录合适的观光路径
    // pathLength：观光路线长度
    
    visited[currentVertex] = true;// 标记当前顶点为已访问，并将其加入路径。
    path.push_back(currentVertex);
    if (path.size() == m_nVexNum) // 完成所有顶点的访问后，输出路径
    {
        printPath(path, pathLength);
    }
    else // 否则，遍历所有未访问且与当前顶点相连的顶点，递归调用 DFS 函数。
    {
        for (int i = 0; i < m_nVexNum; ++i) // 遍历当前顶点的相邻顶点
        {
            if (!visited[i] && m_aAdjMatrix[currentVertex][i] != 0)
            {
                pathLength += m_aAdjMatrix[currentVertex][i];
                DFS(i, visited, path, pathLength);
                pathLength -= m_aAdjMatrix[currentVertex][i];
            }
        }
    }
    // 完成递归时进行回溯，将当前顶点标记为未访问，并从路径中移除。与for循环一起配合，完成多条路径的查询
    visited[currentVertex] = false;
    path.pop_back();
}

// 查找所有可以不重复走完所有顶点的路径
void CGraph::FindAllPaths(int startVertex)
{
    int startIndex = findVertexIndex(startVertex); // 查找景区中是否存在用户输入的编号的景点，findVertexIndex返回景区编号（索引）
    if (startIndex == -1) // 未能找到用户输入的景区编号
    {
        std::cout << "景区编号 " << startVertex << " 不存在." << std::endl;
        return;
    }

    vector<bool> visited(m_nVexNum, false); // 记录下标是否被访问
    vector<int> path; // 记录合适的观光路径
    int pathLength = 0; // 观光路线长度

    DFS(startIndex, visited, path, pathLength); // 深度搜索方法
    cout << "导航系统总计为您查找到: " << pathCount << "条线路" << endl;
    pathCount = 0; // 完成导航，清空线路记录
}

// 测试函数
// 输出图的顶点和边信息
void CGraph::PrintGraph()
{
    cout << "图的顶点总数：" << m_nVexNum << endl;
    cout << "顶点信息:" << endl;
    for (int i = 0; i < m_nVexNum; ++i)
    {
        cout << "景点编号: " << m_aVexs[i].num << ", 景点名称: " << m_aVexs[i].name << ", 景点描述: " << m_aVexs[i].desc << endl;
    }

    cout << "边信息:" << endl;
    for (int i = 0; i < m_nVexNum; ++i)
    {
        for (int j = i + 1; j < m_nVexNum; ++j)
        {
            if (m_aAdjMatrix[i][j] != 0)
            {
                cout << "顶点1 " << m_aVexs[i].name << " 顶点2 " << m_aVexs[j].name << ", 权重: " << m_aAdjMatrix[i][j] << endl;
            }
        }
    }
}


// 查找最短路径
void CGraph::FindShortestPath(int startVertex, int endVertex)
{
    // FindShortestPath 函数：
    // startVertex：用户输入的起点顶点编号。
    // endVertex：用户输入的终点顶点编号。
    // 
    // 首先，将用户输入的顶点编号转换为顶点索引，如果顶点不存在则输出错误信息并返回。
    // 初始化距离数组 dist 为无穷大，前驱数组 prev 为 - 1，标记数组 sptSet 为 false。将起点的距离设为 0。
    // 使用 Dijkstra 算法计算从起点到所有顶点的最短路径。
    // 从终点开始，通过前驱数组回溯得到最短路径。
    // 如果终点的距离仍然是无穷大，说明不存在从起点到终点的路径，输出相应信息；否则，输出最短路径和距离。

    // 调用 findVertexIndex 函数将用户输入的顶点编号转换为图中顶点数组的索引。
    // 如果任何一个顶点的索引未找到（返回 - 1），则输出错误信息并终止函数。
    int startIndex = findVertexIndex(startVertex);
    int endIndex = findVertexIndex(endVertex);

    if (startIndex == -1 || endIndex == -1)
    {
        cout << "未查找到对应景点." << endl;
        return;
    }
    // dist 数组：用于记录从起点到每个顶点的最短距离，初始值都设为 INT_MAX（表示无穷大），将起点的距离设为 0。
    // prev 数组：用于记录每个顶点在最短路径中的前驱顶点，初始值都设为 - 1。
    // sptSet 数组：用于标记每个顶点是否已经被确定了最短路径，初始值都设为 false。
    vector<int> dist(m_nVexNum, INT_MAX);
    vector<int> prev(m_nVexNum, -1);
    vector<bool> sptSet(m_nVexNum, false);

    dist[startIndex] = 0;

    // Dijkstra 算法核心循环
    // 外层循环执行 m_nVexNum - 1 次，因为每次循环会确定一个顶点的最短路径，总共需要确定 m_nVexNum - 1 个顶点（起点已经确定）。
    // 内层第一个循环：
    // 遍历所有未确定最短路径的顶点，找到距离起点最近的顶点 minIndex，其距离为 minDist。
    // 将 minIndex 对应的顶点标记为已确定最短路径（sptSet[minIndex] = true）。
    // 内层第二个循环：
    // 对于所有未确定最短路径的顶点 v，如果 minIndex 到 v 有边（m_aAdjMatrix[minIndex][v] 不为 0），且通过 minIndex 到达 v 的距离比当前记录的 dist[v] 小，则更新 dist[v] 为新的最短距离，并将 prev[v] 更新为 minIndex。
    for (int count = 0; count < m_nVexNum - 1; count++)
    {
        int minDist = INT_MAX, minIndex;

        for (int v = 0; v < m_nVexNum; v++)
        {
            if (!sptSet[v] && dist[v] <= minDist)
            {
                minDist = dist[v];
                minIndex = v;
            }
        }

        sptSet[minIndex] = true;

        for (int v = 0; v < m_nVexNum; v++)
        {
            if (!sptSet[v] && m_aAdjMatrix[minIndex][v] && dist[minIndex] != INT_MAX
                && dist[minIndex] + m_aAdjMatrix[minIndex][v] < dist[v])
            {
                dist[v] = dist[minIndex] + m_aAdjMatrix[minIndex][v];
                prev[v] = minIndex;
            }
        }
    }

    // 如果 dist[endIndex] 仍然是 INT_MAX，说明从起点到终点不存在路径，输出相应信息并终止函数。
    if (dist[endIndex] == INT_MAX)
    {
        cout << "没有直接路径可以直接抵达目的地." << endl;
        return;
    }

    // 从终点开始，通过 prev 数组回溯，将路径上的顶点依次添加到 path 向量中。
    // 由于回溯得到的路径是逆序的，使用 std::reverse 函数将路径反转。
    vector<int> path;
    for (int at = endIndex; at != -1; at = prev[at])
        path.push_back(at);
    reverse(path.begin(), path.end());

    // 遍历 path 向量，输出最短路径上每个顶点的名称，并用 -> 连接。
    // 输出最短路径的总距离 dist[endIndex]。
    cout << "最短路径为: ";
    for (int i = 0; i < path.size(); ++i)
    {
        cout << m_aVexs[path[i]].name;
        if (i < path.size() - 1)
        {
            cout << " -> ";
        }
    }
    cout << ", 路径距离: " << dist[endIndex] << endl;
}


// 使用 Prim 算法创建最小生成树并输出相关信息
void CGraph::CreateMSTUsingPrim()
{
    if (m_nVexNum == 0)
    {
        cout << "该图为空." << endl;
        return;
    }

    vector<int> parent(m_nVexNum, -1); // 存储每个顶点在最小生成树中的父顶点
    vector<int> key(m_nVexNum, INT_MAX); // 存储每个顶点到最小生成树的最小权值
    vector<bool> mstSet(m_nVexNum, false); // 标记每个顶点是否已经在最小生成树中

    // 选择第一个顶点作为起始点
    key[0] = 0;

    for (int count = 0; count < m_nVexNum - 1; count++)
    {
        int minKey = INT_MAX, minIndex;

        // 找到未在最小生成树中且 key 值最小的顶点
        for (int v = 0; v < m_nVexNum; v++)
        {
            if (!mstSet[v] && key[v] < minKey)
            {
                minKey = key[v];
                minIndex = v;
            }
        }

        // 将该顶点加入最小生成树
        mstSet[minIndex] = true;

        // 更新与该顶点相邻的顶点的 key 值和父顶点
        for (int v = 0; v < m_nVexNum; v++)
        {
            if (!mstSet[v] && m_aAdjMatrix[minIndex][v] && m_aAdjMatrix[minIndex][v] < key[v])
            {
                parent[v] = minIndex;
                key[v] = m_aAdjMatrix[minIndex][v];
            }
        }
    }

    // 输出最小生成树的边和总权值
    int totalWeight = 0;
    cout << "最短电路设计（最小生成树）:" << endl;
    for (int i = 1; i < m_nVexNum; i++)
    {
        cout << "从 " << m_aVexs[parent[i]].name << " 接入 " << m_aVexs[i].name << ", 距离: " << m_aAdjMatrix[i][parent[i]] << endl;
        totalWeight += m_aAdjMatrix[i][parent[i]];
    }
    cout << "最短电路总长度: " << totalWeight << endl;
}
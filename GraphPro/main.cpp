#include<iostream>
#include"Graph.h"
#include"Tourism.h"

using std::cin;

void test01()
{
	cout << "测试用例01" << endl;
	CGraph graph;
	graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
	graph.PrintGraph();
}
void test02()
{
	cout << "测试用例02" << endl;
	CGraph graph;
	graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
	cout << "景区信息搜索:" << endl;
	for (int i = 0; i < graph.Get_m_nVexNum(); ++i)
	{
		cout << "景点编号: " << graph.GetVex(i).num << ", 景点名称: " << graph.GetVex(i).name << ", 景点描述: " << graph.GetVex(i).desc << endl;
	}
}
void test03()
{
	cout << "测试用例03" << endl;
	CGraph graph;
	graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
	graph.PrintGraph();
	cout << "景区信息搜索:" << endl
		<< "请输入您要查询的景区编号：" << endl;
	int num;
	cin >> num;
	graph.QueryRelatedEdgesAndVertices(num);
}
void test04()
{
	cout << "测试用例04" << endl;
	CGraph graph;
	graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
	graph.PrintGraph();
	int startVertex;
	cout << "景区导航测试" << endl
		<< "请输入您的初始景区编号: " << endl;
	cin >> startVertex;
	graph.FindAllPaths(startVertex);
}
void test05()
{
	cout << "测试用例05" << endl;
	CGraph graph;
	graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
	graph.PrintGraph();

	int start, end;
	cout << "请分别输入起始景点和目的地编号：" << endl;
	cin >> start >> end;
	graph.FindShortestPath(start, end);
}
void test06()
{
	cout << "测试用例06" << endl;
	CGraph graph;
	graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
	graph.PrintGraph();

	graph.CreateMSTUsingPrim();
}

int main(void)
{
	//test01(); // 测试用例1：检查从文件获取信息并构建图
	//test02(); // 测试用例2：检查GetVex函数是否可以从类外完成对CGraph私有成员的访问
	//test03(); // 测试用例3：测试查询单一顶点的相关信息功能实现情况
	//test04(); // 测试用例4：测试导航功能
	//test05(); // 测试用例5：测试查找最短路径功能
	//test06(); // 测试用例6：最小生成树(最短的电路组网路线)
	 
	////......
	// 创建图对象
	CGraph graph;
	graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");

	int choice;
	while (true)
	{
		//输出界面
		cout << "====景区信息管理系统====" << endl
			<< "1.创建景区景点图" << endl
			<< "2.查询景点信息" << endl
			<< "3.旅游景点导航" << endl
			<< "4.搜索最短路径" << endl
			<< "5.铺设电路规划" << endl
			<< "0.退出" << endl;

		// 非法输入操作判定
		// 输入非数字内容：如果 choice 是 int 类型，而用户输入的是字母、符号或者其他非数字字符，输入操作就会失败。例如，当程序期望用户输入一个整数，可用户输入了 abc，此时 cin >> choice 就会失败。
		// 输入超出范围：若输入的数字超出了 choice 类型所能表示的范围，输入操作同样会失败。例如，choice 是 short 类型，而用户输入了一个非常大的数字，就可能超出 short 类型的表示范围，从而导致输入失败。
		while (!(cin >> choice)) {
			// 该操作的作用是清除 cin 对象的错误标志。
			// 当输入失败时，cin 对象会设置错误标志，之后的输入操作都会受到影响。
			// 使用 cin.clear() 能够清除这些错误标志，让 cin 可以继续进行后续的输入操作。
			cin.clear();
			// 此操作会忽略输入缓冲区里的无效输入。
			// std::numeric_limits<std::streamsize>::max() 表示最大的字符数，'\n' 代表换行符。
			// 这行代码的意思是忽略输入缓冲区中直到遇到换行符为止的所有字符，这样就能把无效输入从缓冲区中清除掉。
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
			cout << "输入无效，请输入一个有效的数字: ";
		}

		switch (choice)
		{
			case 1:
				//graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
				graph.PrintGraph();
				break;
			case 2:
				graph.PrintGraph();
				cout << "景区信息搜索:" << endl
					<< "请输入您要查询的景区编号：" << endl;
				int num;
				cin >> num;
				graph.QueryRelatedEdgesAndVertices(num);
				break;
			case 3:
				int startVertex;
				cout << "景区导航测试" << endl
					<< "请输入您的初始景区编号: " << endl;
				// 无非法输入警告
				cin >> startVertex;
				graph.FindAllPaths(startVertex);
				break;
			case 4:
				int start, end;
				cout << "请分别输入起始景点和目的地编号：" << endl;
				// 无非法输入警告
				cin >> start >> end;
				graph.FindShortestPath(start, end);
				break;
			case 5:
				graph.CreateMSTUsingPrim();
				break;
			case 0:
				return 0;
			default:
				cout << "无效的选择，请输入 0 - 5 之间的数字。" << endl;
		}
		system("pause");
		system("cls");
	}
	return 0;

	//CGraph graph;
	//graph.BuildGraphFromFiles("Vex.txt", "Edge.txt");
	//while (1)
	//{
	//	//输出界面
	//	cout << "====景区信息管理系统= = = =" << endl
	//		<< "1.创建景区景点图" << endl
	//		<< "2.查询景点信息" << endl
	//		<< "3.旅游景点导航" << endl
	//		<< "4.搜索最短路径" << endl
	//		<< "5.铺设电路规划" << endl
	//		<< "0.退出" << endl;
	//}
}
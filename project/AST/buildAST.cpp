#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
using namespace std;

#define MXN 7000
//string fileName = "mytest.c.001t.tu";

string myFileName = "test.cpp";
string fileName = "F:\\.C++_workspace\\AST\\output\\AST_reduced.txt";
string fileName_inter = "F:\\.C++_workspace\\AST\\output\\out_reorder.txt";
string fileName_final = "F:\\.C++_workspace\\AST\\output\\result.txt";
string output_line;

//设计编号映射表
struct numberMap
{
    string oldNum;
    int newNum;
};
//表结点的类型定义
struct vNode
{
    int node_num;     //结点编号
    string node_name; //结点名称
    //int node_type;     //结点类型
    struct eNode *first; //指向第一个字段的指针
};
//字段链表的类型定义
struct eNode
{
    string domain_name;  //字段名称
    string domain_value; //字段值
    int ifLeaf;          //该字段是否为叶子结点，即值是否为@num
    int num;             //子结点编号，即 @num 中的 num
    struct eNode *next;  //指向下一个字段的指针
};

numberMap *nMap = new numberMap[MXN];
vNode *pNode = new vNode[MXN];

//get the first word of a line
string getFirstWordOfLine(string line)
{
    int i = 0;
    while (line[i] != ' ')
        i++;
    return line.substr(0, i);
}

//替换节点标号
string replaceNodeNumber(string line, int mapNum, int row)
{
    int len = line.size();
    int i = 0;
    while (line[i] != ' ')
        i++;
    //替换首节点编号@num为nMap[j].newNum
    line = line.replace(0, i, to_string(nMap[row].newNum));
    len = line.size(); //每次替换后更新字符串长度

    i = 1;
    while (i < len)
    {
        //replace sub node
        if (line[i - 1] == ':' && line[i] == '@')
        {
            int j = i + 1;
            while (j < len && line[j] != ' ')
                j++;
            string nodeOld = line.substr(i, j - i);
            //cout << nodeOld;
            for (int k = 0; k < mapNum; ++k)
            {
                if (nMap[k].oldNum == nodeOld)
                {
                    //替换子结点@num为@nMap[j].newNum
                    line = line.replace(i, j - i, "@" + to_string(nMap[k].newNum));
                    len = line.size();
                    break;
                }
            }
        }
        i++;
    }
    return line;
}

//去除指定字段名的字段
string doTrimDomain(string line, string target)
{
    int position = 0;
    while ((position = line.find(target, position)) != string::npos)
    {
        int j = position + 1;
        while (j < line.size() && line[j] != ' ')
            j++;
        line = line.replace(position, j - position + 1, "");
        //cout << line << endl;
        //cout << "position : " << position << endl;
    }
    return line;
}

//去除冗余字段
string trimDomaint(string line)
{
    //types
    line = doTrimDomain(line, "type:");
    //lngt
    line = doTrimDomain(line, "lngt:");
    //srcp
    //line = doTrimDomain(line, "srcp:");
    //algn
    line = doTrimDomain(line, "algn:");

    //scope_stmt 只保留 begin end line next 字段

    //消除所有指向结点已被消除的字段

    return line;
}

//节点编号化简, 去除冗余字段
void preprocessAST()
{
    cout << "preprocessAST start\n";

    int mapNum = 0;

    ifstream infile(fileName);
    ofstream outfile(fileName_inter);

    if (infile.is_open())
    {
        string line;
        //录入编号映射表
        while (getline(infile, line))
        {
            //printf("%s\n", line.c_str());
            nMap[mapNum].oldNum = getFirstWordOfLine(line);
            //cout << getFirstWordOfLine(line) << endl;
            nMap[mapNum].newNum = mapNum;
            mapNum++;
            //cout << line << endl;
        }

        //结点编号映射算法
        //首先读取一次读取到文件末尾，因此此时的流状态时eof，需要将其设置为goodbit才能成功将文件指针移动到文件开头
        infile.clear(std::ios::goodbit);
        //读指针指向文件头
        infile.seekg(std::ios::beg);
        int trailRow = 0; //记录遍历的行数
        while (getline(infile, line))
        {
            //replace node name
            string tmp = replaceNodeNumber(line, mapNum, trailRow);
            //去冗余字段
            tmp = trimDomaint(tmp);
            //cout << tmp << endl;
            //输出到新文件
            outfile << tmp << endl;
            trailRow++;
        }

        infile.close();
        outfile.close();
    }
    else
    {
        cout << "failed";
    }
}

//建立邻接表
int adjacentAST()
{
    cout << "adjacentAST start\n";
    ifstream infile(fileName_inter);
    //ofstream outfile(fileName_inter);

    string line;
    int trailRow = 0;
    if (infile.is_open())
    {
        //建立抽象语法树
        while (getline(infile, line))
        {
            int len = line.size();
            //先处理结点信息
            //结点编号 也就是第一个单词转化成 int类型
            pNode[trailRow].node_num = stoi(getFirstWordOfLine(line));

            //结点名称  也就是第二个单词
            int i = 0;
            while (i < len && line[i] != ' ')
                i++;
            int j = i + 1;
            while (j < len && line[j] != ' ')
                j++;
            pNode[trailRow].node_name = line.substr(i + 1, j - i - 1);

            //指向第一个字段的指针
            pNode[trailRow].first = nullptr;
            //cout << pNode[trailRow].node_num << pNode[trailRow].node_name << endl;

            /************************************************************/
            //对每个字段建立字段链表
            int p = 0; //记录每行遍历的位置p
            while ((p = line.find(":", p)) != string::npos)
            {
                //申请一个 eNode 类型的指针 q
                eNode *q = new eNode;
                //字段名称
                int j = p;
                while (j >= 0 && line[j] != ' ')
                    j--;
                q->domain_name = line.substr(j + 1, p - j - 1);
                //字段值
                j = p;
                while (j < line.size() && line[j] != ' ')
                    j++;
                q->domain_value = line.substr(p + 1, j - p - 1);
                //cout << q->domain_name << "--" << q->domain_value << endl;
                //cout << p << endl;

                //更新下次下次开始查询的 pos 的为j 可避免字段值中有冒号而带来的错误
                p = j;

                //字段值为子节点
                if (q->domain_value[0] == '@')
                {
                    q->ifLeaf = 0;
                    q->num = stoi(q->domain_value.substr(1));
                    //cout << q->num << endl;
                }
                else
                {
                    q->ifLeaf = 1;
                    q->num = -1;
                }
                //将字段q链接到pNode->next字段链表
                if (pNode[trailRow].first == nullptr)
                {
                    pNode[trailRow].first = q;
                    q->next = nullptr;
                }
                else
                {
                    q->next = pNode[trailRow].first;
                    pNode[trailRow].first = q;
                }
            }
            /**************************************************************************
            eNode *eNode_q = pNode[trailRow].first;
            while (eNode_q != nullptr)
            {
                cout << " " << eNode_q->domain_name << ":" << eNode_q->domain_value;
                eNode_q = eNode_q->next;
            }
            cout << endl;
            **************************************************************************/

            trailRow++;
        }

        infile.close();
        //outfile.close();
    }
    else
    {
        cout << "file open failed";
    }

    return trailRow;
}

//递归遍历显示AST
void show(vNode *vNode_p, int layer)
{
    //显示当前结点vNode_p及各字段信息
    //每次向下递归需要缩格

    for (int i = 0; i < layer; ++i)
    {
        cout << "\t";
        //output_line += "\t";
    }

    cout << vNode_p->node_num << " " << vNode_p->node_name;
    //output_line += (vNode_p->node_num) + " " + vNode_p->node_name;

    //定义头字段指针
    eNode *eNode_q = vNode_p->first;
    while (eNode_q != nullptr)
    {
        cout << " " << eNode_q->domain_name << ":" << eNode_q->domain_value;
        //output_line += " " + eNode_q->domain_name + ":" + eNode_q->domain_value;
        eNode_q = eNode_q->next;
    }
    cout << "\n";
    //output_line += "\n";

    //递归子节点  注意从头开始
    eNode_q = vNode_p->first;
    while (eNode_q != nullptr)
    {
        if (eNode_q->ifLeaf == 0) //&& 该字段为遍历字段
        {
            //cout << eNode_q->num << endl;
            show(&pNode[eNode_q->num], layer + 1);
        }
        eNode_q = eNode_q->next;
    }
    // //把结果输出在文件中
    // ofstream outfile(fileName_final);
    // outfile << output_line << endl;
    // outfile.close();
}

//输出AST
void printAST(int row)
{
    //find root of AST
    int tag = 0;
    for (int i = 0; i < row; i++)
    {
        if (pNode[i].node_name == "function_decl")
        {
            int count = 0;
            eNode *p = pNode[i].first;
            while (p != nullptr)
            {
                if (p->domain_name == "srcp" && p->domain_value.find(myFileName) != string::npos)
                    count++;
                else if (p->domain_name == "body")
                    count++;

                p = p->next;
            }
            if (count >= 1)
            {
                tag = i;
                break;
            }
        }
    }
    //cout << "root is ---------" << tag << "---------" << endl;
    vNode *vNode_p = &pNode[tag];
    int rootNum = vNode_p->node_num;

    //AST的遍历
    //从根结点的node_num开始，设计递归显示算法
    show(vNode_p, 0);
}

int main()
{
    //节点编号化简, 去除冗余字段
    preprocessAST();
    //建立邻接表,存储AST
    int row = adjacentAST();
    //输出AST
    printAST(row);

    return 0;
}

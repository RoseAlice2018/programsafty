#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#define useful 1
#define unknown 2
#define useless 3
#define realuseless 4
using namespace std;

string sourceFileName = "F:\\.C++_workspace\\AST\\src\\test.cpp.001t.tu";
string reducedFileName_tmp1 = "F:\\.C++_workspace\\AST\\output\\AST_reduced_tmp1.txt";
string reducedFileName_tmp2 = "F:\\.C++_workspace\\AST\\output\\AST_reduced_tmp2.txt";
string reducedFileName = "F:\\.C++_workspace\\AST\\output\\AST_reduced.txt";

void func1()
{
    char ch;
    char *str;
    int i = 1;
    int spaceflag = 0; //空格字符标志
    //文件读入
    ifstream infile(sourceFileName, ios::in);
    //打开文件流异常
    if (!infile)
    {
        cout << "open infile error!" << endl;
        exit(1);
    }
    cout << "输出规范化的抽象语法树：" << endl;
    //文件输出
    ofstream outfile(reducedFileName_tmp1, ios::out);
    if (!outfile)
    {
        cout << "open outfile error!" << endl;
        exit(1);
    }
    //in
    while (!infile.eof())
    {
        ch = infile.get(); //当前字符
                           //	int reserved_tag=0;
        switch (ch)
        {
        case 58:
        { //当前字符是冒号
            outfile.put(ch);
            cout << ch;
            spaceflag = 1; //冒号去空格
            break;
        }
        case 32:
        {                       //当前字符是空格 如果已经输出就跳过
            if (spaceflag == 1) //已输出一个空格
                ;
            else
            {
                spaceflag = 1;
                i++;
                outfile.put(' ');
                cout << " ";
            }
            break;
        }
        case 10:
        {                  //当前字符是换行键
            spaceflag = 0; //空格数归零
            ch = infile.get();
            if (ch != 64 && !infile.eof())  //如果本行还没有结束 并且未到文件末尾
                infile.seekg(-1, ios::cur); //退回当前字符
            else
            { //本行结束
                outfile.put('\n');
                cout << endl;
                outfile.put(ch);
                cout << ch;
            }
            break;
        }
        case 13:
        { //tab
            spaceflag = 0;
            ch = infile.get();
            if (ch != 64 && !infile.eof())
                infile.seekg(-1, ios::cur);
            else
            {
                outfile.put('\n');
                cout << endl;
                outfile.put(ch);
                cout << ch;
            }
            break;
        }
        default:
        {
            spaceflag = 0;
            outfile.put(ch);
            cout << ch;
            break;
        }
        }
    }
    infile.close();
    outfile.close();
    ifstream infile1(reducedFileName_tmp1);
    ofstream outfile1(reducedFileName_tmp2);
    if (!outfile1)
    {
        cout << "open outfile error!" << endl;
        exit(1);
    }
    if (!infile1)
    {
        cout << "open infile error!" << endl;
        exit(1);
    }
    string s;
    while (getline(infile1, s))
    {
        string temp = "";
        if (s.size() == 1)
            break;
        for (int i = 0; i + 1 < s.size(); i++)
        {
            if (s[i + 1] == ':')
            {
                if (s[i] >= 'a' && s[i] <= 'z')
                    temp.push_back(s[i]);
                temp.push_back(s[i + 1]);
                i++;
                //cout<<s[i]<<endl;
            }
            else
            {
                temp.push_back(s[i]);
            }
        }
        temp.push_back(s[s.size() - 1]);
        outfile1 << temp << endl;
    }
    infile1.close();
    outfile1.close();
    return;
}

void setNode(vector<pair<string, int>> &dic, int NodeNum)
{
    //对本行进行处理，这里不递归进行了
    string temp = dic[NodeNum].first;
    string num = "";
    for (int i = 0; i < temp.size(); i++)
    {
        if (temp[i] == '@')
        {
            int j = 0;
            for (j = i + 1; j < temp.size(); j++)
            {
                if (temp[j] >= '0' && temp[j] <= '9')
                    num.push_back(temp[j]);
                else
                    break;
            }
            int number = atoi(num.c_str());
            if (dic[NodeNum].second == useful&&dic[number].second!=realuseless)
                dic[number].second = useful;
            else if (dic[number].second == unknown && dic[NodeNum].second == realuseless)
                dic[number].second = useless;
            else if(dic[number].second==unknown && dic[NodeNum].second == useless)
                dic[number].second = useless;
            i = j - 1;
        }
        else
        {
            num.clear();
        }
    }
}

void func2()
{
    ifstream infile(reducedFileName_tmp2);
    //打开文件流异常
    if (!infile)
    {
        cout << "open infile error!" << endl;
        exit(1);
    }
    vector<pair<string, int>> dic;               //存放每一行的信息
    dic.push_back(pair<string, int>("test", realuseless)); //阻塞0
    string s;
    int count = 0;
    while (getline(infile, s)) //第一遍扫描，完成初次标定
    {
        count++; //当前行号
        dic.push_back(pair<string, int>(s, unknown));
        //检测srcp字段
        for (int i = 0; i + 4 < s.size(); i++)
        {
            if (s[i] == 's' && s[i + 1] == 'r' && s[i + 2] == 'c' && s[i + 3] == 'p' && s[i + 4] == ':')
            {
                //检测到srcp字段
                int j = i + 5;
                if (s.size() - i - 5 >= 8 && s[j] == 't' && s[j + 1] == 'e' && s[j + 2] == 's' && s[j + 3] == 't' && s[j + 4] == '.' && s[j + 5] == 'c' && s[j + 6] == 'p' && s[j + 7] == 'p')
                {
                    dic[count].second = useful;
                }
                else
                {
                    dic[count].second = realuseless;
                }
            }
        }
    }
    // cout<<dic[1].second<<endl;
    for (int i = 1; i < dic.size(); i++)
    {
        setNode(dic, i);
    }

    infile.clear(std::ios::goodbit);
    //读指针指向文件头
    infile.seekg(std::ios::beg);
    count = 0;
    while (getline(infile, s)) //第一遍扫描，完成初次标定
    {
        count++; //当前行号
        dic.push_back(pair<string, int>(s, unknown));
        //检测srcp字段
        for (int i = 0; i + 4 < s.size(); i++)
        {
            if (s[i] == 's' && s[i + 1] == 'r' && s[i + 2] == 'c' && s[i + 3] == 'p' && s[i + 4] == ':')
            {
                //检测到srcp字段
                int j = i + 5;
                if (s.size() - i - 5 >= 8 && s[j] == 't' && s[j + 1] == 'e' && s[j + 2] == 's' && s[j + 3] == 't' && s[j + 4] == '.' && s[j + 5] == 'c' && s[j + 6] == 'p' && s[j + 7] == 'p')
                {
                    dic[count].second = useful;
                }
                else
                {
                    dic[count].second = useless;
                }
            }
        }
    }

    //cout<<dic[1].second<<endl;
    // for (int i = 1; i < dic.size(); i++)
    // {
    //     if (dic[i].second == unknown)
    //         dic[i].second = useful;
    // }
    ofstream outfile(reducedFileName);
    for (int i = 1; i < dic.size(); i++)
    {
        if (dic[i].second == useful)
            outfile << dic[i].first << endl;
    }
    infile.close();
    outfile.close();
    return;
}

int main()
{
    func1();
    func2();
}
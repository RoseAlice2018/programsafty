#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#include"func1.h"
#define useful 1
#define unknown 2
#define useless 3
using namespace std;

void setNode(vector<pair<string,int>>& dic,int NodeNum)
{
    //对本行进行处理，这里不递归进行了
    string temp=dic[NodeNum].first;
    string num="";
    for(int i=0;i<temp.size();i++)
    {
        if(temp[i]=='@')
        {
            int j=0;
            for( j=i+1;j<temp.size();j++)
            {
                if(temp[j]>='0'&&temp[j]<='9')
                    num.push_back(temp[j]);
                else 
                   break;
            }
            int number=atoi(num.c_str());
            if(dic[number].second==unknown&&dic[NodeNum].second==useful)
            dic[number].second=useful;
            else if(dic[number].second==unknown&&dic[NodeNum].second==useless)
            dic[number].second=useless;
            i=j-1;
        }
        else
        {
            num.clear();
        }  
    }
}
void func2()
{
    ifstream infile("AST_model.txt");
     //打开文件流异常
	if(!infile)
	{
		cout<<"open infile error!"<<endl;
		exit(1);
	}
    vector<pair<string,int>> dic;//存放每一行的信息
    dic.push_back(pair<string,int>("test",0));//阻塞0
     string s;
     int count=0;
     while(getline(infile,s))//第一遍扫描，完成初次标定
     {
         count++;//当前行号
         dic.push_back(pair<string,int>(s,unknown));
         //检测srcp字段
         for(int i=0;i+4<s.size();i++)
         {
             if(s[i]=='s'&&s[i+1]=='r'&&s[i+2]=='c'&&s[i+3]=='p'&&s[i+4]==':')
             {
                 //检测到srcp字段
                int j=i+5;
                if(s.size()-i-5>=8&&s[j]=='t'&&s[j+1]=='e'&&s[j+2]=='s'&&s[j+3]=='t'&&s[j+4]=='.'&&s[j+5]=='c'&&s[j+6]=='p'&&s[j+7]=='p')
                {
                    dic[count].second=useful;
                }
                else
                {
                    dic[count].second=useless;
                }
             }
         }
     }
    // cout<<dic[1].second<<endl;
    for(int i=1;i<dic.size();i++)
    {
        setNode(dic,i);
    }
    //cout<<dic[1].second<<endl;
    for(int i=1;i<dic.size();i++)
    {
        if(dic[i].second==unknown)
            dic[i].second=useful;
    }
    ofstream outfile("AST_model(1).txt");
    for(int i=1;i<dic.size();i++)
    {
        if(dic[i].second==useful)
         outfile<<dic[i].first<<endl;
    }
    infile.close();
	outfile.close();
    return ;
}
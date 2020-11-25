#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#define useful 1
#define unknown 2
#define useless 3
using namespace std;
void func1()
{
    char ch;
	char *str;
	int i=1;
	int spaceflag=0; //空格字符标志 
	//文件读入
	ifstream infile("test.cpp.001t.tu",ios::in);
	//打开文件流异常
	if(!infile)
	{
		cout<<"open infile error!"<<endl;
		exit(1);
	} 
	cout<<"输出规范化的抽象语法树："<<endl;
	//文件输出
	ofstream outfile("AST_model.txt",ios::out);
	if(!outfile){
		cout<<"open outfile error!"<<endl;
		exit(1);
	} 
	//in
	while(!infile.eof()) {
		ch=infile.get(); //当前字符
	//	int reserved_tag=0;
		switch(ch){
			case 58 :{ //当前字符是冒号
				outfile.put(ch);
				cout<<ch; 
				spaceflag = 1;  	//冒号去空格 
				break;
			}
			case 32 :{ //当前字符是空格 如果已经输出就跳过
				if(spaceflag==1)	//已输出一个空格 
					;
				else
				{				
					spaceflag=1;
					i++;
					outfile.put('\0');
					cout<<" "; 
				}
				break;
			}
			case 10:{ //当前字符是换行键
				spaceflag=0;//空格数归零
				ch=infile.get();
				if(ch!=64&&!infile.eof()) //如果本行还没有结束 并且未到文件末尾
					infile.seekg(-1,ios::cur);//退回当前字符
				else{ //本行结束
					outfile.put('\n');
					cout<<endl; 
					outfile.put(ch);
					cout<<ch;
				}
				break;
			}
			case 13:{ //tab
				spaceflag=0;
				ch=infile.get();
				if(ch!=64&&!infile.eof())
					infile.seekg(-1,ios::cur);
				else{
					outfile.put('\n');
					cout<<endl; 
					outfile.put(ch);
					cout<<ch;
				}
				break;
			}
			default :{
				spaceflag=0;
				outfile.put(ch);
				cout<<ch;
				break;
			}
		}
	}
	infile.close();
	outfile.close();
	ifstream infile1("AST_model.txt");
	ofstream outfile1("AST_model1.txt");
	if(!outfile1){
		cout<<"open outfile error!"<<endl;
		exit(1);
	} 
	if(!infile1)
	{
		cout<<"open infile error!"<<endl;
		exit(1);
	}
	string s;
	while(getline(infile1,s))
	{
		if(s.size()==1)
			break;
		string temp="";
		for(int i=0;i+1<s.size();i++)
		{
			if(s[i+1]==':')
			{
				if(s[i]>='a'&&s[i]<='z')
					temp.push_back(s[i]);
				temp.push_back(s[i+1]);
				i++;
				//cout<<s[i]<<endl;
			}
			else 
			{
				temp.push_back(s[i]);
			}
		}
		temp.push_back(s[s.size()-1]);
		outfile1<<temp<<endl;
	}
	infile1.close();
	outfile1.close();
    return ;
}
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
	char ch;
	char *str;
	int i=1;
	int spaceflag=0; //空格字符标志 
	ifstream infile("D:\main.cpp.001t.tu",ios::in);
	//打开文件流异常
	if(!infile){
		cout<<"open infile error!"<<endl;
		exit(1);
	} 
	
	cout<<"输出规范化的抽象语法树："<<endl;
	
	ofstream outfile("D:\AST model.txt",ios::out);
	if(!outfile){
		cout<<"open outfile error!"<<endl;
		exit(1);
	} 
	
	while(!infile.eof()) {
		ch=infile.get(); 
		switch(ch){
			case 58 :{
				outfile.put(ch);
				cout<<ch; 
				spaceflag = 1;  	//冒号去空格 
				break;
			}
			case 32 :{
				if(spaceflag==1)	//已输出一个空格 
					;
				else{				
					spaceflag=1;
					i++;
					outfile.put('\0');
					cout<<" "; 
				}
				break;
			}
			case 10:{
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
			case 13:{
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
	
	system("pause");
	
	/*ifstream infile2("D:\AST model.txt",ios::in);
	if(!infile2){
		cout<<"open infile error!"<<endl;
		exit(1);
	} 
	ofstream outfile2("D:\AST model2.txt",ios::out);
	if(!outfile2){
		cout<<"open outfile error!"<<endl;
		exit(1);
	} 
	
	while(infile2.eof()){
		infile2.getline(str,100);
		if(str[0]=='@'){
			outfile.put('\n');
			i=0;
			while(str[i]!='\0'){
				outfile.put(str[i]);
				i++;
			}
		}
		if(str[0]!='@'){
			i=0;
			while(str[i]!='\0'){
				outfile.put(str[i]);
				i++;
			}
		}
	}
	
	infile2.close();
	outfile2.close(); 
	*/ 
	return 0;
}

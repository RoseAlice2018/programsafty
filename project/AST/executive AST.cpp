#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
	char ch;
	char *str;
	int i=1;
	int spaceflag=0; //�ո��ַ���־ 
	//�ļ�����
	ifstream infile("test.cpp.001t.tu",ios::in);
	//���ļ����쳣
	if(!infile)
	{
		cout<<"open infile error!"<<endl;
		exit(1);
	} 
	cout<<"����淶���ĳ����﷨����"<<endl;
	//�ļ����
	ofstream outfile("AST_model.txt",ios::out);
	if(!outfile){
		cout<<"open outfile error!"<<endl;
		exit(1);
	} 
	//in
	while(!infile.eof()) {
		ch=infile.get(); //��ǰ�ַ�
		switch(ch){
			case 58 :{ //��ǰ�ַ���ð��
				outfile.put(ch);
				cout<<ch; 
				spaceflag = 1;  	//ð��ȥ�ո� 
				break;
			}
			case 32 :{ //��ǰ�ַ��ǿո� ����Ѿ����������
				if(spaceflag==1)	//�����һ���ո� 
					;
				else{				
					spaceflag=1;
					i++;
					outfile.put('\0');
					cout<<" "; 
				}
				break;
			}
			case 10:{ //��ǰ�ַ��ǻ��м�
				spaceflag=0;//�ո�������
				ch=infile.get();
				if(ch!=64&&!infile.eof()) //������л�û�н��� ����δ���ļ�ĩβ
					infile.seekg(-1,ios::cur);//�˻ص�ǰ�ַ�
				else{ //���н���
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
	
	system("pause"); 
	return 0;
}

#ifndef CIFAFENXI_H_INCLUDED
#define CIFAFENXI_H_INCLUDED


#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<sstream>
#include<algorithm>
using namespace std;
int error_flag=0;



set<string> reserved = {"integer","char","program","array","of","record","end","var",
						"procedure","begin","if","then","else","fi","while","do",
						"endwh","read","write","return","type"};

bool letter(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
	{
		return true;
	}
	return false;
}

bool number(char c)
{
	if (c >= '0' && c <= '9')
	{
		return true;
	}
	return false;
}

class Token
{
public:
	int Line;      //token��������
	string Lex;    //�ʷ���Ϣ
	string Sem;    //������Ϣ
};

void error(int row, vector<string>& cifa)
{
    //cout<<row<<endl;
	cout <<"Word."<<row<<" EXISTS ERROR"<<endl;
	cifa.push_back("Word error! "+to_string(row)+" EXISTS ERROR"+"\n");
	error_flag=1;
}

vector<Token> getToken(string s,vector<string>& cifa)
{
     vector<Token>token;
	int row = 1;
	for(int i=0;i<s.size();i++)
	{
		Token* t = new Token();
		t->Line = row;
		char c = s[i];
//		cout<<c<<endl;
		if (c == ' '||c == '\t')	  //�����ո�
		{
			continue;
		}
		if(number(c))    //�����ִ�ͷ��token
		{
			string temp;
			while (i < s.size() && number(c))	//�����־�һֱ���¶�
			{
				temp += c;
				i++;
				c = s[i];
			}
			i--;		//����һλ��forѭ�����ֻ�+1
			if (letter(c))
			{
				error(row,cifa);
				break;
			}
			else
			{
				t->Lex = "INTC";
				t->Sem = temp;
				token.push_back(*t);
				continue;
			}
		}
		if (letter(c))		//����ĸ��ͷ�����Ǳ����־��Ǳ�ʶ��
		{
			string temp;
			while (i < s.size() && (letter(c) || number(c)))
			{
				temp += c;
				i++;
				c = s[i];
			}
			i--;
			if (reserved.find(temp) != reserved.end())
			{
				transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
				t->Lex = temp;
			}
			else
			{
				t->Lex = "ID";
				t->Sem = temp;
			}
			token.push_back(*t);
			continue;
		}
		if (c == '+')
		{
			t->Lex = "PLUS";
			token.push_back(*t);
			continue;
		}
		if (c == '-')
		{
			t->Lex = "SUB";
			token.push_back(*t);
			continue;

		}
		if (c == '*')
		{
			t->Lex = "MULT";
			token.push_back(*t);
			continue;
		}
		if (c == '/')
		{
			t->Lex = "DIV";
			token.push_back(*t);
			continue;
		}
		if (c == '(')
		{
			t->Lex = "LPAREN";
			token.push_back(*t);
			continue;
		}
		if (c == ')')
		{
			t->Lex = "RPAREN";
			token.push_back(*t);
			continue;
		}
		if (c == '[')
		{
			t->Lex = "LMIDPAREN";
			token.push_back(*t);
			continue;
		}
		if (c == ']')
		{
			t->Lex = "RMIDPAREN";
			token.push_back(*t);
			continue;
		}
		if (c == ';')
		{
			t->Lex = "SEMI";
			token.push_back(*t);
			continue;
		}
		if (c == '<')
		{
			t->Lex = "LT";
			token.push_back(*t);
			continue;
		}
		if (c == '=')
		{
			t->Lex = "EQ";
			token.push_back(*t);
			continue;
		}
		if (c == ':')
		{
			i++;
			c = s[i];
			if (c == '=')
			{
				t->Lex = "ASSIG";
				token.push_back(*t);
				continue;
			}
			else
			{
				error(row,cifa);
				break;
			}
		}
		if (c == '{')
		{
			while (i<s.size()&&c != '}')
			{
				i++;
				c = s[i];

			}
			continue;
		}
		if (c == ',')
		{
			while (i+1<s.size() && (s[i + 1] == ' '||s[i+1]=='\t'))
			{
				i++;
			}
			if (!number(s[i + 1]) && !letter(s[i + 1]))
			{
				error(row,cifa);
				break;
			}
			else
			{
				t->Lex = "COMMA";
				token.push_back(*t);
				continue;
			}
		}
		if (c =='.')
		{
			if (i+1<s.size() && s[i + 1] == '.')
			{
                //�������ֻ��һ��TOken��
				t->Lex = "UNDERANGE";
				token.push_back(*t);
				i++;
				continue;
			}
			else
			{


					t->Lex = "DOT";
					token.push_back(*t);
					continue;

			}
		}
		if (c == '\n')
		{
			row++;
			continue;
		}
		error(row,cifa);
		break;
	}
	Token *t = new Token();
	t->Line = row;
	t->Lex = "EOF";
	token.push_back(*t);
	return token;
}

string readFileIntoString(string filename)
{
	ifstream ifile(filename);
	//���ļ����뵽ostringstream����buf��
	ostringstream buf;
	char ch;
	while (buf && ifile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���
	return buf.str();
}


#endif // CIFAFENXI_H_INCLUDED
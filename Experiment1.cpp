#include<bits/stdc++.h>
using namespace std;
#define MAX 21
/*
	保留字|关键字：1
	操作符|运算符：2
	分界符：3
	标识符：4
	常数：5
	无识别：6
*/
char ch = ' ';
char* keyWord[21] = { "iostream","void","int","char","string","bool","return","using","namespace","main","break","include","begin","end","if","else","while","switch","for","else if","std" };
char takes[20];//定义获取的字符
//判断是否是关键字
bool isKey(char* takes)
{
	for (int i = 0; i < MAX; i++)
	{
		if (strcmp(takes, keyWord[i]) == 0)
			return true;
	}
	return false;
}
//判断是否是字母
bool isLetter(char letter)
{
	if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
		return true;
	else
		return false;
}
//判断是否是数字
bool isDigit(char digit)
{
	if (digit >= '0' && digit <= '9')
		return true;
	else
		return false;
}
//词法分析
void analyze(FILE* fpin)
{

	while ((ch = fgetc(fpin)) != EOF) {
		if (ch == ' ' || ch == '\t' || ch == '\n') {}  //空格 回车
		else if (isLetter(ch)) {              //首是字母
			char takes[30] = { '\0' };
			int i = 0;
			while (isLetter(ch) || isDigit(ch)) {  //后面是字母或者数字
				takes[i] = ch;
				i++;
				ch = fgetc(fpin);
			}
			//回退一个指针
			fseek(fpin, -1L, SEEK_CUR);
			if (isKey(takes)) {
				//关键字
				cout << takes << "\t\t1" << "\t关键字" << endl;
			}
			else {
				//标识符
				cout << takes << "\t\t4" << "\t标识符" << endl;
			}
		}
		else if (isDigit(ch) || (ch == '.'))  //首是数字或者点
		{
			bool mark = true;
			int i = 0;
			char takes[20] = { '\0' };
			while (isDigit(ch) || isLetter(ch) || (ch == '.' && isDigit(fgetc(fpin))))
			{
				if (isLetter(ch))
				{
					mark = false;
					if (ch == '.')fseek(fpin, -1L, SEEK_CUR);
					takes[i] = ch;
					i++;
					ch = fgetc(fpin);
				}
				else {
					if (ch == '.')fseek(fpin, -1L, SEEK_CUR);
					takes[i] = ch;
					i++;
					ch = fgetc(fpin);
				}
			}
			fseek(fpin, -1L, SEEK_CUR);
			//属于无符号常数
			if (mark)
			{
				cout << takes << "\t\t5" << "\t常数" << endl;
			}
			else
				cout << takes << "\t\t7" << "\t错误的标识符" << endl;

		}
		else switch (ch) {
			//运算符
		case '+': {
			ch = fgetc(fpin);
			if (ch == '+')cout << "++" << "\t\t2" << "\t运算符" << endl;
			else {
				cout << "+" << "\t\t2" << "\t运算符" << endl;
				fseek(fpin, -1L, SEEK_CUR);
			}
		}break;
		case '-': {
			ch = fgetc(fpin);
			if (ch == '-')cout << "--" << "\t\t2" << "\t运算符" << endl;
			else {
				cout << "-" << "\t\t2" << "\t运算符" << endl;
				fseek(fpin, -1L, SEEK_CUR);
			}
		}break;
		case '*':cout << ch << "\t\t2" << "\t运算符" << endl; break;
		case '/':cout << ch << "\t\t2" << "\t运算符" << endl; break;
			//分界符
		case '\"':cout << ch << "\t\t2" << "\t分界符" << endl; break;
		case '(':cout << ch << "\t\t3" << "\t分界符" << endl; break;
		case ')':cout << ch << "\t\t3" << "\t分界符" << endl; break;
		case '[':cout << ch << "\t\t3" << "\t分界符" << endl; break;
		case ']':cout << ch << "\t\t3" << "\t分界符" << endl; break;
		case ';':cout << ch << "\t\t3" << "\t分界符" << endl; break;
		case '{':cout << ch << "\t\t3" << "\t分界符" << endl; break;
		case '}':cout << ch << "\t\t3" << "\t分界符" << endl; break;
			//运算符
		case '=': {
			ch = fgetc(fpin);
			if (ch == '=')cout << "==" << "\t\t2" << "\t运算符" << endl;
			else {
				cout << "=" << "\t\t2" << "\t运算符" << endl;
				fseek(fpin, -1L, SEEK_CUR);
			}
		}break;
		case '#': {
			ch = fgetc(fpin);
			if (ch == '=')cout << "#=" << "\t\t2" << "\t运算符" << endl;
			else {
				cout << "#" << "\t\t2" << "\t运算符" << endl;
				fseek(fpin, -1L, SEEK_CUR);
			}
		}break;
		case ':': {
			ch = fgetc(fpin);
			if (ch == '=')cout << ":=" << "\t\t2" << "\t运算符" << endl;
			else {
				cout << ":" << "\t\t2" << "\t运算符" << endl;
				fseek(fpin, -1L, SEEK_CUR);
			}
		}break;
		case '>': {
			ch = fgetc(fpin);
			if (ch == '=')cout << ">=" << "\t\t2" << "\t运算符" << endl;
			else {
				cout << ">" << "\t\t2" << "\t运算符" << endl;
				fseek(fpin, -1L, SEEK_CUR);
			}
		}break;
		case '<': {
			ch = fgetc(fpin);
			if (ch == '=')cout << "<=" << "\t\t2" << "\t运算符" << endl;
			else {
				cout << "<" << "\t\t2" << "\t运算符" << endl;
				fseek(fpin, -1L, SEEK_CUR);
			}
		}break;
			//无识别
		default: cout << ch << "\t\t6" << "\t无识别符" << endl;
		}
	}
}
int main() {
	char input[30];
	FILE* fpin;
	cout << "请输入源文件名：\n" << endl;
	for (;;) {
		cin >> input;
		if ((fpin = fopen(input, "r")) != NULL)
			break;
		else
			cout << "路径输入错误" << endl;
	}
	cout << "词法分析结果:      " << endl;
	analyze(fpin);
	fclose(fpin);
}

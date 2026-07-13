#include<bits/stdc++.h>
using namespace std;

//全局变量，判断输入的源代码是否有词法错误
string miss = "";
int line_num = 1;
int flag = 0;  

//判断是不是关键字
bool isKeywords(string s)
{
	static vector<string> keyVec = { "main", "int", "float", "double", "char",
		"if", "then","else", "switch", "case", "break", "continue", "while",
		"do", "for","struct","." };
	vector<string>::iterator result = find(keyVec.begin(), keyVec.end(), s);
	if (result != keyVec.end())
		return true;
	else return false;
}

//标识符,试验要求:ID=letter(letter|digit)*
bool isIdentifier(string s)
{
	if (!isKeywords(s))//标识符不能是关键字
	{
		if ((s[0] >= 'a'&&s[0] <= 'z') || (s[0] >= 'A'&&s[0] <= 'Z'))//是字母
		{
			for (int i = 1; i < s.length(); i++)
			{
				if ((s[i] >= 'a'&&s[i] <= 'z') || (s[i] >= 'A'&&s[i] <= 'Z')
					|| (s[i] >= '0'&&s[i] <= '9'))
					continue;
				else
				{
					miss = s[i];
					return false;
				}
			}
			miss = "";
			return true;
		}
		else
		{
			miss = s[0];
			return false;
		}
	}
	return false;
}

//整型数字,NUM=digit digit*
bool isDigit(string s)
{
	if (s[0] >= '0'&&s[0] <= '9')
	{
		for (int i = 1; i < s.length(); i++)
		{
			if (s[i] >= '0'&&s[i] <= '9')
				continue;
			else
			{
				miss = s[i];
				return false;
			}
		}
		miss = "";
		return true;
	}
	return false;
}

//判断是不是浮点数
bool isFloat(string s)
{
	int flag3 = 0;
	if (s[0] >= '0'&&s[0] <= '9')
	{
		for (int i = 1; i < s.length(); i++)
		{
			if (s[i] >= '0'&&s[i] <= '9')
				continue;
			else if (s[i] == '.')
			{
				flag3 = 1;
				continue;
			}
			else if (s[i] == 'e')
			{
				continue;
			}
			else
			{
				miss = s[i];
				return false;
			}
		}
		if (flag3 == 0)
		{
			return false;
		}
		miss = "";
		return true;
	}
	return false;
}

//判断是不是运算符 
bool isOperator(string s) 
{
	static vector<string> opeVec = { "=","+","-","*","/","<","<=","==","!=",
		">",">=",";","(",")",":",",","{","}","&&","||","!" };
	vector<string>::iterator result = find(opeVec.begin(), opeVec.end(), s);
	if (result != opeVec.end())
	{
		miss = "";
		return true;
	}
	else
	{
		miss = s;
		return false;
	}
}

//判断单个运算符
bool isOperator(char c)
{
	static vector<char> opeVec = { '=','+','-','*','/','<','>',';','(',')','?',':',',','{','}','!' };
	vector<char>::iterator result = find(opeVec.begin(), opeVec.end(), c);
	if (result != opeVec.end())
		return true;
	else
	{
		miss = c;
		return false;
	}
}

//根据传入的参数s产生对应的输出
string result(string s)
{
	//种别码
	//1.标识符
	if (isIdentifier(s))
		return "ID: " + s;

	//2.关键字
	static map<string, string> keyMap;
	keyMap["int"] = "TYPE: ";
	keyMap["main"] = "MAIN: ";
	keyMap["float"] = "TYPE: ";
	keyMap["if"] = "IF: ";
	keyMap["struct"] = "STRUCT: ";
	keyMap["return"] = "RETURN: ";
	keyMap["else"] = "ELSE: ";
	keyMap["while"] = "WHILE: ";
	keyMap["."] = "DOT: ";
	//这里后续把关键字补全
	if (isKeywords(s))
		return keyMap[s] + s;
	//3.浮点型常量
	if (isFloat(s))
		return "FLOAT: " + s;
	//3.整型常量
	else if (isDigit(s))
		return "INT: " + s;

	//4.运算符
	static map<string, string> opeMap;
	opeMap["="] = "RELOP: =";
	opeMap["<"] = "RELOP: <";
	opeMap["<="] = "RELOP: <=";
	opeMap["=="] = "RELOP: ==";
	opeMap["!="] = "RELOP: !=";
	opeMap[">"] = "RELOP: >";
	opeMap[">="] = "RELOP: >=";
	opeMap[";"] = "SEMI: ;";
	opeMap["+"] = "PLUS: +";
	opeMap["("] = "LP: (";
	opeMap["-"] = "MINUS: -";
	opeMap[")"] = "RP: )";
	opeMap["*"] = "STAR: *";
	opeMap["/"] = "DIV: /";
	opeMap[","] = "COMMA: ,";
	opeMap["{"] = "LC: {";
	opeMap["}"] = "RC: }";
	opeMap["["] = "LB: [";
	opeMap["]"] = "RB: ]";
	opeMap["!"] = "NOT: !";
	opeMap["&&"] = "AND: &&";
	opeMap["||"] = "OR: ||";
	if (isOperator(s))
		return opeMap[s];
	flag = 1;
	string ans = "Error type A at Line " + to_string(line_num) + " Mysterious character '" + miss + "'";
	cout << ans << endl;
	return "Error type A at Line " + to_string(line_num) + " Mysterious character '" + miss + "'";
}

int main()
{
	cout<<"请输入文件名称（也可以带路径）"<<endl;
	string file; cin>>file;
	ifstream input(file);
	//输入文件流,注意编码，文本文件编码格式需和项目一直，否则乱码
	string copy;

	getline(input, copy, '\0');		//将文本的内容输出到屏幕上
	cout<<endl;
	cout<<"文件内容为："<<endl;
	cout << copy << endl;//测试是否正确

	//此时input已经指到了文件尾，为了后面的读取，需要关闭再打开
	input.close();
	input.open(file);

	/*测试结果要求以原数据与结果对照的形式输出并保存在Result.txt中，
	同时要把结果输出到屏幕。
	*/
	cout << "处理后结果:\n";
	string str;
	string words;
	while (getline(input, str)) //读取文件每一次读取一行,遇到EOF结束
	{
		//从输入流中获取单词，需要用到输入流对象，即istringstream
		istringstream strCin(str);
		string s;
		while (strCin >> words)  //这里的words可能是一大长串句子 , 按行处理
		{
			/*注意处理逗号，比如int a,b;这里有一个单词"a,b;”,所以要处理一个字符串里面
			的各种运算符,但是这样会很麻烦，发现没有，用ide写代码写完一句输入分号时，ide
			会自动加入空格，这样就方便处理多了*/

			//1.首先可以确定的是关键字肯定是单独作为一个单词的
			if (isKeywords(words))
			{
				s = result(words);
				continue;
			}

			//2,对单词进行扫描，肯定是标识符，运算符，逗号分号,数字等等混合在一起的单词
			vector<int> index = { 0 };
			vector<string> mulWords;//将words分解为多个单词
			if (words.length() == 1)
			{
				string rel = result(words);
				continue;
			}
			for (int i = 0; i < words.length(); i++)
			{

				//运算符有两位的，比如"<=",">=","==","!="
				if ((i < words.length() - 1) && isOperator(words[i]) && isOperator(words[i + 1]))
				{
					//但是要注意只有以上四种两位运算符，比如+-,))就不是,但是))还是要输出),)
					if (string(words.begin() + i, words.begin() + i + 2) == "<=" ||
						string(words.begin() + i, words.begin() + i + 2) == ">=" ||
						string(words.begin() + i, words.begin() + i + 2) == "==" ||
						string(words.begin() + i, words.begin() + i + 2) == "!=")
					{
						index.push_back(i);
						index.push_back(i + 2);
						i++;
					}
					else if (isOperator(words[i]))
					{
						if (find(index.begin(), index.end(), i ) == index.end() )  //查找失败，index中没有i这个数，插入
							index.push_back(i);
						if (find(index.begin(), index.end(), i + 1) == index.end())   //查找失败，index中没有i+1这个数，插入
							index.push_back(i + 1);

					}
				}
				//逗号，运算符作为分隔 ，也包括最后一个字符是不是运算符
				else if (isOperator(words[i]))
				{
					if (find(index.begin(), index.end(), i) == index.end())
						//比如遇到"a,b"这里下标0和1将a分开，1到2将逗号分开，2到3将b分开
						index.push_back(i);
					if (find(index.begin(), index.end(), i + 1) == index.end())
						index.push_back(i + 1);

					//如果是a<=b这样的呢？一样，先0和1将a分开，1和2将<分开，2和3将=分开
					//3和4将b分开，然后后面分隔单词时，注意如果相邻都是运算符，则忽略，比如
					//后面判断到1和2，2和3都是运算符，则忽略2

				}

			}
			for (int i = 0; i < index.size() - 1; i++)
			{
				string rel;
				//比如遇到"<="，需要提取”<=“
				/*if (isOperator(words[index[i]]) && isOperator(words[index[i + 1]]))
				{
					rel = result(string(words.begin() + index[i], words.begin() + index[i + 2]));
					i++;
				}
				else*/
				rel = result(string(words.begin() + index[i], words.begin() + index[i + 1])); //每次传入前后的两个参数指令
			}
		}
		miss = "";  //换行之后要清空miss
		line_num++;
	}

	if (flag == 0)
	{
		input.close();
		input.open(file);
		str = "";
		words = "";
		while (getline(input, str)) //读取文件每一次读取一行,遇到EOF结束
		{
			//从输入流中获取单词，需要用到输入流对象，即istringstream
			istringstream strCin(str);
			string s;
			while (strCin >> words)  //这里的words可能是一大长串句子 , 按行处理
			{
				/*注意处理逗号，比如int a,b;这里有一个单词"a,b;”,所以要处理一个字符串里面
				的各种运算符,但是这样会很麻烦，发现没有，用ide写代码写完一句输入分号时，ide
				会自动加入空格，这样就方便处理多了*/


				//1.首先可以确定的是关键字肯定是单独作为一个单词的
				if (isKeywords(words))
				{
					s = result(words);
					cout << s << endl;
					continue;
				}

				//2,对单词进行扫描，肯定是标识符，运算符，逗号分号,数字等等混合在一起的单词
				vector<int> index = { 0 };
				vector<string> mulWords;//将words分解为多个单词
				if (words.length() == 1)
				{
					string rel = result(words);
					cout << rel << endl;
					continue;
				}
				for (int i = 0; i < words.length(); i++)
				{

					//运算符有两位的，比如"<=",">=","==","!="
					if ((i < words.length() - 1) && isOperator(words[i]) && isOperator(words[i + 1]))
					{
						//但是要注意只有以上四种两位运算符，比如+-,))就不是,但是))还是要输出),)
						if (string(words.begin() + i, words.begin() + i + 2) == "<=" ||
							string(words.begin() + i, words.begin() + i + 2) == ">=" ||
							string(words.begin() + i, words.begin() + i + 2) == "==" ||
							string(words.begin() + i, words.begin() + i + 2) == "!=")
						{
							index.push_back(i);
							index.push_back(i + 2);
							i++;
						}
						else if (isOperator(words[i]))
						{
							if (find(index.begin(), index.end(), i) == index.end())   //查找失败，index中没有i这个数，插入
								index.push_back(i);
							if (find(index.begin(), index.end(), i + 1) == index.end())   //查找失败，index中没有i+1这个数，插入
								index.push_back(i + 1);

						}
					}
					//逗号，运算符作为分隔 ，也包括最后一个字符是不是运算符
					else if (isOperator(words[i]))
					{
						if (find(index.begin(), index.end(), i) == index.end())
							//比如遇到"a,b"这里下标0和1将a分开，1到2将逗号分开，2到3将b分开
							index.push_back(i);
						if (find(index.begin(), index.end(), i + 1) == index.end())
							index.push_back(i + 1);

						//如果是a<=b这样的呢？一样，先0和1将a分开，1和2将<分开，2和3将=分开
						//3和4将b分开，然后后面分隔单词时，注意如果相邻都是运算符，则忽略，比如
						//后面判断到1和2，2和3都是运算符，则忽略2

					}

				}
				for (int i = 0; i < index.size() - 1; i++)
				{
					string rel;
					//比如遇到"<="，需要提取”<=“
					/*if (isOperator(words[index[i]]) && isOperator(words[index[i + 1]]))
					{
						rel = result(string(words.begin() + index[i], words.begin() + index[i + 2]));
						i++;
					}
					else*/
					rel = result(string(words.begin() + index[i], words.begin() + index[i + 1])); //每次传入前后的两个参数指令
					cout << rel << endl;
				}
				if (index.size() <= 1)
				{
					string rel = result(words);
					cout << rel << endl;
				}
			}
			miss = "";  //换行之后要清空miss
			line_num++;
		}
	}
	input.close();
	system("pause");
	return 0;
}

#include<bits/stdc++.h>
using namespace std;
const int wordnumPerLine = 7;
bool writeFile = false;
FILE* grammerFile;
char fname[50],outfname[50],c,reply[50];
string analyzedString;

#define readLine if( fscanf(grammerFile,"%[^\n]%c",theLine,&c)==EOF)	return;

#pragma region 工具函数
inline bool isInVector(vector<string> vec,string val) {
	vector<string>::iterator iter = find(vec.begin(), vec.end(), val);
	if (iter == vec.end())
		return false;
	return true;
}
inline set<string> getNoneEmptySet(set<string> s) {//得到无空集
	set<string> ans = s;
	set<string>::iterator it = ans.find("#");
	if (it!= ans.end()) {
		ans.erase(it);
	}
	return ans;
}
inline bool isNoneTerminal(string x) {
	return x.length() > 0 && x[0] >= 'A'&&x[0] <= 'Z';
}
inline vector<string> splitString(string oriString, char splitChar = ' ') {
	vector<string> vec;
	oriString += ' ';
	int splitpos;
	while (splitpos = oriString.find(splitChar)!=string::npos) {
		vec.push_back(oriString.substr(0, splitpos));
		oriString = oriString.substr(splitpos+1);
	}
	return vec;
}
inline string vectorToString(vector<string> vec,bool reverse = false) {
	string ansStr = "";
	if (reverse) {
		for (int i = vec.size()-1; i >=0; i--) {
			ansStr += vec[i] + " ";
		}
	}
	else {
		for (int i = 0; i < vec.size(); i++) {
			ansStr += vec[i] + " ";
		}
	}
	return ansStr;
}
#pragma endregion

struct Production{
	string left;
	vector<string>right;
	bool isEmpty = false;
	string getRight() {
		string ans = "";
		for (int i = 0; i < right.size() - 1; i++)
			ans += right[i] + " ";
		if (!right.empty()) {
			ans += right.back();
		}
		return ans;
	}
	string getProduction() {
		return left + "->" + getRight();
	}
};

struct Grammer
{
	vector<Production> productions;//文法产生式
	vector<string> nonTerminal;//文法非终结符
	vector<string> terminal;//文法终结符
	map<string,set<string> > firstSet;//first集
	map<string,set<string> > followSet;//follow集
	vector<set<string> > select;//第i条产生式的select集
	map<string, bool> canReachEmpty;//非终结符表
	string startSymbol= "";//开始符号
	map<pair<string, string>, int> predictionMap;//非终结符A遇到终结符b时，使用第i条产生式
	friend ostream & operator <<(ostream & os, const Grammer & g);
}grammer;

struct Description
{
	string analyzeStr;
	string remainStr;
	string matchStr;
	Description() {}
	Description(string analyzeStr, string remainStr) {
		this->analyzeStr = analyzeStr;
		this->remainStr = remainStr;
	}
};
struct Sentence
{
	vector<string> analyzeStack;
	vector<string> remainString;
	vector<Description > description;
	string sentenceContent;
	void analyzeString() {//分析串
		int productionIndex;
		string nAnaStr, nRemStr;
		vector<string> nRight;
		analyzeStack.clear();//原有分析栈清空
		description.clear();//原有描述清空
		remainString = splitString(sentenceContent + " #");//获取剩余输入串（默认以空格隔开）
		analyzeStack.push_back("#");//压入#
		analyzeStack.push_back(grammer.startSymbol);//压入开始符号
		while (true) {
			nAnaStr = analyzeStack.back(), nRemStr = remainString.front();
			description.push_back(Description(vectorToString(analyzeStack),vectorToString(remainString)));
			if (nAnaStr == nRemStr) {//遇到匹配情况
				analyzeStack.pop_back();//分析栈弹栈
				remainString.erase(remainString.begin());//剩余符号串移除队首
				if (analyzeStack.empty() && remainString.empty()) {//分析完成，接受
					description.back().matchStr = ("接受");
					break;
				}
				description.back().matchStr = ("\"" + nAnaStr + "\"" + "匹配");
			}
			else if (grammer.predictionMap.find(pair<string, string>(nAnaStr, nRemStr)) != grammer.predictionMap.end()) {
				productionIndex = grammer.predictionMap[pair<string, string>(nAnaStr, nRemStr)];//根据分析表取得产生式
				description.back().matchStr = grammer.productions[productionIndex].getProduction();//压入描述
				analyzeStack.pop_back();//分析栈弹栈
				nRight = grammer.productions[productionIndex].right;//获取产生式右部
				if (nRight[0] == "#") {//产生式右部为空，分析栈不做处理

				}
				else {
					for (int i = nRight.size() - 1; i >= 0; i--) {//反向压栈
						analyzeStack.push_back(nRight[i]);
					}
				}
			}
			else {
				//报错
				analyzeStack.push_back("错误：无法匹配，分析结束。");
				break;
			}
		}
	}
}sentence;

ostream & operator <<(ostream & os, Grammer & g) {
	filebuf fb;
	ostream* pos;
	if (writeFile) {
		fb.open(outfname,ios::out);
		pos = new ostream(&fb);
	}
	else {		
		pos = &os;
	}
	*pos << "非终结符：\n";
	for (int i = 0; i < g.nonTerminal.size(); i++) {
		*pos<< g.nonTerminal[i] << "\t" << " ";
		if ((i+1)%wordnumPerLine == 0)
			*pos << endl;
	}
	*pos << endl;
	*pos << "终结符：\n";
	for (int i = 0; i < g.terminal.size(); i++) {
		*pos<< g.terminal[i] << "\t" << " ";
		if ((i+1)%wordnumPerLine == 0)
			*pos << endl;
	}
	*pos << endl;
	*pos << "文法：\n";
	for (int i = 0; i < g.productions.size(); i++) {
		*pos << g.productions[i].left << "->";
		for (int j = 0; j < g.productions[i].right.size(); j++) {
			*pos << " "<< g.productions[i].right[j];
		}
		*pos << endl;
	}
	*pos << "非终结符表：\n";
	for (map<string, bool>::iterator it = grammer.canReachEmpty.begin(); it != grammer.canReachEmpty.end(); ++it) {
		*pos << it->first << ":\t" << ((it->second)?"Y":"N")<<endl;
	}
	*pos << "计算First集：\n";
	for (map<string, set<string> >::iterator it = grammer.firstSet.begin();it!= grammer.firstSet.end();++it) {
		*pos << it->first << ":\t";
		for (set<string>::iterator it2 = grammer.firstSet[it->first].begin(); it2!= grammer.firstSet[it->first].end(); ++it2) {
			*pos << *it2 << "\t";
		}
		*pos << endl;
	}
	*pos << "计算Follow集：\n";
	for (map<string, set<string> >::iterator it = grammer.followSet.begin(); it != grammer.followSet.end(); ++it) {
		*pos << it->first << ":\t";
		for (set<string>::iterator it2 = grammer.followSet[it->first].begin(); it2 != grammer.followSet[it->first].end(); ++it2) {
			*pos << *it2 << "\t";
		}
		*pos << endl;
	}
	*pos << "计算Select集：\n";
	*pos << fixed<<right;
	for (int i = 0; i < g.productions.size(); i++) {
		*pos <<setw(15) <<g.productions[i].left + "->"+ g.productions[i].getRight()+":";
		for (set<string>::iterator it = grammer.select[i].begin(); it != grammer.select[i].end(); ++it) {
			*pos << *it << "\t";
		}
		*pos << endl;
	}
	*pos << "得到预测分析表：\n";
	*pos<<left <<setw(10) <<" ";
	vector<string> terminalAndEmpty(g.terminal);
	terminalAndEmpty.push_back("#");
	for (int i = 0; i < terminalAndEmpty.size(); i++) {//终结符表头
		*pos << setw(10)<<terminalAndEmpty[i];
	}
	*pos << endl;
	//终结符对应的产生式
	for (int i = 0; i < g.nonTerminal.size(); i++) {
		string A = g.nonTerminal[i];
		*pos << setw(10) << A ;
		for (int j = 0; j < terminalAndEmpty.size(); j++) {
			pair<string, string> pkey(A,terminalAndEmpty[j]);
			if (g.predictionMap.find(pkey) != g.predictionMap.end()) {
				int index = g.predictionMap[pkey];
				*pos << setw(10) << g.productions[index].getRight();
				//grammer.predictionMap[pair<string, string>(firstKey, secondKey)] = i;
			}
			else{
				*pos <<setw(10) << "";
			}
		}
		*pos << endl;
	}
	*pos << "开始分析句子:"+sentence.sentenceContent<<endl<<left;
	*pos << setw(30) << "步骤" << setw(30) << "分析栈" << setw(30) << "剩余输入串" << setw(30) << "推导所用的产生式或匹配"<<endl;
	for (int i = 0; i < sentence.description.size();i++) {
		*pos << setw(30) << i+1 << setw(30) << sentence.description[i].analyzeStr << setw(30) << sentence.description[i].remainStr << setw(22) << sentence.description[i].matchStr<<endl;
	}
	if (writeFile)
	{
		fb.close();
		printf("写入文件成功！，请前往查看");
	}
		
	return *pos;
}

//1.获取产生式2.把开头为的空和非终结符加入first集合3.设置直接可达空的非终结符表4.查找开始符号
Production lineToProduction(string line) {
	Production production;
	string rightPart = "";//当前产生式右部的一个单词
	int leftIndex = line.find_first_of('-');
	production.left = line.substr(0, leftIndex);
	if (grammer.startSymbol == "")//开始符号为空则将production.left做为开始符号
		grammer.startSymbol = production.left;
	line = line.substr(leftIndex + 2);//当前line为只剩右部的字符串
	line += ' ';
	if (line[0]=='#'){//右部为空
		production.isEmpty = true;
		production.right.push_back("#");
		grammer.firstSet[production.left].insert("#");//能推导出空，则将其加入first集合
		grammer.canReachEmpty[production.left] = true;
		return production;
	}
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ' ') {
			if (isNoneTerminal(rightPart)) {//非终结符以大写字母开头
				if (!isInVector(grammer.nonTerminal, rightPart)) {//此非终结符不在文法非终结符里面
					grammer.nonTerminal.push_back(rightPart);
				}
			}
			else {//终结符以小写字母开头
				if (production.right.empty()) {//若该单词为第一条产生式，且为终结符，则将其加入first集。
					grammer.firstSet[production.left].insert(rightPart);
				}
				if (!isInVector(grammer.terminal, rightPart)) {//此终结符不在文法终结符里面
					grammer.terminal.push_back(rightPart);
				}
			}
			production.right.push_back(rightPart);
			rightPart = "";
			continue;
		}
		rightPart += line[i];
	}
	return production;
}
//计算非终结符表
void calNonTerminalTable() {
	bool flag = true;//记录当前是否有表的扩大
	while (flag) {//有表的扩大，就继续循环
		flag = false;//初始化flag
		for (int i = 0; i < grammer.productions.size(); i++) {
			Production nowProduction = grammer.productions[i];//获取当前产生式
			string nowLeft = nowProduction.left;//获取当前产生式的左部
			if (grammer.canReachEmpty[nowLeft]) {//如果已经可达空，则无需计算，continue
				continue;
			}
			bool canReach = true;
			for (int j = 0; j < nowProduction.right.size(); j++) {//获取产生式的所有右部
				if (isNoneTerminal(nowProduction.right[j])) {//此元素为非终结符
					if (!grammer.canReachEmpty[nowProduction.right[j]])//当前非终结符无法到达空，标志置为false,break这个产生式
					{
						canReach = false;
						break;
					}
				}
				else {//若不为终结符，则为非终结符，则肯定不能达到空,标志置为false。break这个产生式
					canReach = false;
					break;
				}
			}
			if (canReach)//说明该产生式能推出空
			{
				grammer.canReachEmpty[nowLeft]= true;
				flag = true;
			}
		}
	}
}

void getFirst() {
	//先将能达到#的#填充入个非终结符的first集合
	for (int i = 0; i < grammer.nonTerminal.size(); i++) {
		if (grammer.canReachEmpty[grammer.nonTerminal[i]]){
			grammer.firstSet[grammer.nonTerminal[i]].insert("#");
		}
	}
	bool isExpanded = true;
	int orilen;//原始的first集合大小
	while (isExpanded) {//First集有扩张，则继续扩充
			isExpanded = false;//初始化为没有扩张
		for (int i = 0; i < grammer.productions.size(); i++) {
			Production nowProduction = grammer.productions[i];//获取当前产生式
			if (nowProduction.isEmpty)//右部为空的产生式不参与计算
				continue;
			string nowLeft = nowProduction.left;//获取当前产生式的左部
			orilen = grammer.firstSet[nowLeft].size();//获取扩充前该非终结符长度
			for (int j = 0; j < nowProduction.right.size(); j++) {//从左向右遍历该产生的每一个字符
				if (!isNoneTerminal(nowProduction.right[j])) {//该右部的这个字符为终结符
					grammer.firstSet[nowLeft].insert(nowProduction.right[j]);//插入并跳出
					break;
				}
				else {//为非终结符，则将其插入当前非终结符的first集
					//isExistedNull记录是否存在#
					set<string> insertedSet = getNoneEmptySet(grammer.firstSet[nowProduction.right[j]]);//获取非空集
					grammer.firstSet[nowLeft].insert(insertedSet.begin(), insertedSet.end());//插入非空集
					set<string>::iterator sit = grammer.firstSet[nowLeft].find("#");

					if (!grammer.canReachEmpty[nowProduction.right[j]]){//该非终结符不能到达空，结束，break
						break;
					}
				}
			}
			if (grammer.firstSet[nowLeft].size() > orilen) {//如果该first集扩大了
				isExpanded = true;
			}
		}
	}
}

void getFollow() {
	//在开始符号的follow集里面加入#（默认开始符号为加入的第一个非终结符）
	grammer.followSet[grammer.startSymbol].insert("#");
	//按照2及以后的规则扩充follow集
	bool isExpanded = true;
	int orilen;//原始的Follow集合大小
	while (isExpanded) {//Follow集有扩张，则继续扩充
		isExpanded = false;//初始化为没有扩张
		for (int i = 0; i < grammer.productions.size(); i++) {
			Production nowProduction = grammer.productions[i];//获取当前产生式
			if (nowProduction.isEmpty)//右部为空的产生式不参与计算
				continue;
			string nowLeft = nowProduction.left;//获取当前产生式的左部
			for (int j = 0; j < nowProduction.right.size(); j++) {//从左向右遍历该产生的每一个字符
				if (isNoneTerminal(nowProduction.right[j])){//nowProduction.right[j]为非终结符
					string A = nowProduction.right[j];
					orilen = grammer.followSet[A].size();
					bool betaCanEmpty = true;
					for (int k = j + 1; k < nowProduction.right.size(); k++) {//遍历之后的所有字串
						string beta = nowProduction.right[k];
						if (isNoneTerminal(beta)) {//β为非终结符
							set<string> insertedSet = getNoneEmptySet(grammer.firstSet[beta]);//得到beta非空的first集							
							grammer.followSet[A].insert(insertedSet.begin(), insertedSet.end());//插入first集
							if (!grammer.canReachEmpty[beta]) {//不能星推导出空，follow集计算也到此为止，break跳出
								betaCanEmpty = false;
								break;
							}
						}
						else {//β为终结符，直接加入，break跳出
							betaCanEmpty = false;
							grammer.followSet[A].insert(beta);
							break;
						}
					}
					if (betaCanEmpty) {//说明beta能推导出空，把nowLeft的Follow集加入A的Follow集				
						grammer.followSet[A].insert(grammer.followSet[nowLeft].begin(), grammer.followSet[nowLeft].end());
					}
					if (grammer.followSet[A].size() > orilen) {//有扩充
						isExpanded = true;
					}
				}
			}
		}
	}
}

void getInput() {
	char theLine[100];//这一行产生式
	while (true){
		readLine;
		Production production = lineToProduction(theLine);		
		grammer.productions.push_back(production);
		if (!isInVector(grammer.nonTerminal, production.left))
			grammer.nonTerminal.push_back(production.left);
	}
}

void getSelect() {
	for (int i = 0; i < grammer.productions.size(); i++) {
		Production nowProduction = grammer.productions[i];//获取当前产生式
		set<string> nowSelect;
		if (nowProduction.isEmpty) {//右部为空，则直接把左部的Follow集加入该产生式的Select集
			nowSelect.insert(grammer.followSet[nowProduction.left].begin(), grammer.followSet[nowProduction.left].end());
		}
		else {//右部非空
			bool canEmpty = true;
			for (int j = 0; j < nowProduction.right.size(); j++) {
				string A = nowProduction.right[j];
				if (isNoneTerminal(A)) {//A为非终结符
					set<string> insertedSet = getNoneEmptySet(grammer.firstSet[A]);
					nowSelect.insert(insertedSet.begin(), insertedSet.end());
					if (!grammer.canReachEmpty[A]) {//A不能推出空
						canEmpty = false;
						break;
					}
				}
				else {//为终结符，则加入Select集
					canEmpty = false;
					nowSelect.insert(A);
					break;
				}
			}
			if (canEmpty) {//能星推导出空，把左部的Follow集也加入该产生式的Select集
				nowSelect.insert(grammer.followSet[nowProduction.left].begin(), grammer.followSet[nowProduction.left].end());
			}
		}
		grammer.select.push_back(nowSelect);
	}
}

void getPredictMap() {//预测分析表将Select重新组织成一种易于查找的模式
	string firstKey, secondKey;
	for (int i = 0; i < grammer.productions.size(); i++) {
		firstKey = grammer.productions[i].left;
		for (set<string>::iterator it = grammer.select[i].begin();it!= grammer.select[i].end(); ++it) {
			secondKey = *it;
			//firstkey遇到secondkey则使用产生式i（从0开始编号）
			grammer.predictionMap[pair<string,string>(firstKey,secondKey)] = i;
		}
	}
}

int main() {
	printf("请输入文件名\n");
	scanf("%s", fname);
	printf("是否需要写入文件(Y/N)\n");
	scanf("%s", reply);
	if (reply[0] == 'Y' || reply[0] == 'y') {
		printf("请输入写入文件名\n");
		scanf("%s",outfname);
		writeFile = true;
	}
	grammerFile = fopen(fname, "r");
	if (&grammerFile) {//文件读取成功
		getInput();//读入文法生成grammer
		calNonTerminalTable();//1.计算非终结符表
		getFirst();//2.计算first集
		getFollow();//3.计算Follow集
		getSelect();//3.计算Select集
		getPredictMap();//4.计算预测分析表
	}
	printf("请输入分析串内容\n");//输入分析串
	getchar();
	getline(cin,analyzedString);
	sentence.sentenceContent = analyzedString;
	sentence.analyzeString();//开始分析
	cout << grammer;
	getchar();
	getchar();
	return 0;
}

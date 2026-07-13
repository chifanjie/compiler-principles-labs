#include<bits/stdc++.h>
using namespace std;

typedef struct{
    string formula;//原始文法产生式
}grammarElement;

typedef struct{
	string regular;//项目文法产生式
	int loca;//来自文法的第j个产生式
}condition;

grammarElement  gramOldSet[100];//原始文法的产生式集
grammarElement  gramSet[100];//初始项目文法集
condition cd[200];//原始项目
set<string> I[20];//I是文法G的任一项目集，无序
set<set<string> > C;//LR（0）项目集规范族

int sum;
int gm_n,cd_n=0, I_n = 1;//文法产生式的个数,项目的个数，项目集的序号
string terSymbol;//终结符号
string non_ter;//非终结符号
string allSymbol;//所有符号
string M[200][200];//LR(0)分析表
string str_cin;//输入串
stack <int> Cd;    ///状态栈
stack <char> S;   ///符号栈

int atoi(string s){//字符串转换为数字
	int num = 0;
	for(int i = 0;i < s.length();i++){
		num *=10;
		num += (s[i] - 48);
	}
	return num;
}

string Print_Stack(stack <int> ss){//打印状态栈
    stack <int> sta1;
    sta1 = ss;
    string str_temp;//暂存符号栈
    while(!sta1.empty()){
        str_temp.insert(0,to_string(sta1.top()));
        sta1.pop();
    }
    return str_temp;
}

string PrintStack(stack <char> ss){//打印符号栈
    stack <char> sta1;
    sta1 = ss;
    string str_temp;//暂存符号栈
    while(!sta1.empty()){
        str_temp.insert(0,1,sta1.top());
        sta1.pop();
    }
    return str_temp;
}

string PrintStrCin(string s,int num){//打印输入串
    string str;
    for(int i = num; i<s.length();i++){
        str += s[i];
    }
    return str;
}

void initSet(grammarElement s[],int n1){//原始文法的初始化
    s[0].formula = "S->";//扩广文法S->#E#
    s[0].formula.insert(3,1,s[1].formula[0]);
    non_ter.insert(0,1,'S');//文法符号是开始符号S
    gm_n++;
    for(int i = n1; i >= 1; i--){
        string temp = s[i].formula;//暂时保存需要处理的产生式
        for(int j = s[i].formula.length() - 1; j >= 0; j-- ){
            if(s[i].formula[j]  == '|'){//确定带有 | 的产生式
				int pos = i+1;
				string str0 = temp.erase(3,j-2);//加上新的产生式
				temp = s[i].formula.erase(j);//去除 |
				for(int k = gm_n; k >= pos;k--){
					s[k] = s[k-1];
				}
				s[pos].formula = str0; 
				gm_n++;
            }
        }
    }
	cout<<"\n消除 | 的产生式："<<endl;
	for(int i = 0;i < gm_n; i++){
        cout<<s[i].formula<<endl; 
		gramSet[i] = s[i];//项目初始集
		gramSet[i].formula.insert(3,1,'.');
		int temp = cd_n;
		string str = s[i].formula; 
		for(int j = 3; j <= str.length(); j++){
			cd[cd_n].regular = str;
			cd[cd_n].loca = i;
			cd[cd_n++].regular.insert(j,1,'.');
		}
    }
	I[0].insert(gramSet[0].formula);//I0有扩广文法
    cout<<endl;	
	cout<<"这个文法的项目有："<<endl;
	for(int i = 0; i < cd_n; i++)
		cout<<cd[i].regular<<endl;
	cout<<endl;	
}

set<string> CLOSURE(set <string> I){//构造I的闭包CLOSURE（I）
	int oldSize;
	do{
		oldSize = I.size();
		for(auto it:I){//使用C++新特性循环遍历I，如果需要改变i的值，还需要在前面加上&
			string str0;
			str0 = it;
			for(int i = 3;i < str0.length();i++){
				if((str0[i] == '.' )&&(non_ter.find(str0[i+1]) != -1)){
					for(int j =  0; j < gm_n ;j++){
						if(gramSet[j].formula[0] == str0[i+1])
							I.insert(gramSet[j].formula);
					}
				}
			}			
		}
	}while(I.size() != oldSize);
	return I;
}

set<string> GO(set <string> I,char X){//转换函数
	set <string> J;
	int point;//点的位置
	string str0;
	string str1;
	str0.append(1,X);
	str0 += ".";
	for(auto it:I){
		str1 = it;
		point = str1.find(".");
		if(str1.rfind(X) == (point + 1)){
			str1.replace(point,2,str0);
			J.insert(str1);
			J = CLOSURE(J);
		}else{
			continue;
		}
	}
	return J;
}

void ITEMSETS(){//构造文法G’的LR(0)的项目集规范族
	I[0] = CLOSURE(I[0]);
	C.insert(CLOSURE(I[0]));
	int oldSize;
	do{
		oldSize = C.size();
		for(auto it:C){
			set <string> temp0 = it;
			for(int i = 0; i < allSymbol.length();i++){
				set <string> temp1 = GO(temp0,allSymbol[i]);
				if(!temp1.empty() && (C.count(temp1) == 0)){//GO(I,X)非空且不属于C
					I[I_n++] = temp1;
					C.insert(temp1);
				} 
			}
		}
	}while(C.size() != oldSize);
}

void dealTable(){//构造LR(0)分析表
    terSymbol += '#';
	non_ter.erase(non_ter.begin() + non_ter.find('S'));
	allSymbol = terSymbol + non_ter;
    int cnt = 0;//计数项目的个数
	string st;//保存数字转成字符串
	// memset(M,NULL,sizeof(M));
	do{
		string s = cd[cnt].regular;
		for(int k = 0; k < I_n;k++){
			if(I[k].count(s) == 1){//项目属于Ik
				if((s.find('.') == s.length()-1)){
					if(s[0] == 'S'){//若项目S’→S?属于Ik，则置ACTION[k,#]为"接受",简记为"acc"
						M[k][allSymbol.find('#')] = "acc"; 
					}else{//若项目A→α?属于Ik，那么，对任何终结符a（或结束符#），
						for(int m = 0; m < terSymbol.length(); m++){//置ACTION[k,a]为"用产生式A→α进行归约"
							st = to_string(cd[cnt].loca);//简记为"rj"（假定产生式A→α是文法G的第j个产生式）
							M[k][m] = "r" + st;
						}
					}
				}else if(terSymbol.find(s[s.find('.')+1]) != -1){//若项目A→α?β属于Ik且GO（Ik，a）=Ij，
					for(int j = 0; j < I_n;j++){//a为终结符，则置ACTION[k,a]为"把（j，a）移进栈"，
						if(GO(I[k],s[s.find('.')+1]) == I[j]){//简记为"sj"
							st = to_string(j);
							M[k][terSymbol.find(s[s.find('.')+1])] = "s" + st;
						}
					}
				}
			}
			else{//若GO（Ik，A）=Ij，A为非终结符,，则置GOTO[k,A]=j
				for(int i = terSymbol.length(); i < allSymbol.length();i++)
					for(int j = 0; j < I_n;j++){
						if(GO(I[k],allSymbol[i]) == I[j]){
							st = to_string(j);
							M[k][i] = st;
						}
					}
			}
		}
		cnt++;
	}while(cnt < cd_n);
    cout<<setw(8+allSymbol.length()*6)<<"LR(0)分析表"<<endl;
    cout<<setw(8)<<"状态"<<setw(6*terSymbol.length())<<"       ACTION      "<<setw(6*non_ter.length())<<"   GOTO   "<<endl;
	cout<<setw(8)<<"    ";
	for(int i = 0; i < allSymbol.length(); i++){//表头
		if(i < terSymbol.length()){
			cout<<setw(6)<<allSymbol[i];
		}else{
			cout<<setw(6)<<allSymbol[i];
		}
	}
	cout<<endl;
    for(int i = 0;i < I_n;i++){
		cout<<setw(8)<<i;
        for(int j = 0;j < allSymbol.length();j++)
            cout<<setw(6)<<M[i][j];
        cout<<endl;
    }
	cout<<endl;
}

void allContronl(){//总控程序,LR分析算法描述
    cout<<"待分析的输入串为（‘#’是输入串的结束符）：";
    cin>>str_cin;//输入串
    cout<<"LR(0)分析器的工作过程："<<endl;
    char a;//当前的输入符a
	bool flag = true;
    int j,pos = 0, counter = 0;//pos输入串的指示坐标，counter步骤计数器
    cout<<setw(8)<<std::left<<"步骤"<<setw(12)<<std::left<<"状态栈"<<setw(12)<<std::left<<"符号栈";
    cout<<setw(12)<<std::right<<"输入串"<<"\t"<<setw(12)<<std::left<<"动作"<<endl;
    Cd.push(0);//状态栈置初值
	S.push('#');//符号栈置初值
    cout<<setw(8)<<std::left<<"0"<<setw(12)<<std::left<<Print_Stack(Cd)<<setw(12)<<std::left<<PrintStack(S);
    cout<<setw(12)<<std::right<<PrintStrCin(str_cin,pos)<<"\t"<<setw(12)<<std::left<<"预备"<<endl;
    a = str_cin[pos];//读入第一个符号给a,将S0移进状态栈，#移进符号栈，S为状态栈栈顶状态
	string action;//保存动作
	int x = Cd.top();//状态栈顶
	int y = allSymbol.find(a);
	do{
		if(M[x][y].find('s') != -1){//ACTION（S,a） = si.
			action = "把(,)移进栈";action.insert(action.find(','),1,M[x][y][1]);action.insert(action.find(',')+1,1,a);
			Cd.push(atoi(M[x][y].substr(1)));S.push(a);//PUSH  i,a(分别进栈)
			cout<<setw(8)<<std::left<<++counter<<setw(12)<<std::left<<Print_Stack(Cd)<<setw(12)<<std::left<<PrintStack(S);
    		cout<<setw(12)<<std::right<<PrintStrCin(str_cin,++pos)<<"\t"<<setw(12)<<std::left<<action<<endl;
			a = str_cin[pos];//读入下一个符号给a
		}else if(M[x][y].find('r') != -1){
			int j = atoi(M[x][y].substr(1)); 
			string str0 = gramOldSet[j].formula;
			action = "用产生式"+gramOldSet[j].formula+"进行归约"; 
			str0.erase(0,3);
			for(int i = 0; i < str0.length(); i++){//将状态栈和符号栈分别弹出对应的项
				Cd.pop();
				S.pop();
			}
			S.push(gramOldSet[j].formula[0]);//符号栈压入归约符号
			x = Cd.top();
			y = allSymbol.find(gramOldSet[j].formula[0]);
			Cd.push(atoi(M[x][y].substr(0)));
			cout<<setw(8)<<std::left<<++counter<<setw(12)<<std::left<<Print_Stack(Cd)<<setw(12)<<std::left<<PrintStack(S);
    		cout<<setw(12)<<std::right<<PrintStrCin(str_cin,pos)<<"\t"<<setw(12)<<std::left<<action<<endl;
		}else{
			cout<<"错误"<<endl;
			flag = false;
			break;
		}
		x = Cd.top();//状态栈顶
		y = allSymbol.find(a);
	}while(M[x][y] != "acc");
	if(flag){
		cout<<setw(8)<<std::left<<++counter<<setw(12)<<std::left<<Print_Stack(Cd)<<setw(12)<<std::left<<PrintStack(S);
    	cout<<setw(12)<<std::right<<PrintStrCin(str_cin,pos)<<"\t"<<setw(12)<<std::left<<"acc"<<endl;
		cout<<"\nCongratulation!"<<endl;
	}
}

int main(){
	FILE *stream;
    freopen_s(&stream, "test2.txt", "r", stdin);
    freopen_s(&stream, "result2.txt", "w", stdout);
    ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    cout<<"非终结符号如下：  ";
    cin>>non_ter;
	cout<<non_ter<<endl;
    cout<<"终结符号如下：  ";
    cin>>terSymbol;
	cout<<terSymbol<<endl;
    allSymbol = terSymbol + non_ter;
    cout<<"文法产生式的个数：  ";
    cin>>gm_n;
	cout<<gm_n<<endl;
    cout<<endl;
    for(int i = 1;i <= gm_n;i++){
        cout<<"请输入第"<<i<<"产生式："<<endl;
        cin>>gramOldSet[i].formula;
		cout<<gramOldSet[i].formula<<endl;
    }
    initSet(gramOldSet,gm_n);  
	ITEMSETS();
	cout<<"文法的LR(0)项目集规范族："<<endl;
	for(int i = 0; i < I_n;i++){
		cout<<"I"<<i<<setw(6)<<std::left<<": ";
		for(auto it:I[i])
			cout<<setw(12)<<std::left<<it;
		cout<<endl;
	}
	dealTable();
	allContronl();
    return 0;
}

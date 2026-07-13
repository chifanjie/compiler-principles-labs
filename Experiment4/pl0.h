#include<sstream>

# define norw 13 //关键字个数 
# define txmax 100 //名字表容量 
# define nmax 14 //number 的最大位数 
# define al 10 //符号的最大长度
# define levmax 3 //max depth of block nesting 最大允许过程嵌套声明层数[0，lexmax]
enum symbol {
	nul, ident, number, plus, minus,
	times, slash, oddsym, eql, neq, //slash 斜线
	lss, leq, gtr, geq, lparen, //leq :less than or equal to; gtr: great than; lparen:left parenthesis
	rparen, comma, semicolon, period, becomes,//comma 逗号 semicolon 分号 period 句号 becomes 赋值号
	beginsym, endsym, ifsym, thensym, whilesym,
	writesym, readsym, dosym, callsym, constsym,
	varsym, procsym,
};
#define symnum 32 
enum object { //object 为三种标识符的类型
	constant,
	variable,
	procedur,
};
FILE * fa1; //输出源文件及其各行对应的首地址
FILE * fas; //输出名字表
bool tableswitch; //显示名字表与否
char ch; //获取字符的缓冲区 ,getch 使用
enum symbol sym; //当前符号
char id[al + 1]; //当前 ident
int num; //当前 number 
int cc, ll; //getch 使用的计数器 ,cc 表示当前字符 (ch)的位置
int lineIndex; //当前行数
char line[81]; //读取行缓冲区
char a[al + 1]; //临时符号
char word[norw][al]; //保留字
enum symbol wsym[norw]; //保留字对应的符号值
enum symbol ssym[256]; //单字符的符号值
bool declbegsys[symnum]; //表示声明开始的符号集合 ,declaring begin symbol set 
bool statbegsys[symnum]; //表示语句开始的符号集 , statement 
bool facbegsys[symnum]; //表示因子开始的符号集合 ,factor 

struct tablestruct//有用的属性是前几个,后面两个属性为生成中间代码
{
	char name[al]; // 名字 
	enum object kind; // 类型仅三种
	int val; //const数值
	int level; // 所处层，const 不使用
	int adr; //地址
	int size; //需要分配的数据区空间
};
struct tablestruct table[txmax]; //名字表 
FILE * fin; //fin 文本文件用于指向输入的源程序文件
char fname[al];
int err; //错误计数器
#define getsymdo if(-1==getsym())return -1 
#define getchdo if(-1==getch())return -1 
#define expressiondo(a,b,c) if(-1==expression(a,b,c))return -1 
#define factordo(a,b,c) if(-1==factor(a,b,c))return -1 
#define termdo(a,b,c) if(-1==term(a,b,c))return -1 
#define conditiondo(a,b,c) if(-1==condition(a,b,c))return -1 
#define statementdo(a,b,c) if(-1==statement(a,b,c))return -1 
#define constdeclarationdo(a,b,c) if(-1==constdeclaration(a,b,c))return -1 
#define vardeclarationdo(a,b,c) if(-1==vardeclaration(a,b,c))return -1 
void error(int n);
int getsym();
int getch();
void init();
//集合操作声明
//int inset(int e, bool*s);
int addset(bool*sr, bool*s1, bool*s2, int n);
int subset(bool*sr, bool*s1, bool*s2, int n);
int mulset(bool*sr, bool*s1, bool*s2, int n);
//递归下降主要子程序声明
int block(int lev, int tx, bool* fsys);
int factor(bool* fsys, int* ptx, int lev);
int term(bool*fsys, int*ptx, int lev);
int condition(bool*fsys, int*ptx, int lev);
int expression(bool*fsys, int*ptx, int lev);
int statement(bool*fsys, int*ptx, int lev);
int vardeclaration(int* ptx, int lev, int* pdx);
int constdeclaration(int* ptx, int lev, int* pdx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx, int lev, int* pdx);

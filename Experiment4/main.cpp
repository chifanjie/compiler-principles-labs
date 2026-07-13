#include<stdio.h> 
#include"pl0.h" 
#include"string.h" 
int main()
{
	bool nxtlev[symnum];
	printf("Input pl/0 file ?");
	scanf("%s", fname); 
	fin = fopen(fname, "r"); 
	if (fin)
	{
		fa1 = fopen("fa1.txt", "w");
		fprintf(fa1, "Iput pl/0 file ?");
		fprintf(fa1, "%s\n", fname);
		init(); 
		err = 0; //错误计数器置 0 
		cc = lineIndex = ll = 0;
		ch = ' ';
		if (-1 != getsym())
		{
			fas = fopen("fas.tmp", "w");
			addset(nxtlev, declbegsys, statbegsys, symnum);
			nxtlev[period] = true;
			if (-1 == block(0, 0, nxtlev)) //调用编译程序
			{
				fclose(fa1);
				fclose(fas);
				fclose(fin);
				printf("\n");
			}
			fclose(fa1);
			fclose(fas);
			if (sym != period)
			{
				error(24);
			}
			if (err == 0) {
				printf("\ncomplie sucess!\n");
			}
			else {
				printf("\n%derrors,complie failed.\n",err);
			}
		}
		fclose(fin);
	}
	else
		printf("File doesn't exist! \n");
	printf("\n");
	getchar();
	getchar();
	return 0;
}
//初始化符号表等内容
void init()
{
	int i;
	for (i = 0; i <= 255; i++)
	{
		ssym[i] = nul; //ssym ：单字符的符号值
	}
	ssym['+'] = plus;
	ssym['-'] = minus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym['.'] = period;
	ssym['#'] = neq;
	ssym[';'] = semicolon;
	/*设置保留字名字 ,按照字母顺序 ,便于折半查找 */
	strcpy(&(word[0][0]), "begin");
	strcpy(&(word[1][0]), "call");
	strcpy(&(word[2][0]), "const");
	strcpy(&(word[3][0]), "do");
	strcpy(&(word[4][0]), "end");
	strcpy(&(word[5][0]), "if");
	strcpy(&(word[6][0]), "odd");
	strcpy(&(word[7][0]), "procedure");
	strcpy(&(word[8][0]), "read");
	strcpy(&(word[9][0]), "then");
	strcpy(&(word[10][0]), "var");
	strcpy(&(word[11][0]), "while");
	strcpy(&(word[12][0]), "write");
	/*设置保留字符号 */
	wsym[0] = beginsym;
	wsym[1] = callsym;
	wsym[2] = constsym;
	wsym[3] = dosym;
	wsym[4] = endsym;
	wsym[5] = ifsym;
	wsym[6] = oddsym;
	wsym[7] = procsym;
	wsym[8] = readsym;
	wsym[9] = thensym;
	wsym[10] = varsym;
	wsym[11] = whilesym;
	wsym[12] = writesym;
	/*设置符号集 */
	for (i = 0; i < symnum; i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}
	/*设置声明开始符号集 */
	declbegsys[constsym] = true;
	declbegsys[varsym] = true;
	declbegsys[procsym] = true;
	/*设置语句开始符号集 */
	statbegsys[beginsym] = true;
	statbegsys[callsym] = true;
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;
	/*设置因子开始符号集 */
	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;
}
//集合运算部分
int inset(int e, bool* s)
{
	return s[e];
}
int addset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] || s2[i];
	}
	return 0;
}
int subset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] && (!s2[i]);
	}
	return 0;
}
int mulset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		sr[i] = s1[i] && s2[i];
	}
	return 0;
}
//出错处理，打印出错位置和错误描述
void error(int n)
{
	char space[256];
	memset(space, 0, sizeof(space));
	space[cc - 1] = 0;// 出错时当前符号已经读完，所以 cc-1 
	switch (n)
	{
		case 1:
			sprintf(space,"Error type [%d] at Line [%d]: [变量%s在使用时未经定义].\n", n, lineIndex,id);
			break;
		case 2:
			sprintf(space,"Error type [%d] at Line [%d]: [函数%s在调用时未经定义].\n", n, lineIndex,id);
			break;
		case 3:
			sprintf(space,"Error type [%d] at Line [%d]: [变量%s出现重复定义，或变量与前面定义过的结构体名字重复].\n", n, lineIndex,id);
			break;
		case 4:
			sprintf(space,"Error type [%d] at Line [%d]: [过程%s出现重复定义].\n", n, lineIndex,id);
			break;
		case 5:
			sprintf(space,"Error type [%d] at Line [%d]: [赋值号两边的表达式类型不匹配].\n", n, lineIndex);
			break;
		case 6:
			sprintf(space,"Error type [%d] at Line [%d]: [操作数类型不匹配或操作数类型与操作符不匹配].\n", n, lineIndex);
			break;
		case 7:
			sprintf(space,"Error type [%d] at Line [%d]: [过程声明格式不正确（procedure后不为标识符）].\n", n, lineIndex);
			break;
		case 8:
			sprintf(space,"Error type [%d] at Line [%d]: [read或write函数的调用不完整（括号缺失）].\n", n, lineIndex);
			break;
		case 9:
			sprintf(space,"Error type [%d] at Line [%d]: [read函数的参数不是id].\n", n, lineIndex);
			break;
		case 10:
			sprintf(space,"Error type [%d] at Line [%d]: [调用函数格式非法（call后不为过程标识符）].\n", n, lineIndex);
			break;
		case 11:
			sprintf(space, "Error type [%d] at Line [%d]: [缺少赋值符号或赋值符号不正确].\n", n, lineIndex);//可能发生在语句定义、常变量定义与赋值时。
			break;
//		case 12:
//			sprintf(space, "Error type [%d] at Line [%d]: [一条语句定义（多个）常量或变量时漏掉了逗号或者分号/过程结束时漏了分号].\n", n, lineIndex);// 语句结束时或连续定义时缺少终止符号（end, ; ）
//			break;
//		case 14:
//			sprintf(space, "Error type [%d] at Line [%d]: [数字过长].\n", n, lineIndex);
//			break;
//		case 16:
//			sprintf(space, "Error type [%d] at Line [%d]: [if后缺少then语句].\n", n, lineIndex);
//			break;
//		case 18:
//			sprintf(space, "Error type [%d] at Line [%d]: [while语句缺少do].\n", n, lineIndex);
//			break;
//		case 19:
//			sprintf(space, "Error type [%d] at Line [%d]: [地址超出上界].\n", n, lineIndex);
//			break;
//		case 20:
//			sprintf(space, "Error type [%d] at Line [%d]: [逻辑表达式不合法（逻辑符号不合法）].\n", n, lineIndex);
//			break;
//		case 21:
//			sprintf(space, "Error type [%d] at Line [%d]: [因子标识符为过程].\n", n, lineIndex);
//			break;
//		case 22:
//			sprintf(space, "Error type [%d] at Line [%d]: [应输入)，表达式不完整].\n", n, lineIndex);
//			break;
//		case 24:
//			sprintf(space, "Error type [%d] at Line [%d]: [不以.结尾].\n", n, lineIndex);
//			break;
		default:
			sprintf(space, "Error type [%d] at Line [%d]: [未知错误].\n", n, lineIndex);
			break;
	}
	printf("%s", space);
	err++;
}
//读取字符部分，先把字符存到缓冲区，然后进行读取，这里同时进行了行号的记录
int getch()
{
	if (cc == ll)
	{
		if (feof(fin)) //读到文件尾
		{
			printf("program incomplete");
			return -1;
		}
		ll = 0;
		cc = 0;
		printf("%d ", ++lineIndex);
		fprintf(fa1, "%d ", lineIndex);
		ch = ' ';
		while (ch != 10)
		{
			if (EOF == fscanf(fin, "%c", &ch))
			{
				line[ll] = 0;
				break;
			}
			printf("%c", ch);
			fprintf(fa1, "%c", ch);
			line[ll] = ch;
			ll++;
		}
	}
	ch = line[cc];
	cc++;
	return 0;
}
// 词法分析 获取一个词法单元
int getsym()
{
	int i, j, k;
	while (ch == ' ' || ch == 10 || ch == 9) //忽略空白部分
	{
		getchdo; 

	}
	if (ch >= 'a'&&ch <= 'z')
	{
		k = 0;
		do {
			if (k < al)
			{
				a[k] = ch;
				k++;
			}
			getchdo;
		} while (ch >= 'a'&&ch <= 'z' || ch >= '0'&&ch <= '9');
		a[k] = 0;
		strcpy(id, a);
		i = 0;
		j = norw - 1;
		do {
			k = (i + j) / 2;
			if (strcmp(id, word[k]) <= 0)
			{
				j = k - 1;
			}
			if (strcmp(id, word[k]) >= 0)
			{
				i = k + 1;
			}
		} while (i <= j);
		if (i - 1 > j)
		{
			sym = wsym[k];
		}
		else
		{
			sym = ident;
		}
	}
	else
	{
		if (ch >= '0'&&ch <= '9')
		{
			k = 0;
			num = 0;
			sym = number;
			do {
				num = 10 * num + ch - '0';
				k++;
				getchdo;
			} while (ch >= '0'&&ch <= '9'); /* 获取数字的值 */
			k--;
			if (k > nmax)
			{
				error(14);
			}
		}
		else
		{
			if (ch == ':') // 检测赋值符号 
			{
				getchdo;
				if (ch == '=')
				{
					sym = becomes;
					getchdo;
				}
				else
				{
					sym = nul; // 不能识别的符号 
				}
			}
			else
			{
				if (ch == '<') //检测小于或小于等于符号 
				{
					getchdo;
					if (ch == '=')
					{
						sym = leq;
						getchdo;
					}
					else
					{
						sym = lss;
					}
				}
				else
				{
					if (ch == '>') // 大于或大于等于
					{
						getchdo;
						if (ch == '=')
						{
							sym = geq;
							getchdo;
						}
						else
						{
							sym = gtr;
						}
					}
					else
					{
						sym = ssym[ch];// 按单字符号处理
						if (sym != period) {

							getchdo;
						}
					}
				}
			}
		}
	}
	return 0;
}


//在名字表中加入一项
void enter(enum object k, int *ptx, int lev, int *pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name, id); //全局变量 id 中已存有当前名字的名字
	table[(*ptx)].kind = k;
	switch (k)
	{
		case constant: 
			if (num >= 100000000000000)
			{
				error(19);
				num = 0;
			}
			table[(*ptx)].val = num;
			break;
		case variable: /* 变量名字 */
			table[(*ptx)].level = lev;
			table[(*ptx)].adr = (*pdx);
			(*pdx)++;
			break; /* 过程名字 */
		case procedur:
			table[(*ptx)].level = lev;
			break;
	}
}
//查找ident在表中的位置
int position(char * idt, int tx)
{
	int i;
	strcpy(table[0].name, idt);
	i = tx;
	while (strcmp(table[i].name, idt) != 0)
	{
		i--;
	}
	return i;
}
#pragma region 递归下降
//分程序
int block(int lev, int tx, bool* fsys)
{
	int i;
	int dx; /* 名字分配到的相对地址 */
	int tx0; /*保留初始 tx*/
	bool nxtlev[symnum]; /* 在下级函数的参数中，符号集合均为值参，但由于使用数组实现，传递进来的是指针， 为防止下级函数改变上级函数的集合，开辟新的空间传递给下级函数 */
	dx = 3;
	tx0 = tx; /* 记录本层名字的初始位置 */
	if (lev > levmax)
	{
		error(15);
	}
	do {
		if (sym == constsym) /* 收到常量声明符号，开始处理常量声明 */
		{
			getsymdo;
			do {
				constdeclarationdo(&tx, lev, &dx); /*dx 的值会被 constdeclaration 改变，使用指针 */
				while (sym == comma)
				{
					getsymdo;
					constdeclarationdo(&tx, lev, &dx);
				}
				if (sym == semicolon)
				{
					getsymdo;
				}
				else
				{
					error(12); /* 漏掉了逗号或者分号 */
				}
			} while (sym == ident);
		}
		if (sym == varsym)/* 收到变量声名符号，开始处理变量声名 */
		{
			getsymdo;
			do {
				vardeclarationdo(&tx, lev, &dx);
				while (sym == comma)
				{
					getsymdo;
					vardeclarationdo(&tx, lev, &dx);
				}
				if (sym == semicolon)
				{
					getsymdo;
				}
				else
				{
					error(12);
				}
			} while (sym == ident);
		}
		while (sym == procsym)/* 收到过程声名符号，开始处理过程声名 */
		{
			getsymdo;
			if (sym == ident)
			{
				int i = position(id, dx);
				if (i != 0)
				{
					if (table[i].kind == procedur)//过程重定义
						error(4);
				}
				enter(procedur, &tx, lev, &dx);/* 记录过程名字 */
				getsymdo;
			}
			else
			{
				error(7);/*procedure 后应为标识符 */
				getsymdo;
			}
			if (sym == semicolon)
			{
				getsymdo;
			}
			else
			{
				error(12);/* 漏掉了分号 */
			}
			memcpy(nxtlev, fsys, sizeof(bool)*symnum);
			nxtlev[semicolon] = true;
			if (-1 == block(lev + 1, tx, nxtlev))
			{
				return -1;/* 递归调用 */
			}
			if (sym == semicolon)
			{
				getsymdo;
				memcpy(nxtlev, statbegsys, sizeof(bool)*symnum);
				nxtlev[ident] = true;
				nxtlev[procsym] = true;
			}
			else
			{
				error(12); /* 漏掉了分号 */
			}
		}
		memcpy(nxtlev, statbegsys, sizeof(bool)*symnum);
		nxtlev[ident] = true;
		nxtlev[period] = true;
	} while (inset(sym, declbegsys)); /* 直到没有声明符号 */
	table[tx0].size = dx; /* 声明部分中每增加一条声明都会给dx 增加 1,声明部分已经结束 ,dx 就是当前过程数据的 size*/
	if (tableswitch) /* 输出名字表 */
	{
		printf("TABLE:\n");
		if (tx0 + 1 > tx)
		{
			printf("NULL\n");
		}
		for (i = tx0 + 1; i <= tx; i++)
		{
			switch (table[i].kind)
			{
			case constant:
				printf("%d const %s", i, table[i].name);
				printf("val=%d\n", table[i].val);
				fprintf(fas, "%d const %s", i, table[i].name);
				fprintf(fas, "val=%d\n", table[i].val);
				break;
			case variable:
				printf("%d var%s", i, table[i].name);
				printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
				fprintf(fas, "%d var %s", i, table[i].name);
				fprintf(fas, "lev=%d addr=%d\n", table[i].level, table[i].adr);
				break;
			case procedur:
				printf("%d proc%s", i, table[i].name);
				printf("lev=%d addr=%d size = %d\n", table[i].level, table[i].adr, table[i].size);
				fprintf(fas, "%d proc%s", i, table[i].name);
				fprintf(fas, "lev=%d adr=%d size=%d \n", table[i].level, table[i].adr, table[i].size);
				break;
			}
		}
		printf("\n");
	}
	nxtlev[semicolon] = true;
	nxtlev[endsym] = true;
	statementdo(nxtlev, &tx, lev);
	return 0;
}

//常量声明处理
int constdeclaration(int * ptx, int lev, int * pdx)
{
	if (sym == ident)
	{
		if (position(id, *ptx) != 0) //重定义
			error(3);
		getsymdo;
		if (sym == eql || sym == becomes)//为=或:=
		{
			if (sym == becomes)//重复写赋值符号
			{
				error(11);
			}
			getsymdo;
			if (sym == number)//是常量说明，则向下
			{
				enter(constant, ptx, lev, pdx);
				getsymdo;
			}
			else
			{
				error(5); //常量声明后面不为数字，导致错误5
				getsymdo;
			}
		}
		else
		{
			error(11); /* 常量说明标识后应是 =*/
			getsymdo;
		}
	}
	else
	{
		error(8); /*const 后应是标识 */
	}
	return 0;
}
//变量定义
int vardeclaration(int * ptx, int lev, int * pdx)
{
	if (sym == ident)
	{
		if (position(id, *ptx) != 0) //重定义
			error(3);
		enter(variable, ptx, lev, pdx);// 填写名字表
		getsymdo;
	}
	else
		error(8);
	return 0;
}
//语句
int statement(bool* fsys, int * ptx, int lev)
{
	int i, cx1, cx2;
	bool nxtlev[symnum];
	if (sym == ident)
	{
		i = position(id, *ptx);
		if (i == 0)//未定义
		{
			error(1);
		}
		else if (table[i].kind != variable)
		{
			error(5);//赋值号两边的表达式类型不匹配（左部）
			i = 0;
		}
		getsymdo;
		if (sym == becomes)
		{
			getsymdo;
		}
		else
		{
			error(11);
		}
		memcpy(nxtlev, fsys, sizeof(bool)* symnum);
		expressiondo(nxtlev, ptx, lev);
	}
	else
	{
		if (sym == readsym)
		{
			getsymdo;
			if (sym != lparen)
			{
				error(8);//缺失左括号
			}
			else
			{

				getsymdo;
			}
			do {
				if (sym == ident)
				{
					i = position(id, *ptx);
					if (i == 0)
					{
						error(1);//使用变量时未定义
					}
					//else if(table[i].kind!=)
				}
				else
					error(9);//read函数的参数不为id
				getsymdo;
			} while (sym == comma); /* 一条 read 语句可读多个变量 */
			if (sym != rparen)
				error(8); //缺失右括号
			else
			{

				getsymdo;
			}
		}
		else
		{
			if (sym == writesym) /* 准备按照 write 语句处理，与 read 类似 */
			{
				getsymdo;
				if (sym != lparen) 
					error(8);//缺失左括号
				else {

					getsymdo;
				}
				do {
					memcpy(nxtlev, fsys, sizeof(bool)*symnum);
					nxtlev[rparen] = true;
					nxtlev[comma] = true; /* write 的后跟符号为） or，*/
					expressiondo(nxtlev, ptx, lev);/* 调用表达式处理，此处与 read 不同， read 为给变量赋值 */
				} while (sym == comma);
				if (sym != rparen)
					error(8);//缺失有括号
				else {

					getsymdo;
				}
			}
			else
			{
				if (sym == callsym) /* 准备按照 call 语句处理 */
				{
					getsymdo;
					if (sym != ident)
					{
						error(10); /*call 后应为标识符 */
						getsymdo;
					}
					else
					{
						i = position(id, *ptx);
						if (i == 0)
							error(2); /* 过程未找到 */
						else
						{
							if (table[i].kind != procedur)
								error(10); /*call 后标识符应为过程 */
							
						}
						getsymdo;
					}
				}
				else
				{
					if (sym == ifsym) /* 准备按照 if 语句处理 */
					{
						getsymdo;
						memcpy(nxtlev, fsys, sizeof(bool)*symnum);
						nxtlev[thensym] = true;
						nxtlev[dosym] = true; /* 后跟符号为 then 或 do*/
						conditiondo(nxtlev, ptx, lev); /*调用条件处理（逻辑运算）函数
						*/
						if (sym == thensym) {

							getsymdo;
						}
						else
						{
							error(16); /*缺少 then*/
							//getsymdo;
						}
						statementdo(fsys, ptx, lev); /* 处理 then 后的语句 */
					}
					else
					{
						if (sym == beginsym) /* 准备按照复合语句处理 */
						{
							getsymdo;
							memcpy(nxtlev, fsys, sizeof(bool)*symnum);
							nxtlev[semicolon] = true;
							nxtlev[endsym] = true;/* 后跟符号为分号或 end*/
							/* 循环调用语句处理函数， 直到下一个符号不是语句开始符号或收到 end*/
							statementdo(nxtlev, ptx, lev);
							while (inset(sym, statbegsys) || sym == semicolon)
							{
								if (sym == semicolon) 
								{

									getsymdo;
								}
								else
								{
									error(12);/* 缺少分号 */
									getsymdo;
								}
								statementdo(nxtlev, ptx, lev);
							}
							if (sym == endsym)
							{

								getsymdo;
							}
							else
							{
								error(12); /* 缺少 end 或分号 */
								getsymdo;
							}
						}
						else
						{
							if (sym == whilesym)/* 准备按照 while 语句处理 */
							{
								getsymdo;
								memcpy(nxtlev, fsys, sizeof(bool)*symnum);
								nxtlev[dosym] = true;/* 后跟符号为 do*/
								conditiondo(nxtlev, ptx, lev); /*调用条件处理 */
								if (sym == dosym)
								{
									getsymdo;
								}
								else
								{
									error(18); /* 缺少 do*/
									//return 0;
								}
								statementdo(fsys, ptx, lev); /* 循环体 */							
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
//表达式
int expression(bool*fsys, int*ptx, int lev)
{
	enum symbol addop; /* 用于保存正负号 */
	bool nxtlev[symnum];
	if (sym == plus || sym == minus) /* 开头的正负号，此时当前表达式被看作一个正的或负的项 */
	{
		addop = sym; /* 保存开头的正负号 */
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool)*symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev); /* 处理项 */
	}
	else /* 此时表达式被看作项的加减 */
	{
		memcpy(nxtlev, fsys, sizeof(bool)*symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev); /* 处理项 */
	}
	while (sym == plus || sym == minus)
	{
		addop = sym;
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool)*symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev, ptx, lev); /* 处理项 */
	}
	return 0;
}
//项
int term(bool*fsys, int *ptx, int lev)
{
	enum symbol mulop; /* 用于保存乘除法符号 */
	bool nxtlev[symnum];
	memcpy(nxtlev, fsys, sizeof(bool)*symnum);
	nxtlev[times] = true;
	nxtlev[slash] = true;
	factordo(nxtlev, ptx, lev); /* 处理因子 */
	while (sym == times || sym == slash)
	{
		mulop = sym;
		getsymdo;
		factordo(nxtlev, ptx, lev);
	}
	return 0;
}

//因子
int factor(bool*fsys, int *ptx, int lev)
{
	int i;
	bool nxtlev[symnum];
	if (sym == ident) /* 因子为常量或者变量 */
	{
		i = position(id, *ptx); /* 查找名字 */
		if (i == 0)
		{
			error(1); /* 标识符未声明 */
			//getsymdo;
			//break;
		}
		else if(table[i].kind==procedur)
			error(6); //因子为过程时，使得操作数不合法或表达式运算不合法，导致错误6：操作数类型不匹配或操作数类型与操作符不匹配
		getsymdo;
	}
	else if (sym == number) /* 因子为数*/
	{
		if (num >= 100000000000000)
		{
			error(14);
			num = 0;
		}
		getsymdo;
	}	
	else if (sym == lparen) /* 因子为表达式 */
	{
		getsymdo;
		memcpy(nxtlev, fsys, sizeof(bool)*symnum);
		nxtlev[rparen] = true;
		expressiondo(nxtlev, ptx, lev);
		if (sym == rparen)
		{
			getsymdo;
		}
		else
		{
			error(22); /* 缺少右括号 */
			getsymdo;
		}
	}
	return 0;
}
//条件
//int condition(bool* fsys, int* ptx, int lev)
//{
//	enum symbol relop;
//	bool nxtlev[symnum];
//	if (sym == oddsym) // 准备按照 odd 运算处理
//	{
//		getsymdo;
//		expressiondo(fsys, ptx, lev);
//	}
//	else
//	{
//		memcpy(nxtlev, fsys, sizeof(bool)*symnum);
//		nxtlev[eql] = true;
//		nxtlev[neq] = true;
//		nxtlev[lss] = true;
//		nxtlev[leq] = true;
//		nxtlev[gtr] = true;
//		nxtlev[geq] = true;
//		expressiondo(nxtlev, ptx, lev);
//		if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
//		{
//			error(20);
//		}
//		else
//		{
//			relop = sym;
//			getsymdo;
//			expressiondo(fsys, ptx, lev);
//		}
//	}
//	return 0;
//}

#pragma endregion

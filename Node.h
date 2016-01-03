/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 10:53:55
 *
 **/

#ifndef NODE_H
#define NODE_H
#include<fstream>
#include<string>
#include"Token.h"
#include"SymbolT.h"
using namespace std;

//语法树节点基类
class Node
{
public:
	Node();
	~Node();
	//生成语法树
	void createSTree(int depth, ofstream *file);
	//用于生成语法树的缩进表示
	static void printSpace(int depth, ofstream *file)
	{
		for (int i = 0;i < depth; i++)
		{
			*file << ' ';
		}
	}	
};

//表达式节点
class Expr : public Node
{
public:
	 Expr();
	~ Expr();
	//表达式规约，将表达式规约为一个值
	virtual Expr* reduce(SymbolT sbt,ofstream *file);
	void createSTree(int depth, ofstream *file);
	//返回表达式
	virtual Expr* genC(SymbolT sbt,ofstream *file);
	virtual string toString();
};

//标识符
class Id : public Expr
{
public:
	Word *variable;

	Expr* genC(SymbolT sbt, ofstream *file);
	Expr* reduce(SymbolT sbt,ofstream *file);

public:
	Id(Word *variable);
	void createSTree(int depth, ofstream *file);
	~Id();
	
	string toString();
	void createSTree(int depth, ofstream *file);
};

//逻辑表达式
class Logical : public Expr
{
private:
	Operator *oper;
	Expr *left,*right;

public:
	Expr* genC(SymbolT sbt, ofstream *file);
	Expr* reduce(SymbolT sbt,ofstream *file);

public:
	Logical(Operator *oper,Expr *left,Expr *right);
	void createSTree(int depth, ofstream *file);
	~Logical();
	
	string toString();
	void createSTree(int depth, ofstream *file);
};


//临时变量
class Temp : public Expr
{
private:
	int index;

protected:
	Expr* reduce(SymbolT sbt,ofstream *file);
	Expr* genC(SymbolT sbt,ofstream *file);

public:
	Temp(int index);
	~Temp();
	
	void createSTree(int depth,ofstream *file);
	string toString();	
};

//整数常量
class Constant : public Expr
{
private: 
	Int *mInt;

protected:
	Expr* reduce(SymbolT sbt,ofstream *file);
	Expr* genC(SymbolT sbt,ofstream *file);

public:
	Constant(Int *token);
	void createSTree(int depth, ofstream *file);
	~Constant();
	
	string toString();
	void createSTree(int depth, ofstream *file);
};


//运算表达式 包括addop 和 mulop
class Operation : public Expr
{
public:
	Operation();
	~Operation();

protected:	
	Expr* reduce(SymbolT sbt, ofstream *file);
};

//双目运算
class Arith : public Operation
{
private:
	Operator *op;
	Expr *left,*right;

protected:
	Expr* genC(SymbolT sbt,ofstream *file);

public:
	Arith(Operator *op,Expr *left,Expr *right);
	void createSTree(int depth, ofstream *file);
	~Arith();
	
	string toString();
	void createSTree(int depth, ofstream *file);
};

//单目运算
class Unary : public Operation
{
private:
	Operator* op;
	Expr* left;

protected:
	Expr* genC(SymbolT sbt,ofstream *file);

public:
	Unary(Operator *op,Expr *expr);
	void createSTree(int depth, ofstream *file);
	~Unary();
	
	string toString();
	void createSTree(int depth,ofstream *file);
};
//语句节点
class Stmt : public Node
{
public:
	Stmt();
	~Stmt();
	//生成C代码
	virtual void genC(SymbolT sbt, ofstream *file);
};

//连续的语句序列节点
class Seq : public Stmt
{
private:
	Stmt *left,*right;
public:
	Seq(Stmt *left,Stmt *right);
	void createSTree(int depth, ofstream *file);
	~Seq();

	void createSTree(int depth, ofstream *file);
	void genC(SymbolT sbt,ofstream *file);
};

//空节点
class Nop : public Stmt
{
private:
	Nop();
	~Nop();
	
	const static Nop NOP;
};

//IF语句节点
class If : public Stmt
{
private: 
	Logical *logical;
	Seq  *thenStmt;
public:
	If(Logical *logicalExpr,Seq *thenStmt);
	void createSTree(int depth, ofstream *file);
	~If();
	
	void createSTree(int depth, ofstream *file);
	void genC(SymbolT sbt,ofstream *file);
};

//IF-ELSE语句节点
class IfElse :public Stmt
{
private:
	Logical *logical;
	Seq *thenStmt,*elseStmt;

public:
	IfElse(Logical *logicalExpr,Seq *thenStmt,Seq *elseStmt);
	~IfElse();
	
	void createSTree(int depth, ofstream *file);
	void genC(SymbolT sbt,ofstream *file);
};

//Repeat语句节点
class Repeat : public Stme
{
private:
	Logical *logical;
	Seq *stmt;
public:
	Repeat(Logical *logical, Seq *stmt);
	~Repeat();

	void createSTree(int depth, ofstream *file);
	void genC(SymbolT sbt, ofstream * file);
};

class Read : public Stmt 	
{
private:
	Id *id;
public:
	Read(Id *id);
	~Read();

	void createSTree(int depth, ofstream *file);
	void genC(SymbolT sbt,ofstream *file);	
};

class Write : public Stmt
{
private:
	Expr *expr;
public:
	Write(Expr *expr);
	~Write();

	void createSTree(int depth, ofstream *file);
	void genC(SymbolT sbt,ofstream *file);	
};

class Assign : public Stmt
{
private:
	Id *left;
	Expr *expr;
public:
	Assign(Id *left,Expr *expr);
	~Assign();
	
	void createSTree(int depth, ofstream *file);
	void genC(SymbolT sbt,ofstream *file);	
};

class Program :public Node
{
private:
	SymbolT *sbt;
	Seq *seq;
public:
	Program(SymbolT *sbt,Seq *seq);
	~Program();
	
	void createSTree(int depth,ofstream *file);
	void genC(ofstream *file);
};
#endif
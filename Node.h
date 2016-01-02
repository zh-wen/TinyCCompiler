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
using namespace std;

//语法树节点基类
class Node
{
public:
	Node();
	~Node();
	//生成语法树
	void creatSTree(int depth, fstream file);

protected:
	//用于生成语法树的缩进表示
	static void printSpace(int depth, fstream file)
	{
		for (int i = 0;i < depth; i++)
		{
			file << ' ';
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
	virtual Expr reduce(SymbolT sbt,fstream file);

	//返回表达式
	virtual Expr genC(SymbolT sbt,fstream file);
};

//标识符
class Id : public Expr
{
protected:
	Word variable;

	void genC(SymbolT sbt,fstream file)
	Expr reduce(SymbolT sbt,fstream file);

public:
	Id(Word variable);
	~Id();
	
	string toString();
	void creatSTree(int depth, fstream file);
};

//逻辑表达式
class Logical : public Expr
{
private:
	Operator oper;
	Expr left,right;

protected:
	void genC(SymbolT sbt,fstream file)
	Expr reduce(SymbolT sbt,fstream file);

public:
	Logical(Operator oper,Expr left,Expr right);
	~Logical();
	
	string toString();
	void creatSTree(int depth, fstream file);
};


//临时变量
class Temp : public Expr
{
private:
	int index;

protected:
	Expr reduce(SymbolT sbt,fstream file);
	Expr genC(SymbolT sbt,fstream file);

public:
	Temp(int index);
	~Temp();
	
	void creatSTree(int depth,fstream file);
	string toString();	
};

//整数常量
class Constant : public Expr
{
private: 
	int mInt;

protected:
	Expr reduce(SymbolT sbt,fstream file);
	Expr genC(SymbolT sbt,fstream file);

public:
	Constant(int token);
	~Constant();
	
	string toString();
	void creatSTree(int depth, fstream file);
};


//运算表达式 包括addop 和 mulop
class Operation : public Expr
{
public:
	Operation();
	~Operation();

protected:	
	Expr reduce(SymbolT sbt, fstream file);
};

//双目运算
class Arith : public Operation
{
private:
	Operator op;
	Expr left,right;

protected:
	Expr genC(SymbolT sbt,fstream file);

public:
	Arith(Operator op,Expr left,Expr right);
	~Arith();
	
	string toString();
	void creatSTree(int depth, fstream file);
};

//单目运算
class Unary : public Operation
{
private:
	Operator op;
	Expr left;

protected:
	Expr genC(SymbolT sbt,fstream file);

public:
	Unary(Operator op,Expr expr);
	~Unary();
	
	string toString();
	void creatSTree(int depth,fstream file);
};
//语句节点
class Stmt : public Node
{
public:
	Stmt();
	~Stmt();
	//生成C代码
	virtual void genC(SymbolT sbt,fstream file)
};

//连续的语句序列节点
class Seq : public Stmt
{
private:
	Stmt left,right;
public:
	Seq(Stmt left,Stmt right);
	~Seq();

	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);
};

//空节点
class Nop : public Stmt
{
private:
	Nop();
	~Nop();
	
	static Nop NOP = new Nop();
};

//IF语句节点
class If : public Stmt
{
private: 
	Logical logical;
	Seq  thenStmt;
public:
	If(Logical logicalExpr,Seq thenStmt);
	~If();
	
	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);
};

//IF-ELSE语句节点
class IfElse :public Stmt
{
private:
	Logical logical;
	Seq thenStmt,elseStmt;

public:
	IfElse(Logical logicalExpr,Seq thenStmt,Seq elseStmt);
	~IfElse();
	
	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);
};

//Repeat语句节点
class Repeat : public Stme
{
private:
	Logical logical;
	Seq stmt;
public:
	Repeat(Logical logical, Seq stmt);
	~Repeat();

	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);
};

class Read : public Stmt 	
{
private:
	Id id;
public:
	Read(Id id);
	~Read();

	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);	
};

class Write : public Stmt
{
private:
	Expr expr;
public:
	Write(Expr expr);
	~Write();

	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);	
};

class Assign : public Stmt
{
private:
	Id left;
	Expr expr;
public:
	Assign(Id left,Expr expr);
	~Assign();
	
	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);	
};

#endif
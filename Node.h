/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 10:53:55
 *
 **/

#ifndef NODE.H
#define NODE.H
#include<fstream>
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
	static void print Space(int depth, fstream file)
	{
		for (int i = 0;i < depth; i++)
		{
			out.print(' ');
		}
	}	
};

//语句节点
class Stmt : public Node
{
public:
	 Stmt();
	~ Stmt();
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
	~ IfElse();
	
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
	~ Repeat();
	void creatSTree(int depth, fstream file);
	void genC(SymbolT sbt,fstream file);
};
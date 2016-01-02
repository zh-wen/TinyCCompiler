/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 10:55:14
 *
 **/
#include'Node.h'

Seq::Seq(Stmt left,Stmt right)
{
	this.left = left ;
	this.right = left ;
}

void Seq::createSTree(int depth,fstream file)
{
	left.createSTree(depth, file);
	right.createSTree(depth, file);
}

void Seq::genC(SymbleT sbt, fstream file)
{
	left.genC(sbt,file);
	right.genC(sbt,file);
}

IF::IF(Logical logicExpr, Seq thenStmt)
{
	this.logical = logicExpr;
	this.thenStmt = thenStmt;
}
void IF::createSTree(int depth, fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "if" << endl;
	logical.createSTree(subDepth,file);
	printSpace(depth,file);
	file << "then" << endl;
	thenStmt.createSTree(subDepth,file);
	printSpace(depth,file);
	file << "end" << endl;
}

void IF::genC(SymbleT sbt, fstream file)
{
	Expr logicExpr = logical.genC(sbt,file);
	file << "if(" << log.toString() << "){" << endl;
	thenStmt.genC(sbt,file);
	file << "}" << endl;
}

IfElse::IfElse(Logical logicExpr,Seq thenStmt,Seq elseStmt)
{
	this.logical = logicExpr;
	this.thenStmt = thenStmt;
	this.elseStmt = elseStmt;
}

void IfElse::createSTree(int depth,fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "if" << endl;
	logical.createSTree(subDepth,file);
	printSpace(depth);
	file << "then" << endl;
	thenStmt.createSTree(subDepth,file);
	printSpace(depth,file);
	file << "else" << endl;
	elseStmt.createSTree(subDepth,file);
	printSpace(depth,file);
	file << "end" << endl;
}

void IfElse::genC(int depth,fstream file)
{
	Expr logicExpr = logical.gen(sbt,file);
	file << "if(" << logicalExpr.toString() << "){" << endl;
	thenStmt.gen(sbt,file);
	file << "}" << endl "else" << endl << "{" << endl;
	elseStmt.genC(sbt,file);
	file << "}" << endl;
}
/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 10:55:14
 *
 **/
#include"Node.h"

Id::Id(Word word)
{
	variable = word;
}

void Id::createSTree(int depth, fstream file)
{
	printSpace(depth,file);
	file << "id:" << variable.getLexeme() << endl;
}

string Id::toString()
{
	return variable.getLexeme();
}

Expr Id::reduce(SymbleT sbt,fstream file)
{
	return this;
}

Expr Id::genC(SymbleT sbt, fstream file)
{
	return this;
}

Logical::Logical(Operator op, Expr left, Expr right)
{
	this.oper = op;
	this.left = left;
	this.right = right;
}

void Logical::createSTree(int depth, fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "op:" << oper.getOperator() << endl;
	left.createSTree(subDepth,file);
	right.createSTree(subDepth,file);
}

string Logical::toString()
{
	return string(left.toString() + " " + oper.toString() + " " + right.toString()); 
}

Expr Logical::reduce(SymbleT sbt,fstream file)
{
	this;
}

Expr Logical::genC(SymbleT sbt,fstream file)
{
	return new Logical(oper,left.reduce(sbt,file),right.reduce(sbt,file));
}

Temp::Temp(int index)
{
	this.index = index;
}

string Temp::toString()
{
	return "tmp_" + index;
}

Expr Temp::reduce(SymbleT sbt,fstream file)
{
	return this;
}

Expr Temp::genC(SymbleT sbt,fstream file)
{
	return this;
}

Constant::Constant(int token)
{
	this.mInt = token; 
}

void Constant::createSTree(int depth,fstream file)
{
	printSpace(depth,file);
	file << "const:" <<mInt.getValue() << endl;
}

string Constant::toString()
{
	return string(itoa(mInt.getValue()));
} 

Expr Constant::reduce(SymbleT sbt,fstream file)
{
	return this;
}

Expr Constant::genC(SymbleT sbt,fstream file)
{
	return this;
}

Expr Operation::reduce(SymbleT sbt, fstream file)
{
	Expr expr = genc(sbt,out);
	sbt.tmpPush();
	Temp temp = sbt.obtainTmp();
	file << temp.toString() << " = " << expr.toString() << endl;
	sbt.tmpPop();
	return temp;
}

Arith::Arith(Operator op,Expr left,Expr right)
{
	this.op = op;
	this.left = left;
	this.right = right;
}

void Arith::createSTree(int depth,fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "op:" << op.getOperator() << endl;
	left.createSTree(subDepth,file);
	right.createSTree(subDepth,file);
}

string Arith::toString()
{
	return string(left.toString + " " + op.toString + " "
		right.toString());
}

Expr Arith::genC(SymbleT sbt,fstream file)
{
	return new Arith(op,left.reduce(sbt,file),right.reduce(sbt,file));
}

Unary::Unary(Operator op,Expr expr)
{
	this.op = op;
	this.left = expr;
}

void Unary::createSTree(int depth,fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "op:" << op.getOperator() << endl;
	left.createSTree(subDepth, file);
}

string Unary::toString()
{
	return string(op.getOperator() + left.toString());
}

Expr Unary::genC(SymbleT sbt,fstream file)
{
	retrun new Unary(op,left.reduce(sbt,file));
}

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

Repeat::Reapeat(Logical logicalExpr,Seq stmt)
{
	this.logical = logicalExpr;
	this.stmt = stmt;
}

void Repeat::createSTree(int depth,fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "repeat" << endl;
	stmt.createSTree(subDepth,file);
	printSpace(depth,file);
	file << "until" << endl;
	logical.createSTree(subDepth,file);
}

void Repeat::genC(SymbleT sbt, fstream file)
{
	file << "do {" << endl;
	stmt.genC(sbt,file);
	Expr logicExpr = logical.genC(sbt,file);
	file << "} while( !(" << logicExpr.toString() << "));" << endl;
}

Read::Read(Id id)
{
	this.id = id;
}

void Read::createSTree(int depth,fstream file)
{
	printSpace(depth,file);
	file << "read:" << id.variable.getLexeme() << endl;
}

void Read::genC(SymbleT sbt, fstream file)
{
	file << "scanf(\"%\",&" id.variable.getLexeme() << ");" << endl;
}

Write::Write(Expr expr)
{
	this.expr = expr;
}

void Write::createSTree(int depth,fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "write" << endl;
	expr.createSTree(subDepth,file);
}

void Write::genC(SymbleT sbt, fstream file)
{
	Expr exprTemp = expr.gen(sbt,file).reduce(sbt,file);
	file << "printf(\"%d\", ") << exprTemp.toString() << ");" << endl;
}

Assign::Assign(Id left,Expr expr)
{
	this.left = left;
	this.expr = expr;
}

void Assign::createSTree(int depth,fstream file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	file << "assign to:" << left.variable.getLexeme() << endl;
	expr.createSTree(subDepth, file);
}

void Assign::genC(SymbleT sbt,fstream file)
{
	file << left.variable.getLexeme() << " = " << expr.genC(sbt,file).toString << endl;
}

Program::Program(SymbleT sbt,Seq seq)
{
	this.sbt = sbt;
	this.seq = seq;
}

void Program::createSTree(int depth,fstream file)
{
	seq.createSTree(depth,file);
}

void Program::genC(fstream file)
{
	seq.genC(sbt,file);
	out << "return 0;" << endl << "}" ;
	//输出变量
	//out << sbt.toString();


}
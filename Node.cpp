/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 10:55:14
 *
 **/
#include"Node.h"
#include"SymbolT.h"
#include"SymbolT.h"

Id::Id(Word *word)
{
	variable = word;
}

void Id::createSTree(int depth, ofstream *file)
{
	Node::printSpace(depth,file);
	*file << "id:" << variable->getLexeme() << endl;
}

string Id::toString()
{
	return variable->getLexeme();
}

Expr* Id::reduce(SymbolT sbt,ofstream *file)
{
	return this;
}

Expr* Id::genC(SymbolT sbt, ofstream *file)
{
	return this;
}

Logical::Logical(Operator *op, Expr *left, Expr *right)
{
	this->oper = op;
	this->left = left;
	this->right = right;
}

void Logical::createSTree(int depth, ofstream *file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	*file << "op:" << oper->getMOperator() << endl;
	left->createSTree(subDepth,file);
	right->createSTree(subDepth,file);
}

string Logical::toString()
{
	return string(left->toString() + " " + oper->toString() + " " + right->toString()); 
}

Expr* Logical::reduce(SymbolT sbt,ofstream *file)
{
	return this;
}

Expr* Logical::genC(SymbolT sbt,ofstream *file)
{
	return new Logical(oper,left->reduce(sbt,file),right->reduce(sbt,file));
}

Temp::Temp(int index)
{
	this->index = index;
}

void Temp::createSTree(int depth, ofstream * file)
{
}

string Temp::toString()
{
	return "tmp_" + index;
}

Expr* Temp::reduce(SymbolT sbt,ofstream *file)
{
	return this;
}

Expr* Temp::genC(SymbolT sbt,ofstream *file)
{
	return this;
}

Constant::Constant(Int *token)
{
	this->mInt = token; 
}

void Constant::createSTree(int depth,ofstream *file)
{
	printSpace(depth,file);
	*file << "const:" << mInt->getValue() << endl;
}

string Constant::toString()
{
	char tmp[20];
	_itoa_s(mInt->getValue(), tmp, 10);
	return string(tmp);
} 

Expr* Constant::reduce(SymbolT sbt,ofstream *file)
{
	return this;
}

Expr* Constant::genC(SymbolT sbt,ofstream *file)
{
	return this;
}

Operation::Operation()
{
}

Expr* Operation::reduce(SymbolT sbt, ofstream *file)
{
	Expr* expr = genC(sbt,file);
	sbt.tmpPush();
	Temp *temp = sbt.obtainTmp();
	*file << temp->toString() << " = " << expr->toString() << endl;
	sbt.tmpPop();
	return temp;
}

Arith::Arith(Operator *op,Expr *left,Expr *right)
{
	this->op = op;
	this->left = left;
	this->right = right;
}

void Arith::createSTree(int depth,ofstream *file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	*file << "op:" << op->getMOperator() << endl;
	left->createSTree(subDepth,file);
	right->createSTree(subDepth,file);
}

string Arith::toString()
{
	return string(left->toString() + " " + op->toString() + " " +
		right->toString());
}

Expr* Arith::genC(SymbolT sbt,ofstream *file)
{
	return new Arith(op,left->reduce(sbt,file),right->reduce(sbt,file));
}

Unary::Unary(Operator *op,Expr *expr)
{
	this->op = op;
	this->left = expr;
}

void Unary::createSTree(int depth,ofstream *file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	*file << "op:" << op->getMOperator() << endl;
	left->createSTree(subDepth, file);
}

string Unary::toString()
{
	return string(op->getMOperator() + left->toString());
}

Expr* Unary::genC(SymbolT sbt,ofstream *file)
{
	return new Unary(op,left->reduce(sbt,file));
}

Seq::Seq(Stmt *left,Stmt *right)
{
	this->left = left ;
	this->right = left ;
}

void Seq::createSTree(int depth,ofstream *file)
{
	left->createSTree(depth, file);
	right->createSTree(depth, file);
}

void Seq::genC(SymbolT sbt, ofstream *file)
{
	left->genC(sbt,file);
	right->genC(sbt,file);
}

Nop Nop::NOP = Nop();

If::If(Logical *logicExpr, Seq *thenStmt)
{
	this->logical = logicExpr;
	this->thenStmt = thenStmt;
}

void If::createSTree(int depth, ofstream *file)
{
	int subDepth = depth + 2;
	Node::printSpace(depth,file);
	*file << "if" << endl;
	logical->createSTree(subDepth,file);
	Node::printSpace(depth,file);
	*file << "then" << endl;
	thenStmt->createSTree(subDepth,file);
	printSpace(depth,file);
	*file << "end" << endl;
}

void If::genC(SymbolT sbt, ofstream *file)
{
	Expr *logicExpr = logical->genC(sbt,file);
	*file << "if(" << logicExpr->toString() << "){" << endl;
	thenStmt->genC(sbt,file);
	*file << "}" << endl;
}

IfElse::IfElse(Logical *logicExpr,Seq *thenStmt,Seq *elseStmt)
{
	this->logical = logicExpr;
	this->thenStmt = thenStmt;
	this->elseStmt = elseStmt;
}

void IfElse::createSTree(int depth,ofstream *file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	*file << "if" << endl;
	logical->createSTree(subDepth,file);
	printSpace(depth,file);
	*file << "then" << endl;
	thenStmt->createSTree(subDepth,file);
	printSpace(depth,file);
	*file << "else" << endl;
	elseStmt->createSTree(subDepth,file);
	printSpace(depth,file);
	*file << "end" << endl;
}

void IfElse::genC(SymbolT sbt,ofstream *file)
{
	Expr* logicExpr = logical->genC(sbt,file);
	*file << "if(" << logicExpr->toString() << "){" << endl;
	thenStmt->genC(sbt,file);
	*file << "}" << endl << "else" << endl << "{" << endl;
	elseStmt->genC(sbt,file);
	*file << "}" << endl;
}

Repeat::Repeat(Logical *logicalExpr,Seq *stmt)
{
	this->logical = logicalExpr;
	this->stmt = stmt;
}

void Repeat::createSTree(int depth,ofstream *file)
{
	int subDepth = depth + 2;
	Node::printSpace(depth,file);
	*file << "repeat" << endl;
	stmt->createSTree(subDepth,file);
	Node::printSpace(depth,file);
	*file << "until" << endl;
	logical->createSTree(subDepth,file);
}

void Repeat::genC(SymbolT sbt, ofstream *file)
{
	*file << "do {" << endl;
	stmt->genC(sbt,file);
	Expr* logicExpr = logical->genC(sbt,file);
	*file << "} while( !(" << logicExpr->toString() << "));" << endl;
}

Read::Read(Id *id)
{
	this->id = id;
}

void Read::createSTree(int depth,ofstream *file)
{
	Node::printSpace(depth,file);
	*file << "read:" << id->variable->getLexeme() << endl;
}

void Read::genC(SymbolT sbt, ofstream *file)
{
	*file << "scanf(\"%\",&"  << id->variable->getLexeme() << ");" << endl;
}

Write::Write(Expr *expr)
{
	this->expr = expr;
}

void Write::createSTree(int depth,ofstream *file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	*file << "write" << endl;
	expr->createSTree(subDepth,file);
}

void Write::genC(SymbolT sbt, ofstream *file)
{
	Expr* exprTemp = expr->genC(sbt,file)->reduce(sbt,file);
	*file << "printf(\"%d\", " << exprTemp->toString() << ");" << endl;
}

Assign::Assign(Id *left,Expr *expr)
{
	this->left = left;
	this->expr = expr;
}

void Assign::createSTree(int depth,ofstream *file)
{
	int subDepth = depth + 2;
	printSpace(depth,file);
	*file << "assign to:" << left->variable->getLexeme() << endl;
	expr->createSTree(subDepth, file);
}

void Assign::genC(SymbolT sbt,ofstream *file)
{
	*file << left->variable->getLexeme() << " = " << expr->genC(sbt,file)->toString() << endl;
}

Program::Program(SymbolT *sbt,Seq *seq)
{
	this->sbt = sbt;
	this->seq = seq;
}

void Program::createSTree(int depth,ofstream *file)
{
	seq->createSTree(depth,file);
}

void Program::genC(ofstream *file)
{
	seq->genC(*sbt,file);
	*file << "return 0;" << endl << "}" ;
	//输出变量
	//out << sbt.toString();


}

Stmt::Stmt()
{
}

Node::Node()
{
}

Expr::Expr()
{
}

Nop::Nop()
{
}

void Nop::createSTree(int depth, ofstream * file)
{
}

void Nop::genC(SymbolT sbt, ofstream * file)
{
}

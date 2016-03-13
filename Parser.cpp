/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-03 18:37:55
 *
 **/

#include"Parser.h"

Parser::Parser(Lexer *lexer)
{
	this->lexer = lexer;
}

Program* Parser::parse()
{
	Seq *sequence = NULL;
	if (lexer->nextToken(tokener))
	{
		sequence = seq();
		if (!isEOF())
		{
			cout << "Óï·¨·ÖÎö´íÎó:(";
			cout << tokener.getLine() << ",";
			cout << tokener.getRow() << ")" << "´íÎóµÄ·ûºÅ";
			cout << tokener.getToken()->toString() << endl;

			exit(1);
		}
	}
	else
	{	
		Stmt *nop = &(Nop::NOP);
		sequence = new Seq(nop,nop);
	}
	return new Program(&sbt, sequence);
}

Seq* Parser::seq()
{
	checkEOF();

	Stmt *left = statement();
	bool iseof = isEOF();
	cout << iseof;
	if (isEOF() || !match(';'))
	{
		return new Seq(left, &(Nop::NOP));
	}

	return new Seq(left, seq());
}

Stmt* Parser::statement()
{
	Stmt *stmt = NULL;
	checkEOF();
	switch (tokener.getToken()->getTag())
	{
		case Token::TAG_IF:
			stmt = if_stmt();
			break;
		case Token::TAG_REPEAT:
			stmt = repeat_stmt();
			break;
		case Token::TAG_READ:
			stmt = read_stmt();
			break;
		case Token::TAG_WRITE:
			stmt = write_stmt();
			break;
		case Token::TAG_VARIABLE:
			stmt = assign_stmt();
			break;
		default:
			cout << "Óï·¨·ÖÎö´íÎó:(";
			cout << tokener.getLine() << ",";
			cout << tokener.getRow() << ")" << "²»ÄÜÊ¶±ðµÄ·ûºÅ";
			cout << tokener.getToken()->toString() << endl;
			exit(1);
	}

	return stmt;
}


Stmt* Parser::if_stmt()
{
	if (!match(Token::TAG_IF))
	{
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "ERROR";
		cout << tokener.getToken()->toString() << endl;
		exit(1);;
	}

	Logical *logicalExpr = rel();
	if (!match(Token::TAG_THEN))
	{
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "È±ÉÙthen";
		cout << tokener.getToken()->toString() << endl;
		exit(1);
	}

	Seq *thenStmt = seq();
	Seq *elseStmt = NULL;

	if (match(Token::TAG_ELSE))
	{
		elseStmt = seq();
	}

	if (!match(Token::TAG_END))
	{
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "È±ÉÙend";
		cout << tokener.getToken()->toString() << endl;
		exit(1);;
	}

	Stmt *stmt = NULL;

	if (elseStmt == NULL)
	{
		stmt = new If(logicalExpr, thenStmt);
	}
	else
	{
		stmt = new IfElse(logicalExpr, thenStmt, elseStmt);
	}

	return stmt;
}

Stmt* Parser::repeat_stmt()
{
	next();

	Seq *stmt = seq();

	if (!match(Token::TAG_UNTIL))
	{
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "È±ÉÙuntil";
		cout << tokener.getToken()->toString() << endl;
		exit(1);
	}

	Logical *logicalExpr = rel();

	return new Repeat(logicalExpr, stmt);
}


Stmt* Parser::read_stmt()
{
	next();

	Stmt *stmt = NULL;
	Id *id = NULL;
	Word *word = NULL;
	switch (tokener.getToken()->getTag())
	{
	case Token::TAG_VARIABLE:
		word = (Word*)(tokener.getToken());
		id = new Id(word);
		id = sbt.putVariable(word,id);

		stmt = new Read(id);

		next();
		break;
	default:
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "È±ÉÙ±äÁ¿";
		cout << tokener.getToken()->toString() << endl;
		exit(1);
	}

	return stmt;
}

Stmt* Parser::write_stmt()
{
	next();

	Expr *expression = expr();

	return new Write(expression);
}

Stmt* Parser::assign_stmt()
{
	Word *curWord =  (Word*) tokener.getToken();

	next();
	
	if (!match(Token::TAG_ASSIGN))
	{
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "È±ÉÙ ':='";
		cout << tokener.getToken()->toString() << endl;
		exit(1);
	}

	Expr *expression = expr();
	Id *left = sbt.putVariable(curWord, new Id(curWord));

	return new Assign(left, expression);
}

Logical* Parser::rel()
{
	checkEOF();
	
	Expr *left = expr();
	checkEOF();
	Operator *oper;
	switch (tokener.getToken()->getTag())
	{
	case Token::TAG_EQUAL:
		oper = &(Operator::OP_EQUAL);
		break;
	case Token::TAG_LESS:
		oper = &(Operator::OP_LESS);
		break;
	default:
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "¹ØÏµ±í´ïÊ½´íÎó" << endl;
		exit(1);
	}
	next();
	Expr *right = expr();
	checkEOF();

	return new Logical(oper,left,right);
}

Expr* Parser::expr()
{
	checkEOF();

	Expr *left = term();
	if (isEOF())
	{
		return left;
	}

	bool loopMark = true;
	while (loopMark)
	{
		Operator *oper = NULL;
		switch (tokener.getToken()->getTag())
		{
		case Token::TAG_PLUS:
			oper = &(Operator::OP_PLUS);
			break;
		case Token::TAG_MINUS:
			oper = &(Operator::OP_MINUS);
			break;
		default:
			loopMark = false;
			break;
		}
		if (loopMark)
		{
			next();
			left = new Arith(oper, left, term());
		}
	}

	return left;
}

Expr* Parser::term()
{
	checkEOF();
	Expr *left = unary();
	if (isEOF())
	{
		return left;
	}

	bool loopMark = true;
	while (loopMark)
	{
		Operator *oper = NULL;
		switch (tokener.getToken()->getTag())
		{
		case Token::TAG_MUTL:
			oper = &(Operator::OP_MUTL);
			break;
		case Token::TAG_DIV:
			oper = &(Operator::OP_DIV);
			break;
		default:
			loopMark = false;
			break;
		}
		if (loopMark)
		{
			next();
			left = new Arith(oper, left, unary());
		}
	}
	return left;
}

Expr* Parser::unary()
{
	checkEOF();

	switch (tokener.getToken()->getTag())
	{
	case '-':
		next();
		return new Unary(&Operator::OP_MINUS, unary());
	default:
		return factor();
	}
}

Expr* Parser::factor()
{
	Expr *expression = NULL;
	checkEOF();

	switch (tokener.getToken()->getTag()) {
	case '(':
		next();
		expression = expr();
		if (!match(')')) {
			cout << "Óï·¨·ÖÎö´íÎó:(";
			cout << tokener.getLine() << ",";
			cout << tokener.getRow() << ")" << "È±ÉÙ ')'" << endl;
			exit(1);
		}
		break;
	case Token::TAG_INT:
		expression = new Constant((Int*) (tokener.getToken()));
		if (!match(Token::TAG_INT)) {
			cout << "Óï·¨·ÖÎö´íÎó:(";
			cout << tokener.getLine() << ",";
			cout << tokener.getRow() << ")" << "ERROR" << endl;
			exit(1);
		}
		break;
	case Token::TAG_VARIABLE:
		expression = sbt.getVariable((Word*) (tokener.getToken()));
		if (expression == NULL) {
			cout << "Óï·¨·ÖÎö´íÎó:(";
			cout << tokener.getLine() << ",";
			cout << tokener.getRow() << ")" << "´íÎó·ûºÅ ";
			cout << tokener.getToken()->toString() << endl;
			exit(1);
		}
		if (!match(Token::TAG_VARIABLE)) {
			cout << "Óï·¨·ÖÎö´íÎó:("; 
			cout << tokener.getLine() << ",";
			cout << tokener.getRow() << ")" << "ERROR" << endl;
			exit(1);
		}
		break;
	default:
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "´íÎó·ûºÅ ";
		cout << tokener.getToken()->toString() << endl;
		exit(1);
	}
	return expression;
}

bool Parser::match(int tag)
{
	checkEOF();

	if (tokener.getToken()->getTag() == tag)
	{
		lexer->nextToken(tokener);
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::isEOF() {
	return tokener.getToken()->getTag() == -1;
}

void Parser::checkEOF()
{
	if (isEOF())
	{
		cout << "Óï·¨·ÖÎö´íÎó:(";
		cout << tokener.getLine() << ",";
		cout << tokener.getRow() << ")" << "ERROR" << endl;
		exit(1);
	}
}

void Parser::next()
{
	lexer->nextToken(tokener);
	checkEOF();
}
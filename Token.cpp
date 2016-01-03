/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 17:03:21
 *
 **/

#include"Token.h"

 Token::Token(int tag)
 {
 	this->tag = tag;
 }

 int Token::getTag()
 {
 	return tag;
 }

 bool Token::isReservedWord(Token token)
 {
 	return (token.tag >= TAG_IF) && (token.tag <= TAG_WRITE);
 }

 string Token::toString()
 {
	string str = string("symbol:") + (char)this->tag;
	return str;
 }

Int::Int(int value):Token(TAG_INT)
{
	this->value = value;
}

int Int::getValue()
{
	return this->value;
}

string Int::toString()
{
	char temp[20];
	_itoa_s(this->tag, temp, 10);
	string str= string("integer:") + string(temp);
	return str;
}

Operator::Operator(int tag,string mOp,string cOp):Token(tag)
{
	this->mOp = mOp;
	this->cOp = cOp;
}

string Operator::getMOperator()
{
	return this->mOp;
}

string Operator::getCOperator()
{
	return this->cOp;
}

string Operator::toString()
{
	return string("operator:" + this->mOp);
}

const Operator Operator:: OP_ASSIGN =  Operator(TAG_ASSIGN, ":=", "=");
const Operator Operator:: OP_PLUS =  Operator(TAG_PLUS, "+", "+");
const Operator Operator:: OP_MINUS =  Operator(TAG_MINUS, "-", "-");
const Operator Operator:: OP_MUTL =  Operator(TAG_MUTL, "*", "*");
const Operator Operator:: OP_DIV =  Operator(TAG_DIV, "/", "/");
const Operator Operator:: OP_EQUAL =  Operator(TAG_EQUAL, "=", "==");
const Operator Operator:: OP_LESS =  Operator(TAG_LESS, "<", "<");


Word::Word(int tag, string lexeme) :Token(tag)
{
	this->lexeme = lexeme;
	WORDS_RESERVED.insert(pair<string,Word>(WORD_IF.lexeme, WORD_IF));
	WORDS_RESERVED.insert(pair<string,Word>(WORD_THEN.lexeme, WORD_THEN));
	WORDS_RESERVED.insert(pair<string,Word>(WORD_ELSE.lexeme, WORD_ELSE));
	WORDS_RESERVED.insert(pair<string,Word>(WORD_END.lexeme, WORD_END));
	WORDS_RESERVED.insert(pair<string,Word>(WORD_REPEAT.lexeme, WORD_REPEAT));
	WORDS_RESERVED.insert(pair<string,Word>(WORD_UNTIL.lexeme, WORD_UNTIL));
	WORDS_RESERVED.insert(pair<string,Word>(WORD_READ.lexeme, WORD_READ));
	WORDS_RESERVED.insert(pair<string,Word>(WORD_WRITE.lexeme, WORD_WRITE));
}

Word* Word::getReserve(string lexeme)
{
	map<string, Word>::iterator iter;
	iter = WORDS_RESERVED.find(lexeme);
	if (iter == WORDS_RESERVED.end())
	{
		return NULL;
	}
	else
	{
		return &(iter->second);
	}
}

string Word::getLexeme()
{
	return lexeme;
}

string Word::toString()
{
	string temp;
	if(isReservedWord(*this))
	{
		temp.append("reserved word:");
	}
	else
	{
		temp.append("ID,name=");
	}
	temp.append(lexeme);
	return temp;
}

const Word Word:: WORD_IF = Word(TAG_IF, "if");
const Word Word:: WORD_THEN = Word(TAG_THEN, "then");
const Word Word:: WORD_ELSE = Word(TAG_ELSE, "else");
const Word Word:: WORD_END = Word(TAG_END, "end");
const Word Word:: WORD_REPEAT = Word(TAG_REPEAT, "repeat");
const Word Word:: WORD_UNTIL = Word(TAG_UNTIL, "until");
const Word Word:: WORD_READ = Word(TAG_READ, "read");
const Word Word:: WORD_WRITE = Word(TAG_WRITE, "write");
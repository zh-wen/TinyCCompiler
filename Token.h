/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 16:45:15
 *
 **/


#ifndef TOKEN_H 
#define TOKEN_H
#include<string>
#include<map>
using namespace std;
class Token
{
protected:
	// enum TAG{}
	int tag;

public:
	Token(int tag);
	~Token();
	// static enum TAG{NORMAL,INT,IF,THEN,ELSE,END,REPEAT,UNTIl,READ,WRITE,ASSIGN,
	// 	PULS,MINUS,MUTL,DIV,EQUAL,LESS,VARIABLE};

	//得到标签
	int getTag();
	//用于词法分析，输出词元
	string toString();
	//判断是否是保留字
	bool isReservedWord(Token token);

	const static int TAG_NORMAL = 256;
	const static int TAG_INT = 257; // 整数常量
	const static int TAG_IF = 258; // if
	const static int TAG_THEN = 259; // then
	const static int TAG_ELSE = 260; // else
	const static int TAG_END = 261; // end
	const static int TAG_REPEAT = 262; // repeat
	const static int TAG_UNTIL = 263; // until
	const static int TAG_READ = 264; // read
	const static int TAG_WRITE = 265; // write
	const static int TAG_ASSIGN = 266; // :=  赋值符号
	const static int TAG_PLUS = 267; // +  加号
	const static int TAG_MINUS = 268; // -  减号
	const static int TAG_MUTL = 269; // *  乘号
	const static int TAG_DIV = 270; // /  除号
	const static int TAG_EQUAL = 271; // =  相等
	const static int TAG_LESS = 272; // <  小于
	const static int TAG_VARIABLE = 273; // 变量

};

class Int : public Token
{
private:
	int value;
public:
	Int(int value);
	~Int();
	
	int getValue();
	string toString();
};

class Operator : public Token
{
	const static Operator OP_ASSIGN;
	const static Operator OP_PLUS;
	const static Operator OP_MINUS;
	const static Operator OP_MUTL;
	const static Operator OP_DIV;
	const static Operator OP_EQUAL;
	const static Operator OP_LESS;

private:
	string mOp;  //tiny C运算符
	string cOp;  //C  运算符

public:
	Operator(int tag,string op,string cop);
	~Operator();
	
	string getMOperator();
	string getCOperator();
	string toString();
};

class Word : public Token
{
private:
	const static Word WORD_IF;
	const static Word WORD_THEN;
	const static Word WORD_ELSE;
	const static Word WORD_END;
	const static Word WORD_REPEAT;
	const static Word WORD_UNTIL;
	const static Word WORD_READ;
	const static Word WORD_WRITE;
	static map<string,Word> WORDS_RESERVED ;

	string lexeme;

public:
	Word(int tag, string lexeme);
	~Word();
	
	string getLexeme();
	string toString();
	Word getReserve(string lexme);
};

#endif
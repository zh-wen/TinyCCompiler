/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 19:39:56
 *
 **/

#ifndef LEXER_H
#define LEXER_H
#include"Tokener.h"
#include<ofstream>
using namespace std;

class Lexer
{
private:
	ifstream input;  //输入流
	ofstream output; //输出流 输出语法树
	bool endOfFile = false;
	char peek = 0;
	int row = 0;
	int line = 0;
	int putBack = -1;

	bool getNextToken(Tokener tokener);
	void peekNext();
	bool checkNew(char ch);
	void putBack(char ch);
	
public:
	Lexer(ifstream file);
	~Lexer();
	
};
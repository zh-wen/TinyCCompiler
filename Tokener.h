/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 19:24:36
 *
 **/
#ifndef TOKENER_H
#define TOKENER_H
#include"Token.h"


class Tokener
{
private:
	Token token ;
	int line;
	int row;
public:
	Tokener();
	~Tokener();
	
	void putToken(Token token, int line,int row)
	{
		this->token = token;
		this->line = line;
		this->row = row;
	}

	int getToken()
	{
		return this->token;
	}

	int getLine()
	{
		return this->line;
	}

	int getRow()
	{
		return this->row;
	}
};
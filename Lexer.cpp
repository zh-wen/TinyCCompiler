/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-02 19:53:26
 *
 **/

#include"Lexer.h"

Lexer::Lexer(ifstream *input)
{
	this->input = input;
}

void Lexer::setGramTreeFile(ofstream *file)
{
	this->output = file;
}

bool Lexer::nextToken(Tokener &tokener)
{
	bool nEof = getNextToken(tokener);

	//输出语法树节点
	if(nEof)
	{
		cout << "  " << tokener.toString() << endl;
		*output << "  " << tokener.toString() << "\n";
	}
	else
	{
		*output << "  " << tokener.getLine() << ":EOF" << endl;
	}

	return nEof;
}

bool Lexer::getNextToken(Tokener &tokener)
{
	bool isComment = false;
	bool loopMark = true;
	while(loopMark)
	{
		peekNext();
		if(endOfFile)
		{
			if(isComment)
			{
				cout << line << ',' << row << " 文件末尾出错：缺少'}\n'" << endl;
				exit(1);
			}
			tokener.putToken(new Token(-1),line,row);
			return false;
		}
		switch(peek)
		{
			    case ' ':
                case '\t':
                case '\n':
                    break;
                case '{':
                    if (isComment)
					{
                       cout << line << ',' << row << " 错误的输入：嵌套的注释\n" << endl;
					   exit(1);
                    } 
                    else
                    {
                        isComment = true;
                    }
                    break;
                case '}':
                    if (isComment) {
                        isComment = false;
                    } 
                    else
                    {
                        cout << line << ',' << row << " 错误的输入：嵌套的注释\n" << endl;
						exit(1);
                    }
                    break;
                default:
                    if (!isComment) 
                    {
                        loopMark = false;
                        break;
                    }
		}
	}

	// 2.识别复合词法符号 ":="
	switch(peek)
	{
        case ':':
			if (!checkNext('='))
			{
		        tokener.putToken(new Symbol(':'), line, row);
		        return true;
		    } 
		    else 
		    {
		        tokener.putToken(&Operator::OP_ASSIGN, line, row - 1);
		        return true;
		    }
	}

	//识别各种符号
	switch(peek)
	{
        case '+':
            tokener.putToken(&Operator::OP_PLUS, line, row);
            return true;
        case '-':
            tokener.putToken(&Operator::OP_MINUS, line, row);
            return true;
        case '*':
            tokener.putToken(&Operator::OP_MUTL, line, row);
            return true;
        case '/':
            tokener.putToken(&Operator::OP_DIV, line, row);
            return true;
        case '=':
            tokener.putToken(&Operator::OP_EQUAL, line, row);
            return true;
        case '<':
            tokener.putToken(&Operator::OP_LESS, line, row);
            return true;
	}

	//读取整数常量
	if(isDigit(peek))
	{
		int startRow = row;

		int value = peek - '0';

		while(true)
		{
			peekNext();
			if(endOfFile)
			{
				break;
			}
			if(!isDigit(peek))
			{
				putBack(peek);
				break;
			}
			value = value * 10 + peek - '0';
		}
		tokener.putToken(new Int(value),line,startRow);
		return true;
	}

	//读取标识符
	if(isLetter(peek))
	{
		int startrow = row;

		string str;
		str.append(string(&peek));
		while(true)
		{
			peekNext();
			if(endOfFile)
			{
				break;
			}
			if(!isLetterOrDigit(peek))
			{
				putBack(peek);
				break;
			}
			str.append(string(&peek));
		}

		Word *word = Word::getReserve(str);
		if( word == NULL)
		{
			word = new Word(Token::TAG_VARIABLE,str);
		}

		tokener.putToken((Word*)word,line,startrow);
		return true;
	}

	//剩余的情况
	tokener.putToken(new Token(peek),line,row);
	return true;
}

void Lexer::peekNext()
{
	if(backmark != -1)
	{
		peek = (char) backmark;
		backmark = -1;
		row++;
		endOfFile = false;
		return;
	}

	if(lineBuf == NULL || row >= (int)lineBuf->size())
	{
		if (lineBuf != NULL)
		{
			delete(lineBuf);
		}
	
		lineBuf = NULL;

		string str;


		getline(*input, str);
		if(str.size() == 0)
		{
			endOfFile = true;
			return ;
		}

		line++;
		row = 0;
		lineBuf = new string(str);

		//输出语法树
		if(str.at(lineBuf->size()-1) == '\n')
		{
			str = str.substr(0,str.size()-1);
		}
		*output << line << ':' << str << endl;
	}

	peek = lineBuf->at(row);
	row++;
	endOfFile = false;
}

bool Lexer::checkNext(char ch)
{
	peekNext();
	if(endOfFile)
	{
		return false;
	}
	if(peek == ch)
	{
		return true;
	}
	putBack(peek);

	return false;
}

void Lexer::putBack(char ch)
{
	if(backmark == -1)
	{
		backmark = ch;
		row--;
	}
}

bool Lexer::isDigit(char ch)
{
	return  (ch >= '0' && ch <= '9');
}

bool Lexer::isLetter(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool Lexer::isLetterOrDigit(char ch)
{
	return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}
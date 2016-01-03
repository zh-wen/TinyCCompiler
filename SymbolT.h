#ifndef SYMBOLT.H
#define SYMBOLT.H
#include"Node.h"
#include<vector>
#include<stack>

class SymbolT
{
private:
	map<Word, Id> symbols;
	int mTempMax = 0;
	vector<Temp> temps;
	stack<int> tempStack;

public:
	SymbolT();
	~SymbolT();
	
	void tmpPush();
	void tmpPop();
	Temp obtainTmp();
	string toString();
	Id* putVariable(Word *word, Id *id);
	Id* getVariable(Word *word);
};


#endif  
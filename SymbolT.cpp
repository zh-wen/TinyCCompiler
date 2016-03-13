/**
 * 
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-03 01:45:43
 *
 **/


#include"SymbolT.h"

Id* SymbolT::putVariable(Word *word, Id *id)
{
	map<Word, Id>::iterator iter;
	//iter = symbols.find(*word);
	for (iter = symbols.begin(); iter != symbols.end(); iter++)
	{
		if (((Word *)&(iter->first))->getLexeme() == word->getLexeme())
		{
			break;
		}
	}
	
	if (iter == symbols.end())
	{
		iter = (symbols.insert(pair<Word, Id>(*word, *id)).first);
	}
	return &(iter->second);	
}

 Id* SymbolT::getVariable(Word *word) {
 	map<Word, Id>::iterator iter;
	map<Word, Id>::iterator iter2;
	for (iter = symbols.begin(); iter != symbols.end(); iter++)
	{
		if ( ((Word *) &(iter->first))->getLexeme() == word->getLexeme())
		{
			break;
		}
	}
 	if(iter == symbols.end())
 	{
 		return NULL;
 	}
 	return &(iter->second);
 }

 void SymbolT::tmpPush()
 {
	 tempStack.push(mTempMax);
 }

 void SymbolT::tmpPop()
 {
	 mTempMax = tempStack.top();
	 tempStack.pop();
 }

 Temp* SymbolT::obtainTmp()
 {
	 while (mTempMax >= (int)temps.size())
	 {	 
		 Temp * tmp = new Temp(temps.size());
		 temps.push_back(*tmp);
	 }
	 Temp *t = &(temps.at(mTempMax));
	 mTempMax++;
	 return t;
 }

 string SymbolT::toString()
 {
	 if (symbols.empty() && temps.empty())
	 {
		 return string("");
	 }

	 string str;
	 str.append(string("int"));
	 bool headMark = true;

	 map<Word, Id>::iterator sbIter;
	 for (sbIter = symbols.begin(); sbIter != symbols.end(); sbIter++)
	 {
		 if (!headMark)
		 {
			 str.append(string(","));
		 }
		 headMark = false;
		 str.append(sbIter->second.toString());
	 }

	 vector<Temp>::iterator tmpIter;
	 for (tmpIter = temps.begin(); tmpIter != temps.end(); tmpIter++)
	 {
		 if (!headMark)
		 {
			 str.append(string(","));
		 }
		 headMark = false;
		 str.append(tmpIter->toString());
	 }
	 str.append(string(";"));
	 return str;
 }
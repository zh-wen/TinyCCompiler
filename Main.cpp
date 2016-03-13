/**
 *
 * @authors zh.wen (zh.wen@hotmail.com)
 * @date    2016-01-04 00:29:55
 *
 **/
#include"Parser.h"

int main()
{
	ifstream input;
	ofstream tokenList , sTree, cFile ;
	string filename = "file";

	cout << "input the filename" << endl;
	//cin >> filename;

	input.open(filename, ios::in);
	if (!input)
	{
		cout << "can't read the file '" << filename << "'" << endl;
		exit(1);
	}

	tokenList.open("tokenList.dat", ios::out);
	if (!tokenList)
	{
		cout << "can't write the file 'tokenList.dat'" << endl;
		exit(2);
	}

	sTree.open("sTree.dat", ios::out);
	if(!sTree)
	{
		cout << "can't write the file 'sTree.dat'" << endl;
		exit(2);
	}

	cFile.open("cFile.c", ios::out);

	if (!cFile)
	{
		cout << "can't write the file 'tokenList.dat'" << endl;
		exit(2);
	}

	Lexer *lexer = new Lexer(&input);
	Parser *parser = new Parser(lexer);
	lexer->setGramTreeFile(&tokenList);
	Program *program = parser->parse();
	program->createSTree(0, &sTree);
	program->genC(&cFile);
}
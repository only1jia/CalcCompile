#include <iostream>
#include <string>
#include <vector>
#include "cmorphemeanalysis.h"
#include "csyntaxnodeanalysis.h"
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {
	string input, output;
	while (1) {
		cout << "��������ʽ:";
		cin >> input;
		CMorphemeAnalysis morpheme;
		std::vector<CalcToken> tokenArray;
		if (morpheme.Analysis(input, tokenArray))
		{
			double fValue;
			ProcNode node;
			CSyntaxNodeAnalysis nodeAnalysis;
			if (nodeAnalysis.Analysis(tokenArray, fValue))
			{
				cout << "���Ϊ:" << fValue << endl;
			}
		}
	}
}
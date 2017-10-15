#include "cmorphemeanalysis.h"
#include <cstdlib>
#include <iostream>
CMorphemeAnalysis::CMorphemeAnalysis(void)
{
}

CMorphemeAnalysis::~CMorphemeAnalysis(void)
{
}

bool CMorphemeAnalysis::ProcNumber(std::string &sContent, int &nProcPos, CalcToken& token)
{
	if (sContent.length() <= nProcPos)
	{// ����sContent���Ȳ���
		return false;
	}
	char ch = sContent.at(nProcPos);
	if (ch < '0' || ch > '9')
	{// ������
		return false;
	}

	int nLastNumberPos = sContent.length() - 1;
	// ��ȡ�����Ӵ�
	for (int i = nProcPos + 1; i<sContent.length(); i++)
	{
		char ch = sContent.at(i);
		if ((ch < '0' || ch > '9') && ch != '.')
		{
			nLastNumberPos = i - 1;
			break;
		}
	}

	std::string sSubStr = sContent.substr(nProcPos, nLastNumberPos - nProcPos + 1);
	if (sSubStr.find('.') != -1)
	{
		token.type = enumTokenType_float;
		token.data.fFloat = strtof(sSubStr.c_str(), NULL);
	}
	else
	{
		token.type = enumTokenType_integer;
		token.data.nInteger = strtol(sSubStr.c_str(), NULL, 10);
	}
	// ���õ�ǰ���һ���ַ�λ��
	nProcPos = nLastNumberPos;
	return true;
}

bool CMorphemeAnalysis::Analysis(std::string &sContent, std::vector<CalcToken>& vecToken)
{
	std::string sProcContent = sContent;

	// ����б�
	//sProcContent.Trim();
	vecToken.clear();

	if (sProcContent.empty())
	{
		return true;
	}

	for (int i = 0; i<sProcContent.length(); i++)
	{// �������´���
		char ch = sProcContent.at(i);
		if (ch == ' ')
		{// ȥ���ո�
			continue;
		}
		CalcToken token;
		switch (ch)
		{
		case '+':
		{
			token.type = enumTokenType_add;
			token.data.cChar = ch;
			break;
		}
		case '-':
		{
			token.type = enumTokenType_sub;
			token.data.cChar = ch;
			break;
		}
		case '*':
		{
			token.type = enumTokenType_mul;
			token.data.cChar = ch;
			break;
		}
		case '/':
		{
			token.type = enumTokenType_div;
			token.data.cChar = ch;
			break;
		}
		case '(':
		{
			token.type = enumTokenType_lbt;
			token.data.cChar = ch;
			break;
		}
		case ')':
		{
			token.type = enumTokenType_rbt;
			token.data.cChar = ch;
			break;
		}
		default:
		{
			char error[100];
			if (ch >= '0' && ch <= '9')
			{// ����������أ�����integer��double
				if (!ProcNumber(sProcContent, i, token))
				{
					sprintf(error, "Error: ��λ�ã�%d�����ִ����쳣: ", i);
					std::cout << error << std::endl;
					return false;
				}
			}
			else
			{
				sprintf(error, "Error: ��λ�ã�%d��������Ч�ַ�<%c>:", i, ch);
				std::cout << error << std::endl;
				return false;
			}
			break;
		}
		}
		vecToken.push_back(token);
	}
	return true;
}

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
	{// 输入sContent长度不够
		return false;
	}
	char ch = sContent.at(nProcPos);
	if (ch < '0' || ch > '9')
	{// 非数字
		return false;
	}

	int nLastNumberPos = sContent.length() - 1;
	// 获取数字子串
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
	// 设置当前最后一个字符位置
	nProcPos = nLastNumberPos;
	return true;
}

bool CMorphemeAnalysis::Analysis(std::string &sContent, std::vector<CalcToken>& vecToken)
{
	std::string sProcContent = sContent;

	// 清空列表
	//sProcContent.Trim();
	vecToken.clear();

	if (sProcContent.empty())
	{
		return true;
	}

	for (int i = 0; i<sProcContent.length(); i++)
	{// 依此往下处理
		char ch = sProcContent.at(i);
		if (ch == ' ')
		{// 去除空格
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
			{// 继续向后搜素，处理integer和double
				if (!ProcNumber(sProcContent, i, token))
				{
					sprintf(error, "Error: 在位置（%d）数字处理异常: ", i);
					std::cout << error << std::endl;
					return false;
				}
			}
			else
			{
				sprintf(error, "Error: 在位置（%d）发现无效字符<%c>:", i, ch);
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

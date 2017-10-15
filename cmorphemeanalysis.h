#pragma once
#ifndef CMORPHEMEANALYSIS_H
#define CMORPHEMEANALYSIS_H
#include <string>
#include <vector>

// token：标记，标示，暗号等
// bracket：括号

union TokenData
{
	char cChar;
	int nInteger;
	double fFloat;	// 数字  double: 精度52Bit - 十进制15-16位精度
};
enum EnumTokenType
{
	enumTokenType_none = -1,	// 未定义
	enumTokenType_integer = 0,	// 数字
	enumTokenType_float = 1,	// 浮点数
	enumTokenType_add = 2,	// 加号
	enumTokenType_sub = 3,	// 减号
	enumTokenType_mul = 4,	// 乘号
	enumTokenType_div = 5,	// 除号
	enumTokenType_lbt = 6,	// 左括号
	enumTokenType_rbt = 7,	// 右括号
};

struct CalcToken
{
	EnumTokenType type;
	TokenData	  data;
};


// 词法分析
class CMorphemeAnalysis
{
public:
	CMorphemeAnalysis(void);
	bool ProcNumber(std::string& sContent, int& nProcPos, CalcToken& token);
	bool Analysis(std::string& sContent,std::vector<CalcToken>& vecToken);

public:
	~CMorphemeAnalysis(void);
};


#endif
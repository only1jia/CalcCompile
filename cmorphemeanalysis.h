#pragma once
#ifndef CMORPHEMEANALYSIS_H
#define CMORPHEMEANALYSIS_H
#include <string>
#include <vector>

// token����ǣ���ʾ�����ŵ�
// bracket������

union TokenData
{
	char cChar;
	int nInteger;
	double fFloat;	// ����  double: ����52Bit - ʮ����15-16λ����
};
enum EnumTokenType
{
	enumTokenType_none = -1,	// δ����
	enumTokenType_integer = 0,	// ����
	enumTokenType_float = 1,	// ������
	enumTokenType_add = 2,	// �Ӻ�
	enumTokenType_sub = 3,	// ����
	enumTokenType_mul = 4,	// �˺�
	enumTokenType_div = 5,	// ����
	enumTokenType_lbt = 6,	// ������
	enumTokenType_rbt = 7,	// ������
};

struct CalcToken
{
	EnumTokenType type;
	TokenData	  data;
};


// �ʷ�����
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
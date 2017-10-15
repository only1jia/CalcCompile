#ifndef CSYNTAXNODEANALYSIS_H
#define CSYNTAXNODEANALYSIS_H

#include "cmorphemeanalysis.h"
#include <list>

// ���ȼ���ϵ ()  */  +-
// 1. �������(��ֱ����һ����Ӧ)���м��������ɵ�һ���ڵ���
// 2. ��������*/����������+-���ɵ�һ���ڵ��� 


// ����ƥ��ģʽ�Լ�ƥ���㷨
/*
exp -> term
exp -> term + exp
exp -> term - exp

term -> factor
term -> factor * term
term -> factor / term

factor -> number
factor -> (exp)
*/

// ˼·: ʹ�÷ֿ�˼·�����ݰ����ȼ��ֿ飬��ʹ��()�ֿ飬Ȼ��ʹ��*/�ŷֿ飬�ֿ����������ʱ�������Ҽ���Ϳ����ˡ�
enum EnumProcType
{
	enumProcType_content = 0,	// δ�ֽ�����
};

struct ProcSubNode;
struct ProcNode
{
	EnumProcType			procType;
	std::list<ProcSubNode>	tokenlist;
};
enum EnumProcSubNodeType
{
	enumProcSubNodeType_token = 0,
	enumProcSubNodeType_node = 1,
};
struct ProcSubNode
{
	EnumProcSubNodeType type;
	ProcNode node;
	CalcToken token;
};

class CSyntaxNodeAnalysis
{
public:
	bool Analysis(std::vector<CalcToken>& tokenArray, double& fResult);
	bool ProcBracket(ProcNode& node);
	bool ProcMulAndDiv(ProcNode& node);
	bool CalcTerm(const ProcNode& node, double& result);
};
/*
������: ���� 1+8+(2+3*(5+6))*7
1. ������������ProcNode1: tokenlist, procType��ΪenumProcType_content
2. ���½��ֽ�()
����ProcSubNode�ڵ�ProcNode2: 2+3*(5+6)����tokenlist
�ӽڵ��һ���ֽ�
����ProcSubNode�ڵ�ProcNode3: 5+6 ����tokenlist

ProcNode1���Ϊ: 1 + 8 + ProcNode2 * 7
ProcNode2���Ϊ: 2 + 3 * ProcNode3
ProcNode3���Ϊ: 5 + 6
3. ���ǳ˳�
ProcNode1�ֽ⽨��ProcSubNode�ڵ�ProcNode4: ProcNode2 * 7
ProcNode2�ֽ⽨��ProcSubNode�ڵ�ProcNode5: 3 * ProcNode3

ProcNode1���Ϊ��1 + 8 + ProcNode4
ProcNode2���Ϊ: 2 + ProcNode5
ProcNode3���Ϊ��5 + 6
ProcNode4���Ϊ: ProcNode2 * 7
ProcNode5���Ϊ��3 * ProcNode3

��������ProcNode1 = 1 + 8 + ProcNode4
= 1 + 8 + (ProcNode2 * 7)
= 1 + 8 + ((2 + ProcNode5) * 7)
= 1 + 8 + ((2 + (3 * ProcNode3)) * 7)
= 1 + 8 + ((2 + (3 * (5 + 6)) * 7)
*/

#endif
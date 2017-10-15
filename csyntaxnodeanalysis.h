#ifndef CSYNTAXNODEANALYSIS_H
#define CSYNTAXNODEANALYSIS_H

#include "cmorphemeanalysis.h"
#include <list>

// 优先级关系 ()  */  +-
// 1. 如果发现(，直到下一个对应)，中间结果，生成到一个节点上
// 2. 若果发现*/，则优先与+-生成到一个节点上 


// 建立匹配模式以及匹配算法
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

// 思路: 使用分块思路把内容按优先级分块，先使用()分块，然后使用*/号分块，分块结束；计算时从左往右计算就可以了。
enum EnumProcType
{
	enumProcType_content = 0,	// 未分解内容
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
处理步骤: 例如 1+8+(2+3*(5+6))*7
1. 把整个串放入ProcNode1: tokenlist, procType置为enumProcType_content
2. 向下结点分解()
建立ProcSubNode节点ProcNode2: 2+3*(5+6)放入tokenlist
子节点进一步分解
建立ProcSubNode节点ProcNode3: 5+6 放入tokenlist

ProcNode1结点为: 1 + 8 + ProcNode2 * 7
ProcNode2结点为: 2 + 3 * ProcNode3
ProcNode3结点为: 5 + 6
3. 考虑乘除
ProcNode1分解建立ProcSubNode节点ProcNode4: ProcNode2 * 7
ProcNode2分解建立ProcSubNode节点ProcNode5: 3 * ProcNode3

ProcNode1结点为：1 + 8 + ProcNode4
ProcNode2结点为: 2 + ProcNode5
ProcNode3结点为：5 + 6
ProcNode4结点为: ProcNode2 * 7
ProcNode5结点为：3 * ProcNode3

计算结果：ProcNode1 = 1 + 8 + ProcNode4
= 1 + 8 + (ProcNode2 * 7)
= 1 + 8 + ((2 + ProcNode5) * 7)
= 1 + 8 + ((2 + (3 * ProcNode3)) * 7)
= 1 + 8 + ((2 + (3 * (5 + 6)) * 7)
*/

#endif
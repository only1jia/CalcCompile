#include "csyntaxnodeanalysis.h"
#include <iostream>

bool CSyntaxNodeAnalysis::ProcMulAndDiv(ProcNode& node)
{
	if (node.procType != enumProcType_content)
	{
		return false;
	}

	std::list<ProcSubNode>::iterator iter = node.tokenlist.begin();
	while (iter != node.tokenlist.end())
	{
		if (iter->type == enumProcSubNodeType_node)
		{
			ProcMulAndDiv(iter->node);
			iter++;
		}
		else if (iter->type == enumProcSubNodeType_token)
		{
			if (iter->token.type == enumTokenType_mul || iter->token.type == enumTokenType_div)
			{
				if (iter == node.tokenlist.begin())
				{
					std::cout << "Error: *或/号不匹配" << std::endl;
					return false;
				}
				std::list<ProcSubNode>::iterator iterStart = iter;
				iterStart--;

				ProcSubNode subNode;
				subNode.type = enumProcSubNodeType_node;
				ProcNode& addNode = subNode.node;
				addNode.procType = enumProcType_content;
				std::list<ProcSubNode>::iterator iterProc = iter;
				iterProc++;
				for (; iterProc != node.tokenlist.end(); iterProc++)
				{
					if (iterProc->type == enumProcSubNodeType_node)
					{
						ProcMulAndDiv(iterProc->node);
					}
					else if (iterProc->type == enumProcSubNodeType_token)
					{
						if (iterProc->token.type == enumTokenType_add || iterProc->token.type == enumTokenType_sub)
						{
							break;
						}
					}
				}
				{
					addNode.tokenlist.splice(addNode.tokenlist.begin(), node.tokenlist, iterStart, iterProc);
					std::list<ProcSubNode>::iterator iterAdd = node.tokenlist.insert(iterProc, subNode);
					iter = iterProc;
				}
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}// end while
	return true;
}


bool CSyntaxNodeAnalysis::ProcBracket(ProcNode& node)
{
	if (node.procType != enumProcType_content)
	{
		return false;
	}

	std::list<ProcSubNode>::iterator iter = node.tokenlist.begin();
	while (iter != node.tokenlist.end())
	{
		if (iter->type == enumProcSubNodeType_node)
		{
			ProcBracket(iter->node);
			iter++;
		}
		else if (iter->type == enumProcSubNodeType_token)
		{
			if (iter->token.type == enumTokenType_lbt)
			{
				int nBracketOpenCount = 1;
				ProcSubNode subNode;
				subNode.type = enumProcSubNodeType_node;
				ProcNode& addNode = subNode.node;
				addNode.procType = enumProcType_content;
				std::list<ProcSubNode>::iterator iterProc = iter;
				iterProc++;
				bool bFoundRightBracket = false;
				for (; iterProc != node.tokenlist.end(); iterProc++)
				{
					if (iterProc->type == enumProcSubNodeType_node)
					{
						ProcBracket(iterProc->node);
					}
					else if (iterProc->type == enumProcSubNodeType_token)
					{
						if (iterProc->token.type == enumTokenType_lbt)
						{
							nBracketOpenCount++;
						}
						if (iterProc->token.type == enumTokenType_rbt)
						{
							nBracketOpenCount--;
							if (nBracketOpenCount == 0)
							{// 找到匹配的')'项
								std::list<ProcSubNode>::iterator iterNextPos = iterProc;
								iterNextPos++;
								addNode.tokenlist.splice(addNode.tokenlist.begin(), node.tokenlist, iter, iterNextPos);
								addNode.tokenlist.pop_front();
								addNode.tokenlist.pop_back();
								std::list<ProcSubNode>::iterator iterAdd = node.tokenlist.insert(iterNextPos, subNode);
								iter = iterAdd;
								bFoundRightBracket = true;
								break;
							}
						}
					}
				}
				if (!bFoundRightBracket)
				{// 执行到这个位置，代表未找到匹配的括号
					std::cout << "Error: 括号不匹配" << std::endl;
				}
			}
			else
			{
				iter++;
			}
		}
		else
		{
			iter++;
		}
	}// end while
	return true;
}

bool CSyntaxNodeAnalysis::CalcTerm(const ProcNode& node, double& result)
{
	if (node.procType == enumProcType_content)
	{
		std::list<ProcSubNode>::const_iterator iter = node.tokenlist.begin();
		EnumTokenType operType = enumTokenType_add;
		CalcToken leftValue;
		leftValue.type = enumTokenType_float;
		leftValue.data.fFloat = 0;
		for (; iter != node.tokenlist.end(); iter++)
		{
			CalcToken rightValue;
			rightValue.type = enumTokenType_float;
			if (iter->type == enumProcSubNodeType_token)
			{
				switch (iter->token.type)
				{
				case enumTokenType_mul:
				case enumTokenType_div:
				case enumTokenType_add:
				case enumTokenType_sub:
				{
					operType = iter->token.type;
					continue;
				}
				case enumTokenType_integer:
				{
					rightValue.data.fFloat = iter->token.data.nInteger;
					break;
				}
				case enumTokenType_float:
				{
					rightValue.data.fFloat = iter->token.data.fFloat;
					break;
				}
				}
			}
			else if (iter->type == enumProcSubNodeType_node)
			{
				double fValue = 0;
				CalcTerm(iter->node, fValue);
				rightValue.data.fFloat = fValue;
			}
			switch (operType)
			{
			case enumTokenType_mul:
			{
				leftValue.data.fFloat *= rightValue.data.fFloat;
				break;
			}
			case enumTokenType_div:
			{
				leftValue.data.fFloat /= rightValue.data.fFloat;
				break;
			}
			case enumTokenType_add:
			{
				leftValue.data.fFloat += rightValue.data.fFloat;
				break;
			}
			case enumTokenType_sub:
			{
				leftValue.data.fFloat -= rightValue.data.fFloat;
				break;
			}
			}
		}
		result = leftValue.data.fFloat;
		return true;
	}
	return false;
}


bool CSyntaxNodeAnalysis::Analysis(std::vector<CalcToken>& tokenArray, double& fResult)
{
	if (tokenArray.empty())
	{
		fResult = 0;
		return true;
	}


	ProcNode node;
	node.procType = enumProcType_content;
	for (size_t i = 0; i<tokenArray.size(); i++)
	{
		ProcSubNode subNode;
		subNode.type = enumProcSubNodeType_token;
		subNode.token = tokenArray[i];
		node.tokenlist.push_back(subNode);
	}

	// 结点处理
	if (!ProcBracket(node))
	{
		return false;
	}

	if (!ProcMulAndDiv(node))
	{
		return false;
	}


	fResult = 0;
	if (CalcTerm(node, fResult))
	{
		return true;
	}
	return false;
}
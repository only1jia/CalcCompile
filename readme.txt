编译原理思路实现简单功能计算器

1、	实现词法分析器
词法分析器工作处理输入源程序文本, 按照构词规则分解成一系列token
定义token, 分析出这几种词语，其他情况，认为是异常
union TokenData
{
	char cChar;
	int nInteger;
	double fFloat;
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

词法分析处理，从左到右处理输入字符串，将数字和符号分成一系列token,并定义好相应的token类型，最后存在token类型的vector。
2、	实现语法分析器
定义目标语言的语法规则
exp -> term
exp -> term + exp
exp -> term - exp
term -> factor
term -> factor * term
term -> factor / term
factor -> number
factor -> (exp)
使用分块思路把内容按优先级分块，先使用'(' 和 ')'分块，然后使用'*' 和 '/'号分块，分块结束；计算时从左往右计算就可以了
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

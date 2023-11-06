#include "treenode.h"
#include "parser.h"

//treenode
TreeNode::TreeNode(bool isParsed, SymbolTable& symbolTable) : isParsed(isParsed), symbolTable(symbolTable){};
void TreeNode::setIsParsed(){isParsed = true;}
TreeNode::~TreeNode(){};

//program
ProgramNode::ProgramNode(StatementsNode* statementsNode) : statementsNode(statementsNode){};
ProgramNode::~ProgramNode(){ delete statementsNode; }

void ProgramNode::calculate_statements(){
	statementsNode->calculate_statement();
}


//statements
StatementsNode::StatementsNode(StatementNode* statementNode, bool semi_colon, StatementsNode* statementsNode) : statementNode(statementNode), semi_colon(semi_colon), statementsNode(statementsNode){};
StatementsNode::~StatementsNode(){
	delete statementNode;
	delete statementsNode;
};

void StatementsNode::calculate_statement(){
	statementNode->calculate();
	if (statementsNode != nullptr)
		statementsNode->calculate_statement();
}

//statement
StatementNode::StatementNode(bool isParsed, SymbolTable& symbolTable, std::string ident, bool assignment_op, ExpressionNode* expressionNode) : TreeNode(isParsed, symbolTable), ident(ident), assignment_op(assignment_op), expressionNode(expressionNode){};
double StatementNode::calculate() {
    double value;
    value = expressionNode->calculate();
    symbolTable.set(ident, value);
    return value;
}
StatementNode::~StatementNode(){ delete expressionNode; }

//expression
ExpressionNode::ExpressionNode(bool isParsed, SymbolTable& symbolTable, TermNode* termNode, TermTailNode* termTailNode): TreeNode(isParsed,symbolTable),
                                                                                                                         termTailNode(termTailNode), termNode(termNode){};
ExpressionNode::~ExpressionNode()
{
	delete termNode;
	delete termTailNode;
}
double ExpressionNode::calculate() {
    //if (TreeNode::isParsed == false) return (std::nan("NaN"));
    if (termNode->calculate() == std::nan("NaN"))
        return (std::nan("NaN"));
	if (termTailNode == nullptr)
		return termNode->calculate();
    if (termTailNode->get_op() == 1)
        return termNode->calculate() + termTailNode->calculate();
    if (termTailNode->get_op() == 2)
        return termNode->calculate() - termTailNode->calculate();
    return (std::nan("NaN"));
}

//term
TermNode::TermNode(bool isParsed, SymbolTable& symbolTable, FactorNode* factorNode, FactorTailNode* factorTailNode) : TreeNode(isParsed, symbolTable),
                                                                                                                      factorNode(factorNode), factorTailNode(factorTailNode){};
TermNode::~TermNode()
{
	delete factorNode;
	delete factorTailNode;
}

double TermNode::calculate() {
    if (factorNode->calculate() == std::nan("NaN"))
        return (std::nan("NaN"));
	if (factorTailNode == nullptr)
		return factorNode->calculate();
    if (factorTailNode->get_op() == 1)
        return factorNode->calculate() * factorTailNode->calculate();
    if (factorTailNode->get_op() == 2)
        return factorNode->calculate() / factorTailNode->calculate();
	return (std::nan("NaN"));
}
//termtail
TermTailNode::TermTailNode(bool isParsed, SymbolTable& symbolTable) : TreeNode(isParsed, symbolTable){}
TermTailNode::TermTailNode(bool isParsed, SymbolTable& symbolTable, int add_op, TermNode* termNode, TermTailNode* termTailNode) : TreeNode(isParsed, symbolTable), add_op(add_op), termNode(termNode), termTailNode(termTailNode){};
TermTailNode::~TermTailNode()
{
	delete termNode;
	delete termTailNode;
}
double TermTailNode::calculate() {
    if(termNode->calculate()== std::nan("NaN"))
        return (std::nan("NaN"));
	if (termTailNode == nullptr)
		return termNode->calculate();
    if (termTailNode->get_op() == 1)
        return termNode->calculate() + termTailNode->calculate();
    if (termTailNode->get_op() == 2)
        return termNode->calculate() - termTailNode->calculate();
    return (std::nan("NaN"));
}
int TermTailNode::get_op() {return add_op;}

//factor
FactorNode::FactorNode(bool isParsed, SymbolTable& symbolTable, bool left_paren, ExpressionNode* expressionNode, bool right_paren) : TreeNode(isParsed, symbolTable), left_paren(left_paren), expressionNode(expressionNode), right_paren(right_paren), ident(""), _const(0.0){};
FactorNode::FactorNode(bool isParsed, SymbolTable& symbolTable, std::string ident) : TreeNode(isParsed, symbolTable), ident(ident), expressionNode(nullptr), _const(0.0){};
FactorNode::FactorNode(bool isParsed, SymbolTable& symbolTable, double _const) : TreeNode(isParsed, symbolTable), ident(""), _const(_const), expressionNode(nullptr){};
FactorNode::~FactorNode() { delete expressionNode; }

double FactorNode::calculate() {
    if (isParsed)
    {
        double value;
        if (symbolTable.get(ident, value)) {
            return value;
        }
        if (expressionNode != nullptr)
            return expressionNode->calculate();
		//std::cout << "_const : " << _const << "\n";
        return _const;
    }
	return (std::nan("NaN"));
}

//factortail
FactorTailNode::FactorTailNode(bool isParsed, SymbolTable& symbolTable) : TreeNode(isParsed, symbolTable) {};
FactorTailNode::FactorTailNode(bool isParsed, SymbolTable& symbolTable, int mult_op, FactorNode* factorNode, FactorTailNode* factorTailNode) : TreeNode(isParsed, symbolTable), mult_op(mult_op), factorNode(factorNode), factorTailNode(factorTailNode){};
FactorTailNode::~FactorTailNode()
{
	delete factorNode;
	delete factorTailNode;
}
double FactorTailNode::calculate() {
    if (factorNode->calculate() == std::nan("NaN"))
        return (std::nan("NaN"));
	if (factorTailNode == nullptr)
		return factorNode->calculate();
    if (factorTailNode->get_op() == 1)
        return factorNode->calculate() * factorTailNode->calculate();
    if (factorTailNode->get_op() == 2)
        return factorNode->calculate() / factorTailNode->calculate();
	return (std::nan("NaN"));
}
int FactorTailNode::get_op(){return mult_op;}

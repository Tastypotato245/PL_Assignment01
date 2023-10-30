#include "treenode.h"
#include "parser.h"

//treenode
TreeNode::TreeNode(bool isParsed, SymbolTable& symbolTable) : isParsed(isParsed), symbolTable(symbolTable){};
void TreeNode::setIsParsed(){isParsed = true;}
//program
ProgramNode::ProgramNode(StatementsNode& statementsNode) : statementsNode(statementsNode){};
//statements
StatementsNode::StatementsNode(StatementNode& statementNode, bool semi_colon, StatementsNode& statementsNode) : statementNode(statementNode), semi_colon(semi_colon), statementsNode(statementsNode){};
//statement
StatementNode::StatementNode(bool isParsed, SymbolTable& symbolTable, std::string ident, bool assignment_op, ExpressionNode& expressionNode) : TreeNode(isParsed, symbolTable),
                                                                                                                                                ident(ident), assignment_op(assignment_op), expressionNode(expressionNode){};
double StatementNode::calculate() {
    double value;
    value = expressionNode.calculate();
    symbolTable.set(ident, value);
    return value;
}
//expression
ExpressionNode::ExpressionNode(bool isParsed, SymbolTable& symbolTable, TermNode& termNode, TermTailNode& termTailNode): TreeNode(isParsed,symbolTable),
                                                                                                                         termTailNode(termTailNode), termNode(termNode){};
double ExpressionNode::calculate() {
    if (termTailNode.get_op() == 1)
        return termNode.calculate() + termTailNode.calculate();
    if (termTailNode.get_op() == 2)
        return termNode.calculate() - termTailNode.calculate();
}

//term
TermNode::TermNode(bool isParsed, SymbolTable& symbolTable, FactorNode& factorNode, FactorTailNode& factorTailNode) : TreeNode(isParsed, symbolTable),
                                                                                                                      factorNode(factorNode), factorTailNode(factorTailNode){};
double TermNode::calculate() {
    if (factorTailNode.get_op() == 1)
        return factorNode.calculate() * factorTailNode.calculate();
    if (factorTailNode.get_op() == 2)
        return factorNode.calculate() / factorTailNode.calculate();
}
//termtail
TermTailNode::TermTailNode(bool isParsed, SymbolTable& symbolTable) : TreeNode(isParsed, symbolTable){}
TermTailNode::TermTailNode(bool isParsed, SymbolTable& symbolTable, int add_op, TermNode* termNode, TermTailNode* termTailNode) : TreeNode(isParsed, symbolTable),
                                                                                                                                  add_op(add_op), termNode(termNode), termTailNode(termTailNode){};
double TermTailNode::calculate() {
    if (termTailNode->get_op() == 1)
        return termNode->calculate() + termTailNode->calculate();
    if (termTailNode->get_op() == 2)
        return termNode->calculate() - termTailNode->calculate();
}
int TermTailNode::get_op() {return add_op;}

//factor
FactorNode::FactorNode(bool isParsed, SymbolTable& symbolTable, bool left_paren, ExpressionNode* expressionNode, bool right_paren) : TreeNode(isParsed, symbolTable),
                                                                                                                                     left_paren(left_paren), expressionNode(expressionNode), right_paren(right_paren), ident(""), _const(0.0){};
FactorNode::FactorNode(bool isParsed, SymbolTable& symbolTable, std::string ident) : TreeNode(isParsed, symbolTable),
                                                                                     ident(ident), expressionNode(nullptr), _const(0.0){};
FactorNode::FactorNode(bool isParsed, SymbolTable& symbolTable, double _const) : TreeNode(isParsed, symbolTable),
                                                                                 ident(""), _const(_const), expressionNode(nullptr){};
double FactorNode::calculate() {
    if (isParsed)
    {
        double value;
        if (symbolTable.get(ident, value))
            return value;
        if (expressionNode != nullptr)
            return expressionNode->calculate();
        return _const;
    }
}

//factortail
FactorTailNode::FactorTailNode(bool isParsed, SymbolTable& symbolTable) : TreeNode(isParsed, symbolTable) {};
FactorTailNode::FactorTailNode(bool isParsed, SymbolTable& symbolTable, int mult_op, FactorNode* factorNode, FactorTailNode* factorTailNode) : TreeNode(isParsed, symbolTable),
                                                                                                                                               mult_op(mult_op), factorNode(factorNode), factorTailNode(factorTailNode){};
double FactorTailNode::calculate() {
    if (factorTailNode->get_op() == 1)
        return factorNode->calculate() * factorTailNode->calculate();
    if (factorTailNode->get_op() == 2)
        return factorNode->calculate() / factorTailNode->calculate();
}
int FactorTailNode::get_op(){return mult_op;}

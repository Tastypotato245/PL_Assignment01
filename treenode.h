// TreeNode.h

#ifndef TREENODE_H
#define TREENODE_H

#include "utils.h"

class TreeNode {
public:
    TreeNode(bool isParsed, SymbolTable& symbolTable);
	virtual ~TreeNode();
    virtual double calculate() = 0;
    void setIsParsed();

protected:
    bool isParsed;
    SymbolTable& symbolTable;
};

class FactorNode : public TreeNode {
public:
    FactorNode(bool isParsed, SymbolTable& symbolTable, bool left_paren, class ExpressionNode* expressionNode, bool right_paren);
    FactorNode(bool isParsed, SymbolTable& symbolTable, std::string ident);
    FactorNode(bool isParsed, SymbolTable& symbolTable, double _const);
	~FactorNode();
    double calculate();

private:
    bool left_paren;
    ExpressionNode* expressionNode;
    bool right_paren;
    std::string ident;
    double _const;
};

class FactorTailNode : public TreeNode {
public:
    FactorTailNode(bool isParsed, SymbolTable& symbolTable);
    FactorTailNode(bool isParsed, SymbolTable& symbolTable, int mult_op, FactorNode* factorNode, FactorTailNode* factorTailNode);
	~FactorTailNode();
    double calculate();
    int get_op();

private:
    int mult_op;
    FactorNode* factorNode;
    FactorTailNode* factorTailNode;
};

class TermNode : public TreeNode {
public:
    TermNode(bool isParsed, SymbolTable& symbolTable, FactorNode* factorNode, FactorTailNode* factorTailNode);
	~TermNode();
    double calculate();

private:
    FactorNode* factorNode;
    FactorTailNode* factorTailNode;
};

class TermTailNode : public TreeNode {
public:
    TermTailNode(bool isParsed, SymbolTable& symbolTable);
    TermTailNode(bool isParsed, SymbolTable& symbolTable, int add_op, TermNode* termNode, TermTailNode* termTailNode);
	~TermTailNode();
    double calculate();
    int get_op();

private:
    int add_op;
    TermNode* termNode;
    TermTailNode* termTailNode;
};

class ExpressionNode : public TreeNode {
public:
    ExpressionNode(bool isParsed, SymbolTable& symbolTable, TermNode* termNode, TermTailNode* termTailNode);
	~ExpressionNode();
    double calculate();

private:
    TermNode* termNode;
    TermTailNode* termTailNode;
};

class StatementNode : public TreeNode {
public:
    StatementNode(bool isParsed, SymbolTable& symbolTable, std::string ident, bool assignment_op, ExpressionNode* expressionNode);
	~StatementNode();
    double calculate();

private:
    std::string ident;
    bool assignment_op;
    ExpressionNode* expressionNode;
};

class StatementsNode{
public:
    StatementsNode(StatementNode* statementNode, bool semi_colon, StatementsNode* statementsNode);
	~StatementsNode();

	void calculate_statement();
private:
    StatementNode* statementNode;
    StatementsNode* statementsNode;
    bool semi_colon;
};

class ProgramNode {
public:
    ProgramNode(StatementsNode* statementsNode);
	~ProgramNode();
	void calculate_statements();
private:
    StatementsNode* statementsNode;
};

#endif //TREENODE_H

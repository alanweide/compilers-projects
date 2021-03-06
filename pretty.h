#include <rose.h>
using namespace std;

string printExpression(SgExpression* expr);
string printType(SgType* type);
string printOperatorForBinaryOp(SgBinaryOp* op);
string printBinaryOp(SgBinaryOp* expr);
string printStatement(SgStatement* stmt);
string printForStmt(SgForStatement* for_stmt);
string printWhileStmt(SgWhileStmt* while_stmt);
string printDoWhileStmt(SgDoWhileStmt* dow_stmt);
string printIfStmt(SgIfStmt* stmt);
string printBasicBlock(SgBasicBlock* block);
string printVariableDeclaration(SgVariableDeclaration* decl);
string printFunctionDeclaration(SgFunctionDeclaration* decl);
string prettyPrint(SgProject* project);

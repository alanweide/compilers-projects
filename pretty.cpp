#include <rose.h>
using namespace std;

void examineScopeStatement(SgScopeStatement* scope, string name);
void examineVariableDeclaration(SgVariableDeclaration* decl);
void examineFunctionDeclaration(SgFunctionDeclaration* decl);
void examineBasicBlock(SgBasicBlock* block);
void examineExpression(SgExpression* expr);
void examineBinaryOp(SgBinaryOp* expr);

string printType(SgType* type) {
  switch(type->variantT()) {
    case V_SgTypeInt:
      return "int";
    case V_SgTypeLong:
      return "long";
    case V_SgTypeFloat:
      return "float";
    case V_SgTypeDouble:
      return "double";
    default:
      return "[UNHANDLED printType] " + type->unparseToString();
  }
}

string printOperatorForBinaryOp(SgBinaryOp* op) {
  switch(op->variantT()) {
    case V_SgAddOp:
      return " + ";
    case V_SgAndOp:
      return " && ";
    case V_SgAssignOp:
      return " = ";
    case V_SgBitAndOp:
      return " & ";
    case V_SgBitOrOp:
      return " | ";
    case V_SgBitXorOp:
      return " ^ ";

    // Begin Compound Assignments
    case V_SgAndAssignOp:
      return " &= ";
    case V_SgDivAssignOp:
      return " /= ";
    case V_SgIorAssignOp:
      return " |= ";
    case V_SgLshiftAssignOp:
      return " <<= ";
    case V_SgMinusAssignOp:
      return " -= ";
    case V_SgModAssignOp:
      return " %= ";
    case V_SgMultAssignOp:
      return " *= ";
    case V_SgPlusAssignOp:
      return " += ";
    case V_SgRshiftAssignOp:
      return " >>= ";
    case V_SgXorAssignOp:
      return " ^= ";
    // End Compound Assignments

    case V_SgDivideOp:
      return " / ";
    case V_SgEqualityOp:
      return " == ";
    case V_SgGreaterOrEqualOp:
      return " >= ";
    case V_SgGreaterThanOp:
      return " > ";
    case V_SgLessOrEqualOp:
      return " < ";
    case V_SgLessThanOp:
      return " <= ";
    case V_SgLshiftOp:
      return " << ";
    case V_SgModOp:
      return " % ";
    case V_SgMultiplyOp:
      return " * ";
    case V_SgNotEqualOp:
      return " != ";
    case V_SgOrOp:
      return " || ";
    case V_SgRshiftOp:
      return " >> ";
    case V_SgSubtractOp:
      return " - ";
    default:
      return " [UNHANDLED printOperatorForBinaryOp] ";
  }
}

void examineBinaryOp(SgBinaryOp* expr) {
  examineExpression(bi_expr->get_lhs_operand());
  cout << printOperatorForBinaryOp(bi_expr);
  examineExpression(bi_expr->get_rhs_operand());
}

void examineStatement(SgStatement* stmt) {
    switch(stmt->variantT()) {
      case V_SgVariableDeclaration: {
        SgVariableDeclaration* d_stmt = isSgVariableDeclaration(stmt);
        examineVariableDeclaration(d_stmt);
        break;
      }
      case V_SgExprStatement: {
        SgExprStatement* e_stmt = isSgExprStatement(stmt);
        examineExpression(stmt->get_expression());
        cout << ";" << endl;
      }
      default:
        cout << "[UNHANDLED examineStatement] " << stmt->unparseToString() << endl;
    }
}

void examineBasicBlock(SgBasicBlock* block) {
  SgStatementPtrList& stmt_list = block->get_statements();
  SgStatementPtrList::const_iterator iter;
  cout << "{" << endl;
  for (iter=stmt_list.begin(); iter != stmt_list.end(); iter++) {
    SgStatement* stmt = *iter;
    examineStatement(stmt);
  }
  cout << "}" << endl;
}

void examineScopeStatement(SgScopeStatement* scope, string name) {

  // Debug info

  SgSymbolTable* symbol_table = scope->get_symbol_table();
  set<SgNode*> symbol_nodes = symbol_table->get_symbols();
  set<SgNode*>::const_iterator symbol_iter;
  int num_vars = 0;
  for (symbol_iter = symbol_nodes.begin(); 
       symbol_iter != symbol_nodes.end(); 
       ++symbol_iter) {
    SgSymbol* symbol = isSgSymbol(*symbol_iter);
    // cout << "[Scope " << name << "] Symbol: "<<symbol->get_name().getString()<<endl;
    if (isSgVariableSymbol(symbol)) {
      num_vars++;
    }
  }
  // cout << "[Scope " << name << "] Num symbols: " << symbol_nodes.size() << endl;
  // cout << "[Scope " << name << "] Num variable symbols: " << num_vars << endl;

  // Pretty print it

  switch(scope->variantT()) {
    case V_SgBasicBlock: {
      SgBasicBlock* block = isSgBasicBlock(scope);
      examineBasicBlock(block);
      break;
    }
    default:
      cout << "[UNHANDLED examineScopeStatement] " << scope->unparseToString();
  }
}

void examineVariableDeclaration(SgVariableDeclaration* decl) {
  SgInitializedNamePtrList& name_list = decl->get_variables();
  SgInitializedNamePtrList::const_iterator name_iter;
  for (name_iter = name_list.begin(); name_iter != name_list.end(); name_iter++) {
    SgInitializedName* name = *name_iter;
    SgSymbol* symbol = name->get_symbol_from_symbol_table();
    cout << printType(symbol->get_type()) << " " << symbol->get_name().getString();
    SgInitializer* init_expr = name->get_initializer();
    if (init_expr) {
      cout << " = ";
      // examineExpression(init_expr);
      cout << "[INITIALIZER (" << init_expr->unparseToString() << ")]";
    }
    cout << ";" << endl;
  }
}

void examineExpression(SgExpression* expr) {
  switch(expr->variantT()) {
    case V_SgBinaryOp: {
      SgBinaryOp* bi_expr = isSgBinaryOp(expr);
      examineBinaryOp(bi_expr);
      break;
    }
    case V_SgIntVal: {
      SgIntVal* v_exp = isSgIntVal(expr);
      cout << v_exp->get_valueString();
      break;
    }
    case V_SgLongIntVal: {
      SgLongIntVal* v_exp = isSgLongIntVal(expr);
      cout << v_exp->get_valueString() << "L";
      break;
    }
    case V_SgFloatVal: {
      SgFloatVal* v_exp = isSgFloatVal(expr);
      cout << v_exp->get_valueString() << "F";
      break;
    }
    case V_SgDoubleVal: {
      SgDoubleVal* v_exp = isSgDoubleVal(expr);
      cout << v_exp->get_valueString();
      break;
    }
    case V_SgAssignInitializer: {
      SgAssignInitializer* init_expr = isSgAssignInitializer(expr);
      examineExpression(init_expr->get_operand());
      break;
    }
    default:
      cout << "[UNHANDLED examineExpression (" << expr->class_name() << ")] " << expr->unparseToString();
  }
}

void examineFunctionDeclaration(SgFunctionDeclaration* decl) {
  SgSymbol* symbol = decl->get_symbol_from_symbol_table();
  SgFunctionDefinition* def = decl->get_definition();
  if (def) {
    SgFunctionDeclaration* f_decl = def->get_declaration();
    cout << printType(f_decl->get_orig_return_type()) << " " << f_decl->get_name().getString() << "()" << endl;

    // TODO: parameter list

    SgBasicBlock* body = def->get_body();
    SgStatementPtrList& stmt_list = body->get_statements();
    // cout << "[Func] - " << stmt_list.size() << " statements" << endl;
    // An SgBasicBlock is a subclass of SgScopeStatement; process the symbol table for this scope
    examineScopeStatement(body,symbol->get_name().getString());
  } else if (symbol) {
    cout << symbol->get_type()->class_name() << " " << symbol->get_name().getString() << "();" << endl;
  }
}

string prettyPrint(SgProject* project) {
  SgFilePtrList& file_list = project->get_fileList();
  SgFilePtrList::const_iterator file_iter;
  for (file_iter = file_list.begin(); file_iter != file_list.end(); file_iter++) {
    SgSourceFile* file = isSgSourceFile(*file_iter);
    cout << "[Print] File: " << file->getFileName() << endl;

    // process the symbol table at the global scope; SgGlobal is a
    // subclass of SgScopeStatement
    SgGlobal* global_scope = file->get_globalScope(); 
    examineScopeStatement(global_scope,"global");

    // get the actual statements that are in this global scope
    SgDeclarationStatementPtrList& decl_list = global_scope->get_declarations();
    SgDeclarationStatementPtrList::const_iterator decl_iter;
    for(decl_iter = decl_list.begin(); decl_iter != decl_list.end(); decl_iter++) {
      SgDeclarationStatement* decl = *decl_iter;
      if (isSgFunctionDeclaration(decl)) {
        examineFunctionDeclaration(isSgFunctionDeclaration(decl));
      }
      if (isSgVariableDeclaration(decl)) {
        examineVariableDeclaration(isSgVariableDeclaration(decl));
      }
    }
  }
  return "\nnot implemented yet!\n";
}

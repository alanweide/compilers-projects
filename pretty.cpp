#include <rose.h>
using namespace std;

string printScopeStatement(SgScopeStatement* scope, string name);
string printVariableDeclaration(SgVariableDeclaration* decl);
string printFunctionDeclaration(SgFunctionDeclaration* decl);
string printBasicBlock(SgBasicBlock* block);
string printExpression(SgExpression* expr);
string printBinaryOp(SgBinaryOp* expr);

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
    case V_SgArrayType:
      return type->get_base_type() + "[" + printExpression(type->get_index()) + "]";
    default:
      return "[UNHANDLED printType " + type->class_name() + "] " + type->unparseToString();
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

string printBinaryOp(SgBinaryOp* expr) {
  string output = "";
  output = output + printExpression(expr->get_lhs_operand());
  output = output + printOperatorForBinaryOp(expr);
  output = output + printExpression(expr->get_rhs_operand());
  return output;
}

string printStatement(SgStatement* stmt) {
  string output = "";
    switch(stmt->variantT()) {
      case V_SgVariableDeclaration: {
        SgVariableDeclaration* d_stmt = isSgVariableDeclaration(stmt);
        output = output + printVariableDeclaration(d_stmt);
        break;
      }
      case V_SgExprStatement: {
        SgExprStatement* e_stmt = isSgExprStatement(stmt);
        output = output + printExpression(e_stmt->get_expression());
        output = output + ";\n";
      }
      default:
        output = output + "[UNHANDLED examineStatement] " + stmt->unparseToString() + "\n";
    }
    return output;
}

string printBasicBlock(SgBasicBlock* block) {
  string output = "";
  SgStatementPtrList& stmt_list = block->get_statements();
  SgStatementPtrList::const_iterator iter;
  output = output + "{\n";
  for (iter=stmt_list.begin(); iter != stmt_list.end(); iter++) {
    SgStatement* stmt = *iter;
    output = output + printStatement(stmt);
  }
  output = output + "}\n";
  return output;
}

string printScopeStatement(SgScopeStatement* scope, string name) {
  string output = "";

  // Debug info

  // SgSymbolTable* symbol_table = scope->get_symbol_table();
  // set<SgNode*> symbol_nodes = symbol_table->get_symbols();
  // set<SgNode*>::const_iterator symbol_iter;
  // int num_vars = 0;
  // for (symbol_iter = symbol_nodes.begin(); 
  //      symbol_iter != symbol_nodes.end(); 
  //      ++symbol_iter) {
  //   SgSymbol* symbol = isSgSymbol(*symbol_iter);
  //   // cout << "[Scope " << name << "] Symbol: "<<symbol->get_name().getString()<<endl;
  //   if (isSgVariableSymbol(symbol)) {
  //     num_vars++;
  //   }
  // }
  // cout << "[Scope " << name << "] Num symbols: " << symbol_nodes.size() << endl;
  // cout << "[Scope " << name << "] Num variable symbols: " << num_vars << endl;

  // Pretty print it

  switch(scope->variantT()) {
    case V_SgBasicBlock: {
      SgBasicBlock* block = isSgBasicBlock(scope);
      output = output + printBasicBlock(block);
      break;
    }
    default:
      output = output + "[UNHANDLED examineScopeStatement] " << scope->unparseToString();
  }
  return output;
}

string printVariableDeclaration(SgVariableDeclaration* decl) {
  string output = "";
  SgInitializedNamePtrList& name_list = decl->get_variables();
  SgInitializedNamePtrList::const_iterator name_iter;
  for (name_iter = name_list.begin(); name_iter != name_list.end(); name_iter++) {
    SgInitializedName* name = *name_iter;
    SgSymbol* symbol = name->get_symbol_from_symbol_table();
    output = output + printType(symbol->get_type()) + " " + symbol->get_name().getString();
    SgInitializer* init_expr = name->get_initializer();
    if (init_expr) {
      output = output + " = " + printExpression(init_expr);
    }
    output = output + ";\n";
  }
  return output;
}

string printExpression(SgExpression* expr) {
  string output = "";
  switch(expr->variantT()) {
    case V_SgBinaryOp: {
      SgBinaryOp* bi_expr = isSgBinaryOp(expr);
      output = output + printBinaryOp(bi_expr);
      break;
    }
    case V_SgIntVal: {
      SgIntVal* v_exp = isSgIntVal(expr);
      output = output + v_exp->get_value();
      break;
    }
    case V_SgLongIntVal: {
      SgLongIntVal* v_exp = isSgLongIntVal(expr);
      output = output + v_exp->get_value() << "L";
      break;
    }
    case V_SgFloatVal: {
      SgFloatVal* v_exp = isSgFloatVal(expr);
      output = output + v_exp->get_value() << "F";
      break;
    }
    case V_SgDoubleVal: {
      SgDoubleVal* v_exp = isSgDoubleVal(expr);
      output = output + v_exp->get_value();
      break;
    }
    case V_SgAssignInitializer: {
      SgAssignInitializer* init_expr = isSgAssignInitializer(expr);
      output = output + printExpression(init_expr->get_operand());
      break;
    }
    default:
      output = output + "[UNHANDLED examineExpression (" << expr->class_name() << ")] " + expr->unparseToString();
  }
  return output;
}

string printFunctionDeclaration(SgFunctionDeclaration* decl) {
  string output = "";
  SgSymbol* symbol = decl->get_symbol_from_symbol_table();
  SgFunctionDefinition* def = decl->get_definition();
  if (def) {
    SgFunctionDeclaration* f_decl = def->get_declaration();
    output = output + printType(f_decl->get_orig_return_type()) + " " + f_decl->get_name().getString() + "()";

    // // TODO: parameter list
    // cout << "(";
    // SgInitializedNamePtrList& params = f_decl->get_args();
    // SgInitializedNamePtrList::const_iterator param_iter;
    // int firstOne = 1;
    // for (param_iter = params.begin(); param_iter != params.end(); param_iter++) {
    //   SgInitializedName* param = *param_iter;
    //   if (!firstOne) {
    //     cout << ", ";
    //     firstOne = 0;
    //   }
    //   cout << printType(param->get_type()) << " " << param->get_name().getString();
    // }
    // cout << ")" << endl;

    SgBasicBlock* body = def->get_body();
    SgStatementPtrList& stmt_list = body->get_statements();
    // cout << "[Func] - " << stmt_list.size() << " statements" << endl;
    // An SgBasicBlock is a subclass of SgScopeStatement; process the symbol table for this scope
    output = output + printScopeStatement(body,symbol->get_name().getString());
  } else if (symbol) {
    output = output + symbol->get_type()->class_name() + " " + symbol->get_name().getString() + "();\n";
  }
  return output;
}

string prettyPrint(SgProject* project) {
  string output = "";
  SgFilePtrList& file_list = project->get_fileList();
  SgFilePtrList::const_iterator file_iter;
  for (file_iter = file_list.begin(); file_iter != file_list.end(); file_iter++) {
    SgSourceFile* file = isSgSourceFile(*file_iter);
    cout << "[Print] File: " << file->getFileName() << endl;

    // process the symbol table at the global scope; SgGlobal is a
    // subclass of SgScopeStatement
    SgGlobal* global_scope = file->get_globalScope(); 
    output = output + printScopeStatement(global_scope,"global");

    // get the actual statements that are in this global scope
    SgDeclarationStatementPtrList& decl_list = global_scope->get_declarations();
    SgDeclarationStatementPtrList::const_iterator decl_iter;
    for(decl_iter = decl_list.begin(); decl_iter != decl_list.end(); decl_iter++) {
      SgDeclarationStatement* decl = *decl_iter;
      if (isSgFunctionDeclaration(decl)) {
        output = output + printFunctionDeclaration(isSgFunctionDeclaration(decl));
      }
      if (isSgVariableDeclaration(decl)) {
        output = output + printVariableDeclaration(isSgVariableDeclaration(decl));
      }
    }
  }
  return output;
}

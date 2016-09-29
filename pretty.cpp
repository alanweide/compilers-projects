#include <rose.h>
using namespace std;

string printScopeStatement(SgScopeStatement* scope);
string printVariableDeclaration(SgVariableDeclaration* decl);
string printFunctionDeclaration(SgFunctionDeclaration* decl);
string printBasicBlock(SgBasicBlock* block);
string printExpression(SgExpression* expr);
string printBinaryOp(SgBinaryOp* expr);
string printForStmt(SgForStatement* for_stmt);
string printIfStmt(SgIfStmt* stmt);

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
    case V_SgArrayType: {
      SgArrayType* a_type = isSgArrayType(type);
      return printType(a_type->get_base_type()) + "[" + printExpression(a_type->get_index()) + "]";
    }
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
  string output = printExpression(expr->get_lhs_operand());
  output = output + printOperatorForBinaryOp(expr);
  output = output + printExpression(expr->get_rhs_operand());
  return output;
}

string printStatement(SgStatement* stmt) {
  string output = "";
    switch(stmt->variantT()) {
      case V_SgVariableDeclaration: {
        SgVariableDeclaration* d_stmt = isSgVariableDeclaration(stmt);
        output = printVariableDeclaration(d_stmt) + ";\n";
        break;
      }
      case V_SgExprStatement: {
        SgExprStatement* e_stmt = isSgExprStatement(stmt);
        output = printExpression(e_stmt->get_expression()) + ";\n";
        break;
      }
      case V_SgReturnStmt: {
        SgReturnStmt* r_stmt = isSgReturnStmt(stmt);
        output = "return " + printExpression(r_stmt->get_expression()) + ";\n";
        break;
      }
      case V_SgIfStmt: {
        SgIfStmt* i_stmt = isSgIfStmt(stmt);
        output = printIfStmt(i_stmt);
        break;
      }
      case V_SgWhileStmt: {
        SgWhileStmt* while_stmt = isSgWhileStmt(stmt);
        SgExprStatement* the_test = isSgExprStatement(while_stmt->get_condition());
        SgStatement* the_body = while_stmt->get_body();
        output = "while(" + printStatement(the_test) + ")\n" + printStatement(the_body);
        break;
      }
      case V_SgDoWhileStmt: {
        SgWhileStmt* while_stmt = isSgWhileStmt(stmt);
        SgExprStatement* the_test = isSgExprStatement(while_stmt->get_condition());
        SgStatement* the_body = while_stmt->get_body();
        output = "do\n" + printStatement(the_body) + "while (" + printStatement(the_test) + ");\n";
        break;
      }
      case V_SgForStatement: {
        SgForStatement* for_stmt = isSgForStatement(stmt);
        output = printForStmt(for_stmt);
        break;
      }
      case V_SgBasicBlock: {
        SgBasicBlock* block = isSgBasicBlock(stmt);
        output = printScopeStatement(block);
        break;
      }
      default:
        output = "[UNHANDLED printStatement] " + stmt->unparseToString() + ";\n";
    }
    return output;
}

string printForStmt(SgForStatement* for_stmt) {
  string output = "";
  SgForInitStatement* init_stmt = for_stmt->get_for_init_stmt();
  SgStatementPtrList& init_stmt_list = init_stmt->get_init_stmt();
  SgExprStatement* the_init = isSgExprStatement(*init_stmt_list.begin());
  SgExprStatement* the_test = isSgExprStatement(for_stmt->get_test());
  SgExpression* the_incr = for_stmt->get_increment();
  SgStatement* the_body = for_stmt->get_loop_body();
  output = "for (" + printExpression(the_init->get_expression()) + "; ";
  output = output + printExpression(the_test->get_expression()) + "; ";
  output = output + printExpression(the_incr) + ")\n";
  output = output + printStatement(the_body);
  return output;
}

string printIfStmt(SgIfStmt* stmt) {
  string output = "";
  SgExprStatement* condition = isSgExprStatement(stmt->get_conditional());
  SgStatement* true_body = isSgStatement(stmt->get_true_body());
  SgStatement* false_body = isSgStatement(stmt->get_false_body());
  output = "if (" + printStatement(condition) + ")\n" + printStatement(true_body);
  if (false_body) {
    output = output + "else\n" + printStatement(false_body);
  }
  return output;
}

string printBasicBlock(SgBasicBlock* block) {
  string output = "";
  SgStatementPtrList& stmt_list = block->get_statements();
  SgStatementPtrList::const_iterator iter;
  output = "{\n";
  for (iter=stmt_list.begin(); iter != stmt_list.end(); iter++) {
    SgStatement* stmt = *iter;
    output = output + printStatement(stmt);
  }
  output = output + "}\n";
  return output;
}

string printScopeStatement(SgScopeStatement* scope) {
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
      output = printBasicBlock(block);
      break;
    }
    default:
      output = "[UNHANDLED printScopeStatement] " + scope->unparseToString();
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
  }
  return output;
}

string printExpression(SgExpression* expr) {
  string output = "";
  switch(expr->variantT()) {
    case V_SgAddOp:
    case V_SgAndOp:
    case V_SgAssignOp:
    case V_SgBitAndOp:
    case V_SgBitOrOp:
    case V_SgBitXorOp:
    case V_SgAndAssignOp:
    case V_SgDivAssignOp:
    case V_SgIorAssignOp:
    case V_SgLshiftAssignOp:
    case V_SgMinusAssignOp:
    case V_SgModAssignOp:
    case V_SgMultAssignOp:
    case V_SgPlusAssignOp:
    case V_SgRshiftAssignOp:
    case V_SgXorAssignOp:
    case V_SgDivideOp:
    case V_SgEqualityOp:
    case V_SgGreaterOrEqualOp:
    case V_SgGreaterThanOp:
    case V_SgLessOrEqualOp:
    case V_SgLessThanOp:
    case V_SgLshiftOp:
    case V_SgModOp:
    case V_SgMultiplyOp:
    case V_SgNotEqualOp:
    case V_SgOrOp:
    case V_SgRshiftOp:
    case V_SgSubtractOp: {
      SgBinaryOp* bi_expr = isSgBinaryOp(expr);
      output = printBinaryOp(bi_expr);
      break;
    }
    case V_SgIntVal: {
      SgIntVal* v_exp = isSgIntVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = convert.str();
      break;
    }
    case V_SgLongIntVal: {
      SgLongIntVal* v_exp = isSgLongIntVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = convert.str() + "L";
      break;
    }
    case V_SgUnsignedLongVal: {
      SgUnsignedLongVal* v_exp = isSgUnsignedLongVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = convert.str() + "UL";
      break;
    }
    case V_SgFloatVal: {
      SgFloatVal* v_exp = isSgFloatVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = convert.str() + "F";
      break;
    }
    case V_SgDoubleVal: {
      SgDoubleVal* v_exp = isSgDoubleVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = convert.str();
      break;
    }
    case V_SgAssignInitializer: {
      SgAssignInitializer* init_expr = isSgAssignInitializer(expr);
      output = printExpression(init_expr->get_operand());
      break;
    }
    case V_SgVarRefExp: {
      SgVarRefExp* v_exp = isSgVarRefExp(expr);
      output = v_exp->get_symbol()->get_name().getString();
      break;
    }
    // case V_SgPntrArrRefExp: {
    //   SgPntrArrRefExp* p_exp = isSgPntrArrRefExp(expr);
    //   output = printExpression(p_exp->get_lhs_operand()) + "[" + printExpression(p_exp->get_rhs_operand()) = "]";
    //   break;
    // }
    case V_SgMinusOp: {
      SgMinusOp* m_exp = isSgMinusOp(expr);
      output = "-" + printExpression(m_exp->get_operand());
      break;
    }
    case V_SgUnaryAddOp: {
      SgUnaryAddOp* a_exp = isSgUnaryAddOp(expr);
      output = "+" + printExpression(a_exp->get_operand());
      break;
    }
    case V_SgPlusPlusOp: {
      SgPlusPlusOp* p_exp = isSgPlusPlusOp(expr);
      if (p_exp->get_mode() == SgUnaryOp::prefix) {
        output = "++" + printExpression(p_exp->get_operand());
      } else {
        output = printExpression(p_exp->get_operand()) + "++";
      }
      break;
    }
    case V_SgMinusMinusOp: {
      SgMinusMinusOp* p_exp = isSgMinusMinusOp(expr);
      if (p_exp->get_mode() == SgUnaryOp::prefix) {
        output = "--" + printExpression(p_exp->get_operand());
      } else {
        output = printExpression(p_exp->get_operand()) + "--";
      }
      break;
    }
    default:
      output = "/**/" + expr->unparseToString();
  }
  return output;
}

string printFunctionDeclaration(SgFunctionDeclaration* decl) {
  string output = "";
  SgSymbol* symbol = decl->get_symbol_from_symbol_table();
  SgFunctionDefinition* def = decl->get_definition();
  if (def) {
    SgFunctionDeclaration* f_decl = def->get_declaration();
    output = printType(f_decl->get_orig_return_type()) + " " + f_decl->get_name().getString() + "()";

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
    // output = output + printScopeStatement(body,symbol->get_name().getString());
    output = output + printScopeStatement(body);
  } else if (symbol) {
    output = symbol->get_type()->class_name() + " " + symbol->get_name().getString() + "();\n";
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
    // output = output + printScopeStatement(global_scope,"global");

    // get the actual statements that are in this global scope
    SgDeclarationStatementPtrList& decl_list = global_scope->get_declarations();
    SgDeclarationStatementPtrList::const_iterator decl_iter;
    for(decl_iter = decl_list.begin(); decl_iter != decl_list.end(); decl_iter++) {
      SgDeclarationStatement* decl = *decl_iter;
      if (isSgFunctionDeclaration(decl)) {
        output = output + printFunctionDeclaration(isSgFunctionDeclaration(decl));
      }
      if (isSgVariableDeclaration(decl)) {
        output = output + printVariableDeclaration(isSgVariableDeclaration(decl)) + ";\n";
      }
    }
  }
  return output;
}

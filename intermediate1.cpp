#include <rose.h>
#include <sstream>
using namespace std;

int curTemp = 0;

struct ExpressionNode
{
  string code;
  string addr;
}; 

ostringstream tempVars;

string newTemp();

string printExpression(SgExpression* expr);
ExpressionNode translatedExpression(SgExpression* expr);
string printType(SgType* type);
string printOperatorForBinaryOp(SgBinaryOp* op);
string printOperatorForUnaryOp(SgUnaryOp* op);
string printBinaryOp(SgBinaryOp* expr);
ExpressionNode translatedBinaryOp(SgBinaryOp* expr);
ExpressionNode translatedPntrArrRefExp(SgPntrArrRefExp* expr);
ExpressionNode translatedUnaryOp(SgUnaryOp* expr);
string printStatement(SgStatement* stmt);
string printForStmt(SgForStatement* for_stmt);
string printWhileStmt(SgWhileStmt* while_stmt);
string printDoWhileStmt(SgDoWhileStmt* dow_stmt);
string printIfStmt(SgIfStmt* stmt);
string printBasicBlock(SgBasicBlock* block);
string printVariableDeclaration(SgVariableDeclaration* decl);
string printArrayDec(SgSymbol *symbol);
string printFunctionDeclaration(SgFunctionDeclaration* decl);
string prettyPrint(SgProject* project);

string newTemp(SgType* type) {
  curTemp++;
  ostringstream output;
  output << "_t" << curTemp;
  tempVars << printType(type) << " " << output.str() << ";\n";
  return output.str();
}

string printExpression(SgExpression* expr) {
  string output = "";
  if (expr->get_need_paren()) {
    output = output + "(";
  }
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
      output = output + printBinaryOp(bi_expr);
      break;
    }
    case V_SgIntVal: {
      SgIntVal* v_exp = isSgIntVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = output + convert.str();
      break;
    }
    case V_SgLongIntVal: {
      SgLongIntVal* v_exp = isSgLongIntVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = output + convert.str() + "L";
      break;
    }
    case V_SgUnsignedLongVal: {
      SgUnsignedLongVal* v_exp = isSgUnsignedLongVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = output + convert.str() + "UL";
      break;
    }
    case V_SgFloatVal: {
      SgFloatVal* v_exp = isSgFloatVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = output + convert.str() + "F";
      break;
    }
    case V_SgDoubleVal: {
      SgDoubleVal* v_exp = isSgDoubleVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output = output + convert.str();
      break;
    }
    case V_SgAssignInitializer: {
      SgAssignInitializer* init_expr = isSgAssignInitializer(expr);
      output = output + printExpression(init_expr->get_operand());
      break;
    }
    case V_SgVarRefExp: {
      SgVarRefExp* v_exp = isSgVarRefExp(expr);
      output = output + v_exp->get_symbol()->get_name().getString();
      break;
    }
    case V_SgPntrArrRefExp: {
      SgPntrArrRefExp* p_exp = isSgPntrArrRefExp(expr);
      output = output + printExpression(p_exp->get_lhs_operand()) + "[" + printExpression(p_exp->get_rhs_operand()) + "]";
      break;
    }
    case V_SgPointerDerefExp: {
      SgPointerDerefExp* p_exp = isSgPointerDerefExp(expr);
      output = output + "*" + printExpression(p_exp->get_operand());
      break;
    }
    case V_SgMinusOp: {
      SgMinusOp* m_exp = isSgMinusOp(expr);
      output = output + "-" + printExpression(m_exp->get_operand());
      break;
    }
    case V_SgUnaryAddOp: {
      SgUnaryAddOp* a_exp = isSgUnaryAddOp(expr);
      output = output + "+" + printExpression(a_exp->get_operand());
      break;
    }
    case V_SgBitComplementOp: {
      SgBitComplementOp* b_exp = isSgBitComplementOp(expr);
      output = output + "~" + printExpression(b_exp->get_operand());
      break;
    }
    case V_SgAddressOfOp: {
      SgAddressOfOp* a_exp = isSgAddressOfOp(expr);
      output = output + "&" + printExpression(a_exp->get_operand());
      break;
    }
    case V_SgPlusPlusOp: {
      SgPlusPlusOp* p_exp = isSgPlusPlusOp(expr);
      if (p_exp->get_mode() == SgUnaryOp::prefix) {
        output = output + "++" + printExpression(p_exp->get_operand());
      } else {
        output = output + printExpression(p_exp->get_operand()) + "++";
      }
      break;
    }
    case V_SgMinusMinusOp: {
      SgMinusMinusOp* p_exp = isSgMinusMinusOp(expr);
      if (p_exp->get_mode() == SgUnaryOp::prefix) {
        output = output + "--" + printExpression(p_exp->get_operand());
      } else {
        output = output + printExpression(p_exp->get_operand()) + "--";
      }
      break;
    }
    default:
      output = output + "/*UNHANDLED " + expr->class_name() + "*/\n";// + expr->unparseToString();
      break;
  }
  if (expr->get_need_paren()) {
    output = output + ")";
  }
  return output;
}

ExpressionNode translatedExpression(SgExpression* expr) {
  ExpressionNode output;
  switch(expr->variantT()) {
    case V_SgEqualityOp:
    case V_SgGreaterOrEqualOp:
    case V_SgGreaterThanOp:
    case V_SgLessOrEqualOp:
    case V_SgLessThanOp:
    case V_SgNotEqualOp:

    case V_SgAddOp:
    case V_SgAndOp:
    case V_SgBitAndOp:
    case V_SgBitOrOp:
    case V_SgBitXorOp:
    case V_SgDivideOp:
    case V_SgLshiftOp:
    case V_SgModOp:
    case V_SgMultiplyOp:
    case V_SgOrOp:
    case V_SgRshiftOp:
    case V_SgSubtractOp: {
      SgBinaryOp* bi_expr = isSgBinaryOp(expr);
      output = translatedBinaryOp(bi_expr);
      break;
    }
    case V_SgAssignOp:
    case V_SgAndAssignOp:
    case V_SgDivAssignOp:
    case V_SgIorAssignOp:
    case V_SgLshiftAssignOp:
    case V_SgMinusAssignOp:
    case V_SgModAssignOp:
    case V_SgMultAssignOp:
    case V_SgPlusAssignOp:
    case V_SgRshiftAssignOp:
    case V_SgXorAssignOp: {
      SgBinaryOp* bi_expr = isSgBinaryOp(expr);
      output.addr = printExpression(bi_expr->get_lhs_operand());
      ExpressionNode rhs_e = translatedExpression(bi_expr->get_rhs_operand());
      string op = printOperatorForBinaryOp(bi_expr);
      output.code = rhs_e.code + output.addr + op + rhs_e.addr + ";\n";
      break;
    }
    case V_SgIntVal: {
      SgIntVal* v_exp = isSgIntVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output.addr = convert.str();
      output.code = "";
      break;
    }
    case V_SgLongIntVal: {
      SgLongIntVal* v_exp = isSgLongIntVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output.addr = convert.str() + "L";
      output.code = "";
      break;
    }
    case V_SgUnsignedLongVal: {
      SgUnsignedLongVal* v_exp = isSgUnsignedLongVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output.addr = convert.str() + "UL";
      output.code = "";
      break;
    }
    case V_SgFloatVal: {
      SgFloatVal* v_exp = isSgFloatVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output.addr = convert.str() + "F";
      output.code = "";
      break;
    }
    case V_SgDoubleVal: {
      SgDoubleVal* v_exp = isSgDoubleVal(expr);
      ostringstream convert;
      convert << v_exp->get_value();
      output.addr = convert.str();
      output.code = "";
      break;
    }
    case V_SgAssignInitializer: {
      SgAssignInitializer* init_expr = isSgAssignInitializer(expr);
      output.code = printExpression(init_expr->get_operand());
      break;
    }
    case V_SgVarRefExp: {
      SgVarRefExp* v_exp = isSgVarRefExp(expr);
      output.addr = v_exp->get_symbol()->get_name().getString();
      output.code = "";
      break;
    }
    case V_SgPntrArrRefExp: {
      SgPntrArrRefExp* p_exp = isSgPntrArrRefExp(expr);
      output = translatedPntrArrRefExp(p_exp);
      break;
    }
    case V_SgMinusOp:
    case V_SgUnaryAddOp:
    case V_SgBitComplementOp: {
      SgUnaryOp* u_exp = isSgUnaryOp(expr);
      output = translatedUnaryOp(u_exp);
    }
    case V_SgPlusPlusOp: {
      SgPlusPlusOp* p_exp = isSgPlusPlusOp(expr);
      if (p_exp->get_mode() == SgUnaryOp::prefix) {
        output.code = "++" + printExpression(p_exp->get_operand());
      } else {
        output.code = printExpression(p_exp->get_operand()) + "++";
      }
      break;
    }
    case V_SgMinusMinusOp: {
      SgMinusMinusOp* p_exp = isSgMinusMinusOp(expr);
      if (p_exp->get_mode() == SgUnaryOp::prefix) {
        output.code = "--" + printExpression(p_exp->get_operand());
      } else {
        output.code = printExpression(p_exp->get_operand()) + "--";
      }
      break;
    }
    default:
      output.code = "/*UNHANDLED " + expr->class_name() + "*/\n";// + expr->unparseToString();
      break;
  }
  return output;
}

string printType(SgType* type) {
  switch(type->variantT()) {
    case V_SgTypeVoid:
      return "void";
    case V_SgTypeInt:
      return "int";
    case V_SgTypeLong:
      return "long";
    case V_SgTypeFloat:
      return "float";
    case V_SgTypeDouble:
      return "double";
    case V_SgPointerType: {
      SgPointerType* p_type = isSgPointerType(type);
      return printType(p_type->get_base_type()) + "*";
    }
    default:
      return "/*UNHANDLED " + type->class_name() + "*/\n";// + type->unparseToString();
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
      return " <= ";
    case V_SgLessThanOp:
      return " < ";
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
      return " /*UNHANDLED binary operator*/, ";
  }
}

string printBinaryOp(SgBinaryOp* expr) {
  string output = printExpression(expr->get_lhs_operand());
  output = output + printOperatorForBinaryOp(expr);
  output = output + printExpression(expr->get_rhs_operand());
  return output;
}

ExpressionNode translatedBinaryOp(SgBinaryOp* expr) {
  ExpressionNode lhs = translatedExpression(expr->get_lhs_operand());
  ExpressionNode rhs = translatedExpression(expr->get_rhs_operand());
  ExpressionNode out;
  out.addr = newTemp(expr->get_type());
  out.code = lhs.code + rhs.code;
  out.code = out.code + out.addr + " = " + lhs.addr + printOperatorForBinaryOp(expr) + rhs.addr + ";\n";
  return out;
}

ExpressionNode translatedPntrArrRefExp(SgPntrArrRefExp* expr) {
  ExpressionNode lhs = translatedExpression(expr->get_lhs_operand());
  ExpressionNode rhs = translatedExpression(expr->get_rhs_operand());
  ExpressionNode out;
  out.addr = newTemp(expr->get_type());
  out.code = rhs.code;
  out.code = out.code + out.addr + " = " + lhs.addr + "[" + rhs.addr + "]";
  return out;
}

string printOperatorForUnaryOp(SgUnaryOp* op) {
  switch (op->variantT()) {
    case V_SgMinusOp:
      return "-";
    case V_SgUnaryAddOp:
      return "+";
    case V_SgBitComplementOp:
      return "~";
  }
}

ExpressionNode translatedUnaryOp(SgUnaryOp* expr) {
  ExpressionNode output;
  ExpressionNode op = translatedExpression(expr->get_operand());
  output.addr = newTemp(expr->get_type());
  string oper = printOperatorForUnaryOp(expr);
  output.code = op.code + output.addr + " = " + oper + op.addr;
  return output;
}

string printStatement(SgStatement* stmt) {
  string output = "";
    switch(stmt->variantT()) {
      case V_SgVariableDeclaration: {
        SgVariableDeclaration* d_stmt = isSgVariableDeclaration(stmt);
        output = output + printVariableDeclaration(d_stmt) + ";\n";
        break;
      }
      case V_SgExprStatement: {
        SgExprStatement* e_stmt = isSgExprStatement(stmt);
        ExpressionNode e = translatedExpression(e_stmt->get_expression());
        output = output + e.code;
        break;
      }
      case V_SgReturnStmt: {
        SgReturnStmt* r_stmt = isSgReturnStmt(stmt);
        ExpressionNode e = translatedExpression(r_stmt->get_expression());
        output = output + e.code;
        output = output + "return " + e.addr + ";\n";
        break;
      }
      case V_SgIfStmt: {
        SgIfStmt* i_stmt = isSgIfStmt(stmt);
        output = output + printIfStmt(i_stmt);
        break;
      }
      case V_SgWhileStmt: {
        SgWhileStmt* while_stmt = isSgWhileStmt(stmt);
        output = output + printWhileStmt(while_stmt);
        break;
      }
      case V_SgDoWhileStmt: {
        SgDoWhileStmt* while_stmt = isSgDoWhileStmt(stmt);
        output = output + printDoWhileStmt(while_stmt);
        break;
      }
      case V_SgForStatement: {
        SgForStatement* for_stmt = isSgForStatement(stmt);
        output = output + printForStmt(for_stmt);
        break;
      }
      case V_SgBasicBlock: {
        SgBasicBlock* block = isSgBasicBlock(stmt);
        output = output + printBasicBlock(block);
        break;
      }
      default:
        output = "/*UNHANDLED " + stmt->class_name() + "*/\n";// + stmt->unparseToString() + ";\n";
        break;
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
  output = output + "for (" + printExpression(the_init->get_expression()) + "; ";
  output = output + printExpression(the_test->get_expression()) + "; ";
  output = output + printExpression(the_incr) + ")\n";
  output = output + printStatement(the_body);
  return output;
}

string printWhileStmt(SgWhileStmt* while_stmt) {
  string output = "";
  SgExprStatement* the_test = isSgExprStatement(while_stmt->get_condition());
  SgStatement* the_body = while_stmt->get_body();
  output = output + "while(" + printExpression(the_test->get_expression()) + ")\n";
  output = output + printStatement(the_body);
  return output;
}

string printDoWhileStmt(SgDoWhileStmt* dow_stmt) {
  string output = "";
  SgExprStatement* the_test = isSgExprStatement(dow_stmt->get_condition());
  SgStatement* the_body = dow_stmt->get_body();
  output = output + "do\n" + printStatement(the_body);
  output = output + "while (" + printExpression(the_test->get_expression()) + ");\n";
  return output;
}

string printIfStmt(SgIfStmt* stmt) {
  string output = "";
  SgExprStatement* condition = isSgExprStatement(stmt->get_conditional());
  SgStatement* true_body = isSgStatement(stmt->get_true_body());
  SgStatement* false_body = isSgStatement(stmt->get_false_body());
  output = output + "if (" + printExpression(condition->get_expression()) + ")\n";
  output = output + printStatement(true_body);
  if (false_body) {
    output = output + "else\n";
    output = output + printStatement(false_body);
  }
  return output;
}

string printBasicBlock(SgBasicBlock* block) {
  string output = "{\n";
  SgStatementPtrList& stmt_list = block->get_statements();
  SgStatementPtrList::const_iterator iter;
  for (iter=stmt_list.begin(); iter != stmt_list.end(); iter++) {
    SgStatement* stmt = *iter;
    output = output + printStatement(stmt);
  }
  output = output + "}\n";
  return output;
}

string printVariableDeclaration(SgVariableDeclaration* decl) {
  string output = "";
  SgInitializedNamePtrList& name_list = decl->get_variables();
  SgInitializedNamePtrList::const_iterator name_iter;
  for (name_iter = name_list.begin(); name_iter != name_list.end(); name_iter++) {
    SgInitializedName* name = *name_iter;
    SgSymbol* symbol = name->get_symbol_from_symbol_table();
    SgType *type = symbol->get_type();
    if (isSgArrayType(type)) {
      output = output + printArrayDec(symbol);
    } else {
      output = output + printType(symbol->get_type()) + " " + symbol->get_name().getString();
    }
    SgInitializer* init_expr = name->get_initializer();
    if (init_expr) {
      output = output + " = " + printExpression(init_expr);
    }
  }
  return output;
}

string printArrayDec(SgSymbol* symbol) {
  string output = "";

  SgType *baseType = symbol->get_type();
  SgArrayType *a_type;
  while (a_type = isSgArrayType(baseType)) {
    output = output + "[" + printExpression(a_type->get_index()) + "]";
    baseType = a_type->get_base_type();
  }

  return printType(baseType) + " " + symbol->get_name().getString() + output;
}

string printFunctionDeclaration(SgFunctionDeclaration* decl) {
  string output = "";
  SgSymbol* symbol = decl->get_symbol_from_symbol_table();
  SgFunctionDefinition* def = decl->get_definition();
  if (def) {
    SgFunctionDeclaration* f_decl = def->get_declaration();
    output = printType(f_decl->get_orig_return_type()) + " " + f_decl->get_name().getString() + "()\n";

    SgBasicBlock* body = def->get_body();
    SgStatementPtrList& stmt_list = body->get_statements();
    string fullBody = printBasicBlock(body);
    output = output + tempVars.str() + fullBody;
    tempVars.str("");
  } else if (symbol) {
    // output = "// Function " + symbol->get_name().getString() + " has no body; assuming a builtin or included function.\n";
  } else {
    // output = "// Function has no symbol!\n";
  }
  return output;
}

string prettyPrint(SgProject* project) {
  string output = "";
  SgFilePtrList& file_list = project->get_fileList();
  SgFilePtrList::const_iterator file_iter;
  for (file_iter = file_list.begin(); file_iter != file_list.end(); file_iter++) {
    SgSourceFile* file = isSgSourceFile(*file_iter);
    // cout << "[Print] File: " << file->getFileName() << endl;

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

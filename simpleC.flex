import java_cup.runtime.*;

%%

%public
%class Scanner
%implements sym

%line
%column

%cup
%cupdebug

%{
  StringBuffer string = new StringBuffer();
  
  private Symbol symbol(int type) {
    return new MySymbol(type, yyline+1, yycolumn+1);
  }

  private Symbol symbol(int type, Object value) {
    return new MySymbol(type, yyline+1, yycolumn+1, value);
  }
%}

/* White spaces */
WhiteSpace = [ \t\f\r\n]

/* Hex prefix */
HexPrefix = 0[xX]

/* Hex digit */
HexDigit = [0-9a-fA-F]

/* Comments in C files: no need to worry about them: assume the C
 * preprocessor was already executed - it strips away comments 
 */

/* Identifiers: */

Identifier = [A-Za-z_][A-Za-z0-9_]*

/* Integer literals: */

DecIntegerLiteral = 0|([1-9][0-9]*)
HexIntegerLiteral = {HexPrefix}{HexDigit}+
OctIntegerLiteral = 0[1-7][0-7]*

LDecIntegerLiteral = {DecIntegerLiteral}[lL]
LHexIntegerLiteral = {HexIntegerLiteral}[lL]
LOctIntegerLiteral = {OctIntegerLiteral}[lL]
UDecIntegerLiteral = {DecIntegerLiteral}[uU]
UHexIntegerLiteral = {HexIntegerLiteral}[uU]
UOctIntegerLiteral = {OctIntegerLiteral}[uU]
ULDecIntegerLiteral = ({LDecIntegerLiteral}[uU])|({UDecIntegerLiteral}[lL])
ULHexIntegerLiteral = ({LHexIntegerLiteral}[uU])|({UHexIntegerLiteral}[lL])
ULOctIntegerLiteral = ({LOctIntegerLiteral}[uU])|({UOctIntegerLiteral}[lL])

/* Floating point literals: */        

/* Replace this placeholder with your own definitions */
DecFloatConstant = [0-9]+
DecFracConstant = ([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+)

HexFloatConstant = {HexDigit}+
HexFracConstant = ({HexDigit}+\.{HexDigit}*)|({HexDigit}*\.{HexDigit}+)

DecDoubleLiteral = {DecFracConstant}
HexDoubleLiteral = {HexPrefix}{HexFracConstant}

DecExpLiteral = ({DecFracConstant}|{DecFloatConstant})[eE][\+\-]?[0-9]+
HexExpLiteral = {HexPrefix}({HexFracConstant}|{HexFloatConstant})[pP][\+\-]?{HexDigit}+

DecFloatLiteral = ({DecDoubleLiteral}|{DecExpLiteral}|{DecFloatConstant})[fF]
HexFloatLiteral = ({HexDoubleLiteral}|{HexExpLiteral}|{HexFloatConstant})[fF]

DoubleLiteral = {DecDoubleLiteral}|{HexDoubleLiteral}|{DecExpLiteral}|{HexExpLiteral}
FloatLiteral = {DecFloatLiteral}|{HexFloatLiteral}

%%

<YYINITIAL> {

  /* Keywords: */

  "auto"        { return symbol(AUTO); }
  "break"       { return symbol(BREAK); }
  "case"        { return symbol(CASE); }
  "char"        { return symbol(CHAR); }
  "const"       { return symbol(CONST); }
  "continue"    { return symbol(CONTINUE); }
  "default"     { return symbol(DEFAULT); }
  "do"          { return symbol(DO); }
  "double"      { return symbol(DOUBLE); }
  "else"        { return symbol(ELSE); }
  "enum"        { return symbol(ENUM); }
  "extern"      { return symbol(EXTERN); }
  "float"       { return symbol(FLOAT); }
  "for"         { return symbol(FOR); }
  "goto"        { return symbol(GOTO); }
  "if"          { return symbol(IF); }
  "inline"      { return symbol(INLINE); }
  "int"         { return symbol(INT); }
  "long"        { return symbol(LONG); }
  "register"    { return symbol(REGISTER); }
  "restrict"    { return symbol(RESTRICT); }
  "return"      { return symbol(RETURN); }
  "short"       { return symbol(SHORT); }
  "signed"      { return symbol(SIGNED); }
  "sizeof"      { return symbol(SIZEOF); }
  "static"      { return symbol(STATIC); }
  "struct"      { return symbol(STRUCT); }
  "switch"      { return symbol(SWITCH); }
  "typedef"     { return symbol(TYPEDEF); }
  "union"       { return symbol(UNION); }
  "unsigned"    { return symbol(UNSIGNED); }
  "void"        { return symbol(VOID); }
  "volatile"    { return symbol(VOLATILE); }
  "while"       { return symbol(WHILE); }
  "_Bool"       { return symbol(BOOL); }
  "_Complex"    { return symbol(COMPLEX); }
  "_Imaginary"  { return symbol(IMAGINARY); }
  
  /* Punctuators: */

  "("                            { return symbol(LPAREN); }
  ")"                            { return symbol(RPAREN); }
  "{"                            { return symbol(LBRACE); }
  "}"                            { return symbol(RBRACE); }
  "["                            { return symbol(LBRACK); }
  "]"                            { return symbol(RBRACK); }
  ";"                            { return symbol(SEMICOLON); }
  ","                            { return symbol(COMMA); }
  "="                            { return symbol(ASSIGN); }
  "<"                            { return symbol(LT); }
  ">"                            { return symbol(GT); }
  "++"                           { return symbol(PLUSPLUS); }
  "+"                            { return symbol(PLUS); }
  "-"                            { return symbol(MINUS); }
  "/"                            { return symbol(DIV); }
  "*"                            { return symbol(STAR); }
  "."                            { return symbol(DOT); }
  "->"                           { return symbol(ARROW); }
  "--"                           { return symbol(MINUSMINUS); }
  "&"                            { return symbol(BITAND); }
  "~"                            { return symbol(BITNOT); }
  "!"                            { return symbol(NOT); }
  "%"                            { return symbol(REMAINDER); }
  "<<"                           { return symbol(BITSHIFTL); }
  ">>"                           { return symbol(BITSHIFTR); }
  "<="                           { return symbol(LTEQ); }
  ">="                           { return symbol(GTEQ); }
  "=="                           { return symbol(EQ); }
  "!="                           { return symbol(NEQ); }
  "^"                            { return symbol(XOR); }
  "|"                            { return symbol(BITOR); }
  "&&"                           { return symbol(AND); }
  "||"                           { return symbol(OR); }
  "?"                            { return symbol(QUESTION); }
  ":"                            { return symbol(COLON); }
  "..."                          { return symbol(ELLIPSIS); }
  "*="                           { return symbol(STAREQ); }
  "/="                           { return symbol(DIVEQ); }
  "%="                           { return symbol(REMEQ); }
  "+="                           { return symbol(PLUSEQ); }
  "-="                           { return symbol(MINUSEQ); }
  "<<="                          { return symbol(BITSHIFTLEQ); }
  ">>="                          { return symbol(BITSHIFTREQ); }
  "&="                           { return symbol(ANDEQ); }
  "^="                           { return symbol(XOREQ); }
  "|="                           { return symbol(OREQ); }

  /* Integer literals: */

  /* replace this placeholder with your own definitions */
  {DecIntegerLiteral}            { return symbol(sym.INTEGER_LITERAL, Integer.valueOf(yytext(), 10)); }
  {HexIntegerLiteral}            { return symbol(sym.INTEGER_LITERAL, Integer.valueOf(yytext().substring(2), 16)); }
  {OctIntegerLiteral}            { return symbol(sym.INTEGER_LITERAL, Integer.valueOf(yytext(), 8)); }
  {LDecIntegerLiteral}        { 
      String integerPart = yytext().substring(0, yylength() - 1);
      return symbol(sym.INTEGER_LITERAL, Long.valueOf(integerPart, 10)); 
    }
  {LHexIntegerLiteral}        { 
      String integerPart = yytext().substring(2, yylength() - 1);
      return symbol(sym.INTEGER_LITERAL, Long.valueOf(integerPart, 16)); 
    }
  {LOctIntegerLiteral}        { 
      String integerPart = yytext().substring(0, yylength() - 1);
      return symbol(sym.INTEGER_LITERAL, Long.valueOf(integerPart, 8));
    }
  {UDecIntegerLiteral}        { 
      String integerPart = yytext().substring(0, yylength() - 1);
      return symbol(sym.INTEGER_LITERAL, Integer.valueOf(integerPart, 10)); 
    }
  {UHexIntegerLiteral}        { 
      String integerPart = yytext().substring(2, yylength() - 1);
      return symbol(sym.INTEGER_LITERAL, Integer.valueOf(integerPart, 16)); 
    }
  {UOctIntegerLiteral}        { 
      String integerPart = yytext().substring(0, yylength() - 1); 
      return symbol(sym.INTEGER_LITERAL, Integer.valueOf(integerPart, 8)); 
    }
  {ULDecIntegerLiteral}       { 
      String integerPart = yytext().substring(0, yylength() - 2);
      return symbol(sym.INTEGER_LITERAL, Long.valueOf(integerPart, 10)); 
    }
  {ULHexIntegerLiteral} { 
      String integerPart = yytext().substring(2, yylength() - 2); 
      return symbol(sym.INTEGER_LITERAL, Long.valueOf(integerPart, 16)); 
    }
  {ULOctIntegerLiteral} { 
      String integerPart = yytext().substring(0, yylength() - 2); 
      return symbol(sym.INTEGER_LITERAL, Long.valueOf(integerPart, 8));
    }


  /* Floating-point literals: */

  {DoubleLiteral}                { return symbol(FLOATING_POINT_LITERAL, Double.parseDouble(yytext())); }
  {FloatLiteral}                 { return symbol(FLOATING_POINT_LITERAL, Float.parseFloat(yytext())); }
  
  /* whitespace */
  {WhiteSpace}                   { /* ignore */ }

  /* identifiers */ 
  {Identifier}                   { return symbol(IDENTIFIER, yytext()); }  
}

/* error fallback */
.|\n                             { throw new RuntimeException("Illegal character \""+yytext()+ "\" at line "+(yyline+1)+", column "+(yycolumn+1)); }
<<EOF>>                          { return symbol(EOF); }

%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "ast.h"
#include "symtab.h"

int yylex ();
void yyerror(char* s);
int yyparse();

node* raiz = NULL;
node* aux = NULL;
node* aux_statement = NULL;
symtab * symbTab=NULL;
is_program * test=NULL;
is_program * test2=NULL;



int line;
int column;
int ultima_coluna;
char* string;
int error_syntax = 0;




%}

%union{

char* value;
struct node* node;

}

%token SEMICOLON BLANKID PACKAGE RETURN  COMMA LBRACE LPAR LSQ RBRACE RPAR RSQ ELSE VAR INT FLOAT32 BOOL STRING PARSEINT FUNC CMDARGS  FOR PRINT
%token <value> REALLIT INTLIT RESERVED STRLIT ID 
%type<node>   PRINT RETURN PARSEINT FOR IF  DIV AND ASSIGN STAR MINUS MOD NOT OR PLUS GE GT EQ LE LT NE Program Declarations DeclarationsAux VarDeclaration VarSpec VarSpecAux Type FuncDeclaration Parameters ParametersAux VarsAndStatements Statement StatementSEMICOLON StatementExprSTRLIT ParseArgs FuncInvocation FuncInvocationAux Expr FuncBody  IdAux



%left COMMA
%right ASSIGN

%left OR 
%left AND
%left LT LE GT GE EQ NE
%left PLUS MINUS
%left STAR DIV MOD

%right NOT
%left LPAR RPAR LSQ RSQ

%nonassoc ELSE IF

%%
Program:
    PACKAGE ID SEMICOLON Declarations                                   {raiz = newNode("Program",NULL,line,column);
                                                                         addChild(raiz, $4);
                                                                         
                                                                         
                                                                         
                                                                         /*
                                                                         symbTab=newSymtab();
                                                                         test=newProgram("calc","int");
                                                                         test2=newProgram("factorial","int");
                                                                         addGlobalVardec(newVardec("FILENAME","str",1), symbTab);
                                                                         addVardec(newVardec("zAux","str",1),test,symbTab);
                                                                         addVardec(newVardec("yAux","int",1),test,symbTab);
                                                                         addVardec(newVardec("xAux","str",1),test,symbTab);
                                                                         addVardec(newVardec("yAux","int",1),test,symbTab);
                                                                         addVardec(newVardec("tamanh","int",1),test2,symbTab);
                                                                         addVardec(newVardec("lenght","int",1),test2,symbTab);
                                                                         addProgram(symbTab,test);
                                                                         addProgram(symbTab,test2);*/
                                                                         
                                                                        
                                                                         

                                                                         
                                                                         //printVardec(findVardec(symbTab,test2,"tamanho"));
                                                                        

                                                                         //(test->lvdec);
                                                                         //printf("%s\n",test->lvdec->id);
                                                                         //printVardec(test->lvdec);
                                                                         
                                                                         }
    ;

Declarations:

    DeclarationsAux                             {$$ = $1;}
    |                                            {$$ = NULL;}
    ;

DeclarationsAux:
    DeclarationsAux VarDeclaration SEMICOLON                {$$ = $1;addBrother($1,$2);}
    |DeclarationsAux FuncDeclaration SEMICOLON              {$$ = $1;addBrother($1,$2);}
    |VarDeclaration SEMICOLON                               {$$ = $1;}
    |FuncDeclaration SEMICOLON                              {$$ = $1;}
    ;

VarDeclaration:
    VAR VarSpec                                             {$$ =$2;}
    |VAR LPAR VarSpec SEMICOLON RPAR                        {$$ = $3;}
    ;

VarSpec:
    IdAux Type                                             {$$ = newNode("VarDecl",NULL,$1->line,$1->column);
                                                            addChild($$,$2);
                                                            addBrother($2,$1);}
    |IdAux VarSpecAux Type                                  {$$ = newNode("VarDecl",NULL,line,column);
                                                            addChild($$,$3);
                                                            addBrother($3,$1);
                                                            addBrother($$,$2);
                                                            aux_statement=$$->brother;
                                                            while(aux_statement!=NULL&&strcmp(aux_statement->child->type,"faketype")==0){
                                                                                                        
                                                                strcpy(aux_statement->child->type,$3->type);
                                                                
                                                                aux_statement=aux_statement->brother;
                                                            }}
    ;

VarSpecAux:
    COMMA IdAux VarSpecAux                              {$$ = newNode("VarDecl",NULL,line,column);
                                                        aux = newNode("faketype",NULL,line,column);
                                                        addBrother($$,$3);
                                                        addChild($$,aux);
                                                        addBrother(aux,$2);}
    | COMMA IdAux                                       {$$ = newNode("VarDecl",NULL,line,column);
                                                        aux = newNode("faketype",NULL,line,column);
                                                        addChild($$,aux);
                                                        addBrother(aux,$2);}
    ;

Type:
    INT                                                 {$$ = newNode("Int",NULL,line,column);}
    |FLOAT32                                            {$$ = newNode("Float32",NULL,line,column);}
    |BOOL                                               {$$ = newNode("Bool",NULL,line,column);}
    |STRING                                              {$$ = newNode("String",NULL,line,column);}

    ;

FuncDeclaration:
    FUNC IdAux LPAR RPAR FuncBody                               {$$ = newNode("FuncDecl",NULL,$2->line,$2->column);
                                                                aux = newNode("FuncHeader",NULL,line,column);
                                                                addChild($$,aux);
                                                                addChild(aux,$2);
                                                                addBrother($2,newNode("FuncParams",NULL,line,column));
                                                                addBrother(aux, $5);
                                                                }
    |FUNC IdAux LPAR Parameters RPAR FuncBody                   {$$ = newNode("FuncDecl",NULL,$2->line,$2->column);
                                                                aux = newNode("FuncHeader",NULL,line,column);
                                                                addChild($$,aux);
                                                                addBrother(aux,$6);
                                                                addChild(aux,$2);
                                                                addBrother($2,$4);
                                                                }
    |FUNC IdAux LPAR RPAR Type FuncBody                         {$$ = newNode("FuncDecl",NULL,$2->line,$2->column);
                                                                aux = newNode("FuncHeader",NULL,line,column);
                                                                addChild($$,aux);
                                                                addBrother(aux,$6);
                                                                addChild(aux,$2);
                                                                addBrother($2,$5);
                                                                addBrother($5,newNode("FuncParams",NULL,line,column));}
    |FUNC IdAux LPAR Parameters RPAR Type FuncBody              {$$ = newNode("FuncDecl",NULL,$2->line,$2->column);
                                                                aux = newNode("FuncHeader",NULL,line,column);
                                                                addChild($$,aux);
                                                                addBrother(aux,$7);
                                                                addChild(aux,$2);
                                                                addBrother($2,$6);
                                                                addBrother($6,$4);}
    ;


Parameters:
    ParametersAux                   {$$ = newNode("FuncParams",NULL,line,column);
                                    addChild($$,$1);}
    ;

ParametersAux:
    IdAux Type COMMA ParametersAux                  {$$ = newNode("ParamDecl",NULL,line,column);
                                                    addChild($$,$2);
                                                    addBrother($2,$1);
                                                    addBrother($$,$4);}                                                                                                                                                         
    |IdAux Type                                        {$$ = newNode("ParamDecl",NULL,line,column);
                                                        addChild($$,$2);
                                                        addBrother($2,$1);}
    ;

FuncBody:
    LBRACE VarsAndStatements RBRACE             {$$ = newNode("FuncBody",NULL,line,column);
                                                addChild($$,$2);}
    ;

VarsAndStatements:
    VarsAndStatements SEMICOLON                                     {$1 = $1;}          
    |VarsAndStatements VarDeclaration SEMICOLON                     {$1 = $1; addBrother($1,$2);}
    |VarsAndStatements Statement SEMICOLON                          {$1 = $1;addBrother($1,$2);}
    |                                                                  {$$ = newNode("NULL",NULL,line,column);}
    ;

Statement:
    IdAux ASSIGN Expr                               {$$ = newNode("Assign",NULL,$2->line,$2->column);
                                                    addChild($$,$1);addBrother($1,$3);}
    |LBRACE RBRACE                                  {$$ = newNode("NULL",NULL,line,column);}
    |LBRACE StatementSEMICOLON RBRACE               {aux_statement = $2;
                                                    int count = 0;
                                                    while(aux_statement != NULL){
                                                        if(strcmp(aux_statement->type,"NULL")!=0){
                                                            count++;
                                                        }
                                                        aux_statement = aux_statement ->brother;
                                                        
                                                    }
                                                    if(count >= 2){
                                                        $$ = newNode("Block",NULL,line,column);
                                                        addChild($$,$2);
                                                    }else{
                                                        $$ = $2;
                                                    }
                                                    }
    |IF Expr LBRACE RBRACE                          {$$ = newNode("If",NULL,$1->line,$1->column);
                                                    addChild($$,$2);
                                                    aux = newNode("Block",NULL,line,column);
                                                    addBrother($2,aux);
                                                    addBrother(aux,newNode("Block",NULL,line,column));}
    |IF Expr LBRACE StatementSEMICOLON RBRACE       {$$ = newNode("If",NULL,$1->line,$1->column);
                                                    addChild($$,$2);
                                                    aux = newNode("Block",NULL,line,column);
                                                    addBrother($2,aux);
                                                    addChild(aux,$4);
                                                    addBrother(aux,newNode("Block",NULL,$1->line,$1->column));}
    |IF Expr LBRACE RBRACE ELSE LBRACE RBRACE       {$$ = newNode("If",NULL,line,column);
                                                    addChild($$,$2);
                                                    aux = newNode("Block",NULL,line,column);
                                                    addBrother($2,aux);
                                                    addBrother(aux,newNode("Block",NULL,line,column));}
    |IF Expr LBRACE StatementSEMICOLON RBRACE ELSE LBRACE RBRACE        {$$ = newNode("If",NULL,$1->line,$1->column);
                                                                        addChild($$,$2);
                                                                        aux = newNode("Block",NULL,line,column);
                                                                        addBrother($2,aux);
                                                                        addChild(aux,$4);
                                                                        addBrother(aux,newNode("Block",NULL,line,column));
                                                                        }
    |IF Expr LBRACE RBRACE ELSE LBRACE StatementSEMICOLON RBRACE        {$$ = newNode("If",NULL,$1->line,$1->column);
                                                                        addChild($$,$2);
                                                                        aux = newNode("Block",NULL,line,column);
                                                                        aux_statement = newNode("Block",NULL,line,column);
                                                                        addBrother($2,aux);
                                                                        addBrother(aux,aux_statement);
                                                                        addChild(aux_statement,$7);}
    |IF Expr LBRACE StatementSEMICOLON RBRACE ELSE LBRACE StatementSEMICOLON RBRACE         {$$ = newNode("If",NULL,$1->line,$1->column);
                                                                                            addChild($$,$2);
                                                                                            aux = newNode("Block",NULL,line,column);
                                                                                            aux_statement = newNode("Block",NULL,line,column);
                                                                                            addBrother($2,aux);
                                                                                            addChild(aux,$4);
                                                                                            addBrother(aux,aux_statement);
                                                                                            addChild(aux_statement,$8);}
    |FOR LBRACE RBRACE                                              {$$ = newNode("For",NULL,$1->line,$1->column);
                                                                    aux = newNode("Block",NULL,line,column);
                                                                    addChild($$,aux);}
    |FOR Expr LBRACE RBRACE                                         {$$ = newNode("For",NULL,$1->line,$1->column);
                                                                    addChild($$,$2);
                                                                    addBrother($2,newNode("Block",NULL,line,column));}
    |FOR LBRACE StatementSEMICOLON RBRACE                           {$$ = newNode("For",NULL,$1->line,$1->column);
                                                                
                                                                    aux = newNode("Block",NULL,line,column);
                                                                    addChild($$,aux);
                                                                    addChild(aux,$3);
                                                                    }
    |FOR Expr LBRACE StatementSEMICOLON RBRACE                  {$$ = newNode("For",NULL,$1->line,$1->column);
                                                                addChild($$,$2);
                                                                aux = newNode("Block",NULL,line,column);
                                                                addBrother($2,aux);
                                                                addChild(aux,$4);}
    |RETURN                                             {$$ = newNode("Return",NULL,$1->line,$1->column);}
    |RETURN Expr                                        {$$ = newNode("Return",NULL,$1->line,$1->column);
                                                        addChild($$,$2);}
    |FuncInvocation                                      {$$ = newNode("Call",NULL,$1->line,$1->column);addChild($$,$1);}
    |ParseArgs                                             {$$ = $1;}
    |PRINT LPAR StatementExprSTRLIT RPAR                {$$ = newNode("Print",NULL,$1->line,$1->column);
                                                        addChild($$,$3);}
    |error                                              {$$ = newNode("Error",NULL,line,column);error_syntax = 1;}

    ;

StatementSEMICOLON:
    StatementSEMICOLON Statement SEMICOLON                          {$$ = $1;addBrother($1,$2);}           
    |Statement SEMICOLON                                            {$$ = $1;}
    ;

StatementExprSTRLIT:
    Expr                                                                {$$ = $1;}
    |STRLIT                                                             {$$ = newNode("StrLit",$1,line,column-strlen($1));}
    ;

ParseArgs:
    IdAux COMMA BLANKID ASSIGN PARSEINT LPAR CMDARGS LSQ Expr RSQ RPAR          {$$ = newNode("ParseArgs",NULL,$5->line,$5->column);
                                                                                addChild($$,$1);addBrother($1,$9);}
    |IdAux COMMA BLANKID ASSIGN PARSEINT LPAR error RPAR                        {$$ = newNode("ParseArgs",NULL,$5->line,$5->column);
                                                                                addChild($$,$1);addBrother($1,newNode("Error",NULL,line,column));error_syntax = 1;}
    ;

FuncInvocation:
    IdAux LPAR RPAR                                 {$$ = $1;yylval.node = newNode("NULL",NULL,$1->line,$1->column);}
    |IdAux LPAR Expr RPAR                           {$$ = $1;yylval.node = newNode("NULL",NULL,$1->line,$1->column); addBrother($1,$3);}
    |IdAux LPAR Expr FuncInvocationAux RPAR         {$$ = $1;yylval.node = newNode("NULL",NULL,$1->line,$1->column);addBrother($1,$3);addBrother($3,$4);}
    |IdAux LPAR error RPAR                          {$$ = $1;addBrother($1,newNode("Error",NULL,line,column));error_syntax = 1;}
    ;

FuncInvocationAux:
    FuncInvocationAux COMMA Expr                      {$$ = $1;addBrother($1,$3);}                                                  
    | COMMA Expr                                     {$$ = $2;}                                             
;

IdAux:
    ID                                              {$$ = newNode("Id",yylval.value,line,column-strlen(yylval.value));}
    ;

Expr:
    Expr OR Expr                                    {$$ = newNode("Or",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr AND Expr                                  {$$ = newNode("And",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr LT Expr                                   {$$ = newNode("Lt",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr GT Expr                                   {$$ = newNode("Gt",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr EQ Expr                                   {$$ = newNode("Eq",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr NE Expr                                   {$$ = newNode("Ne",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr LE Expr                                   {$$ = newNode("Le",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr GE Expr                                   {$$ = newNode("Ge",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr PLUS Expr                                 {$$ = newNode("Add",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr MINUS Expr                                {$$ = newNode("Sub",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr STAR Expr                                 {$$ = newNode("Mul",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr DIV Expr                                  {$$ = newNode("Div",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |Expr MOD Expr                                  {$$ = newNode("Mod",NULL,$2->line,$2->column);free($2);
                                                    addChild($$,$1);
                                                    addBrother($1,$3);}
    |NOT Expr                                       {$$ = newNode("Not",NULL,$1->line,$1->column);free($1);
                                                    addChild($$,$2);}
    |MINUS Expr     %prec NOT                       {$$ = newNode("Minus",NULL,$1->line,$1->column);free($1);
                                                    addChild($$,$2);}
    |PLUS Expr      %prec NOT                       {$$ = newNode("Plus",NULL,$1->line,$1->column);free($1);
                                                    addChild($$,$2);}
    |INTLIT                                         {$$ = newNode("IntLit",$1,line,column-strlen($1));}
    |REALLIT                                        {$$ = newNode("RealLit",$1,line,column-strlen($1));}
    |IdAux                                          {$$ = $1;}
    |FuncInvocation                                 {$$ = newNode("Call",NULL,$1->line,$1->column);
                                                    addChild($$,$1);}
    |LPAR Expr RPAR                                 {$$ = $2;}
    |LPAR error RPAR                                {$$ = newNode("Error",NULL,line,column);error_syntax = 1;}
    ;

%%





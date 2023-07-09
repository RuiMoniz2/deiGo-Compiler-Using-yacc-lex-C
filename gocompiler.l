/* Filipe Ribeiro  Rui Moniz */




%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "y.tab.h"
//#include "ast.h"
#include "symtab.h"

int column = 1;
int line = 1;
int missing_semicolon=0;
int line_b = 1;
int column_b = 1;
int flag = 0;
int invalid_e=0;
int parsing = 0;

char* string = "";

char string_strlit[1024];
extern node* raiz;
extern symtab *symbTab;
extern int error_syntax;
extern int error_semantic;

%}

%option nounput
%option noinput

%X     STRINGLIT
%X     NORMALCOMMENT
%X     MULTICOMMENT




NUMBER                                  [0-9]+
ID                                      ([a-z]|[A-Z]|"_")({NUMBER}|[a-z]|[A-Z]|"_")* 
INTLIT                                  {NUMBER}|(0(x|X)([A-F]|[a-f]|[0-9])+) 
OCTAL_ERROR                              0[0-7]*[89][0-9]*                                            
EXPOENTS                                ("e"|"E")("+"|"-")?{NUMBER}
DIGIT                                   [0-9]
REALLIT                                 {NUMBER}"."{DIGIT}+|({NUMBER}"."{DIGIT}+{EXPOENTS}?)|("."{NUMBER}{EXPOENTS}?)|{NUMBER}{EXPOENTS}|"."{NUMBER}






RESERVED                                "++"|"--"|"break"|"case"|"chan"|"const"|"continue"|"default"|"defer"|"fallthrough"|"func"|"go"|"goto"|"import"|"interface"|"map"|"range"|"select"|"struct"|"switch"|"type"
NEWLINE                                 "\n"|"\r"|"\r\n"
SPACES                                  "\t"|" "|"\f"
ESCAPE                                  \\f|\\n|\\r|\\t|\\\\|(\\\")

SEMICOLON                               ";"
COMMA                                   ","
BLANKID                                 "_"
ASSIGN                                  "="
STAR                                    "*"
DIV                                     "/"
MINUS                                   "-"
PLUS                                    "+"
EQ                                    "=="
GE                                    ">="
GT                                    ">"
LBRACE                                    "{"
LE                                    "<="
LPAR                                    "("
LSQ                                    "["
LT                                    "<"
MOD                                    "%"
NE                                    "!="
NOT                                    "!"
AND                                    "&&"
OR                                    "||"
RBRACE                                    "}"
RPAR                                    ")"
RSQ                                    "]"
PACKAGE                                    "package"
RETURN                                    "return"
ELSE                                    "else"
FOR                                    "for"
IF                                    "if"
VAR                                    "var"
INT                                    "int"
FLOAT32                                    "float32"
BOOL                                    "bool"
STRING                                    "string"
PRINT                                    "fmt.Println"
PARSEINT                                    "strconv.Atoi"
FUNC                                    "func"
CMDARGS                                    "os.Args"

%%
{SPACES}    {column+=yyleng;string = yytext;}

                            
{SEMICOLON}     {if(flag ==1) 
                printf("SEMICOLON\n");
                column+=yyleng;missing_semicolon=0;
                if(parsing == 1){
                    yylval.value = (char*) strdup(yytext);return SEMICOLON;}}
{BLANKID}       {if(flag ==1) printf("BLANKID\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return BLANKID;}}
{PACKAGE}       {if(flag ==1) printf("PACKAGE\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return PACKAGE;}}

{AND}           {if(flag ==1) printf("AND\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return AND;}}
{ASSIGN}        {if(flag ==1) printf("ASSIGN\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return ASSIGN;}}
{STAR}          {if(flag ==1) printf("STAR\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return STAR;}}
{COMMA}         {if(flag ==1) printf("COMMA\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return COMMA;}}
{DIV}           {if(flag ==1) printf("DIV\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return DIV;}}
{EQ}            {if(flag ==1) printf("EQ\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return EQ;}}
{GE}            {if(flag ==1) printf("GE\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return GE;}}
{GT}            {if(flag ==1) printf("GT\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return GT;}}
{LBRACE}        {if(flag ==1) printf("LBRACE\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return LBRACE;}}
{LE}            {if(flag ==1) printf("LE\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return LE;}}
{LPAR}          {if(flag ==1) printf("LPAR\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return LPAR;}}
{LSQ}           {if(flag ==1) printf("LSQ\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return LSQ;}}
{LT}            {if(flag ==1) printf("LT\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return LT;}}
{MINUS}			{if(flag ==1) printf("MINUS\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return MINUS;}}
{MOD}		    {if(flag ==1) printf("MOD\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return MOD;}}
{NE}			{if(flag ==1) printf("NE\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return NE;}}
{NOT}		    {if(flag ==1) printf("NOT\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return NOT;}}
{OR}			{if(flag ==1) printf("OR\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return OR;}}
{PLUS}			{if(flag ==1) printf("PLUS\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return PLUS;}}
{RBRACE}                    {if(flag ==1) printf("RBRACE\n");column+=yyleng;missing_semicolon=1;if(parsing == 1){yylval.value = (char*) strdup(yytext);return RBRACE;}}
{RPAR}			            {if(flag ==1) printf("RPAR\n");column+=yyleng;missing_semicolon=1;if(parsing == 1){yylval.value = (char*) strdup(yytext);return RPAR;}}                            
{RSQ}                       {if(flag ==1) printf("RSQ\n");column+=yyleng;missing_semicolon=1;if(parsing == 1){yylval.value = (char*) strdup(yytext);return RSQ;}}
{ELSE}			            {if(flag ==1) printf("ELSE\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return ELSE;}}
{FOR}			            {if(flag ==1) printf("FOR\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return FOR;}}
{IF}			            {if(flag ==1) printf("IF\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return IF;}}
{VAR}			            {if(flag ==1) printf("VAR\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return VAR;}}
{INT}			            {if(flag ==1) printf("INT\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return INT;}}
{FLOAT32}		            {if(flag ==1) printf("FLOAT32\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return FLOAT32;}}
{BOOL}			            {if(flag ==1) printf("BOOL\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return BOOL;}}
{STRING}		            {if(flag ==1) printf("STRING\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return STRING;}}
{PRINT}                     {if(flag ==1) printf("PRINT\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return PRINT;}}
{PARSEINT}                  {if(flag ==1) printf("PARSEINT\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return PARSEINT;}}
{FUNC}			            {if(flag ==1) printf("FUNC\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return FUNC;}}
{CMDARGS}                   {if(flag ==1) printf("CMDARGS\n");column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return CMDARGS;}}
{RESERVED}                  {if(flag ==1) printf("RESERVED(%s)\n",yytext);column+=yyleng;missing_semicolon=0;if(parsing == 1){yylval.value = (char*) strdup(yytext);return RESERVED;}}
{OCTAL_ERROR}                   {printf("Line %d, column %d: invalid octal constant (%s)\n",line, column, yytext);column+=yyleng;}

{INTLIT}                        { if(flag ==1) printf("INTLIT(%s)\n",yytext);column+=yyleng;missing_semicolon=1;if(parsing == 1){yylval.value = (char*) strdup(yytext);return INTLIT;}}
{REALLIT}                       { if(flag ==1) printf("REALLIT(%s)\n",yytext);column+=yyleng;missing_semicolon=1;if(parsing == 1){yylval.value = (char*) strdup(yytext);return REALLIT;}}

{RETURN}                        { if(flag ==1) printf("RETURN\n");column+=yyleng;missing_semicolon=1;if(parsing == 1){yylval.value = (char*) strdup(yytext);yylval.node = newNode("NULL",NULL,line,column-strlen(yytext));return RETURN;}}
{ID}                            { if(flag ==1) printf("ID(%s)\n",yytext);column+=yyleng;missing_semicolon=1;if(parsing == 1){yylval.value = (char*) strdup(yytext);return ID;}}

\"          {strcpy(string_strlit,"\0"); 
            BEGIN STRINGLIT;
            strcat(string_strlit,yytext); 
            line_b = line; 
            column_b = column; 
            column+= yyleng; 
            invalid_e = 0;
            }
<STRINGLIT>{NEWLINE}               {BEGIN 0;
                                    missing_semicolon = 0;
                                    printf("Line %d, column %d: unterminated string literal\n",line_b,column_b);
                                    line++ ; 
                                    column = 1;
                                    strcpy(string_strlit,"\0") ;
                                    
                                    }
<STRINGLIT>{ESCAPE}                {column+=yyleng;strcat(string_strlit,yytext);}
<STRINGLIT>(\\.)|(\\)              {invalid_e = 1;
                                    missing_semicolon = 0;
                                    printf("Line %d, column %d: invalid escape sequence (%s)\n",line, column, yytext);
                                    column+= yyleng;
                                    strcpy(string_strlit,"\0");}
<STRINGLIT><<EOF>>                 {column+=yyleng;
                                    printf("Line %d, column %d: unterminated string literal\n",line_b,column_b);
                                    strcpy(string_strlit,"\0");
                                    BEGIN 0 ;
                                    yyterminate();
                                    }

<STRINGLIT>"\""                    {BEGIN 0;
                                    string = string_strlit;
                                    column+= yyleng;
                                    missing_semicolon = 1; 
                                    strcat(string_strlit,yytext); 
                                    if(flag ==1){
                                        if(invalid_e == 0){
                                            printf("STRLIT(%s)\n",string_strlit); 
                                        }
                                    }
                                    
                                    if(parsing == 1){
                                        if(invalid_e == 0){
                                            yylval.value = (char*) strdup(string_strlit);return STRLIT;
                                    }
                                    }
                                    }
<STRINGLIT>.                       {column+=yyleng;strcat(string_strlit,yytext);}



"//"       {BEGIN NORMALCOMMENT; column+= 2;}
<NORMALCOMMENT>{NEWLINE}        {column = 1; line++; BEGIN 0;}
<NORMALCOMMENT>.                {column+= yyleng;}


"/*"        {BEGIN MULTICOMMENT; line_b = line; column_b = column; column+=2;}
<MULTICOMMENT>.                 {column+=yyleng;}
<MULTICOMMENT>{NEWLINE}         {column = 1; line++;}
<MULTICOMMENT>"*/"              {BEGIN 0; column+= yyleng;}
<MULTICOMMENT><<EOF>>           {printf("Line %d, column %d: unterminated comment\n",line_b,column_b); BEGIN 0;yyterminate();}

{NEWLINE}    {if(missing_semicolon==1){
                line++;column = 1;missing_semicolon=0;
                if(flag == 1){ 
                printf("SEMICOLON\n");
                
                }
                
                if(parsing == 1){string="";yylval.value = (char*) strdup(yytext);return SEMICOLON;}
                }else{
                    line++;column=1;
                    
                }
}


<<EOF>>                     {if(missing_semicolon == 1){
                                missing_semicolon = 0;
                                if(flag == 1){
                                    printf("SEMICOLON\n");
                                    
                                }
                                if(parsing == 1){
                                    return SEMICOLON;
                                }
                            }
                            yylval.value = (char*) strdup("EOF");
                            yyterminate();
                            return EOF;
                            
                }

.                           {printf("Line %d, column %d: illegal character (%s)\n",line, column, yytext); column+=yyleng;} 


%%
int main(int argc, char *argv[]){
    
    if(argc > 1){

    
        if(strcmp(argv[1], "-l") == 0){
            flag = 1;
            yylex(); 
        }
        if(strcmp(argv[1], "-t") == 0){
            parsing = 1;
            yyparse();
            if(error_syntax == 0){
      
                printAST(raiz,0);
            }
        }
         if(strcmp(argv[1],"-s")==0){
            parsing=1;
            yyparse();
            if(error_syntax==0){
            symbTab=newSymtab();
            
            createSymtab(symbTab,raiz);
                
            
            
            printSymbtab(symbTab);
            printAST(raiz,0);
                
               

			}
        }
    }else{
        parsing = 1;
        yyparse();
        symbTab=newSymtab();
            
        createSymtab(symbTab,raiz);
    }
    
   delAst(raiz);
return 0;
}
int yywrap()
{
return 1;
}


void yyerror(char *s){
    int i=0;
    error_syntax = 1;
    while(string[i]!='\0'){
        i++;
    }
    if(strcmp(yylval.value,"\n")==0){
        printf("Line %d, column %d: %s: %s\n",line,column-i,s,yytext);
    }
    else{
        if(strcmp(yylval.value,"EOF")==0){
            printf("Line %d, column %d: %s: %s\n",line,column,s,yytext);
            
            return;
        }
        else{
            i=0;
            while(yylval.value[i]!='\0'){
                i++;
            }
            printf("Line %d, column %d: %s: %s\n",line,column-i,s,yylval.value);
        }
        
    }
    
    
}
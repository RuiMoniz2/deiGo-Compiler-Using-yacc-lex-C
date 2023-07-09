

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"


typedef struct is_vardec {
	char* type;
	char* id;
	int isUsed;
	int isParam;
	int column;
	int line;
	struct is_vardec* next;
} is_vardec;

typedef struct is_statdec {
	char* type;
	char* id;
	struct is_statdec* next;
} is_statdec;

typedef struct is_program {
	char* id;
	char* returnType;
	struct is_statdec* lsdec;
	struct is_vardec* lvdec;
	struct is_program* next;
	int declared;
} is_program;

typedef struct symtab {
	struct is_vardec* globalVariables;
	struct is_program* root;
} symtab;


void printVardec(struct is_vardec* varDec);
void printVardecList(struct is_vardec* varDec);
void printProgram(struct is_program* prog);
void printSymbtab(struct symtab* symbtab);



struct is_vardec* newVardec(char* id, char* type, int isParam,int line,int column);
struct is_program* newProgram(char* id, char* returnType);
struct symtab* newSymtab();

void addGlobalVardec(struct is_vardec* newV, struct symtab* symbtab, int line, int column);
void addVardec(struct is_vardec* newV, struct is_program* prog, struct symtab* symbtab, int line, int column);
void addProgram(struct symtab* symbtab, struct is_program* prog, int line, int column);


char * programParamTypes(struct is_program* prog);

struct is_program* findProgram(struct symtab* symbtab, char* id);
struct is_program* findProgram2(struct symtab* symbtab, char* id);
struct is_vardec* findGlobalVardec(struct symtab* symbtab, char* id);
struct is_vardec* findVardec(struct symtab* symbtab, struct is_program* prog, char* id);
char* statements(struct node* current, struct is_program* prog, struct symtab* symbtab);
void createSymtab(struct symtab* symbtab, struct node* current);




/*
void printProgram(struct is_program* program) {
	char* params;

}
void findParams(struct is_program* program) {

}*/

/*
struct is_program* insert_program(struct symtab* symbtab, char* id);
struct is_program* insert_program(struct symtab* symbtab, char* id) {

	struct is_vardec_list* nvdecL = malloc(sizeof(struct is_vardec_list*));
	nvdecL->root = NULL;
	struct is_statdec_list* nsdecL = malloc(sizeof(struct is_statdec_list*));;
	nsdecL->root = NULL;

	struct is_program* newP = malloc(sizeof(struct is_program*));
	newP->id = id;
	newP->lsdec = nsdecL;
	newP->lvdec = nvdecL;
	newP->next = NULL;

	struct is_program* aux;
	aux = symbtab->root;
	while (aux != NULL) {
		aux = aux->next;
	}
	aux = newP;
	return aux;


}
void printSYMTAB(struct symtab* symbtab) {
	struct is_program* aux = symbtab->root;
	while (aux != NULL) {
		printProgram(aux);
		aux = aux->next;
	}
}

void printStatdec(struct is_statdec* statDec) {
	printf("%s          %s\n", statDec->id, statDec->type);
}
void printVardecList(struct is_vardec_list* vdecL) {
	struct is_vardec* aux = vdecL->root;
	while (aux != NULL) {
		printVardec(aux);
		aux = aux->next;
	}
}
void printStatdecList(struct is_statdec_list* sdecL) {
	struct is_statdec* aux = sdecL->root;
	while (aux != NULL) {
		printStatdec(aux);
		aux = aux->next;
	}
}
void printProgram(struct is_program* prog) {
	printf("=====  %s  =====\n", prog->id);
	printVardecList(prog->lvdec);
	printStatdecList(prog->lsdec);
}
*/

#include "symtab.h"
//#include "ast.h"
#include <ctype.h> 
int error_semantic;
int flag_erros = 0;
char * idtype;
char * programParamTypes(struct is_program* prog) {
	char text[100] = "(";
	
	is_vardec* aux = prog->lvdec;
	int flag = 0;
	while (aux != NULL) {
		if (aux->isParam == 1) {
			if (flag != 0) {
				strcat(text, ",");
			}
			flag = 1;
			strcat(text, aux->type);
			
		}
		aux = aux->next;
	}
	strcat(text, ")");
	
	
	char* ret = malloc(sizeof(char)*100);
	strcpy(ret, text);

	
	return ret;
}

void printVardec(struct is_vardec* varDec) {
	if (varDec->isParam == 1) {
		printf("%s\t\t%s\tparam\n", varDec->id, varDec->type);
	}
	else {
		printf("%s\t\t%s\n", varDec->id, varDec->type);
		
	}
	
}
void printVardecList(struct is_vardec* varDec) {
	struct is_vardec* aux = varDec;
	while (aux != NULL) {
		printVardec(aux);
		aux = aux->next;
	}
}
void printProgram(struct is_program* prog) {
	printf("===== Function %s%s Symbol Table =====\n", prog->id,programParamTypes(prog));
	printf("return\t\t%s\n", prog->returnType);
	printVardecList(prog->lvdec);

}
void printSymbtab(struct symtab* symbtab) {
	struct is_program* aux = symbtab->root;
	printf("===== Global Symbol Table =====\n");
	
	printVardecList(symbtab->globalVariables);
	while (aux != NULL) {
		printf("%s\t%s\t%s\n", aux->id,programParamTypes(aux), aux->returnType);
		aux = aux->next;
	}
	printf("\n");
	aux = symbtab->root;
	while (aux != NULL) {
		printProgram(aux);
		aux = aux->next;
		printf("\n");
	}
	
    
}

struct is_vardec* newVardec(char* id, char* type, int isParam,int line,int column) {
	struct is_vardec* aux =(struct is_vardec *) malloc(sizeof(is_vardec));
	aux->id = id;
	aux->type = type;
	aux->isParam = isParam;
	aux->isUsed = 0;
	aux->next = NULL;
	aux->line = line;
	aux->column = column;

	return aux;

}
struct is_program* newProgram(char* id, char* returnType) {
	struct is_program * aux = (struct is_program*)malloc(sizeof(is_program));
	aux->id = id;
	aux->returnType = returnType;
	aux->lvdec = NULL;
	aux->lsdec = NULL;
	aux->next = NULL;
	aux-> declared =0;

	return aux;
}
struct symtab* newSymtab() {
	struct symtab* aux = (struct symtab*)malloc(sizeof(symtab));
	aux->root = NULL;
	aux->globalVariables = NULL;
	

	return aux;
}


void addVardec(struct is_vardec* newV, struct is_program* prog,struct symtab * symbtab,int line,int column) {
    
	if (findVardec(symbtab,prog, newV->id) != NULL) {
		printf("Line %d, column %d: Symbol %s already defined\n", line, column,newV->id);
		error_semantic=1;
		return;
	}

	struct is_vardec * auxAtual = prog->lvdec;
	struct is_vardec * auxAnterior = prog->lvdec;
    if (auxAtual == NULL) {
		prog->lvdec= newV;
	}
	else {
		
		while (auxAtual != NULL) {
			auxAnterior = auxAtual;
			auxAtual = auxAtual->next;
		}
		auxAnterior->next= newV;
	}

	
}
void addGlobalVardec(struct is_vardec* newV, struct symtab* symbtab,int line, int column) {
	if (findGlobalVardec(symbtab, newV->id) != NULL || findProgram(symbtab, newV->id) != NULL) {
		
		printf("Line %d, column %d: Symbol %s already defined\n", line, column, newV->id);
		error_semantic=1;
		//return;
	}else{
		struct is_vardec* auxAtual = symbtab->globalVariables;
		struct is_vardec* auxAnterior = symbtab->globalVariables;
		if (auxAtual == NULL) {
			symbtab->globalVariables = newV;
		}
		else {

			while (auxAtual != NULL) {
				auxAnterior = auxAtual;
				auxAtual = auxAtual->next;
			}
			auxAnterior->next = newV;
		}

	}
	
}

void addProgram(struct symtab* symbtab, struct is_program* prog, int line, int column) {
	
	
	
	struct is_program* auxAtual = symbtab->root;
	struct is_program* auxAnterior = symbtab->root;
	if (auxAtual == NULL) {
		symbtab->root = prog;
	}
	else {

		while (auxAtual != NULL) {
			auxAnterior = auxAtual;
			auxAtual = auxAtual->next;
		}
		auxAnterior->next = prog;
	}

	

	



}


struct is_program* findProgram2(struct symtab* symbtab, char * id) {
	struct is_program* aux = symbtab->root;
	
	if (aux == NULL) {
		return NULL;
	}
	else {
		while (aux != NULL) {
			if (strcmp(aux->id, id) == 0) {
				if(aux->declared==0){
					aux->declared=1;
                  return aux;
				}
				return NULL;
				
			}


			aux = aux->next;
		}
		return NULL;
	}

}

struct is_program* findProgram(struct symtab* symbtab, char * id) {
	struct is_program* aux = symbtab->root;
	
	if (aux == NULL) {
		return NULL;
	}
	else {
		while (aux != NULL) {
			if (strcmp(aux->id, id) == 0) {
				return aux;
			}


			aux = aux->next;
		}
		return NULL;
	}

}
struct is_vardec* findGlobalVardec(struct symtab* symbtab, char* id) {
	struct is_vardec* aux;
	if(symbtab->globalVariables != NULL){
		aux = symbtab->globalVariables;
	}else{
		return NULL;
	}
	
	
	
	while (aux != NULL) {
		
			if (strcmp(aux->id, id) == 0) {
				
				return aux;
			}
			
			aux = aux->next;
	
	}
	
	
	return NULL;
}
/*struct is_vardec* findVardec(struct symtab* symbtab, struct is_program* prog, char* id) {
	if (findGlobalVardec(symbtab, id) != NULL) {
		return findGlobalVardec(symbtab, id);
	}
	else{
		struct is_vardec* auxProg = prog->lvdec;

		while (auxProg != NULL) {
			if (strcmp(auxProg->id, id) == 0) {
				return auxProg;
			}
			auxProg = auxProg->next;
		}
		return NULL;
	}
	
}*/
struct is_vardec* findVardec(struct symtab* symbtab, struct is_program* prog, char* id) {

		struct is_vardec* auxProg = prog->lvdec;
		if (auxProg == NULL) {
			return NULL;
		}
		else {
			while (auxProg != NULL) {
				if (strcmp(auxProg->id, id) == 0) {
					return auxProg;
				}
				auxProg = auxProg->next;
			}
			return NULL;
		}
}

void createSymtab(struct symtab* symbtab,struct node* no) {
	
	node * current = no->child;
	node* root = current;
	while (current != NULL) {
		
		if (strcmp(current->type, "FuncDecl") == 0) {
			if(findProgram(symbtab,current->child->child->value)==NULL && findGlobalVardec(symbtab,current->child->child->value) == NULL){
			if (strcmp(current->child->child->brother->type, "Int") == 0) {
				addProgram(symbtab, newProgram(current->child->child->value, "int"), current->line, current->column);
			}
			if (strcmp(current->child->child->brother->type, "Float32") == 0) {
				addProgram(symbtab, newProgram(current->child->child->value, "float32"), current->line, current->column);
			}
			if (strcmp(current->child->child->brother->type, "String") == 0) {
				addProgram(symbtab, newProgram(current->child->child->value, "string"), current->line, current->column);
			}
			if (strcmp(current->child->child->brother->type, "Bool") == 0) {
				addProgram(symbtab, newProgram(current->child->child->value, "bool"), current->line, current->column);
			}

			if (strcmp(current->child->child->brother->type, "FuncParams") == 0) {		
				addProgram(symbtab, newProgram(current->child->child->value, "none"), current->line, current->column);
			}
			is_program* prog = findProgram(symbtab,current->child->child->value);
			
			node* auxParams = malloc(sizeof(node));
			if (strcmp(current->child->child->brother->type, "FuncParams") == 0) {
				auxParams = current->child->child->brother->child;
				
			}
			else {
				auxParams = current->child->child->brother->brother->child;
				
			}
			while (auxParams != NULL) {
					if (strcmp(auxParams->type, "ParamDecl") == 0) {
						if (strcmp(auxParams->child->type, "Int") == 0) {
							addVardec(newVardec(auxParams->child->brother->value, "int", 1,current->line,current->column), prog, symbtab, auxParams->line, auxParams->column);
						}
						if (strcmp(auxParams->child->type, "Float32") == 0) {
							addVardec(newVardec(auxParams->child->brother->value, "float32", 1,current->line,current->column), prog, symbtab, auxParams->line, auxParams->column);
						}
						if (strcmp(auxParams->child->type, "String") == 0) {
							addVardec(newVardec(auxParams->child->brother->value, "string", 1,current->line,current->column), prog, symbtab, auxParams->line, auxParams->column);
						}
						if (strcmp(auxParams->child->type, "Bool") == 0) {
							addVardec(newVardec(auxParams->child->brother->value, "bool", 1,current->line,current->column), prog, symbtab, auxParams->line, auxParams->column);
						}



					}

					auxParams = auxParams->brother;
				}

			}
			else {
				printf("Line %d, column %d: Symbol %s already defined\n",current->line, current->column, current->child->child->value);
				error_semantic =1;
			}
			
		}
		else if (strcmp(current->type, "VarDecl") == 0) {
			
			if (strcmp(current->child->type, "Int") == 0) {
				addGlobalVardec(newVardec(current->child->brother->value, "int", 0,current->child->brother->line,current->child->brother->column), symbtab, current->line, current->column);
			}
			if (strcmp(current->child->type, "Float32") == 0) {
				addGlobalVardec(newVardec(current->child->brother->value, "float32", 0,current->child->brother->line,current->child->brother->column), symbtab, current->line, current->column);
			}
			if (strcmp(current->child->type, "String") == 0) {
				addGlobalVardec(newVardec(current->child->brother->value, "string", 0,current->child->brother->line,current->child->brother->column), symbtab, current->line, current->column);
			}
			if (strcmp(current->child->type, "Bool") == 0) {
				addGlobalVardec(newVardec(current->child->brother->value, "bool", 0,current->child->brother->line,current->child->brother->column), symbtab, current->line, current->column);
			}
			
		}
		
		current = current->brother;
		
	}
	
	current = root;
	
	while (current != NULL) {
		
		if (strcmp(current->type, "FuncDecl") == 0) {
			
			is_program* prog = findProgram2(symbtab,current->child->child->value);
			
			
			
			
			if (prog != NULL) {
				
		
				

				node* aux = malloc(sizeof(node));
				aux = current->child->brother->child;
			
				while (aux!=NULL) {
					
					if (strcmp(aux->type, "VarDecl") == 0) {    
						
						if (strcmp(aux->child->type, "Int") == 0) {
							addVardec(newVardec(aux->child->brother->value, "int", 0,aux->child->brother->line,aux->child->brother->column), prog, symbtab, aux->line, aux->column);
						}
						if (strcmp(aux->child->type, "Float32") == 0) {
							addVardec(newVardec(aux->child->brother->value, "float32", 0,aux->child->brother->line,aux->child->brother->column), prog, symbtab, aux->line, aux->column);
						}
						if (strcmp(aux->child->type, "String") == 0) {
							addVardec(newVardec(aux->child->brother->value, "string", 0,aux->child->brother->line,aux->child->brother->column), prog, symbtab, aux->line, aux->column);
						}
						if (strcmp(aux->child->type, "Bool") == 0) {
							addVardec(newVardec(aux->child->brother->value, "bool", 0,aux->child->brother->line,aux->child->brother->column), prog, symbtab, aux->line, aux->column);
						}



					}
					else if (strcmp(aux->type, "NULL") != 0) {    
						                                   // caso em que e um statement
						statements(aux, prog,symbtab);
						
					}
					
					aux = aux->brother;
				}
				

			}
		
		
		}
		current = current->brother;
	}
	
	/*struct is_vardec* auxGlobal = malloc(sizeof(is_vardec));
	//struct is_program* auxProg = malloc(sizeof(is_program));
	auxGlobal = symbtab->globalVariables;
	while (auxGlobal != NULL) {
		if (auxGlobal->isUsed == 0) {
			printf("Line %d, column %d: Symbol %s declared but never used\n",auxGlobal->line,auxGlobal->column,auxGlobal->id);
		}
		auxGlobal = auxGlobal->next;
	}*/
	struct is_program* auxProg = malloc(sizeof(is_program));
	auxProg = symbtab->root;
	while (auxProg != NULL) {
		struct is_vardec* auxLocal = malloc(sizeof(is_vardec));
		auxLocal = auxProg->lvdec;
		while (auxLocal != NULL) {
			if (auxLocal->isUsed == 0 && auxLocal->isParam==0) {
				printf("Line %d, column %d: Symbol %s declared but never used\n",auxLocal->line,auxLocal->column, auxLocal->id);
			}
			auxLocal = auxLocal->next;
		}
		auxProg = auxProg->next;
	}
	

}

char* statements(struct node* current, struct is_program* prog, struct symtab* symbtab) {
	
	if (strcmp(current->type, "If") == 0) {
		char* type = statements(current->child, prog, symbtab);

		if (strcmp(type, "bool") != 0) {
			printf("Line %d, column %d: Incompatible type %s in %s statement\n", current->child->line, current->child->column,type, "if");
			error_semantic = 1;
		}
		statements(current->child->brother, prog,symbtab);                 
		statements(current->child->brother->brother, prog, symbtab);       
	}        
	else if (strcmp(current->type, "IntLit") == 0) {
		if (current->value[0] == '0') {            
			if ((current->value[1] == 'x' || current->value[1] == 'X')) {   
				addNote(current, "int");
				return "int";

			}
			int i = 1;

			while (current->value[i] != '\0') {     
				if ((current->value[i] < '0' || current->value[i]>'7')) {
					printf("Line %d, column %d: Invalid octal constant: %s\n", current->line, current->column, current->value);
					error_semantic = 1;
					addNote(current, "int");
					return "int";


				}
				i++;
			}

		}
		addNote(current, "int");
		return "int";
	}
	else if (strcmp(current->type, "RealLit") == 0) {
		addNote(current, "float32");
		return "float32";
	}
	else if (strcmp(current->type, "Bool") == 0) {
		addNote(current, "bool");
		return "bool";
	}
	else if (strcmp(current->type, "StrLit") == 0) {
		addNote(current, "string");
		return "string";
	}
	else if (strcmp(current->type, "Id") == 0) {
		
		if (findVardec(symbtab, prog, current->value) == NULL && findGlobalVardec(symbtab, current->value) == NULL) {
			printf("Line %d, column %d: Cannot find symbol %s\n", current->line, current->column, current->value);
			error_semantic = 1;
		}
		
		else if (findVardec(symbtab, prog, current->value) != NULL) {
			
			addNote(current, findVardec(symbtab, prog, current->value)->type);
			findVardec(symbtab, prog, current->value)->isUsed = 1;
			return findVardec(symbtab, prog, current->value)->type;
		}
		else if (findGlobalVardec(symbtab, current->value) != NULL) {
			
			findGlobalVardec(symbtab, current->value)->isUsed = 1;
			addNote(current, findGlobalVardec(symbtab, current->value)->type);
			return findGlobalVardec(symbtab, current->value)->type;
		}		
		
		addNote(current, "undef");
		return "undef";

	}
	else if (strcmp(current->type, "Eq") == 0) {
		char* type = statements(current->child, prog, symbtab);
		char* type2 = statements(current->child->brother, prog, symbtab);
		if (strcmp(type, type2) != 0) {
			printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "==", type, type2);
			error_semantic = 1;
			
			addNote(current, "bool");
			return "bool";
		}
		else {
			addNote(current, "bool");
			return "bool";
		}

	}
	else if (strcmp(current->type, "Ne") == 0) {
		char* type = statements(current->child, prog, symbtab);
		char* type2 = statements(current->child->brother, prog, symbtab);
		if (strcmp(type, type2) != 0) {
			printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "!=", type, type2);
			error_semantic = 1;
			
		}
		else {
			addNote(current, "bool");
			return "bool";
		}

	}
	else if (strcmp(current->type, "Gt") == 0) {
		char* type = statements(current->child, prog, symbtab);
		char* type2 = statements(current->child->brother, prog, symbtab);
		if (strcmp(type, type2) != 0 || strcmp(type, "bool") == 0 || strcmp(type2, "bool") == 0) {
			printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, ">", type, type2);
			error_semantic = 1;
			
			addNote(current, "bool");
			return "bool";
		}
		else {
			if (strcmp(type, "int") == 0 && strcmp(type2, "int") == 0) {
				addNote(current, "bool");
				return "bool";
			}
			if (strcmp(type, "float32") == 0 && strcmp(type2, "float32") == 0) {
				addNote(current, "bool");
				return "bool";
			}
			if (strcmp(type, "string") == 0 && strcmp(type2, "string") == 0) {
				addNote(current, "bool");
				return "bool";
			}
		}

	}
	else if (strcmp(current->type, "Ge") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "bool") == 0 || strcmp(type2, "bool") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, ">=", type, type2);
		error_semantic = 1;
		
		addNote(current, "bool");
		return "bool";
	}
	else {
		if (strcmp(type, "int") == 0 && strcmp(type2, "int") == 0) {
			addNote(current, "bool");
			return "bool";
		}
		if (strcmp(type, "float32") == 0 && strcmp(type2, "float32") == 0) {
			addNote(current, "bool");
			return "bool";
		}
		if (strcmp(type, "string") == 0 && strcmp(type2, "string") == 0) {
			addNote(current, "bool");
			return "bool";
		}
	}

	}
	else if (strcmp(current->type, "Lt") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "bool") == 0 || strcmp(type2, "bool") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "<", type, type2);
		error_semantic = 1;
		
		addNote(current, "bool");
		return "bool";
	}
	else {
		if (strcmp(type, "int") == 0 && strcmp(type2, "int") == 0) {
			addNote(current, "bool");
			return "bool";
		}
		if (strcmp(type, "float32") == 0 && strcmp(type2, "float32") == 0) {
			addNote(current, "bool");
			return "bool";
		}
		if (strcmp(type, "string") == 0 && strcmp(type2, "string") == 0) {
			addNote(current, "bool");
			return "bool";
		}
	}

	}
	else if (strcmp(current->type, "Le") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "bool") == 0 || strcmp(type2, "bool") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "<=", type, type2);
		error_semantic = 1;
		
		addNote(current, "bool");
		return "bool";
	}
	else {
		if (strcmp(type, "int") == 0 && strcmp(type2, "int") == 0) {
			addNote(current, "bool");
			return "bool";
		}
		if (strcmp(type, "float32") == 0 && strcmp(type2, "float32") == 0) {
			addNote(current, "bool");
			return "bool";
		}
		if (strcmp(type, "string") == 0 && strcmp(type2, "string") == 0) {
			addNote(current, "bool");
			return "bool";
		}
	}

	}
	else if (strcmp(current->type, "Assign") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	/*if(strcmp(current->child->brother->type,"Ge") == 0 || strcmp(current->child->brother->type,"Le") == 0 || strcmp(current->child->brother->type,"Lt") == 0 || strcmp(current->child->brother->type,"Gt") == 0 || strcmp(current->child->brother->type,"And") == 0 || strcmp(current->child->brother->type,"Or") == 0){
		flag_erros = 1;
	}*/
	
	if (strcmp(type, type2) == 0 && strcmp(type,"undef") !=0 ) {
		addNote(current, type);
		//printf("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n");
		return type;
		
	 }
	else {
		
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "=", type, type2);

			
			
			
			
		addNote(current, "undef");
		return "undef";

		
		
	}
	
	

	 }
	else if (strcmp(current->type, "Add") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "undef") == 0 || strcmp(type2, "undef") == 0 || strcmp(type2, "bool") == 0 || strcmp(type, "bool") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "+", type, type2);
		error_semantic = 1;
		addNote(current, "undef");
		return "undef";
	}
	else {
		addNote(current, type);
		return type;
	}


	 }
	else if (strcmp(current->type, "Sub") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "undef") == 0 || strcmp(type2, "undef") == 0 || strcmp(type2, "bool") == 0 || strcmp(type, "bool") == 0 || strcmp(type, "string") == 0 || strcmp(type2, "string") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "-", type, type2);
		error_semantic = 1;
		addNote(current, "undef");
		return "undef";
	   }
	else {
		addNote(current, type);
		return type;
	}


     }
	else if (strcmp(current->type, "Mul") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "undef") == 0 || strcmp(type2, "undef") == 0 || strcmp(type2, "bool") == 0 || strcmp(type, "bool") == 0 || strcmp(type, "string") == 0 || strcmp(type2, "string") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "*", type, type2);
		error_semantic = 1;
		addNote(current, "undef");
		return "undef";
	}
	else {
		addNote(current, type);
		return type;
	}


	 }
	else if (strcmp(current->type, "Div") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "undef") == 0 || strcmp(type2, "undef") == 0 || strcmp(type2, "bool") == 0 || strcmp(type, "bool") == 0 || strcmp(type, "string") == 0 || strcmp(type2, "string") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "/", type, type2);
		error_semantic = 1;
		addNote(current, "undef");
		return "undef";
	}
	else {
		addNote(current, type);
		return type;
	}


	 }
	else if (strcmp(current->type, "Mod") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, type2) != 0 || strcmp(type, "undef") == 0 || strcmp(type2, "undef") == 0 || strcmp(type2, "bool") == 0 || strcmp(type, "bool") == 0 || strcmp(type, "string") == 0 || strcmp(type2, "string") == 0 || strcmp(type, "float32") == 0 || strcmp(type2, "float32") == 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "%", type, type2);
		error_semantic = 1;
		addNote(current, "undef");
		return "undef";
	}
	else {
		addNote(current, type);
		return type;
	}


	 }
	else if (strcmp(current->type, "Minus") == 0) {
	char* type = statements(current->child, prog, symbtab);
	if (strcmp(type, "int") == 0) {
		addNote(current, "int");
		return "int";
	}
	if (strcmp(type, "float32") == 0) {
		addNote(current, "float32");
		return "float32";
	}
	printf("Line %d, column %d: Operator %s cannot be applied to type %s\n", current->line, current->column, "-", type);
	error_semantic = 1;
	addNote(current, "undef");
	return "undef";
	

	 }
	else if (strcmp(current->type, "Plus") == 0) {
	char* type = statements(current->child, prog, symbtab);
	if (strcmp(type, "int") == 0) {
		addNote(current, "int");
		return "int";
	}
	if (strcmp(type, "float32") == 0) {
		addNote(current, "float32");
		return "float32";
	}
	printf("Line %d, column %d: Operator %s cannot be applied to type %s\n", current->line, current->column, "+", type);
	error_semantic = 1;
	addNote(current, "undef");
	return "undef";


	 }
	else if (strcmp(current->type, "ParseArgs") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, "int") != 0 || strcmp(type2, "int") != 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "strconv.Atoi", type, type2);
		error_semantic = 1;
		addNote(current, "undef");
		return "undef";
	}
	
	addNote(current, "int");
	return "int";
	

	
}
	else if (strcmp(current->type, "Or") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, "bool") == 0 && strcmp(type, "bool") == 0) {
		addNote(current, "bool");
		return "bool";
     }
	else {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "||", type, type2);
		error_semantic = 1;
		addNote(current, "bool");
		return "bool";
	   }
     }
	else if (strcmp(current->type, "And") == 0) {
	char* type = statements(current->child, prog, symbtab);
	char* type2 = statements(current->child->brother, prog, symbtab);
	if (strcmp(type, "bool") == 0 && strcmp(type2, "bool") == 0) {
		addNote(current, "bool");
		return "bool";
	}
	else {
		printf("Line %d, column %d: Operator %s cannot be applied to types %s, %s\n", current->line, current->column, "&&", type, type2);
		error_semantic = 1;
		addNote(current, "bool");
		return "bool";
	}
	 }
	else if (strcmp(current->type, "VarDecl") == 0) {
	return "null";
	}
	else if (strcmp(current->type, "Print") == 0) {
	char* type =statements(current->child, prog,symbtab);
	if (strcmp(type, "undef") == 0){
		printf("Line %d, column %d: Incompatible type %s in fmt.Println statement\n", current->line, current->child->column,type);
	}
	return "null";
	}
	else if (strcmp(current->type, "Not") == 0) {
	char* type = statements(current->child, prog, symbtab);
	if (strcmp(type,"bool") != 0) {
		printf("Line %d, column %d: Operator %s cannot be applied to type %s\n", current->line, current->column, "!", type);
		error_semantic = 1;
		addNote(current, "bool");
	}
	
	addNote(current, "bool");
	return "bool";
	
}
	else if (strcmp(current->type, "Block") == 0) {
	if (current->child != NULL) {

		node* aux = current->child;

		while (aux!=NULL) {

			statements(aux, prog,symbtab);
			aux = aux->brother;
		}

	}

    }
	else if (strcmp(current->type, "For") == 0) {
	if (strcmp(current->child->type, "Block") == 0) {
		statements(current->child, prog,symbtab);
		return "null";
	}
	else {
		char* type = statements(current->child, prog, symbtab);
		if (strcmp(type, "bool") != 0 && strcmp(type,"undef") != 0) {
			printf("Line %d, column %d: Incompatible type %s in %s statement\n", current->child->line, current->child->column, type, "for");
			error_semantic = 1;
			addNote(current, "undef");
		}
		statements(current->child->brother, prog,symbtab);

		return "null";
	}
}
	else if (strcmp(current->type, "Return") == 0) {
	if (current->child != NULL) {
		char* type = statements(current->child, prog, symbtab);
		if (strcmp(type, prog->returnType) != 0) {
			printf("Line %d, column %d: Incompatible type %s in %s statement\n", current->child->line, current->child->column, type, "return");
			error_semantic = 1;
		}
	}
	else {
		if (strcmp("none", prog->returnType) != 0) {
			printf("Line %d, column %d: Incompatible type %s in %s statement\n", current->line, current->column, "none", "return");
			error_semantic = 1;
		}
	}
	return "null";
}
	else if (strcmp(current->type, "Call") == 0) {
		
		char* funcName = current->child->value;
		struct node* aux = current->child->brother;
		char funcParams[1024] = "\0";
		strcat(funcParams, "(");
		while (aux != NULL) {
			
			char* type = statements(aux, prog, symbtab);
			//printf("%s\n",type);
			if (strcmp(funcParams, "(") == 0) {
				
				strcat(funcParams, type);
			}
			else {
				strcat(funcParams, ",");
				strcat(funcParams, type);
			}
			aux = aux->brother;
		}
		strcat(funcParams, ")");
		//printf("%s\n",funcParams);
		if(findProgram(symbtab, funcName) == NULL){
			
			if (strcmp(funcParams, "(null)") == 0) {
				
				printf("Line %d, column %d: Cannot find symbol %s()\n", current->line, current->column, funcName);
			}
			else {
				
				printf("Line %d, column %d: Cannot find symbol %s%s\n", current->line, current->column, funcName, funcParams);
			}
			error_semantic = 1;
			addNote(current, "undef");
			return "undef";
		}
		else {
			//printf("%s   %s   %s\n",funcParams,funcName,programParamTypes(findProgram(symbtab, funcName)));
			if (strcmp(funcParams, programParamTypes(findProgram(symbtab, funcName))) != 0){
				
				if (strcmp(funcParams, "(null)") == 0) {
				//printf("funcparams: %s  programparams: %s\n",funcParams,programParamTypes(findProgram(symbtab, funcName)));
				//printf("OLA0\n");	
				printf("Line %d, column %d: Cannot find symbol %s()\n", current->line, current->column, funcName);
			}
			else {
				

				
					printf("Line %d, column %d: Cannot find symbol %s%s\n", current->line, current->column, funcName, funcParams);
				
				//printf("%s   %s\n",funcParams,funcName);
				//printf("OLA1\n");
				
			
			}
			
			
				error_semantic = 1;
				addNote(current, "undef");
				return "undef";
				
			
			}
			else {
				//printf("OLA3\n");
				if(strcmp(findProgram(symbtab, funcName)->returnType,"none") == 0){
					//statements(current->child, prog, symbtab);
					//addNote(current, type);
                    //printf("AHHHHHHHHHHH\n");
					addNote(current->child, programParamTypes(findProgram(symbtab, funcName)));
					return "";
				}else{
					//printf("OLA2\n");
					//addNote(current, findProgram(symbtab, funcName)->returnType);
					//char* type = statements(current->child, prog, symbtab);
					//addNote(current, type);
					addNote(current->child, programParamTypes(findProgram(symbtab, funcName)));
                    addNote(current, findProgram(symbtab, funcName)->returnType);
					
					return findProgram(symbtab, funcName)->returnType;
				}

					
			}


		}
		
		//printf("%s\n", funcParams);
	}

	return "null";
}





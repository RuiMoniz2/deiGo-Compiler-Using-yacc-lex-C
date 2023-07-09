#include "ast.h"

node *newNode(char* type, char* value, int line, int column)
{

    node *new = (node *)malloc(sizeof(node));
    new->type = (char *)strdup(type);
    new->child = NULL;
    new->brother = NULL;
    new->line = line;
    new->column = column;
    new->note = NULL;

    if (value == NULL)
    {
        new->value = NULL;
        return new;
    }
    new->value = (char *)strdup(value);
    return new;
}
void addBrother(node *brother, node *newBrother)
{
    if (brother != NULL && newBrother != NULL)
    {

        node *auxBrother = brother;

        while (auxBrother->brother != NULL)
        {
            auxBrother = auxBrother->brother;
        }
        auxBrother->brother = newBrother;
    }
}
void addChild(node *dad, node *child)
{
    if (dad != NULL && child != NULL)
    {
        dad->child = child;
    }
}
void printAST(node *current, int npontos)
{
    if (current == NULL)
    {
        return;
    }
    if (current != NULL)
    {
        if (current->type != NULL)
        {
            if(strcmp(current->type,"NULL")!=0){
                for (int i = 0; i < npontos; i++)
                printf("..");

                if (current->value != NULL)
                {
                    if (current->note != NULL) {
                        printf("%s(%s) - %s\n", current->type, current->value, current->note);
                    }
                    else {
                        printf("%s(%s)\n", current->type, current->value);
                    }
                   
                }
                else
                {
                    if (current->note != NULL) {
                        printf("%s - %s\n", current->type,current->note);
                    }
                    else {
                        printf("%s\n", current->type);
                    }
                    
                }
                if (current->child != NULL)
                    printAST(current->child, npontos + 1);
                if (current->brother != NULL)
                    printAST(current->brother, npontos);
            }
                else{
                    if (current->child != NULL)
                        printAST(current->child, npontos );
                    if (current->brother != NULL)
                        printAST(current->brother, npontos);
            }
            
        }

        
    }
}
void addNote(node* node, char* note) {

    node->note = note;
    return;
}

void delAst(node* current ){
    if (current==NULL)
        return;
    if (!current->type)
    {
        free(current->type);
        current->type=NULL;

    }
    if(!current->value){
        free(current->value);
        current->value=NULL;
    }

    delAst(current->child);
    current->child = NULL;
    
    delAst(current->brother);
    current->brother = NULL;

    free(current);
    current = NULL;
}
#include "structures.h"
#include "symbol_table.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern prog_env* mySemantic;

void show_tables(prog_env* program)
{
    table_element* glob_table = program->global;
    environment_list* methods_list = program->methods;
    printf("===== Class %s Symbol Table =====\n", glob_table->name);
    glob_table = glob_table->next;
    while (glob_table != NULL){
        printf("%s\t%s\n", glob_table->name, glob_table->type);
        glob_table = glob_table->next;
    }
    while (methods_list != NULL){
        printf("\n===== Method %s Symbol Table =====\n", methods_list->name);
        table_element* method_element = methods_list->locals;
        while (method_element != NULL){
            if (method_element->param == 1) 
                printf("%s\t%s\tparam\n", method_element->name, method_element->type);
            else
                printf("%s\t%s\n", method_element->name, method_element->type);

            method_element = method_element->next;
        }
        methods_list = methods_list->next;
    }
}

table_element* insert_element(char* ele_name, char* ele_type, int ele_par)
{
    table_element* new_symbol = (table_element*)malloc(sizeof (table_element));
    strcpy(new_symbol->name, ele_name);
    if (strcmp(ele_type, "Int") == 0) 
        strcpy(new_symbol->type, "int");
    else
        if (strcmp(ele_type, "Void") == 0) 
            strcpy(new_symbol->type, "void");
        else
            if (strcmp(ele_type, "Bool") == 0) 
                strcpy(new_symbol->type, "boolean");
            else
                if (strcmp(ele_type, "IntArray") == 0) 
                    strcpy(new_symbol->type, "int[]");
                else
                    if (strcmp(ele_type, "BoolArray") == 0) 
                        strcpy(new_symbol->type, "boolean[]");
                    else
                        if (strcmp(ele_type, "StringArray") == 0) 
                            strcpy(new_symbol->type, "String[]");
                        else{
                            strcpy(new_symbol->type, ele_type);
                        }

    new_symbol->param = ele_par;
    new_symbol->next = NULL;
    return new_symbol;
}

environment_list* insert_method(char* name, char* type)
{
    environment_list* new_list = (environment_list*)malloc(sizeof (environment_list));
    strcpy(new_list->name, name);
    strcpy(new_list->type, type);
    new_list->next = NULL;
    return new_list;
}


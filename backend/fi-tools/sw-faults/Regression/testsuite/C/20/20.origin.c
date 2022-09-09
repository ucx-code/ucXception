#include "structures.h"
#include "semantics.h"
#include "symbol_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* sem_type[] = {    "Program",
                        "VarDecl",
                        "MethodDecl", "MethodParams", "MethodBody", "ParamDeclaration",
                        "CompoundStat", "IfElse", "Print", "Return", "Store", "StoreArray", "While",
                        "Or", "And", "Eq", "Neq", "Lt", "Gt", "Leq", "Geq", "Add", "Sub", "Mul", "Div", "Mod", "Not", "Minus", "Plus", "Length", "LoadArray", "Call", "NewInt", "NewBool", "ParseArgs",
                        "Int", "Bool", "IntArray", "BoolArray", "StringArray", "Void", "Id", "IntLit", "BoolLit",
                        "Null" };

int debug = 0;

/* ************************************
 * Construccao da tabelas de simbolos *
 ************************************ */

prog_env *check_program(is_node *node)
{
	prog_env *prog = (prog_env*) malloc(sizeof(prog_env));
	prog->global = NULL;
	prog->methods = NULL;

	if(node->type == Program)
		check_class(node->child, prog);

	return prog;
}

void check_class(is_node *node, prog_env *prog)
{
    is_node* aux;
    char* aux_type;

	while(node != NULL)
	{
		switch(node->type)
		{
			case Id:
				insert_global_table(prog, node->id, "");
				break;

			case VarDecl:
                aux_type = sem_type[node->child->type];
                aux = node->child->next;
                while(aux != NULL)
                {
                    insert_global_table(prog, aux->id, aux_type);
                    aux = aux->next;
                }
                break;

			case MethodDecl:
				insert_global_table(prog, node->child->next->id, "method");
				insert_methods_table(prog, node->child->next->id, sem_type[node->child->type]);

				check_methodDecl(prog, node->child); // verificar filhos do no MethodDecl
				break;

			default:
				break;
		}
		node = node->next;
	}
}

void check_methodDecl(prog_env* prog, is_node* node)
{
    char* method_name = node->next->id;

    insert_method_elements(prog, method_name, sem_type[node->type], "return", 0);

    while(node != NULL)
    {
        switch(node->type)
        {
            case MethodParams:
                check_methodParams(prog, method_name, node->child);
                break;

            case MethodBody:
                check_methodBody(prog, method_name, node->child);
                break;

            default:
                break;
        }
        node = node->next;
    }
}

void check_methodBody(prog_env* prog, char* name, is_node* node)
{
    is_node* aux;
    char* type_aux;

    while(node != NULL)
    {
        switch (node->type)
        {
            case VarDecl:

                type_aux = sem_type[node->child->type];
                aux = node->child->next;
                while(aux != NULL)
                {
                    insert_method_elements(prog, name, type_aux, aux->id, 0);
                    aux = aux->next;
                }
                break;

            default:
                break;
        }
        node = node->next;
    }
}

void check_methodParams(prog_env* prog, char* name, is_node* node)
{
    while(node != NULL)
    {
        insert_method_elements(prog, name, sem_type[node->child->type], node->child->next->id, 1);
        node = node->next;
    }
}

void insert_global_table(prog_env* prog, char* id, char* type)
{
	table_element* temp;
	temp = prog->global;

	if(prog->global == NULL)
		prog->global = insert_element(id, type, 0);

	else
	{
		while(temp->next != NULL)
        {
            check_defined(temp->next->name, id);
			temp = temp->next;
        }
		temp->next = insert_element(id, type, 0);
	}
}

void insert_methods_table(prog_env* prog, char* id, char* type)
{
	environment_list* temp;
	temp = prog->methods;

	if(prog->methods == NULL)
		prog->methods = insert_method(id, type);

	else
	{
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = insert_method(id, type);
	}
}

void insert_method_elements(prog_env* prog, char* method_name, char* type, char* name, int par)
{
    environment_list* methods = prog->methods;

    while(methods != NULL)
    {
        if(strcmp(methods->name, method_name) == 0)
        {
            table_element* method_element = methods->locals;
            table_element* prev = methods->locals;

            if(methods->locals == NULL)
                methods->locals = insert_element(name, type, par);

            else
            {
                while(method_element != NULL)
                {
                    check_defined(method_element->name, name);
                    prev = method_element;
                    method_element = method_element->next;
                }
                prev->next = insert_element(name, type, par);
            }
        }
        methods = methods->next;
    }
}

/* ************************************
 * Verificacao dos erros semanticos   *
 ************************************ */

void check_semantic(is_node* myProgram, prog_env* mySemantic)
{
    is_node* aux = myProgram->child;
    is_node* aux2;
    char* method_name;

    while(aux != NULL) // Percorrer os filhos do no Programa
    {
        if(aux->type == MethodDecl) // Caso seja um MethodDecl
        {
            aux2 = aux->child;
            method_name = aux->child->next->id;

            while(aux2 != NULL)
            {
                if(aux2->type == MethodBody)
                    semantic_statements(mySemantic, method_name, aux2->child);

                aux2 = aux2->next;
            }
        }
        aux = aux->next;
    }
}

void semantic_statements(prog_env* prog, char* name, is_node* node)
{
    char* expr_type;
    char* expr_type2;
    char* expr_type3;

    while(node != NULL)
    {
        switch(node->type)
        {
            case CompoundStat:
                semantic_statements(prog, name, node->child);
                break;

            case IfElse:
                expr_type = expr_recursive(prog, name, node->child);
                if(strcmp(expr_type, "boolean") != 0)
                {
                    printf("Incompatible type in if statement (got %s, required boolean)\n", expr_type);
                    exit(0);
                }
                semantic_statements(prog, name, node->child->next);
                break;

            case Print:
                expr_type = expr_recursive(prog, name, node->child);

                if(strcmp(expr_type, "boolean") != 0 && strcmp(expr_type, "int") != 0)
                {
                    printf("Incompatible type in System.out.println statement (got %s, required boolean or int)\n", expr_type);
                    exit(0);
                }
                break;

            case Return:
                if(node->child != NULL)
                {
                    expr_type = expr_recursive(prog, name, node->child);
                    expr_type2 = method_return(prog, name);
                    if(strcmp(expr_type2, expr_type) != 0)
                    {
                        printf("Incompatible type in return statement (got %s, required %s)\n", expr_type, expr_type2);
                        exit(0);
                    }
                } else
                {
                    expr_type2 = method_return(prog, name);
                    if(strcmp(expr_type2, "void") != 0)
                    {
                        printf("Incompatible type in return statement (got void, required %s)\n", expr_type2);
                        exit(0);
                    }
                }
                break;

            case Store:
                expr_type = expr_recursive(prog, name, node->child);
                expr_type2 = expr_recursive(prog, name, node->child->next);
                if(strcmp(expr_type, expr_type2) != 0)
                {
                    printf("Incompatible type in assignment to %s (got %s, required %s)\n", node->child->id, expr_type2, expr_type);
                    exit(0);
                }
                break;

            case StoreArray:
                expr_type = expr_recursive(prog, name, node->child);
                expr_type2 = expr_recursive(prog, name, node->child->next);
                expr_type3 = expr_recursive(prog, name, node->child->next->next);

                if(strcmp(expr_type, "int[]") != 0 && strcmp(expr_type, "boolean[]") != 0)
                {
                    printf("Operator [ cannot be applied to types %s, %s\n", expr_type, expr_type2);
                    exit(0);
                }

                if(strcmp(expr_type2, "int") != 0)
                {
                    printf("Operator [ cannot be applied to types %s, %s\n", expr_type, expr_type2);
                    exit(0);
                }

                if(strcmp(expr_type, "boolean[]") == 0)
                    expr_type = "boolean";
                else if(strcmp(expr_type, "int[]") == 0)
                    expr_type = "int";

                if(strcmp(expr_type, expr_type3) != 0)
                {
                    printf("Incompatible type in assignment to %s[] (got %s, required %s)\n", node->child->id, expr_type3, expr_type);
                    exit(0);
                }
                break;

            case While:
                expr_type = expr_recursive(prog, name, node->child);

                if(strcmp(expr_type, "boolean") != 0)
                {
                    printf("Incompatible type in while statement (got %s, required boolean)\n", expr_type);
                    exit(0);
                }
                semantic_statements(prog, name, node->child->next);
                break;

            default:
                break;
        }
        node = node->next;
    }
}

char* expr_recursive(prog_env* prog, char* method_name, is_node* expr)
{
    char* expr_type;
    char* expr_type1;

    switch (expr->type)
    {

        case LoadArray:
            expr_type = expr_recursive(prog, method_name, expr->child);
            expr_type1 = expr_recursive(prog, method_name, expr->child->next);

            if((strcmp(expr_type, "int[]") != 0 && strcmp(expr_type, "boolean[]") != 0) || strcmp(expr_type1, "int") != 0)
            {
                printf("Operator [ cannot be applied to types %s, %s\n", expr_type, expr_type1);
                exit(0);
            }

            if(strcmp(expr_type, "int[]") == 0)
                expr_type = "int";
            else if(strcmp(expr_type, "boolean[]") == 0)
                expr_type = "boolean";

            return expr_type;
            break;



        default:
            break;
    }
    return "erro";
}

void check_symbol_existence(is_node* node, prog_env* prog, char* method_name, int type)
{
    int found = 0;

    table_element* temp;
    temp = prog->global;

    environment_list* aux;
    aux = prog->methods;

    if(type == 0) // procurar variavel
    {
        // verifica nas variveis globais
        while(temp != NULL)
        {
            if(strcmp(temp->name, node->id) == 0 && strcmp(temp->type, "method") != 0)
                found = 1;
            temp = temp->next;
        }

        // verifica nas variaveis locais do metodo em q esta
        while(aux != NULL)
        {
            if(strcmp(aux->name, method_name) == 0)
            {
                table_element* local = aux->locals;
                while(local != NULL)
                {
                    if(strcmp(local->name, node->id) == 0)
                        found = 1;
                    local = local->next;
                }
            }
            aux = aux->next;
        }

    } else // procurar metodo
    {
        //printf("Verificar se o metodo %s existe\n", node->id);
        while(aux != NULL)
        {
            if(strcmp(aux->name, node->id) == 0)
            {
                //printf("metodo encontrado\n\n");
                found = 1;
            }
            aux = aux->next;
        }
    }

    if(found == 0)
    {
        printf("Cannot find symbol %s\n", node->id);
        exit(0);
    }
}

void check_arguments(is_node* call, prog_env* prog, char* method_name)
{
    char* method_called = call->id;
    char* tipo;
    char* tipo2;
    call = call->next;
    int n_argument = 0;

    while(call != NULL)
    {
        tipo = expr_recursive(prog, method_name, call);
        tipo2 = getArgumentType(prog, method_called, n_argument);

        if(strcmp(tipo, tipo2) != 0)
        {
            printf("Incompatible type of argument %d in call to method %s (got %s, required %s)\n", n_argument, method_called, tipo, tipo2);
            exit(0);
        }
        call = call->next;
        n_argument++;
    }

    tipo2 = getArgumentType(prog, method_called, n_argument);
    if(strcmp(tipo2, "void") != 0)
    {
        printf("Incompatible type of argument %d in call to method %s (got void, required %s)\n", n_argument, method_called, tipo2);
        exit(0);
    }
}

void check_literal(char* lit)
{
    if(strlen(lit) > 1 && lit[0] == '0' && lit[1] != 'x')
    {
        printf("Invalid literal %s\n", lit);
		exit(0);
    }
}

void check_defined(char* name, char* nameNew)
{
	if(strcmp(name, nameNew) == 0)
    {
		printf ("Symbol %s already defined\n", name);
		exit(0);
	}
}

char* getType(char* nameP, prog_env* prog, char* methodName)
{
	table_element* temp;
	temp = prog->global;

	environment_list* aux;
	aux = prog->methods;

    // verifica variaveis locais do metodo em questao
	while(aux != NULL)
    {
		if(strcmp(aux->name, methodName) == 0)
        {
			table_element* local = aux->locals;
			while(local != NULL)
            {
				if(strcmp(local->name, nameP) == 0)
					return local->type;
				local = local->next;
			}
		}
		aux = aux->next;
	}

	// verifica variaveis globais
	while(temp != NULL)
    {
		if(strcmp(temp->name, nameP) == 0 && strcmp(temp->type, "method") != 0)
			return temp->type;
		temp = temp->next;
	}
	return NULL;
}

char* method_return(prog_env* prog, char* method_name)
{
	environment_list* aux;
	aux = prog->methods;

	while(aux != NULL)
    {
		if(strcmp(aux->name, method_name) == 0)
        {
			table_element* local = aux->locals;
            return local->type;
        }
        aux = aux->next;
    }
    return NULL;
}

char* getArgumentType(prog_env* prog, char* method_called, int index)
{
    environment_list* aux;
	aux = prog->methods;

    while(aux != NULL) // percorre a lista de metodos
    {
        if(strcmp(method_called, aux->name) == 0) // se encontrar o metodo
        {
            int counter = 0;
            table_element* local = aux->locals; // percorrer a lista de simbolos
            local = local->next; // ignorar o return
            while(local != NULL)
            {
                if(counter == index)
                    return(local->type);
                local = local->next;
                counter++;
            }
        }
        aux = aux->next;
    }
    return "void";
}




#include "structures.h"
#include "semantics.h"
#include "symbol_table.h"
#include "codegen.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char* gen_type[] = {"Program", "VarDecl", "MethodDecl", "MethodParams", "MethodBody", "ParamDeclaration", "CompoundStat", "IfElse", "Print", "Return", "Store", "StoreArray", "While", "Or", "And", "Eq", "Neq", "Lt", "Gt", "Leq", "Geq", "Add", "Sub", "Mul", "Div", "Mod", "Not", "Minus", "Plus", "Length", "LoadArray", "Call", "NewInt", "NewBool", "ParseArgs", "Int", "Bool", "IntArray", "BoolArray", "StringArray", "Void", "Id", "IntLit", "BoolLit", "Null"};
int counter = 0;
int if_c = 0;

void generate_code(is_node* node, prog_env* mySemantic)
{
    is_node* aux;
    is_node* aux2;
    char* method_name;
    char* return_type;
    printf("\ndeclare i32 @printf(i8*, ...)\n");
    printf("declare i32 @atoi(i8*) nounwind readonly\n");
    printf("@str = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\"\n");
    printf("@.str2 = private unnamed_addr constant [4 x i8] c\"%%s\\0A\\00\"\n");
    printf("@.str = private unnamed_addr constant [5 x i8] c\"true\\00\"\n");
    printf("@.true = global i8* getelementptr inbounds ([5 x i8]* @.str, i32 0, i32 0)\n");
    printf("@.str1 = private unnamed_addr constant [6 x i8] c\"false\\00\"\n");
    printf("@.false = global i8* getelementptr inbounds ([6 x i8]* @.str1, i32 0, i32 0)\n\n");
    aux = node->child;
    while (aux != NULL){
        if (aux->type == VarDecl) {
            if (aux->child->type == Int) {
                aux2 = aux->child->next;
                while (aux2 != NULL){
                    printf("@%s = global i32 0\n", aux2->id);
                    aux2 = aux2->next;
                }
            }else
                if (aux->child->type == Bool) {
                    aux2 = aux->child->next;
                    while (aux2 != NULL){
                        printf("@%s = global i1 0\n", aux2->id);
                        aux2 = aux2->next;
                    }
                }
        }else
            if (aux->type == MethodDecl) {
                counter = 0;
                method_name = aux->child->next->id;
                return_type = gen_type[aux->child->type];
                if (strcmp(method_name, "main") == 0 && aux->child->next->next->child != NULL) 
                    printf("\ndefine i32 @main(i32 %%.argc, i8** %%.argv)\n{\n");
                else{
                    char* return_type = gen_code_type(gen_type[aux->child->type]);
                    is_node* params = aux->child->next->next;
                    printf("\ndefine %s @%s(", return_type, method_name);
                    if (params->child != NULL) {
                        is_node* paramDecl = params->child;
                        while (paramDecl != NULL){
                            if (paramDecl->next != NULL) 
                                printf("%s %%%s, ", gen_code_type(gen_type[paramDecl->child->type]), paramDecl->child->next->id);
                            else
                                printf("%s %%%s", gen_code_type(gen_type[paramDecl->child->type]), paramDecl->child->next->id);

                            paramDecl = paramDecl->next;
                        }
                    }
                    printf(")\n{\n");
                }
                aux2 = aux->child;
                while (aux2 != NULL){
                    if (aux2->type == MethodBody) 
                        gen_statements(method_name, aux2->child, mySemantic);

                    aux2 = aux2->next;
                }
                printf("\tret %s 0\n}\n", gen_code_type(return_type));
            }

        aux = aux->next;
    }
}

void gen_statements(char* method_name, is_node* node, prog_env* prog)
{
    char* expr;
    char* type;
    is_node* aux;
    int glob;
    int if_else_c;
    while (node != NULL){
        switch (node->type){
            case VarDecl:
                aux = node->child->next;
                while (aux != NULL){
                    printf("\t%%%s = alloca %s\n", aux->id, gen_code_type(gen_type[node->child->type]));
                    aux = aux->next;
            }
            break;
        case CompoundStat:
            break;
        case IfElse:
            expr = gen_expression(method_name, node->child, prog);
            if_else_c = 0;
            printf("%%if_%d = icmp eq i1 1 , %s\n", if_else_c, expr);
            printf("br i1 %%if_%d, label %%then_%d, label %%else_%d \n", if_else_c, if_else_c, if_else_c);
            break;
        case Print:
            if (node->child->type == Id) 
                type = get_type(node->child->id, prog, method_name);
            else
                type = gen_code_type(gen_type[node->child->type]);

            if (strcmp(type, "i32") == 0) {
                expr = gen_expression(method_name, node->child, prog);
                counter++;
                printf("\t%%%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @str, i32 0, i32 0), i32 %s)\n", counter, expr);
        }else{
            expr = gen_expression(method_name, node->child, prog);
            if_c++;
            printf("\t%%ifcond_%d = icmp eq i1 1 , %s\n", if_c, expr);
            printf("\tbr i1 %%ifcond_%d, label %%then_%d, label %%else_%d\n", if_c, if_c, if_c);
            printf("\tthen_%d:\n", if_c);
            counter++;
            printf("\t%%%d = load i8** @.true\n", counter);
            printf("\t%%%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str2, i32 0, i32 0), i8* %%%d)\n", counter + 1, counter);
            printf("\tbr label %%ifcount_%d\n", if_c);
            counter++;
            printf("\telse_%d:\n", if_c);
            printf("\t%%%d = load i8** @.false\n", ++counter);
            printf("\t%%%d = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8]* @.str2, i32 0, i32 0), i8* %%%d)\n", counter + 1, counter);
            printf("\tbr label %%ifcount_%d\n", if_c);
            counter++;
            printf("\tifcount_%d:\n", if_c);
        }
        break;
    case Return:
        break;
    case Store:
        glob = verifyGlobal(node->child->id, prog, method_name);
        type = get_type(node->child->id, prog, method_name);
        expr = gen_expression(method_name, node->child->next, prog);
        if (!glob) 
            printf("\tstore %s %s, %s* %%%s\n", type, expr, type, node->child->id);
        else
            printf("\tstore %s %s, %s* @%s\n", type, expr, type, node->child->id);

        break;
    case StoreArray:
        break;
    case While:
        break;
    default:
        break;
}
node = node->next;
}
}

char* gen_expression(char* method_name, is_node* expr, prog_env* prog)
{
char* expr_type;
char* expr_type1;
int glob;
char* c = (char*)malloc(sizeof (char*));
switch (expr->type){
case Call:
    break;
case Or:
    break;
case And:
    break;
case Not:
    break;
case Eq:
    break;
case Neq:
    break;
case Lt:
    break;
case Gt:
    break;
case Leq:
    break;
case Geq:
    break;
case Add:
    expr_type = gen_expression(method_name, expr->child, prog);
    expr_type1 = gen_expression(method_name, expr->child->next, prog);
    printf("\t%%%d = add i32 %s,%s\n", ++counter, expr_type, expr_type1);
    sprintf(c, "%%%d", counter);
    return c;
    break;
case Sub:
    break;
case Mul:
    break;
case Div:
    break;
case Mod:
    break;
case Plus:
    break;
case Minus:
    break;
case Length:
    c = (char*)malloc(sizeof (char*));
    counter++;
    printf("\t%%%d = sub i32 %%.argc, 1\n", counter);
    sprintf(c, "%%%d", counter);
    return c;
    break;
case LoadArray:
    break;
case NewInt:
    break;
case NewBool:
    break;
case ParseArgs:
    counter++;
    printf("\t%%%d = getelementptr inbounds i8** %%.argv, i64 %d\n", counter, atoi(expr->child->next->id) + 1);
    printf("\t%%%d = load i8** %%%d\n", counter + 1, counter);
    counter += 2;
    printf("\t%%%d = call i32 @atoi(i8* %%%d)\n", counter, counter - 1);
    c = (char*)malloc(sizeof (char*));
    sprintf(c, "%%%d", counter);
    return c;
    break;
case IntLit:
    c = (char*)malloc(sizeof (char*));
    int hexa;
    hexa = strtol(expr->id, NULL, 0);
    sprintf(c, "%d", hexa);
    return c;
    break;
case BoolLit:
    if (strcmp(expr->id, "true") == 0) 
        return "1";
    else
        return "0";

    break;
case Id:
    glob = verifyGlobal(expr->id, prog, method_name);
    expr_type = get_type(expr->id, prog, method_name);
    counter++;
    sprintf(c, "%%%d", counter);
    if (!glob) 
        printf("\t%%%d = load %s* %%%s\n", counter, expr_type, expr->id);
    else
        printf("\t%%%d = load %s* @%s\n", counter, expr_type, expr->id);

    return c;
    break;
default:
    break;
}
return "erro";
}

char* gen_code_type(char* AST_type)
{
if (strcmp(AST_type, "Int") == 0) 
return "i32";
else
    if (strcmp(AST_type, "Bool") == 0) 
        return "i1";
    else
        if (strcmp(AST_type, "Void") == 0) 
            return "i32";
        else
            if (strcmp(AST_type, "IntLit") == 0) 
                return "i32";
            else
                if (strcmp(AST_type, "BoolLit") == 0) 
                    return "i1";
                else
                    if (strcmp(AST_type, "StringArray") == 0) 
                        return "args";
                    else
                        if (strcmp(AST_type, "IntArray") == 0) 
                            return "i32";
                        else
                            if (strcmp(AST_type, "BoolArray") == 0) 
                                return "i1";
                            else
                                if (strcmp(AST_type, "ParseArgs") == 0) 
                                    return "i32";
                                else
                                    if (strcmp(AST_type, "Add") == 0) 
                                        return "i32";
                                    else
                                        if (strcmp(AST_type, "Sub") == 0) 
                                            return "i32";
                                        else
                                            if (strcmp(AST_type, "Mul") == 0) 
                                                return "i32";
                                            else
                                                if (strcmp(AST_type, "Div") == 0) 
                                                    return "i32";
                                                else
                                                    if (strcmp(AST_type, "Mod") == 0) 
                                                        return "i32";
                                                    else
                                                        if (strcmp(AST_type, "Or") == 0) 
                                                            return "i1";
                                                        else
                                                            if (strcmp(AST_type, "And") == 0) 
                                                                return "i1";
                                                            else
                                                                if (strcmp(AST_type, "Eq") == 0) 
                                                                    return "i1";
                                                                else
                                                                    if (strcmp(AST_type, "Neq") == 0) 
                                                                        return "i1";
                                                                    else
                                                                        if (strcmp(AST_type, "Lt") == 0) 
                                                                            return "i1";
                                                                        else
                                                                            if (strcmp(AST_type, "Gt") == 0) 
                                                                                return "i1";
                                                                            else
                                                                                if (strcmp(AST_type, "Leq") == 0) 
                                                                                    return "i1";
                                                                                else
                                                                                    if (strcmp(AST_type, "Geq") == 0) 
                                                                                        return "i1";
                                                                                    else
                                                                                        if (strcmp(AST_type, "Not") == 0) 
                                                                                            return "i1";
                                                                                        else
                                                                                            if (strcmp(AST_type, "Minus") == 0) 
                                                                                                return "i32";
                                                                                            else
                                                                                                if (strcmp(AST_type, "Plus") == 0) 
                                                                                                    return "i32";
                                                                                                else
                                                                                                    if (strcmp(AST_type, "Length") == 0) 
                                                                                                        return "i32";
                                                                                                    else
                                                                                                        if (strcmp(AST_type, "int") == 0) 
                                                                                                            return "i32";
                                                                                                        else
                                                                                                            if (strcmp(AST_type, "boolean") == 0) 
                                                                                                                return "i1";
                                                                                                            else
                                                                                                                if (strcmp(AST_type, "void") == 0) 
                                                                                                                    return "i32";
                                                                                                                else
                                                                                                                    if (strcmp(AST_type, "int[]") == 0) 
                                                                                                                        return "i32";
                                                                                                                    else
                                                                                                                        if (strcmp(AST_type, "boolean[]") == 0) 
                                                                                                                            return "i1";
                                                                                                                        else
                                                                                                                            if (strcmp(AST_type, "String[]") == 0) 
                                                                                                                                return "args";

printf("\n\n>>>> ERRO <<<< method gen_code_type()\n\n");
return "erro";
}

int verifyGlobal(char* var_name, prog_env* prog, char* method_name)
{
table_element* temp;
temp = prog->global;
environment_list* aux;
aux = prog->methods;
while (aux != NULL){
    if (strcmp(aux->name, method_name) == 0) {
        table_element* local = aux->locals;
        while (local != NULL){
            if (strcmp(local->name, var_name) == 0) 
                return 0;

            local = local->next;
        }
    }
    aux = aux->next;
}
while (temp != NULL){
    if (strcmp(temp->name, var_name) == 0 && strcmp(temp->type, "method") != 0) 
        return 1;

    temp = temp->next;
}
return 0;
}
char* get_type(char* var_name, prog_env* prog, char* method_name)
{
table_element* temp;
temp = prog->global;
environment_list* aux;
aux = prog->methods;
while (aux != NULL){
    if (strcmp(aux->name, method_name) == 0) {
        table_element* local = aux->locals;
        while (local != NULL){
            if (strcmp(local->name, var_name) == 0) 
                return gen_code_type(local->type);

            local = local->next;
        }
    }
    aux = aux->next;
}
while (temp != NULL){
    if (strcmp(temp->name, var_name) == 0 && strcmp(temp->type, "method") != 0) 
        return gen_code_type(temp->type);

    temp = temp->next;
}
return NULL;
}

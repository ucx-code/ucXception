#include "functions.h"
#include "structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
is_node* insertProgram(is_node* progbody)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = progbody;
    node->next = NULL;
    node->type = Program;
    node->id = NULL;
    return node;
}

is_node* insertProgAux(is_node* id, is_node* fieldsmethods)
{
    if (fieldsmethods != NULL) 
        id->next = fieldsmethods;

    return id;
}

is_node* insertMethodDecl(is_node* type, is_node* id, is_node* params, is_node* varDecl, is_node* stats)
{
    int changes = 0;
    is_node* node = (is_node*)malloc(sizeof (is_node));
    is_node* methodParams = (is_node*)malloc(sizeof (is_node));
    node->child = type;
    type->next = id;
    if (params != NULL) {
        id->next = params;
        changes = 1;
    }else{
        methodParams->type = MethodParams;
        methodParams->id = NULL;
        methodParams->next = NULL;
        id->next = methodParams;
    }
    is_node* methodBody = (is_node*)malloc(sizeof (is_node));
    methodBody->type = MethodBody;
    methodBody->id = NULL;
    methodBody->next = NULL;
    methodBody->child = NULL;
    if (changes == 1) 
        params->next = methodBody;
    else
        methodParams->next = methodBody;

    if (varDecl != NULL) {
        is_node *aux, *aux2;
        methodBody->child = varDecl;
        if (stats != NULL) {
            aux = varDecl;
            while (aux->next != NULL)
                aux = aux->next;
            aux->next = stats;
        }
    }else{
        if (stats != NULL) 
            methodBody->child = stats;
    }
    node->next = NULL;
    node->type = MethodDecl;
    node->id = NULL;
    return node;
}
is_node* insertFormalParams1(is_node* type, is_node* id)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    is_node* MethodParamsnode = (is_node*)malloc(sizeof (is_node));
    node->child = type;
    type->next = id;
    node->type = ParamDeclaration;
    node->id = NULL;
    MethodParamsnode->child = node;
    MethodParamsnode->next = NULL;
    MethodParamsnode->id = NULL;
    MethodParamsnode->type = MethodParams;
    return MethodParamsnode;
}

is_node* insertFormalParams2(is_node* type, is_node* id, is_node* moreNodes)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    is_node* MethodParamsnode = (is_node*)malloc(sizeof (is_node));
    node->child = type;
    type->next = id;
    node->type = ParamDeclaration;
    node->id = NULL;
    if (moreNodes != NULL) 
        node->next = moreNodes;

    MethodParamsnode->child = node;
    MethodParamsnode->next = NULL;
    MethodParamsnode->id = NULL;
    MethodParamsnode->type = MethodParams;
    return MethodParamsnode;
}

is_node* insertFPRepetition(is_node* type, is_node* id, is_node* extra_node)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = type;
    type->next = id;
    node->type = ParamDeclaration;
    node->id = NULL;
    if (extra_node != NULL) 
        node->next = extra_node;

    return node;
}
is_node* insertVarDecl(is_node* type, is_node* id, is_node* moreIDS)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = type;
    type->next = id;
    if (moreIDS != NULL) 
        id->next = moreIDS;

    node->type = VarDecl;
    node->id = NULL;
    return node;
}
is_node* insertST_compoundstat(is_node* stat)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    if (stat != NULL) {
        if (stat->next != NULL) {
            node->child = stat;
            node->next = NULL;
            node->type = CompoundStat;
            node->id = NULL;
        }else
            return stat;
    }else
        return NULL;

    return node;
}
is_node* insertST_if_else(is_node* expr, is_node* stat, is_node* stat2)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->type = IfElse;
    node->id = NULL;
    node->child = expr;
    if (stat != NULL && stat2 != NULL) {
        expr->next = stat;
        stat->next = stat2;
    }else
        if (stat != NULL && stat2 == NULL) {
            expr->next = stat;
            stat->next = nullNode();
        }else
            if (stat == NULL && stat2 != NULL) {
                expr->next = nullNode();
                expr->next->next = stat2;
            }else{
                expr->next = nullNode();
                expr->next->next = nullNode();
            }

    return node;
}
is_node* insertST_while_expr_stat(is_node* expr, is_node* stat)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr;
    if (stat != NULL) 
        expr->next = stat;
    else
        expr->next = nullNode();

    node->type = While;
    node->id = NULL;
    return node;
}

is_node* insertST_print_expr_sem(is_node* expr)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr;
    node->type = Print;
    node->id = NULL;
    return node;
}
is_node* insertST_id_expr_assign_expr(is_node* id, is_node* expr1, is_node* expr2)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = id;
    id->next = expr1;
    expr1->next = expr2;
    node->type = StoreArray;
    node->id = NULL;
    return node;
}

is_node* insertST_id_assign_exp_sem(is_node* id, is_node* expr)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = id;
    id->next = expr;
    node->type = Store;
    node->id = NULL;
    return node;
}

is_node* insertST_ret_exp_sem(is_node* expr)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr;
    node->next = NULL;
    node->type = Return;
    node->id = NULL;
    return node;
}

is_node* insertST_ret_null()
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = NULL;
    node->next = NULL;
    node->type = Return;
    node->id = NULL;
    return node;
}

is_node* insert_expr_ope_expr(is_node* expr1, char* oper, is_node* expr2)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr1;
    expr1->next = expr2;
    node->id = NULL;
    if (strcmp(oper, "||") == 0) 
        node->type = Or;
    else
        if (strcmp(oper, "&&") == 0) 
            node->type = And;
        else
            if (strcmp(oper, "==") == 0) 
                node->type = Eq;
            else
                if (strcmp(oper, "!=") == 0) 
                    node->type = Neq;
                else
                    if (strcmp(oper, "<") == 0) 
                        node->type = Lt;
                    else
                        if (strcmp(oper, ">") == 0) 
                            node->type = Gt;
                        else
                            if (strcmp(oper, "<=") == 0) 
                                node->type = Leq;
                            else
                                if (strcmp(oper, ">=") == 0) 
                                    node->type = Geq;
                                else
                                    if (strcmp(oper, "+") == 0) 
                                        node->type = Add;
                                    else
                                        if (strcmp(oper, "-") == 0) 
                                            node->type = Sub;
                                        else
                                            if (strcmp(oper, "*") == 0) 
                                                node->type = Mul;
                                            else
                                                if (strcmp(oper, "/") == 0) 
                                                    node->type = Div;
                                                else
                                                    if (strcmp(oper, "%") == 0) 
                                                        node->type = Mod;

    return node;
}

is_node* insert_expr_squares_expr(is_node* expr1, is_node* expr2)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr1;
    expr1->next = expr2;
    node->type = LoadArray;
    node->id = NULL;
    return node;
}
is_node* insert_length(is_node* expr)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr;
    node->id = NULL;
    node->type = Length;
    return node;
}

is_node* insert_Oper_Exp(char* oper, is_node* expr)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr;
    node->id = NULL;
    if (strcmp(oper, "!") == 0) 
        node->type = Not;
    else
        if (strcmp(oper, "-") == 0) 
            node->type = Minus;
        else
            if (strcmp(oper, "+") == 0) 
                node->type = Plus;

    return node;
}

is_node* insert_new_exp(node_type type, is_node* expr)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = expr;
    node->type = type;
    node->id = NULL;
    return node;
}
is_node* insert_ParseInt(is_node* id, is_node* expr)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = id;
    id->next = expr;
    node->type = ParseArgs;
    node->id = NULL;
    return node;
}

is_node* insert_id_args(is_node* id, is_node* args)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = id;
    if (args != NULL) 
        id->next = args;

    node->type = Call;
    node->id = NULL;
    return node;
}

is_node* insert_ID(char* str)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = NULL;
    node->next = NULL;
    node->type = Id;
    node->id = str;
    return node;
}
is_node* insert_INTLIT(char* str)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = NULL;
    node->next = NULL;
    node->type = IntLit;
    node->id = str;
    return node;
}

is_node* insert_BOOLLIT(char* str)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = NULL;
    node->next = NULL;
    node->type = BoolLit;
    node->id = str;
    return node;
}

is_node* insertType(node_type type)
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = NULL;
    node->next = NULL;
    node->id = NULL;
    node->type = type;
    return node;
}

is_node* insertRepetition(is_node* x, is_node* y)
{
    if (x->next == NULL) 
        x->next = y;
    else{
        y->next = x->next;
        x->next = y;
    }
    return x;
}

is_node* insertStatRepetition(is_node* stat, is_node* stat2)
{
    if (stat != NULL && stat2 != NULL) 
        stat->next = stat2;
    else
        if (stat == NULL && stat2 != NULL) 
            return stat2;

    return stat;
}

is_node* nullNode()
{
    is_node* node = (is_node*)malloc(sizeof (is_node));
    node->child = NULL;
    node->next = NULL;
    node->id = NULL;
    node->type = Null;
    return node;
}

#define _GNU_SOURCE // for asprintf, or gives a warning
#include "./tac.h"
#include "./ast.h"

int varCount, lableID;

char *freshVar()
{
    char *varName;
    // auto allocation
    asprintf(&varName, "t%d", varCount++);
    return varName;
}

bool isMathNode(char *str)
{
    if (strcmp(str, "+") == 0)
        return true;
    if (strcmp(str, "-") == 0)
        return true;
    if (strcmp(str, "*") == 0)
        return true;
    if (strcmp(str, "/") == 0)
        return true;
    return false;
}

bool isFuncCall(char *str)
{
    if (strcmp(str, "FUNCTION CALL NO PARAMS") == 0)
        return true;

    if (strcmp(str, "FUNCTION CALL") == 0)
        return true;
    return false;
}

bool isPtrNode(char *str)
{
    if (strcmp(str, "&") == 0)
        return true;
    if (strcmp(str, "^") == 0)
        return true;
    return false;
}

bool isSimpleNode(char *str)
{
    if (strcmp(str, "INTEGER") == 0)
        return true;
    if (strcmp(str, "IDENT") == 0)
        return true;
    if (strcmp(str, "boolean") == 0)
        return true;

    return false;
}

bool isBooleanNode(char *str)
{
    if (strcmp(str, "==") == 0)
        return true;

    if (strcmp(str, "!=") == 0)
        return true;
    if (strcmp(str, ">") == 0)
        return true;
    if (strcmp(str, "<") == 0)
        return true;
    if (strcmp(str, ">=") == 0)
        return true;
    if (strcmp(str, "<=") == 0)
        return true;
    return false;
}

bool isNegNode(char *str)
{
    if (strcmp(str, "NEG") == 0)
        return true;

    if (strcmp(str, "!") == 0)
        return true;
    return false;
}

char *getId(Node *head)
{
    printf("$$: getId\n");
    while (head)
    {
        if (isSimpleNode(head->data))
            return head->left->data;
        head = head->left;
    }
    return "ERROR";
}

char *parseUnary(char *str)
{
    if (strcmp(str, "NEG") == 0)
        return "-";
    if (strcmp(str, "!") == 0)
        return "!";
}
/** 
 * Program start
 */
char *genProg(Node *ast)
{
    printf("$$: genProg\n");

    char *code = "";
    if (strcmp(ast->left->data, "CODE") == 0)
    {
        asprintf(&code, "%s", genCode(ast->left, NULL)); // handle left multi block
        if (ast->right)
            asprintf(&code, "%s%s", code, genStatment(ast->right, NULL)); // handle right single block
    }
    else
        asprintf(&code, "%s", genStatment(ast->left, NULL)); // handle right single block

    return code;
}
/** 
 * Nodes handling
 */
char *genCode(Node *codeNode, int *size)
{
    printf("$$: genCode\n");
    char *code = "";

    if (!codeNode)
        return code;

    if (strcmp(codeNode->left->data, "CODE") == 0)
        asprintf(&code, "%s", genCode(codeNode->left, size)); // handle left multi block
    else
        asprintf(&code, "%s", genStatment(codeNode->left, size)); // handle left single block
    if (codeNode->right)
        asprintf(&code, "%s%s", code, genStatment(codeNode->right, size)); // handle right single block
    return code;
}

/** 
 * MAIN STATMENT HANDLING
 */
char *genStatment(Node *stmtNode, int *size)
{
    printf("$$: genStatment\n");

    char *code = "";
    if (!stmtNode)
        return code;

    if (strcmp(stmtNode->data, "FUNCTION") == 0)
        return genFunc(stmtNode);

    if (strcmp(stmtNode->data, "RETURN") == 0)
        return genReturn(stmtNode, size);

    if (strcmp(stmtNode->data, "=") == 0 || strcmp(stmtNode->data, "^=") == 0)
        return genAss(stmtNode, size);

    if (strcmp(stmtNode->data, "VARIABLES DECLERATION") == 0)
        return "";

    if (strcmp(stmtNode->data, "IF") == 0)
        return "";

    else
        asprintf(&code, "%s%s", code, "unhandled statement"); // handle right single block
    return code;
}

char *genReturn(Node *returnNode, int *size)
{
    char *code = "";
    Tac *E = genExrp(returnNode->left, size);
    asprintf(&code, "%s\tReturn %s\n", E->code, E->var);
    return code;
}

char *genFunc(Node *funcNode)
{
    printf("$$: genFunc\n");

    int size = 0;
    char *code = "";

    // for function inside a function
    // save global counters in temps
    int currVarCount = varCount;
    int currLableID = lableID;
    // reset counters
    varCount = 0;
    lableID = 0; // lables start from line 2

    code = genBlock(funcNode->right, &size);

    varCount = currVarCount;
    lableID = currLableID;

    asprintf(&code, "%s:\n\tBeginFunc %d\n%s\tEndFunc\n", funcNode->left->middle->left->data, size, code);
    return code;
}

char *genBlock(Node *blockNode, int *size)
{
    printf("$$: genBlock\n");

    char *code = "";

    if (strcmp(blockNode->data, "EMPTY BLOCK") == 0)
        return code;

    if (strcmp(blockNode->left->data, "CODE") == 0)
        asprintf(&code, "%s", genCode(blockNode->left, size)); // handle left multi block
    else
        asprintf(&code, "%s", genStatment(blockNode->left, size)); // handle right single block

    if (blockNode->right)
        asprintf(&code, "%s%s", code, genStatment(blockNode->right, size)); // handle right single block

    return code;
}

char *genAss(Node *assNode, int *size)
{
    printf("$$: genAss\n");
    char *code = "";
    *size += getSize(assNode->right); // test
    Tac *E = genExrp(assNode->right, size);
    asprintf(&code, "%s%s", E->code, genTac(assNode->left->left->left->data, "=", E->var));
    return code;
}

char *genTac(char *idvar, char *str, char *evar)
{
    printf("$$: genTac\n");
    char *code = "";
    asprintf(&code, "L%d\t%s%s%s \n", lableID, idvar, str, evar);
    lableID += 1;
    return code;
}

Tac *pushParams(Node *paramsNode, int *size)
{
    Tac *E = calloc(1, sizeof(Tac));
    E->code = "";
    E->var = "";

    if (strcmp(paramsNode->data, "EXPR") == 0)
    {
        Tac *E1 = genExrp(paramsNode, size);
        asprintf(&(E->code), "%s%s", E1->code, genTac("PushParam", " ", E1->var));
    }
    else
    {
        Tac *E1 = pushParams(paramsNode->left, size);
        Tac *E2 = pushParams(paramsNode->right, size);
        asprintf(&(E->code), "%s%s", E1->code, E2->code);
    }

    return E;
}

Tac *genExrp(Node *exprNode, int *size)
{
    printf("$$: genExrp\n");
    printInOrder(exprNode, 0);

    Tac *E = calloc(1, sizeof(Tac));
    E->code = "";
    E->var = "";

    if (!exprNode)
        return E;

    if (strcmp(exprNode->left->data, "EXPR") == 0)
        return genExrp(exprNode->left, size);

    if (isFuncCall(exprNode->left->data))
    {
        if (strcmp(exprNode->left->data, "FUNCTION CALL") == 0)
        {
            E->var = freshVar();
            *size += getSize(exprNode);
            int currSize = *size;
            Tac *params = pushParams(exprNode->left->right, size);
            char *popSize = "";
            asprintf(&popSize, "%d", *size - currSize);
            char *paramsCall = genTac(E->var, "=LCall ", getId(exprNode->left));
            char *paramsPop = genTac("Pop", " ", popSize);
            asprintf(&(E->code), "%s%s%s%s", params->code, E->code, paramsCall, paramsPop);
        }
        else
        {
            E->var = freshVar();
            *size += getSize(exprNode);
            asprintf(&(E->code), "%s%s", E->code, genTac(E->var, "= LCall ", getId(exprNode->left)));
        }
    }

    else if (isSimpleNode(exprNode->left->data) || isPtrNode(exprNode->left->data))
    {
        E->var = freshVar();
        *size += getSize(exprNode);
        asprintf(&(E->code), "%s%s", E->code, genTac(E->var, "=", getId(exprNode->left)));
    }

    else if (isMathNode(exprNode->left->data))
    {
        E->var = freshVar();
        Tac *E1 = genExrp(exprNode->left->left, size);
        Tac *E2 = genExrp(exprNode->left->right, size);
        char *gen = "";
        asprintf(&gen, "%s %s %s", E1->var, exprNode->left->data, E2->var);
        asprintf(&(E->code), "%s%s%s", E1->code, E2->code, genTac(E->var, "=", gen));
    }

    else if (isNegNode(exprNode->left->data))
    {
        E->var = freshVar();
        *size += getSize(exprNode->left->left);
        char *sign = "";
        asprintf(&sign, "=%s", parseUnary(exprNode->left->data));
        asprintf(&(E->code), "%s%s", E->code, genTac(E->var, sign, getId(exprNode)));
    }

    else if (isBooleanNode(exprNode->left->data))
    {
        E->var = freshVar();
        Tac *E1 = genExrp(exprNode->left->left, size);
        Tac *E2 = genExrp(exprNode->left->right, size);
        char *gen = "";
        asprintf(&gen, "%s %s %s", E1->var, exprNode->left->data, E2->var);
        asprintf(&(E->code), "%s%s%s", E1->code, E2->code, genTac(E->var, "=", gen));
    }

    return E;
}

int getSize(Node *expr)
{
    printf("$$: getSize\n");

    if (expr->type == INTEGER)
        return 4;

    if (expr->type == STRING)
        return 8;

    if (expr->type == INT_PTR)
        return 8;

    if (expr->type == CHAR_PTR)
        return 8;

    if (expr->type == CHAR)
        return 1;
    if (expr->type == BOOLEAN)
        return 1;

    return 0;
}

int getVarsCount(Node *multiVars)
{
    printf("$$: getVarsCount\n");
    if (strcmp(multiVars->left->data, "MULTIPLE IDENTIFIERS") == 0)
        return getVarsCount(multiVars->left) + 1;

    return 2;
}

int getVarsSize(Node *varsNode)
{
    printf("$$: getVarsSize\n");
    int typeSize = getSize(varsNode->left);

    if (strcmp(varsNode->right->data, "MULTIPLE IDENTIFIERS") == 0)
        return typeSize * getVarsCount(varsNode->right);

    return typeSize;
}
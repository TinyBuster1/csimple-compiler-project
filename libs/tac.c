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

    return false;
}

char *getId(Node *head)
{
    printf("$$: getId\n");
    while (head)
    {
        printInOrder(head, 0);
        if (isSimpleNode(head->data))
            return head->left->data;
        head = head->left;
    }
    return "ERROR";
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

    if (strcmp(stmtNode->data, "=") == 0 || strcmp(stmtNode->data, "^=") == 0)
        return genAss(stmtNode, size);

    if (strcmp(stmtNode->data, "VARIABLES DECLERATION") == 0)
        return "";

    else
        asprintf(&code, "%s%s", code, "unhandled statement"); // handle right single block
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
    lableID = 2; // lables start from line 2

    // what to do with params???????????????????

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
    Tac *E = genExrp(assNode->right, size);
    asprintf(&code, "%s%s", E->code, genTac(assNode->left->left->left->data, "=", E->var));
    return code;
}

char *genTac(char *idvar, char *str, char *evar)
{
    printf("$$: genTac\n");
    char *code = "";
    asprintf(&code, "\t%s %s %s \n", idvar, str, evar);
    return code;
}

Tac *genExrp(Node *exprNode, int *size)
{
    printf("$$: genExrp\n");

    Tac *E = calloc(1, sizeof(Tac));
    E->code = "";
    E->var = "";

    if (isSimpleNode(exprNode->left->data) || isPtrNode(exprNode->left->data))
    {
        E->var = freshVar();
        asprintf(&(E->code), "%s%s", E->code, genTac(E->var, "=", getId(exprNode->left)));
    }

    else if (isMathNode(exprNode->left->data))
    {
        E->var = freshVar();
        Tac *E1 = genExrp(exprNode->left->left, size);
        Tac *E2 = genExrp(exprNode->left->right, size);
        char *gen = "";
        asprintf(&gen, "%s + %s", E1->var, E2->var);
        asprintf(&(E->code), "%s%s%s", E1->code, E2->code, genTac(E->var, "=", gen));
    }

    return E;
}

int getSize(Node *codeNode)
{
    printf("$$: getSize\n");
    if (!codeNode)
        return 0;

    if (strcmp(codeNode->data, "CODE") == 0)
        return getSize(codeNode->left) + getSize(codeNode->right);

    if (strcmp(codeNode->data, "VARIABLES DECLERATION") == 0)
        return getVarsSize(codeNode);

    if (strcmp(codeNode->data, "=") == 0)
        return getVarsSize(codeNode);

    return 0;
}
int getVarSize(Node *varsNode)
{
    printf("$$: getVarSize\n");
    if (strcmp(varsNode->data, "int") == 0)
        return 4;

    if (strcmp(varsNode->data, "char") == 0)
        return 1;
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
    int typeSize = getVarSize(varsNode->left);

    if (strcmp(varsNode->right->data, "MULTIPLE IDENTIFIERS") == 0)
        return typeSize * getVarsCount(varsNode->right);

    return typeSize;
}
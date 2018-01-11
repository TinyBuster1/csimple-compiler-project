#define _GNU_SOURCE // for asprintf, or gives a warning
#include "./tac.h"
#include "./ast.h"

int varCount, lableCount;

char *freshVar()
{
    char *varName;
    // auto allocation
    asprintf(&varName, "t%d", varCount++);
    return varName;
}

/** 
 * Program start
 */
char *genTac(Node *ast)
{
    printf("$$: genTac\n");
    // set global vars
    varCount = 0;
    lableCount = 0;

    char *code = "";
    // multiline
    if (strcmp(ast->left->data, "CODE") == 0)
    {
        asprintf(&code, "%s\n%s", code, genCode(ast->left));  // handle left multi block
        asprintf(&code, "%s\n%s", code, genCode(ast->right)); // handle right single block
    }
    // one line or one function
    else
        asprintf(&code, "%s\n%s", code, genCode(ast->left));

    return code;
}
/** 
 * Nodes handling
 */
char *genCode(Node *codeNode)
{
    printf("$$: genCode\n");
    char *code = "";

    if (!codeNode)
        return code;

    if (strcmp(codeNode->data, "CODE") == 0)
    {
        asprintf(&code, "%s\n%s", code, genCode(codeNode->left));  // handle left multi block
        asprintf(&code, "%s\n%s", code, genCode(codeNode->right)); // handle right single block
    }

    else if (strcmp(codeNode->data, "FUNCTION") == 0)
        return genFunc(codeNode);

    else
    {
        printf("ERROR!\n");
        printInOrder(codeNode, 0);
        exit(1);
    }

    return code;
}

char *genFunc(Node *funcNode)
{
    printf("$$: genFunc\n");
    int size = 0;
    char *code = "";

    // for function inside a function
    // save global counters in temps
    int currVar = varCount;
    int currLableCounter = lableCount;
    // reset counters
    varCount = 0;
    lableCount = 2; // lables start from line 2

    code = genBlock(funcNode->right, &size);

    varCount = currVar;
    lableCount = currLableCounter;

    asprintf(&code, "%s:\n\tBeginFunc %d\n\t%sEndFunc\n\t", funcNode->left->middle->left->data, size, code);
    return code;
}

char *genBlock(Node *blockNode, int *size)
{
    printf("$$: genBlock\n");
    char *code = "";
    if (strcmp(blockNode->left->data, "CODE") == 0)

        return code;
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
#define _GNU_SOURCE // for asprintf, or gives a warning
#include "./tac.h"
#include "./ast.h"

int varCount, lableID;
bool lableFlag;

char *intToString(int x)
{
    char *str = "";
    asprintf(&str, "%d", x);
    return str;
}

char *freshVar()
{
    char *varName;
    // auto allocation
    asprintf(&varName, "t%d", varCount++);
    return varName;
}

bool isLogicLoop(char *str)
{
    if (strcmp(str, "IF") == 0)
        return true;
    if (strcmp(str, "IF/ELSE") == 0)
        return true;

    if (strcmp(str, "WHILE LOOP") == 0)
        return true;

    if (strcmp(str, "DO WHILE") == 0)
        return true;

    if (strcmp(str, "FOR LOOP") == 0)
        return true;

    return false;
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
    if (strcmp(str, "CHAR") == 0)
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
    if (strcmp(str, "||") == 0)
        return true;
    if (strcmp(str, "&&") == 0)
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

    lableFlag = false;
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

    if (strcmp(stmtNode->data, "BLOCK") == 0)
        return genBlock(stmtNode, size);

    if (!stmtNode)
        return code;

    if (strcmp(stmtNode->data, "FUNCTION") == 0)
        return genFunc(stmtNode);

    if (strcmp(stmtNode->data, "RETURN") == 0)
        return genReturn(stmtNode, size);

    if (strcmp(stmtNode->data, "RETURN VOID") == 0)
        return genReturn(stmtNode, size);

    if (strcmp(stmtNode->data, "=") == 0 || strcmp(stmtNode->data, "^=") == 0)
        return genAss(stmtNode, size);

    if (strcmp(stmtNode->data, "VARIABLES DECLERATION") == 0)
        return "";

    if (strcmp(stmtNode->data, "FUNCTION CALL NO PARAMS") == 0)
    {
        *size += getSize(stmtNode);
        asprintf(&(code), "%s%s", code, genTac("ACall", " ", getId(stmtNode->left)));
        return code;
    }

    if (strcmp(stmtNode->data, "FUNCTION CALL") == 0)
    {
        *size += getSize(stmtNode->right);
        int currSize = *size;
        Tac *params = pushParams(stmtNode->right, size);
        char *popSize = "";
        asprintf(&popSize, "%d", *size - currSize);
        char *paramsCall = genTac("", "ACall ", getId(stmtNode->left));
        char *paramsPop = genTac("Pop", " ", popSize);
        asprintf(&(code), "%s%s%s%s", params->code, code, paramsCall, paramsPop);
        return code;
    }

    else if (isLogicLoop(stmtNode->data))
        return genLogicLoop(stmtNode, size);

    else
        asprintf(&(code), "%s%s", code, "unhandled statement"); // handle right single block
    return code;
}

char *genReturn(Node *returnNode, int *size)
{
    if (strcmp(returnNode->data, "RETURN VOID") == 0)
        return genTac("Return", "", "");

    char *code = "";
    Tac *E = genExrp(returnNode->left, size);
    char *rTac = genTac("Return", " ", E->var);
    asprintf(&code, "%s%s", E->code, rTac);
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
    lableID = 1; // lables start from line 2
    code = genBlock(funcNode->right, &size);

    char *begin = "";
    asprintf(&begin, "\tBeginFunc %d\n", size);
    char *end = genTac("EndFunc ", "", "");
    varCount = currVarCount;
    lableID = currLableID;

    asprintf(&code, "%s:\n%s%s%s\n", funcNode->left->middle->left->data, begin, code, end);
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

Tac *genStringIndex(Node *stringNode, int *size)
{
    printInOrder(stringNode, 0);
    Tac *E = calloc(1, sizeof(Tac));
    E->var = "";
    E->code = "";

    Tac *indexTac = genExrp(stringNode->right, size);

    Tac *offsetSizeTac = calloc(1, sizeof(Tac));
    offsetSizeTac->var = freshVar();
    offsetSizeTac->code = genTac(offsetSizeTac->var, "=", intToString(getSize(stringNode->right)));

    Tac *offsetTac = calloc(1, sizeof(Tac));
    offsetTac->var = freshVar();
    char *mulString = "";
    asprintf(&mulString, "%s * %s", indexTac->var, offsetSizeTac->var);
    offsetTac->code = genTac(offsetTac->var, "=", mulString);

    Tac *offsetAddTac = calloc(1, sizeof(Tac));
    offsetAddTac->var = freshVar();
    char *addString = "";
    asprintf(&addString, "%s + %s", offsetTac->var, getId(stringNode->left));
    offsetAddTac->code = genTac(offsetAddTac->var, "=", addString);

    E->var = offsetAddTac->var;
    asprintf(&(E->code), "%s%s%s%s", indexTac->code, offsetSizeTac->code, offsetTac->code, offsetAddTac->code);
    return E;
}

Tac *genLhs(Node *lhsNode, int *size)
{
    Tac *lhs = calloc(1, sizeof(Tac));
    lhs->var = "";
    lhs->code = "";
    if (strcmp(lhsNode->left->data, "IDENT") == 0)
        lhs->var = lhsNode->left->left->data;
    else
        // string access
        return genStringIndex(lhsNode->left, size);

    return lhs;
}

char *genAss(Node *assNode, int *size)
{
    printf("$$: genAss\n");
    char *code = "";
    Tac *left = genLhs(assNode->left, size);
    Tac *E = genExrp(assNode->right, size);
    *size += getSize(assNode->right); // test

    if (strcmp(assNode->data, "^=") == 0 || (assNode->left->left && strcmp(assNode->left->left->data, "ARRAY ACCESS") == 0))
        asprintf(&(left->var), "*(%s)", left->var);
    asprintf(&code, "%s%s%s", left->code, E->code, genTac(left->var, "=", E->var));

    return code;
}

char *genTac(char *idvar, char *str, char *evar)
{
    printf("$$: genTac\n");
    char *code = "";
    if (lableFlag)
        asprintf(&code, "L%d\t%s%s%s\n", lableID, idvar, str, evar);
    else
        asprintf(&code, "\t%s%s%s\n", idvar, str, evar);

    lableFlag = false;
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
Tac *genFunctionCall(Node *funcNode, int *size)
{
    printf("$$: genFunctionCall\n");
    Tac *E = calloc(1, sizeof(Tac));
    E->code = "";
    E->var = "";
    if (strcmp(funcNode->data, "FUNCTION CALL") == 0)
    {
        printf("$$: PARAMS\n");
        E->var = freshVar();
        *size += getSize(funcNode);
        int currSize = *size;
        Tac *params = pushParams(funcNode->right, size);
        char *popSize = "";
        asprintf(&popSize, "%d", *size - currSize);
        char *paramsCall = genTac(E->var, "=LCall ", getId(funcNode));
        char *paramsPop = genTac("Pop", " ", popSize);
        asprintf(&(E->code), "%s%s%s%s", params->code, E->code, paramsCall, paramsPop);
    }
    else
    {
        printf("$$: NO PARAMS\n");
        E->var = freshVar();
        *size += getSize(funcNode);
        asprintf(&(E->code), "%s%s", E->code, genTac(E->var, "= LCall ", getId(funcNode)));
    }
    return E;
}

Tac *genAddress(Node *addressNode, int *size)
{
    Tac *E = calloc(1, sizeof(Tac));
    E->code = "";
    E->var = "";

    E->var = freshVar();
    *size += getSize(addressNode);
    char *right = getId(addressNode->left);
    asprintf(&right, "*(%s)", right);
    asprintf(&(E->code), "%s%s", E->code, genTac(E->var, "=", right));
    return E;
}

Tac *genContent(Node *contNode, int *size)
{
    printf("$$: genContent\n");
    Tac *E = calloc(1, sizeof(Tac));
    E->code = "";
    E->var = "";

    E->var = freshVar();
    *size += getSize(contNode);
    asprintf(&(E->var), "*(%s)", E->var);
    asprintf(&(E->code), "%s%s", E->code, genTac(E->var, "=", getId(contNode->left)));
    return E;
}

char *genLogicLoop(Node *logicNode, int *size)
{
    printf("$$: genLogicLoop\n");
    char *code = "";
    if (strcmp(logicNode->data, "IF") == 0)
    {
        Tac *ifTrue = calloc(1, sizeof(Tac));
        Tac *logicExpr = genExrp(logicNode->left, size);
        char *trueCode = genStatment(logicNode->right, size);
        asprintf(&code, "%s", logicExpr->code);
        asprintf(&code, "%s\tifZ %s Goto L%s\n%s", code, logicExpr->var, intToString(lableID), trueCode);
        lableFlag = true;
    }
    else if (strcmp(logicNode->data, "IF/ELSE") == 0)
    {
        Tac *ifTrue = calloc(1, sizeof(Tac));
        Tac *ifFalse = calloc(1, sizeof(Tac));
        Tac *logicExpr = genExrp(logicNode->left, size);
        char *trueCode = genStatment(logicNode->middle, size);
        int falseLable = lableID;
        lableFlag = true;
        char *falseCode = genStatment(logicNode->right, size);

        asprintf(&code, "%s", logicExpr->code);
        asprintf(&code, "%s\tifZ %s Goto L%s\n%s\tGoto L%s\n%s", code, logicExpr->var, intToString(falseLable), trueCode, intToString(lableID), falseCode);
        lableFlag = true;
    }

    else if (strcmp(logicNode->data, "WHILE LOOP") == 0)
    {
        Tac *ifTrue = calloc(1, sizeof(Tac));
        Tac *ifFalse = calloc(1, sizeof(Tac));

        int logicLable = lableID;
        lableFlag = true;
        Tac *logicExpr = genExrp(logicNode->left, size);
        char *trueCode = genStatment(logicNode->right, size);
        int falseLable = lableID;

        asprintf(&code, "%s", logicExpr->code);
        asprintf(&code, "%s\tifZ %s Goto L%s\n%s\tGoto L%s\n", code, logicExpr->var, intToString(falseLable), trueCode, intToString(logicLable));
        lableFlag = true;
    }

    else if (strcmp(logicNode->data, "DO WHILE") == 0)
    {
        Tac *ifTrue = calloc(1, sizeof(Tac));
        Tac *ifFalse = calloc(1, sizeof(Tac));

        int doLable = lableID;
        lableFlag = true;
        char *trueCode = genStatment(logicNode->left, size);
        Tac *logicExpr = genExrp(logicNode->right, size);
        int afterdoLable = lableID;
        lableFlag = true;
        asprintf(&code, "%s", trueCode);
        asprintf(&code, "%s%s\tifZ %s Goto L%d\n\tGoto L%d\n", code, logicExpr->code, logicExpr->var, afterdoLable, doLable);
    }

    else if (strcmp(logicNode->data, "FOR LOOP") == 0)
    {

        Node *logicData = logicNode->left;
        char *forInit = genAss(logicData->left, size);
        lableFlag = true;
        int beforeLable = lableID;
        Tac *forLogic = genExrp(logicData->middle, size);
        char *forExpr = genAss(logicData->right, size);
        char *trueCode = genStatment(logicNode->right, size);
        int afterLable = lableID;
        lableFlag = true;

        asprintf(&code, "%s", forInit);
        asprintf(&code, "%s%s", code, forLogic->code);
        asprintf(&code, "%s\tifZ %s Goto L%d\n", code, forLogic->var, afterLable);
        asprintf(&code, "%s%s", code, forExpr);
        asprintf(&code, "%s%s\tGoto L%d\n", code, trueCode, beforeLable);
    }

    return code;
}

Tac *genExrp(Node *exprNode, int *size)
{
    printf("$$: genExrp\n");

    Tac *E = calloc(1, sizeof(Tac));
    E->code = "";
    E->var = "";

    if (!exprNode)
        return E;

    if (strcmp(exprNode->left->data, "EXPR") == 0)
        return genExrp(exprNode->left, size);

    if (isFuncCall(exprNode->left->data))
        return genFunctionCall(exprNode->left, size);

    else if (isPtrNode(exprNode->left->data))
    {
        if (strcmp(exprNode->left->data, "&") == 0)
            genAddress(exprNode->left, size);

        else
            genContent(exprNode->left, size);
    }

    else if (isSimpleNode(exprNode->left->data))
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
        Tac *E1 = genExrp(exprNode->left->left, size);
        Tac *E2 = genExrp(exprNode->left->right, size);
        E->var = freshVar();
        char *gen = "";
        asprintf(&gen, "%s %s %s", E1->var, exprNode->left->data, E2->var);
        asprintf(&(E->code), "%s%s%s", E1->code, E2->code, genTac(E->var, "=", gen));
    }

    else if (strcmp("ARRAY ACCESS", exprNode->left->data) == 0)
        return genStringIndex(exprNode->left, size);

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
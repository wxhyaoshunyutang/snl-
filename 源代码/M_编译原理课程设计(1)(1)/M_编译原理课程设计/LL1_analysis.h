#include<iostream>
#include<vector>
#include<set>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<sstream>
#include<algorithm>
using namespace std;



vector<Token> token;
vector<string>ll1_file;

typedef enum {
	PROGRAM, ID, TYPE, INTEGER, CHAR, ARRAY, OF, RECORD, END, VAR, PROCEDURE, BEGIN, IF, THEN, ELSE, FI, WHILE,
	DO, ENDWH, READ, WRITE, RETURN, INTC, SEMI, COMMA, LPAREN, RPAREN, ASSIG, LMIDPAREN, RMIDPAREN, DOT, LT,
	PLUS, SUB, MULT, DIV, EQ, UNDERANGE
}terminals;  //终极符

string terminals_string[38] = { "PROGRAM", "ID", "TYPE", "INTEGER", "CHAR", "ARRAY", "OF", "RECORD", "END", "VAR",
	"PROCEDURE", "BEGIN", "IF", "THEN", "ELSE", "FI", "WHILE","DO", "ENDWH", "READ", "WRITE", "RETURN", "INTC", "SEMI", "COMMA",
	"LPAREN","RPAREN", "ASSIG", "LMIDPAREN", "RMIDPAREN", "DOT", "LT","PLUS", "SUB", "MULT", "DIV", "EQ", "UNDERANGE" };

typedef enum {
	Program, ProgramHead, ProgramName, DeclarePart,
	TypeDec, TypeDecpart, TypeDecList, TypeDecMore,
	TypeId, TypeDef, BaseType, StructureType,
	ArrayType, Low, Top, RecType,
	FieldDecList, FieldDecMore, IdList, IdMore,
	VarDecpart, VarDec, VarDecList, VarDecMore,
	VarIdList, VarIdMore, ProcDecpart, ProcDec,
	ProcDecMore, ProcName, ParamList, ParamDecList,
	ParamMore, Param, FormList, FidMore,
	ProcDecPart, ProcBody, ProgramBody, StmList,
	StmMore, Stm, AssCall, AssignmentRest,
	ConditionalStm, StmL, LoopStm, InputStm,
	Invar, OutputStm, ReturnStm, CallStmRest,
	ActParamList, ActParamMore, RelExp, OtherRelE,
	Exp, OtherTerm, Term, OtherFactor,
	Factor, Variable, VariMore, FieldVar,
	FieldVarMore, CmpOp, AddOp, MultOp
}nterminals; //非终极符

string nterminals_string[68] = {
	"Program", "ProgramHead", "ProgramName", "DeclarePart",
	"TypeDec", "TypeDecpart", "TypeDecList", "TypeDecMore",
	"TypeId", "TypeDef", "BaseType", "StructureType",
	"ArrayType", "Low", "Top", "RecType",
	"FieldDecList", "FieldDecMore", "IdList", "IdMore",
	"VarDecpart", "VarDec", "VarDecList", "VarDecMore",
	"VarIdList", "VarIdMore", "ProcDecpart", "ProcDec",
	"ProcDecMore", "ProcName", "ParamList", "ParamDecList",
	"ParamMore", "Param", "FormList", "FidMore",
	"ProcDecPart", "ProcBody", "ProgramBody", "StmList",
	"StmMore", "Stm", "AssCall", "AssignmentRest",
	"ConditionalStm", "StmL", "LoopStm", "InputStm",
	"Invar", "OutputStm", "ReturnStm", "CallStmRest",
	"ActParamList", "ActParamMore", "RelExp", "OtherRelE",
	"Exp", "OtherTerm", "Term", "OtherFactor",
	"Factor", "Variable", "VariMore", "FieldVar",
	"FieldVarMore", "CmpOp", "AddOp", "MultOp"
};

int LL1Table[68][38];

void initLL1Table() {
	for (int i = 0;i < 68;i++) {
		for (int j = 0;j < 38;j++) {
			LL1Table[i][j] = 0;
		}
	}
	LL1Table[Program][PROGRAM] = 1;

	LL1Table[ProgramHead][PROGRAM] = 2;

	LL1Table[ProgramName][ID] = 3;

	LL1Table[DeclarePart][TYPE] = 4;
	LL1Table[DeclarePart][VAR] = 4;
	LL1Table[DeclarePart][PROCEDURE] = 4;
	LL1Table[DeclarePart][BEGIN] = 4;

	LL1Table[TypeDecpart][VAR] = 5;
	LL1Table[TypeDecpart][PROCEDURE] = 5;
	LL1Table[TypeDecpart][BEGIN] = 5;

	LL1Table[TypeDecpart][TYPE] = 6;

	LL1Table[TypeDec][TYPE] = 7;

	LL1Table[TypeDecList][ID] = 8;

	LL1Table[TypeDecMore][VAR] = 9;
	LL1Table[TypeDecMore][PROCEDURE] = 9;
	LL1Table[TypeDecMore][BEGIN] = 9;

	LL1Table[TypeDecMore][ID] = 10;

	LL1Table[TypeId][ID] = 11;

	LL1Table[TypeDef][INTEGER] = 12;
	LL1Table[TypeDef][CHAR] = 12;

	LL1Table[TypeDef][ARRAY] = 13;
	LL1Table[TypeDef][RECORD] = 13;

	LL1Table[TypeDef][ID] = 14;

	LL1Table[BaseType][INTEGER] = 15;

	LL1Table[BaseType][CHAR] = 16;

	LL1Table[StructureType][ARRAY] = 17;

	LL1Table[StructureType][RECORD] = 18;

	LL1Table[ArrayType][ARRAY] = 19;

	LL1Table[Low][INTC] = 20;

	LL1Table[Top][INTC] = 21;

	LL1Table[RecType][RECORD] = 22;

	LL1Table[FieldDecList][INTEGER] = 23;
	LL1Table[FieldDecList][CHAR] = 23;

	LL1Table[FieldDecList][ARRAY] = 24;

	LL1Table[FieldDecMore][END] = 25;

	LL1Table[FieldDecMore][INTEGER] = 26;
	LL1Table[FieldDecMore][CHAR] = 26;
	LL1Table[FieldDecMore][ARRAY] = 26;

	LL1Table[IdList][ID] = 27;

	LL1Table[IdMore][SEMI] = 28;

	LL1Table[IdMore][COMMA] = 29;

	LL1Table[VarDecpart][PROCEDURE] = 30;
	LL1Table[VarDecpart][BEGIN] = 30;

	LL1Table[VarDecpart][VAR] = 31;

	LL1Table[VarDec][VAR] = 32;

	LL1Table[VarDecList][INTEGER] = 33;
	LL1Table[VarDecList][CHAR] = 33;
	LL1Table[VarDecList][ARRAY] = 33;
	LL1Table[VarDecList][RECORD] = 33;
	LL1Table[VarDecList][ID] = 33;

	LL1Table[VarDecMore][PROCEDURE] = 34;
	LL1Table[VarDecMore][BEGIN] = 34;

	LL1Table[VarDecMore][INTEGER] = 35;
	LL1Table[VarDecMore][CHAR] = 35;
	LL1Table[VarDecMore][ARRAY] = 35;
	LL1Table[VarDecMore][RECORD] = 35;
	LL1Table[VarDecMore][ID] = 35;

	LL1Table[VarIdList][ID] = 36;

	LL1Table[VarIdMore][SEMI] = 37;

	LL1Table[VarIdMore][COMMA] = 38;

	LL1Table[ProcDecpart][BEGIN] = 39;

	LL1Table[ProcDecpart][PROCEDURE] = 40;

	LL1Table[ProcDec][PROCEDURE] = 41;

	LL1Table[ProcName][ID] = 44;

	LL1Table[ParamList][RPAREN] = 45;

	LL1Table[ParamList][INTEGER] = 46;
	LL1Table[ParamList][CHAR] = 46;
	LL1Table[ParamList][ARRAY] = 46;
	LL1Table[ParamList][RECORD] = 46;
	LL1Table[ParamList][ID] = 46;
	LL1Table[ParamList][VAR] = 46;

	LL1Table[ParamDecList][INTEGER] = 47;
	LL1Table[ParamDecList][CHAR] = 47;
	LL1Table[ParamDecList][ARRAY] = 47;
	LL1Table[ParamDecList][RECORD] = 47;
	LL1Table[ParamDecList][ID] = 47;
	LL1Table[ParamDecList][VAR] = 47;

	LL1Table[ParamMore][RPAREN] = 48;

	LL1Table[ParamMore][SEMI] = 49;

	LL1Table[Param][INTEGER] = 50;
	LL1Table[Param][CHAR] = 50;
	LL1Table[Param][ARRAY] = 50;
	LL1Table[Param][RECORD] = 50;
	LL1Table[Param][ID] = 50;

	LL1Table[Param][VAR] = 51;

	LL1Table[FormList][ID] = 52;

	LL1Table[FidMore][SEMI] = 53;
	LL1Table[FidMore][RPAREN] = 53;

	LL1Table[FidMore][COMMA] = 54;

	LL1Table[ProcDecPart][TYPE] = 55;
	LL1Table[ProcDecPart][VAR] = 55;
	LL1Table[ProcDecPart][PROCEDURE] = 55;
	LL1Table[ProcDecPart][BEGIN] = 55;

	LL1Table[ProcBody][BEGIN] = 56;

	LL1Table[ProgramBody][BEGIN] = 57;

	LL1Table[StmList][ID] = 58;
	LL1Table[StmList][IF] = 58;
	LL1Table[StmList][WHILE] = 58;
	LL1Table[StmList][RETURN] = 58;
	LL1Table[StmList][READ] = 58;
	LL1Table[StmList][WRITE] = 58;

	LL1Table[StmMore][ELSE] = 59;
	LL1Table[StmMore][FI] = 59;
	LL1Table[StmMore][END] = 59;
	LL1Table[StmMore][ENDWH] = 59;

	LL1Table[StmMore][SEMI] = 60;

	LL1Table[Stm][IF] = 61;

	LL1Table[Stm][WHILE] = 62;

	LL1Table[Stm][READ] = 63;

	LL1Table[Stm][WRITE] = 64;

	LL1Table[Stm][RETURN] = 65;

	LL1Table[Stm][ID] = 66;

	LL1Table[AssCall][ASSIG] = 67;
	LL1Table[AssCall][LMIDPAREN] = 67;
	LL1Table[AssCall][DOT] = 67;

	LL1Table[AssCall][LPAREN] = 68;

	LL1Table[AssignmentRest][LMIDPAREN] = 69;
	LL1Table[AssignmentRest][DOT] = 69;
	LL1Table[AssignmentRest][ASSIG] = 69;

	LL1Table[ConditionalStm][IF] = 70;

	LL1Table[LoopStm][WHILE] = 71;

	LL1Table[InputStm][READ] = 72;

	LL1Table[Invar][ID] = 73;

	LL1Table[OutputStm][WRITE] = 74;

	LL1Table[ReturnStm][RETURN] = 75;

	LL1Table[CallStmRest][LPAREN] = 76;

	LL1Table[ActParamList][RPAREN] = 77;

	LL1Table[ActParamList][LPAREN] = 78;
	LL1Table[ActParamList][INTC] = 78;
	LL1Table[ActParamList][ID] = 78;

	LL1Table[ActParamMore][RPAREN] = 79;

	LL1Table[ActParamMore][COMMA] = 80;

	LL1Table[RelExp][LPAREN] = 81;
	LL1Table[RelExp][INTC] = 81;
	LL1Table[RelExp][ID] = 81;

	LL1Table[OtherRelE][LT] = 82;
	LL1Table[OtherRelE][EQ] = 82;

	LL1Table[Exp][LPAREN] = 83;
	LL1Table[Exp][INTC] = 83;
	LL1Table[Exp][ID] = 83;

	LL1Table[OtherTerm][LT] = 84;
	LL1Table[OtherTerm][EQ] = 84;
	LL1Table[OtherTerm][RMIDPAREN] = 84;
	LL1Table[OtherTerm][THEN] = 84;
	LL1Table[OtherTerm][ELSE] = 84;
	LL1Table[OtherTerm][FI] = 84;
	LL1Table[OtherTerm][DO] = 84;
	LL1Table[OtherTerm][ENDWH] = 84;
	LL1Table[OtherTerm][RPAREN] = 84;
	LL1Table[OtherTerm][END] = 84;
	LL1Table[OtherTerm][SEMI] = 84;
	LL1Table[OtherTerm][COMMA] = 84;

	LL1Table[OtherTerm][PLUS] = 85;
	LL1Table[OtherTerm][SUB] = 85;

	LL1Table[Term][LPAREN] = 86;
	LL1Table[Term][INTC] = 86;
	LL1Table[Term][ID] = 86;

	LL1Table[OtherFactor][PLUS] = 87;
	LL1Table[OtherFactor][SUB] = 87;
	LL1Table[OtherFactor][LT] = 87;
	LL1Table[OtherFactor][EQ] = 87;
	LL1Table[OtherFactor][RMIDPAREN] = 87;
	LL1Table[OtherFactor][THEN] = 87;
	LL1Table[OtherFactor][ELSE] = 87;
	LL1Table[OtherFactor][FI] = 87;
	LL1Table[OtherFactor][DO] = 87;
	LL1Table[OtherFactor][ENDWH] = 87;
	LL1Table[OtherFactor][RPAREN] = 87;
	LL1Table[OtherFactor][END] = 87;
	LL1Table[OtherFactor][SEMI] = 87;
	LL1Table[OtherFactor][COMMA] = 87;

	LL1Table[OtherFactor][MULT] = 88;
	LL1Table[OtherFactor][DIV] = 88;

	LL1Table[Factor][LPAREN] = 89;

	LL1Table[Factor][INTC] = 90;

	LL1Table[Factor][ID] = 91;

	LL1Table[Variable][ID] = 92;

	LL1Table[VariMore][ASSIG] = 93;
	LL1Table[VariMore][MULT] = 93;
	LL1Table[VariMore][DIV] = 93;
	LL1Table[VariMore][PLUS] = 93;
	LL1Table[VariMore][SUB] = 93;
	LL1Table[VariMore][LT] = 93;
	LL1Table[VariMore][EQ] = 93;
	LL1Table[VariMore][THEN] = 93;
	LL1Table[VariMore][ELSE] = 93;
	LL1Table[VariMore][FI] = 93;
	LL1Table[VariMore][DO] = 93;
	LL1Table[VariMore][ENDWH] = 93;
	LL1Table[VariMore][RPAREN] = 93;
	LL1Table[VariMore][END] = 93;
	LL1Table[VariMore][SEMI] = 93;
	LL1Table[VariMore][COMMA] = 93;
	LL1Table[VariMore][RMIDPAREN] = 93;

	LL1Table[VariMore][LMIDPAREN] = 94;

	LL1Table[VariMore][DOT] = 95;

	LL1Table[FieldVar][ID] = 96;

	LL1Table[FieldVarMore][ASSIG] = 97;
	LL1Table[FieldVarMore][MULT] = 97;
	LL1Table[FieldVarMore][DIV] = 97;
	LL1Table[FieldVarMore][PLUS] = 97;
	LL1Table[FieldVarMore][SUB] = 97;
	LL1Table[FieldVarMore][LT] = 97;
	LL1Table[FieldVarMore][EQ] = 97;
	LL1Table[FieldVarMore][THEN] = 97;
	LL1Table[FieldVarMore][ELSE] = 97;
	LL1Table[FieldVarMore][FI] = 97;
	LL1Table[FieldVarMore][DO] = 97;
	LL1Table[FieldVarMore][ENDWH] = 97;
	LL1Table[FieldVarMore][RPAREN] = 97;
	LL1Table[FieldVarMore][END] = 97;
	LL1Table[FieldVarMore][SEMI] = 97;
	LL1Table[FieldVarMore][COMMA] = 97;

	LL1Table[FieldVarMore][LMIDPAREN] = 98;

	LL1Table[CmpOp][LT] = 99;

	LL1Table[CmpOp][EQ] = 100;

	LL1Table[AddOp][PLUS] = 101;

	LL1Table[AddOp][SUB] = 102;

	LL1Table[MultOp][MULT] = 103;

	LL1Table[MultOp][DIV] = 104;
}




struct StackNode {
	int ntflag;  //判断终极符或非终极符,0-非终极，1-终极
	nterminals n;
	terminals t;
	StackNode* next;
	TreeNode* treenode;
};

class AnalysisStack {
public:
	StackNode* top;
	AnalysisStack() {
		top = NULL;
	}
	void push(int flag, int nt) {
		StackNode* p = new StackNode;
		p->ntflag = flag;
		p->next = top;
		if (flag == 0) {
			p->n = (nterminals)nt;
			cout <<"非终极符:"<< nterminals_string[nt] << "压栈" << endl;
			ll1_file.push_back("非终极符:"+ nterminals_string[nt]+"压栈"+"\n");
		}
		else {
			p->t = (terminals)nt;
			cout <<"终极符:"<< terminals_string[nt] << "压栈" << endl;
			ll1_file.push_back("终极符:" + terminals_string[nt] + "压栈"+"\n");
		}
		top = p;
	}
	void pop() {
		top = top->next;
	}
};

AnalysisStack* ana = new AnalysisStack();

void processChild(int flag, int nt, string name, TreeNode* tree) {
	ana->push(flag, nt);
	TreeNode* t = new TreeNode(name);
	tree->add_node2(t);
	ana->top->treenode = t;
}

void process(int num) {
	TreeNode* curTree = ana->top->treenode;
	//cout << "树根名字：" <<ana->top->treenode->name<< endl;
	ana->pop();
	switch (num)
	{
	case(1):
		processChild(1, DOT, "DOT", curTree);
		processChild(0, ProgramBody, "ProgramBody", curTree);
		processChild(0, DeclarePart, "DeclarePart", curTree);
		processChild(0, ProgramHead, "ProgramHead", curTree);
		break;
	case(2):
		processChild(0, ProgramName, "ProgramName", curTree);
		processChild(1, PROGRAM, "PROGRAM", curTree);
		break;
	case(3):
		processChild(1, ID, "ID", curTree);
		break;
	case(4):
		processChild(0, ProcDecpart, "ProcDecpart", curTree);
		processChild(0, VarDecpart, "VarDecPart", curTree);
		processChild(0, TypeDecpart, "TypeDecPart", curTree);
		break;
	case(5):break;
	case(6):
		processChild(0, TypeDec, "TypeDec", curTree);
		break;
	case(7):
		processChild(0, TypeDecList, "TypeDecList", curTree);
		processChild(1, TYPE, "TYPE", curTree);
		break;
	case(8):
		processChild(0, TypeDecMore, "TypeDecMore", curTree);
		processChild(1, SEMI, "SEMI", curTree);
		processChild(0, TypeDef, "TypeDef", curTree);
		processChild(1, EQ, "EQ", curTree);
		processChild(0, TypeId, "TypeId", curTree);
		break;
	case(9):break;
	case(10):
		processChild(0, TypeDecList, "TypeDecList", curTree);
		break;
	case(11):
		processChild(1, ID, "ID", curTree);
		break;
	case(12):
		processChild(0, BaseType, "BaseType", curTree);
		break;
	case(13):
		processChild(0, StructureType, "StructureType", curTree);
		break;
	case(14):
		processChild(1, ID, "ID", curTree);
		break;
	case(15):
		processChild(1, INTEGER, "INTEGER", curTree);
		break;
	case(16):
		processChild(1, CHAR, "CHAR", curTree);
		break;
	case(17):
		processChild(0, ArrayType, "ArrayType", curTree);
		break;
	case(18):
		processChild(0, RecType, "RecType", curTree);
		break;
	case(19):
		processChild(0, BaseType, "BaseType", curTree);
		processChild(1, OF, "OF", curTree);
		processChild(1, RMIDPAREN, "RMIDPAREN", curTree);
		processChild(0, Top, "Top", curTree);
		processChild(1, UNDERANGE, "UNDERANGE", curTree);
		processChild(0, Low, "Low", curTree);
		processChild(1, LMIDPAREN, "LMIDPAREN", curTree);
		processChild(1, ARRAY, "ARRAY", curTree);
		break;
	case(20):
		processChild(1, INTC, "INTC", curTree);
		break;
	case(21):
		processChild(1, INTC, "INTC", curTree);
		break;
	case(22):
		processChild(1, END, "END", curTree);
		processChild(0, FieldDecList, "FieldDecList", curTree);
		processChild(1, RECORD, "RECORD", curTree);
		break;
	case(23):
		processChild(0, FieldDecMore, "FieldDecMore", curTree);
		processChild(1, SEMI, "SEMI", curTree);
		processChild(0, IdList, "IDList", curTree);
		processChild(0, BaseType, "BaseType", curTree);
		break;
	case(24):
		processChild(0, FieldDecMore, "FieldDecMore", curTree);
		processChild(1, SEMI, "SEMI", curTree);
		processChild(0, IdList, "IDList", curTree);
		processChild(0, ArrayType, "ArrayType", curTree);		
		break;
	case(25):break;
	case(26):
		processChild(0, FieldDecList, "FieldDecList", curTree);
		break;
	case(27):
		processChild(0, IdMore, "IdMore", curTree);
		processChild(1, ID, "ID", curTree);
		break;
	case(28):break;
	case(29):
		processChild(0, IdList, "IDList", curTree);
		processChild(1, COMMA, "COMMA", curTree);
		break;
	case(30):break;
	case(31):
		processChild(0, VarDec, "VarDec", curTree);
		break;
	case(32):
		processChild(0, VarDecList, "VarDecList", curTree);
		processChild(1, VAR, "VAR", curTree);
		break;
	case(33):
		processChild(0, VarDecMore, "VarDecMore", curTree);
		processChild(1, SEMI, "SEMI", curTree);
		processChild(0, VarIdList, "VarIDList", curTree);
		processChild(0, TypeDef, "TypeDef", curTree);
		break;
	case(34):break;
	case(35):
		processChild(0, VarDecList, "VarDecList", curTree);
		break;
	case(36):
		processChild(0, VarIdMore, "VarIDMore", curTree);
		processChild(1, ID, "ID", curTree);
		break;
	case(37):break;
	case(38):
		processChild(0, VarIdList, "VarIDList", curTree);
		processChild(1, COMMA, "COMMA", curTree);
		break;
	case(39):break;
	case(40):
		processChild(0, ProcDec, "ProcDec", curTree);
		break;
	case(41):
		processChild(0, ProcDecpart, "ProcDecpart", curTree);
		processChild(0, ProcBody, "ProcBody", curTree);
		processChild(0, ProcDecPart, "ProcDecPart", curTree);
		processChild(1, SEMI, "SEMI", curTree);
		processChild(1, RPAREN, "RPAREN", curTree);
		processChild(0, ParamList, "ParamList", curTree);
		processChild(1, LPAREN, "LPAREN", curTree);
		processChild(0, ProcName, "ProcName", curTree);
		processChild(1, PROCEDURE, "PROCEDURE", curTree);
		break;
	case(42):break;
	case(43):break;
	case(44):
		processChild(1, ID, "ID", curTree);
		break;
	case(45):break;
	case(46):
		processChild(0, ParamDecList, "ParamDecList", curTree);
		break;
	case(47):
		processChild(0, ParamMore, "ParamMore", curTree);
		processChild(0, Param, "Param", curTree);
	case(48):break;
	case(49):
		processChild(0, ParamDecList, "ParamDecList", curTree);
		processChild(1, SEMI, "SEMI", curTree);
		break;
	case(50):
		processChild(0, FormList, "FormList", curTree);
		processChild(0, TypeDef, "TypeDef", curTree);
		break;
	case(51):
		processChild(0, FormList, "FormList", curTree);
		processChild(0, TypeDef, "TypeDef", curTree);
		processChild(1, VAR, "VAR", curTree);
		break;
	case(52):
		processChild(0, FidMore, "FidMore", curTree);
		processChild(1, ID, "ID", curTree);
		break;
	case(53):break;
	case(54):
		processChild(0, FormList, "FormList", curTree);
		processChild(1, COMMA, "COMMA", curTree);
		break;
	case(55):
		processChild(0, DeclarePart, "DeclarePart", curTree);
		break;
	case(56):
		processChild(0, ProgramBody, "ProgramBody", curTree);
		break;
	case(57):
		processChild(1, END, "END", curTree);
		processChild(0, StmList, "StmList", curTree);
		processChild(1, BEGIN, "BEGIN", curTree);
		break;
	case(58):
		processChild(0, StmMore, "StmMore", curTree);
		processChild(0, Stm, "Stm", curTree);
		break;
	case(59):break;
	case(60):
		processChild(0, StmList, "StmList", curTree);
		processChild(1, SEMI, "SEMI", curTree);
		break;
	case(61):
		processChild(0, ConditionalStm, "ConditionalStm", curTree);
		break;
	case(62):
		processChild(0, LoopStm, "LoopStm", curTree);
		break;
	case(63):
		processChild(0, InputStm, "InputStm", curTree);
		break;
	case(64):
		processChild(0, OutputStm, "OutputStm", curTree);
		break;
	case(65):
		processChild(0, ReturnStm, "ReturnStm", curTree);
		break;
	case(66):
		processChild(0, AssCall, "AssCall", curTree);
		processChild(1, ID, "ID", curTree);
		break;
	case(67):
		processChild(0, AssignmentRest, "AssignmentRest", curTree);
		break;
	case(68):
		processChild(0, CallStmRest, "CallStmRest", curTree);
		break;
	case(69):
		processChild(0, Exp, "Exp", curTree);
		processChild(1, ASSIG, "ASSIG", curTree);
		processChild(0, VariMore, "VariMore", curTree);
		break;
	case(70):
		processChild(1, FI, "FI", curTree);
		processChild(0, StmList, "StmList", curTree);
		processChild(1, ELSE, "ELSE", curTree);
		processChild(0, StmList, "StmList", curTree);
		processChild(1, THEN, "THEN", curTree);
		processChild(0, RelExp, "RelExp", curTree);
		processChild(1, IF, "IF", curTree);
		break;
	case(71):
		processChild(1, ENDWH, "ENDWH", curTree);
		processChild(0, StmList, "StmList", curTree);
		processChild(1, DO, "DO", curTree);
		processChild(0, RelExp, "RelExp", curTree);
		processChild(1, WHILE, "WHILE", curTree);
		break;
	case(72):
		processChild(1, RPAREN, "RPAREN", curTree);
		processChild(0, Invar, "Invar", curTree);
		processChild(1, LPAREN, "LPAREN", curTree);
		processChild(1, READ, "READ", curTree);
		break;
	case(73):
		processChild(1, ID, "ID", curTree);
		break;
	case(74):
		processChild(1, RPAREN, "RPAREN", curTree);
		processChild(0, Exp, "Exp", curTree);
		processChild(1, LPAREN, "LPAREN", curTree);
		processChild(1, WRITE, "WRITE", curTree);
		break;
	case(75):
		processChild(1, RETURN, "RETURN", curTree);
		break;
	case(76):
		processChild(1, RPAREN, "RPAREN", curTree);
		processChild(0, ActParamList, "ActparamList", curTree);
		processChild(1, LPAREN, "LPAREN", curTree);
		break;
	case(77):break;
	case(78):
		processChild(0, ActParamMore, "ActParamMore", curTree);
		processChild(0, Exp, "Exp", curTree);
		break;
	case(79):break;
	case(80):
		processChild(0, ActParamList, "ActparamList", curTree);
		processChild(1, COMMA, "COMMA", curTree);
		break;
	case(81):
		processChild(0, OtherRelE, "OtherRelE", curTree);
		processChild(0, Exp, "Exp", curTree);
		break;
	case(82):
		processChild(0, Exp, "Exp", curTree);
		processChild(0, CmpOp, "CmpOp", curTree);
		break;
	case(83):
		processChild(0, OtherTerm, "OtherTerm", curTree);
		processChild(0, Term, "Term", curTree);
		break;
	case(84):break;
	case(85):
		processChild(0, Exp, "Exp", curTree);
		processChild(0, AddOp, "AddOp", curTree);
		break;
	case(86):
		processChild(0, OtherFactor, "OtherFactor", curTree);
		processChild(0, Factor, "Factor", curTree);
		break;
	case(87):break;
	case(88):
		processChild(0, Term, "Term", curTree);
		processChild(0, MultOp, "MultOp", curTree);
		break;
	case(89):
		processChild(1, RPAREN, "RPAREN", curTree);
		processChild(0, Exp, "Exp", curTree);
		processChild(1, LPAREN, "LPAREN", curTree);
		break;
	case(90):
		processChild(1, INTC, "INTC", curTree);
		break;
	case(91):
		processChild(0, Variable, "Variable", curTree);
		break;
	case(92):
		processChild(0, VariMore, "VariMore", curTree);
		processChild(1, ID, "ID", curTree);
		break;
	case(93):break;
	case(94):
		processChild(1, RMIDPAREN, "RMIDPAREN", curTree);
		processChild(0, Exp, "Exp", curTree);
		processChild(1, LMIDPAREN, "LMIDPAREN", curTree);
		break;
	case(95):
		processChild(0, FieldVar, "FieldVar", curTree);
		processChild(1, DOT, "DOT", curTree);
		break;
	case(96):
		processChild(0, FieldVarMore, "FieldVarMore", curTree);
		processChild(1, ID, "ID", curTree);
		break;
	case(97):break;
	case(98):
		processChild(1, RMIDPAREN, "RMIDPAREN", curTree);
		processChild(0, Exp, "Exp", curTree);
		processChild(1, LMIDPAREN, "LMIDPAREN", curTree);
		break;
	case(99):
		processChild(1, LT, "LT", curTree);
		break;
	case(100):
		processChild(1, EQ, "EQ", curTree);
		break;
	case(101):
		processChild(1, PLUS, "PLUS", curTree);
		break;
	case(102):
		processChild(1, SUB, "SUB", curTree);
		break;
	case(103):
		processChild(1, MULT, "MULT", curTree);
		break;
	case(104):
		processChild(1, DIV, "DIV", curTree);
		break;
	default:
		break;
	}
}

terminals string2enum(string s) {
	if (s == "PROGRAM") {
		return PROGRAM;
	}
	if (s == "ID") {
		return ID;
	}
	if (s == "TYPE") {
		return TYPE;
	}
	if (s == "INTEGER") {
		return INTEGER;
	}
	if (s == "CHAR") {
		return CHAR;
	}
	if (s == "ARRAY") {
		return ARRAY;
	}
	if (s == "OF") {
		return OF;
	}
	if (s == "RECORD") {
		return RECORD;
	}
	if (s == "END") {
		return END;
	}
	if (s == "VAR") {
		return VAR;
	}
	if (s == "PROCEDURE") {
		return PROCEDURE;
	}
	if (s == "BEGIN") {
		return BEGIN;
	}
	if (s == "IF") {
		return IF;
	}
	if (s == "THEN") {
		return THEN;
	}
	if (s == "ELSE") {
		return ELSE;
	}
	if (s == "FI") {
		return FI;
	}
	if (s == "WHILE") {
		return WHILE;
	}
	if (s == "DO") {
		return DO;
	}
	if (s == "ENDWH") {
		return ENDWH;
	}
	if (s == "READ") {
		return READ;
	}
	if (s == "WRITE") {
		return WRITE;
	}
	if (s == "RETURN") {
		return RETURN;
	}
	if (s == "INTC") {
		return INTC;
	}
	if (s == "SEMI") {
		return SEMI;
	}
	if (s == "COMMA") {
		return COMMA;
	}
	if (s == "LPAREN") {
		return LPAREN;
	}
	if (s == "RPAREN") {
		return RPAREN;
	}
	if (s == "ASSIG") {
		return ASSIG;
	}
	if (s == "LMIDPAREN") {
		return LMIDPAREN;
	}
	if (s == "RMIDPAREN") {
		return RMIDPAREN;
	}
	if (s == "DOT") {
		return DOT;
	}
	if (s == "LT") {
		return LT;
	}
	if (s == "PLUS") {
		return PLUS;
	}
	if (s == "SUB") {
		return SUB;
	}
	if (s == "MULT") {
		return MULT;
	}
	if (s == "DIV") {
		return DIV;
	}
	if (s == "EQ") {
		return EQ;
	}
	if (s == "UNDERANGE") {
		return UNDERANGE;
	}
}

TreeNode* LL1() {
	ana->push(0, Program);
	//cout << ana->top->n << endl;
	TreeNode* root = new TreeNode("Program");
	ana->top->treenode = root;
	int index = 0;
	initLL1Table();
	int count=0;
	//cout << "进入循环" << endl;
	while (ana->top != NULL) {
		Token temp_token = token[index];
		if (ana->top->ntflag == 0) {
			if (LL1Table[ana->top->n][string2enum(temp_token.Lex)] == 0) {
				cout <<"错误行数：" <<temp_token.Line<< "    错误的产生式的非终极符:" << nterminals_string[ana->top->n] << " 终极符：" << temp_token.Lex << endl;
				ll1_file.push_back("错误行数："+ to_string(temp_token.Line) + "    错误的产生式的非终极符:" + nterminals_string[ana->top->n] + "终极符：" + temp_token.Lex + "\n");
				return NULL;
			}
			process(LL1Table[ana->top->n][string2enum(temp_token.Lex)]);
		}
		else {
			if (ana->top->t == string2enum(temp_token.Lex)) {
				cout << "第" << temp_token.Line << "行的" << temp_token.Lex << "匹配成功,弹栈" << endl;
				ll1_file.push_back("第"+ to_string(temp_token.Line)+"行的"+ temp_token.Lex+ "匹配成功,弹栈"+"\n");
				ana->top->treenode->token = &token[index];
				ana->pop();
			}
			else {
				cout << "第" << temp_token.Line << "行的" << temp_token.Lex <<"和"<< terminals_string[ana->top->t] << "不匹配" << endl;
				ll1_file.push_back("第"+to_string(temp_token.Line)+"行的"+ temp_token.Lex + terminals_string[ana->top->t] +"不匹配"+"\n");
				return NULL;
			}
			index++;
		}
		count++;
	}
	if (token[index].Lex == "EOF") {
		cout <<"栈空，当前token为EOF，LL1分析完成，无语法错误" << endl;
		ll1_file.push_back("栈空，当前token为EOF，LL1分析完成，无语法错误\n");
	}
	else {
		cout << "token未完，当前token为："<<token[index].Lex << endl;
		ll1_file.push_back("token未完，当前token为："+ token[index].Lex+"\n");
		return NULL;
	}
	return root;
}








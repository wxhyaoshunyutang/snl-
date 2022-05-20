// Compilation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <syntax_analysis.h>
#include <morpheme_analysis.h>
using namespace std;
#define initoff 7;





class Semantic {
    int level = -1;            //scope栈的下标，表示当前层数,第一次进入局部化单位时会将index置为0
    int offset;           //当前层数的偏移量
    struct TypeIR;
    struct SymbolTable;
    


    enum snl_TypeKind { intTy, charTy, arraryTy, recordTy, boolTy };
    enum AccessKind { dir, indir };
    enum IdKind { typekind, varkind, prockind };
    enum Function_Class { actua, formal };
    struct intPtr { int size = 1; snl_TypeKind intTy; };//无实际用途
    struct charPtr { int size = 1; snl_TypeKind cahrTy; };//无实际用途
    struct arryPtr { int size; snl_TypeKind arryayTy; int up; int low;  snl_TypeKind elemTy; };//无实际用途
    class Fieldchain { public: string idname; TypeIR* unitType; int offset; Fieldchain* next; }; //记录域表，记录结构体的body域，这个结构体是有实际用途的
    struct recordPtr { int size; snl_TypeKind recordTy; Fieldchain* body; };  //单表结构中无实际用途

    class ParamTable { public: string param_name; SymbolTable* location; ParamTable* next; };//参数表
   //struct RecordTable { string idname; TypeIR unitType; int offset; RecordTable* next; };//记录域表
    
    
    
    
        //类型的内部表示
    struct TypeIR {
        int size;
        snl_TypeKind snlkind_Ty;
        union {
            struct {
                int up;
                int low;
                TypeIR* elemType;
            }array;               //数组类型有效
            Fieldchain* body;      //类型为结构体，枚举等时有效
        };
    };
    
    
    
    typedef struct {
        struct TypeIR* idtpye;       //属性域中表示标识符的类型
        IdKind kind;        //标识符的种类
        union {
            struct {
                AccessKind access;
                int level;
                int off;
            }VarAttr;
            struct {
                Function_Class fclass;        //表示函数是过程或者形式函数
                int off;       //对过程函数有效，表示偏移
                int level;
                ParamTable* param;         //指向下一个形参位置，若为NULL则表示该形参是最后一个参数
                int code;
                int size;
            }ProcAttr;
        }VarOrPorc;
    }AttributeIR;
    
    class SymbolTable {
    public:
        string symbolid;
        AttributeIR atribute;
        SymbolTable* next;
    };



    SymbolTable* entry = NULL;  //该指针用于查找符号表中的元素，找到则指向该表项
    SymbolTable* tail = NULL;   //指向符号表的最后一项



    //以上是符号表的数据结构定义
    SymbolTable* Scope[100]; //Scope栈


    void CreateTable() {             //新进入局部化单位后初始化一个符号表
        level++;//层数加一
        Scope[level] = NULL;
        offset = initoff;   //该层的符号表偏移赋初值
        tail = NULL;
    }


    void DropTable() {        //移动栈指针，删除一个符号表
        Scope[level] = NULL;
        level--;
    }



    bool FindSymbol();

    //新建一个表项并写入内容，参数应该传入tail
    bool WriteToTable(string id_name, AttributeIR* attributeIR, SymbolTable* Tail) {
        /*
        bool flag = FindSymbol(id_name, level, Entry);
        if (flag == true) {
            printf("error，标识符重复声明");
            return false;
        }                        //重复声明的工作放在函数之外进行
        */
        //else {
            SymbolTable sTable;
            sTable.atribute = *attributeIR;
            sTable.symbolid = id_name;
            if (Tail == NULL) {
                Scope[level] = &sTable;
            }
            else {
                Tail->next = &sTable;
            }
            Tail = &sTable;
            return  true;
        //}
    }


    //在当前层查表，是FindSymbol的子函数，此时Entry应该传入entry
    bool SearchoneTable(string id, int currentlevel, SymbolTable* Entry) {
        SymbolTable* p = Scope[currentlevel];
        entry = NULL;        //先将entry置空
        while (p != NULL) {
            if (p->symbolid == id) {
                Entry = p;
                return true;
            }
            p = p->next;
        }
        return false;
    }


    //递归地向上查表
    bool FindSymbol(string id, int currentlevel, SymbolTable* Entry) {
        bool flag = SearchoneTable(id, currentlevel, Entry);
        if (flag) {
            return true;
        }
        else {
            if (currentlevel > -1) {
                currentlevel--;
                FindSymbol(id, currentlevel, Entry);
            }
            else {
                return false;
            }
        }
    }


    //在记录域表中查找标识符
    bool FindField(char* id, Fieldchain* head, Fieldchain* Entry) {
        Fieldchain* h = head;
        while (h != NULL) {
            if (h->idname == id) {
                Entry = h;
                return true;
            }
            h = h->next;
        }
        return false;
    }



    //打印符号表的内容
    void PrintTable() {
        for (int plevel = 0; plevel <= level; plevel++) {
            SymbolTable* p = Scope[plevel];
            while (p != NULL) {
                cout << p->symbolid << " " << p->atribute.idtpye->snlkind_Ty << p->atribute.kind; //先打印这些内容
                p = p->next;
            }
        }
    }


    
    
    //以上是对符号表的操作函数
     
    
    
    
    //初始化intPtr，charPtr，boolPtr,此函数是否正确存疑，按照无用处理
    void Initialize() {
        TypeIR* intPtr;
        TypeIR* charPtr;
        TypeIR* boolPtr;
        intPtr->snlkind_Ty = intTy;
        intPtr->size = 1;
        charPtr->snlkind_Ty = charTy;
        charPtr->size = 1;
        boolPtr->snlkind_Ty = boolTy;
        boolPtr->size = 1;
    }
    //返回一个intPtr指针，指向int内部表示
    TypeIR* IntPtr() {
        TypeIR* intPtr;
        intPtr->snlkind_Ty = intTy;
        intPtr->size = 1;
        return intPtr;
    }
    //返回一个charPtr指针，指向char内部表示
    TypeIR* CharPtr() {
        TypeIR* charPtr;
        charPtr->snlkind_Ty = charTy;
        charPtr->size = 1;
        return charPtr;
    }
    //返回一个boolPtr指针，指向bool内部表示
    TypeIR* BoolPtr() {
        TypeIR* boolPtr;
        boolPtr->snlkind_Ty = boolTy;
        boolPtr->size = 1;
        return boolPtr;
    }
    //基础类型处理函数，调用IntPtr()和CharPtr()
    TypeIR* BaseType(TreeNode* t) {
        if (t->child[0]->name == "interger") {
            return IntPtr();
        }
        if (t->child[0]->name == "char") {
            return CharPtr();
        }
        else {
            Error_1();         //此处出现第一个错误，期望之外的基类型
        }
    }
    //检查数组并且返回数组类型指针,此时t应该传入ArryType
    TypeIR* ArrayPtr(TreeNode* t) {
        int low = atoi(t->child[2]->child[0]->token->Sem.c_str());
        int up = atoi(t->child[4]->child[0]->token->Sem.c_str());
        if (low > up) {
            Error_2();            //此时出现第二个错误，数组下标越界
        }
        TypeIR* elemPtr = BaseType(t->child[7]);     //处理元素类型
        TypeIR* arrayPtr;
        arrayPtr->size = up - low;
        arrayPtr->snlkind_Ty = arraryTy;
        arrayPtr->array.elemType = elemPtr;
        arrayPtr->array.low = low;
        arrayPtr->array.up =  up;
        return arrayPtr;
    }
    //检查记录域并且返回记录类型指针，此时t应传入RecType
    TypeIR* RecPtr(TreeNode* t) {
        TypeIR* recPtr;
        recPtr->snlkind_Ty = recordTy;
        t = t->child[1];     //将t移动到FiledDecList
        Fieldchain* fchain = FieldDecList(t);
        recPtr->body = fchain;
        return recPtr;
    }
    //该函数是RecPtr的子函数，被RecPtr递归调用处理FieldDecList节点,t应该传入FieldDecList
    Fieldchain* FieldDecList(TreeNode* t) {
        Fieldchain* fchainhead = NULL;      
        TypeIR* UType;
        Fieldchain* fchaintail = fchainhead;
        int offset = 0;
        while (t->child[2]->name == "FieldDecList") {
            if (t->child[0]->name == "BaseType") {
                UType = BaseType(t->child[0]);
            }
            if (t->child[0]->name == "ArrayType") {
                UType = ArrayPtr(t->child[0]);
            }                                     //处理child[0],UnitType
            if (t->child[1]->name == "IdList") {
                TreeNode* i = t->child[1];         // i指向IdList
                while (i->name == "IdList") {
                    string id = i->child[0]->name;
                    Fieldchain* fchaintool = new Fieldchain();   //当碰到一个id,生成一个新表项
                    if (fchaintail == NULL) {
                        fchaintail = fchaintool;
                    }
                    else if(fchaintail != NULL) {
                        fchaintail->next = fchaintool;
                        fchaintail = fchaintool;
                    }
                    
                    fchaintool->idname = id;
                    fchaintool->unitType = UType;
                    fchaintool->offset = offset;
                    offset += UType->size;
                    if (i->child[1]->child[0] == NULL) {
                        break;
                    }
                    else if(i->child[1]->child[0]->name == "IdList") {
                        i = i->child[1]->child[1];
                    }            //循环处理IdList节点
                }
            }
            if (t->child[2]->child[0] == NULL) {
                break;
            }
            else if(t->child[3]->child[0]->name == "FieldDecList") {
                t = t->child[3]->child[0];
            }            //循环处理FieldDecList节点
        }
        return fchainhead;
    }

    //处理复合类型的函数，将节点StructrueTyoe传入函数
    TypeIR* StructrueType(TreeNode* t) {
        if (t->child[0]->name == "ArrayType") {
            return ArrayPtr(t->child[0]);
        }
        if (t->child[0]->name == "RecType") {
            return RecPtr(t->child[0]);
        }
        else {
            Error_3();       //此处出现第三个错误，期望之外的复合类型
        }
    }


    //处理自定义类型的函数，应将ID作为参数传入
    TypeIR* IDType(TreeNode* t) {
        string id_name = t->token->Sem;
        SymbolTable* e;
        bool flag = FindSymbol(id_name, level, entry);
        if (flag == false) {
            Error_4();       //此时出现第四个错误，未定义的类型
        }
        else {
            if (entry->atribute.kind != typekind) {
                Error_5();            //此时出现的五个错误，该标识符不是自定义类型
            }
            else {
                TypeIR* idPtr = entry->atribute.idtpye;
                return idPtr;
            }
        }
    }



    
    

    //构造类型的内部表示，此时节点t应该传入TypeDef
    TypeIR* TypeProcess(TreeNode* t) {
        if (t->child[0]->name == "BaseType") {
            return BaseType(t->child[0]);   //调用基类型处理函数，将节点BaseType传入函数
        }
        if (t->child[0]->name == "StructrueType") {
            return StructrueType(t->child[0]);     //调用复合类型处理函数，将节点StructrueType传入函数
        }
        if (t->child[0]->name == "ID") {
            return IDType(t->child[0]);    //调用将ID传入函数
        }

    }
    

    //处理声明部分的函数,此时将TypeDecPart传入
    void TypeDecPart(TreeNode* t) {
        if (t->child[0] == NULL) {
            return;
        }
        else {
            t = t->child[0];      //此时t为TypeDec
            t = t->child[1];      //此时t为TypeDecList
            while (t->name == "TypeDecList") {
                string ty_id = t->child[0]->child[0]->token->Sem;
                bool flag = FindSymbol(ty_id, level, entry);
                if (flag == true) {
                    Error_6();        //此时出现第六个错误，重复声明的类型
                }
                else {
                    AttributeIR* air;
                    air->kind = typekind;
                    TypeIR* ty_tpir = TypeProcess(t->child[2]);  //将TypeDef传入
                    air->idtpye = ty_tpir;
                    //VarOrProc不做处理
                    WriteToTable(ty_id, air, tail);
                    if (t->child[4]->child[0] == NULL) {
                        break;
                    }
                    else if (t->child[4]->child[0]->name == "TypeDecList") {
                        t = t->child[4]->child[0];
                    }
                }
            }
        }
    }
    
    
    
    //以上已经将TypeDecPart部分写完
    
    
    
    //VarDecPart部分
    
    
    
    //VarDecPart,Param TreeNode* is VarDecPart
    void VarDecPart(TreeNode* t) {
        if (t->child[0] == NULL) {
            return;
        }
        else {
            t = t->child[0];   //t is VarDec now
            t = t->child[1];   //t is VarDecList now
            if (t->name == "VarDecList") {
                while (t->name == "VarDecList") {
                    TypeIR* v_ir = TypeProcess(t->child[0]);       //param is TypeDef
                    TreeNode* i = t->child[1];      //i is VarIdList
                    if (i->name == "VarIdList") {
                        while (i->name == "VarIdList") {
                            string v_id = i->child[0]->token->Sem;
                            bool flag = FindSymbol(v_id, level, entry);
                            if (flag) {
                                Error_7();         //此时出现第七个错误，重复声明的变量
                            }
                            else {
                                AttributeIR* v_atr;
                                v_atr->idtpye = v_ir;
                                v_atr->kind = varkind;
                                v_atr->VarOrPorc.VarAttr.level = level;
                                v_atr->VarOrPorc.VarAttr.off = offset;
                                offset += v_ir->size;
                                WriteToTable(v_id, v_atr, tail);
                            }
                            if (i->child[0] == NULL) {
                                break;
                            }
                            else if (i->child[1]->name == "VarIdList") {
                                i = i->child[1];
                            }
                        }
                    }
                    if (t->child[2]->child[0] == NULL) {
                        break;
                    }
                    else if (t->child[3]->child[0]->name == "VarDecList") {
                        t->child[3]->child[0];
                    }
                }
            }
        }
    
    }
    
    //以上是对变量声明的所有处理


    //以下处理过程声明
    
    //参数处理函数，此时参数传入ParamList节点
    ParamTable* ParamList(TreeNode* t) {
        CreateTable();           //表示新进入一个局部化区域，level++;
        ParamTable* ptable = NULL;
        if (t->child[0] == NULL) {
            return ptable;
        }
        else {
            t = t->child[0];             //此时t为ParamDecList
            ptable = ParamDecList(t);
            return ptable;
        }
    }
    

    //ParamList的子函数，此时参数传入ParamList
    ParamTable* ParamDecList(TreeNode* t) {
        ParamTable* p_tail = NULL;
        ParamTable* p_head = p_tail;
        while (t->name == "ParamDecList") {
            TypeIR* p_ir = TypeProcess(t->child[0]->child[1]);
            if (t->child[0]->child[2]->name == "FormList") {
                TreeNode* i = t->child[0]->child[2];        //i is FormList
                while (i->name == "FormList") {
                    string p_id = i->child[0]->token->Sem;
                    AttributeIR* p_atr;
                    p_atr->idtpye = p_ir;
                    p_atr->kind = varkind;
                    p_atr->VarOrPorc.VarAttr.level = level;
                    p_atr->VarOrPorc.VarAttr.off = offset;
                    WriteToTable(p_id, p_atr, tail);             //对符号表操作
                    
                    ParamTable* p = new ParamTable();
                    p->location = tail;
                    p->param_name = p_id; 
                    if (p_tail == NULL) {
                        p_tail = p;
                    }
                    else {
                        p_tail->next = p;
                        p_tail = p;
                    }                                          //生成参数表
                 
                    if (i->child[1]->child[0] == NULL) {
                        break;
                    }
                    else if(i->child[1]->child[1]->name == "FormList") {
                        i = i->child[1]->child[1];
                    }
                }
            }
            if (t->child[1]->child[0] == NULL) {
                break;
            }
            else if (t->child[1]->child[1]->name == "ParamDecList") {
                t = t->child[1]->child[1];
            }
        }
        return p_head;
    }


    //处理函数本身的符号表项的函数，t应传入ProcDec,因为需要连接参数表，因此该函数调用ParamList函数
    void ProcDec(TreeNode* t) {
        AttributeIR* pr_atr;
        string proc_id = t->child[1]->child[0]->token->Sem;
        pr_atr->idtpye = NULL;
        pr_atr->kind = prockind;
        pr_atr->VarOrPorc.ProcAttr.param = ParamList(t->child[3]);
        WriteToTable(proc_id, pr_atr, tail);
        ProcDecpart(t->child[6]);      //传入ProcDecpart
        ProcBody(t->child[7]);
    }
    

    //处理声明部分的函数，t传入DeclarePart
    void DeclarePart(TreeNode* t) {
        for (int i = 0; i <= 2; i++) {
            t = t->child[i];      //此时的t分别为TypeDecPart，VarDecPart，ProcDecPart
            if (t->name == "TypedDecPart") {
                TypeDecPart(t);
            }
            if (t->name == "VarDecPart") {
                VarDecPart(t);
            }
            if (t->name == "ProcDecPart") {
                ProcDecPart(t);
            }
        }
    }

    //仅调用DeclarePart函数，t为ProDecPart,注意该p是小写，与前一个ProcDecPart不同
    void ProcDecpart(TreeNode* t) {
        DeclarePart(t->child[0]);
    }

    //仅调用ProgramBody函数，t为ProcBody
    void ProcBody(TreeNode* t) {
        ProgramBody(t->child[0]);
    }

    //处理函数声明部分的函数，传入ProcDecPart
    void ProcDecPart(TreeNode* t) {
        if (t->child[0] == NULL) {
            return;
        }
        else if(t->child[0]->name == "ProcDec") {
            ProcDec(t->child[0]);
        }
    }




    //对函数体部分处理的函数，t传入ProgramBody
    void ProgramBody(TreeNode* t) {
        if (t->child[1]->name == "StmList") {
            StmList(t->child[1]);           //此时t为StmList
        }
    }


    //t is StmList
    void StmList(TreeNode* t) {
        while (t->name == "StmList") {
            Stm(t->child[0]);
            if (t->child[1]->child[0] == NULL) {
                return;
            }
            else if (t->child[1]->child[1]->name == "StmList") {
                t = t->child[1]->child[1];
            }
        }
    }



    //t is Stm
    void Stm(TreeNode* t) {
        if (t->child[0]->name == "ConditionalStm") {
            ConditionalStm(t->child[0]);
        }
        if (t->child[0]->name == "LoopStm") {
            LoopStm(t->child[0]);
        }
        if (t->child[0]->name == "InputStm") {
            InputStm(t->child[0]);
        }
        if (t->child[0]->name == "OutputStm") {
            OutputStm(t->child[0]);
        }
        if (t->child[0]->name == "ReturnStm") {
            ReturnStm(t->child[0]);
        }
        if (t->child[1]->name == "AssCall") {
            AssCall(t->child[1],t->child[0]->token->Sem);
        }
    }


    //处理表达式的函数
    TypeIR* Exp(TreeNode* t) {
        TypeIR* Eptr;
        while (t->name == "Exp") {
            if (t->child[0]->name == "Term") {
                Eptr = Term(t->child[0]);
            }
            if (t->child[1]->child[0] == NULL) {
                break;
            }
            else if (t->child[1]->child[1]->name == "Exp") {
                t = t->child[1]->child[1];
            }
        }
        return Eptr;
    }


    //t is Term
    TypeIR* Term(TreeNode* t) {
        TypeIR* Eptr;
        while (t->name == "Term") {
            if (t->child[0]->child[0]->name == "INTC") {
                Eptr->snlkind_Ty = intTy;
                Eptr->size = 1;
            }
            if (t->child[0]->child[1]->name == "Exp") {
                Eptr = Exp(t->child[0]->child[1]);
            }
            if (t->child[0]->child[0]->name == "Varriable") {
                Eptr = Varriable(t->child[0]->child[0]);
            }
            if (t->child[1]->child[0] == NULL) {
                break; 
            }
            else if (t->child[1]->child[0]->name == "Term") {
                t = t->child[1]->child[0];
            }
        }
        return Eptr;
    }


    //t is Varriable
    TypeIR* Varriable(TreeNode* t) {
        TypeIR* Eptr;
        string v_id = t->child[0]->token->Sem;
        if (t->child[1]->child[0] == NULL) {              //VariMore is NULL
            bool flag = FindSymbol(v_id,level,entry);
            if (flag == false) {                        //未找到标识符
                Error_8();                     //此时出现第八个错误，未定义的标识符
            }
            else {                                        //找到标识符
                if (entry->atribute.kind == varkind) {
                    Eptr = entry->atribute.idtpye;
                }
                else {
                    Error_9();              //此时出现第九个错误，该标识符不是变量类型
                }
            }
        }
        else {                              //VarriableMore is not null
            if (t->child[1]->child[0]->name == ".") {
                Eptr = recordVar(t->child[1],v_id);         //t is VariMore
            }
            else {
                Eptr = arrayVar(t->child[1],v_id);              //t is VariMore
            }
        }
        return Eptr;
    }



    //t is VariMore
    TypeIR* arrayVar(TreeNode* t,string id) {
        bool flag = FindSymbol(id, level, entry);
        TypeIR* Eptr;
        if (flag == false) {
            Error_8();          //未定义的标识符
        }
        else {
            if (entry->atribute.kind != varkind) {
                Error_9();
            }
            else {
                if (entry->atribute.idtpye->snlkind_Ty != arraryTy) {
                    Error_10();            //此时出现第十个错误，该变量不是数组类型变量
                }
                else {
                    TypeIR* ty = Exp(t->child[1]);
                    if (ty->snlkind_Ty != intTy) {
                        Error_11();               //此时出现第十一个错误，期望之外的数组下标类型
                    }
                    else {
                        Eptr = entry->atribute.idtpye->array.elemType;
                    }
                }
            }
        }
        return Eptr;
    }


    //t is VariMore
    TypeIR* recordVar(TreeNode* t, string id) {
        bool flag = FindSymbol(id, level, entry);
        TypeIR* Eptr;
        if (flag == false) {
            Error_8();               //未定义的标识符
        }
        else {
            if (entry->atribute.kind != varkind) {
                Error_9();
            }
            else {
                if (entry->atribute.idtpye->snlkind_Ty != recordTy) {
                    Error_12();     //此时出现第十二个错误，该变量不是记录类型
                }
                else {
                    bool fg = false;
                    Fieldchain* p = entry->atribute.idtpye->body;
                    while (p != NULL) {
                        if (p->idname == t->child[1]->child[0]->token->Sem) {
                            fg = true;
                            break;
                        }
                        p = p->next;
                    }
                    if (fg == false) {
                        Error_13();            //此时出现第十三个错误，记录域中无该变量
                    }
                    else {
                        Eptr = p->unitType;
                    }
                }
            }
        }
        return Eptr;
    }



    //t is AssCall
    void AssCall(TreeNode* t,string id) {
        if (t->child[0]->name == "AssignmentRest") {
            AssignmentRest(t->child[0],id);
        }
        if (t->child[0]->name == "CallStmRest") {
            CallStmRest(t->child[0],id);
        }
    }


    //t is AssignmentRest
    void AssignmentRest(TreeNode* t,string id) {
        TypeIR* Eptrleft;
        TypeIR* Eptrright;
        bool flag = FindSymbol(id,level,entry);
        if (flag == false) {             //表示未找到左标识符
            Error_8();
        }
        else {
            if (entry->atribute.kind != varkind) {         //左标识符不是变量
                Error_9();
            }
            else {
                if (t->child[0]->child[0] == NULL) {          //左标识符是基础类型
                    Eptrleft = entry->atribute.idtpye;
                }
                else {                       //左标识符是数组或者记录类型
                    if (t->child[0]->child[0]->name == ".") {
                        Eptrleft = recordVar(t->child[0], id);
                    }
                    if (t->child[0]->child[0]->name == "[") {
                        Eptrleft = arrayVar(t->child[0], id);
                    }
                }
            }
        }
        //操作右标识符
        Eptrright = Exp(t->child[3]);
        //判断是否相容
        if (Eptrleft->snlkind_Ty != Eptrright->snlkind_Ty) {
            Error_14();
        }
        else {
            return;
        }
    }


    //t is CallStmRest
    void  CallStmRest(TreeNode* t,string id) {
        bool flag = FindSymbol(id, level, entry);
        ParamTable* p = entry->atribute.VarOrPorc.ProcAttr.param;
        if (!flag) {
            Error_8();
        }
        else {
            if (entry->atribute.kind != prockind) {
                Error_15();               //此时出现第十五个错误，该变量不是函数类型
            }
            else {
                TreeNode* i = t->child[1];
                while(i->name == "ActParamList") {
                    if (i->child[0] == NULL) {
                        break;
                    }
                    else {
                        TypeIR* Aptr = Exp(i->child[0]);
                        TypeIR* Eptr = p->location->atribute.idtpye;
                        if (Aptr->snlkind_Ty != Eptr->snlkind_Ty) {
                            Error_16();             //此时出现第十六个错误，形参与实参类型不匹配
                        }
                        i = i->child[1];
                        p = p->next;               //移动形参和实参的位置
                        if (i->child[0] == NULL && p != NULL) {
                            Error_17();         //此时出现第十七个错误，形参与实参个数不匹配
                        }
                        if (i->child[0] != NULL && p == NULL) {
                            Error_17();
                        }
                        if (i->child[0] == NULL && p == NULL) {
                            return;
                        }
                    }
                    if (i->child[1]->name == "ActParamList") {
                        i = i->child[1];
                    }
                }
            }
        }
    }

    //t is RelExp
    bool RelExp(TreeNode* t) {
        TypeIR* Eptrl;
        TypeIR* Eptrr;
        Eptrl = Exp(t->child[0]);
        Eptrr = Exp(t->child[1]->child[1]);
        if (t->child[1]->child[0]->child[0]->name != "LT" || t->child[1]->child[0]->child[0]->name != "EQ") {
            Error_18();              //此时出现第十八个错误，条件表达式不为bool类型
            return false;
        }
        if (Eptrl->snlkind_Ty != Eptrr->snlkind_Ty) {
            Error_14();
            return false;
        }
        return true;
    }



    //t is ConditionalStm
    void ConditionalStm(TreeNode* t) {

        RelExp(t->child[1]);
        StmList(t->child[3]);
        StmList(t->child[5]);
    }

    //t is LoopStm
    void LoopStm(TreeNode* t) {
        RelExp(t->child[1]);
        StmList(t->child[3]);
    }

    //t is InputStm
    void InputStm(TreeNode* t) {
        string id = t->child[2]->child[0]->token->Sem;
        bool flag = FindSymbol(id, level, entry);
        if (!flag) {
            Error_8();
        }
        else {
            if (entry->atribute.idtpye->snlkind_Ty != varkind) {
                Error_9();
            }        
        }
    }
    

    //t is OutputStm
    void OutputStm(TreeNode* t) {
        Exp(t->child[1]);
    }

    //t is ReturnStm
    void ReturnStm(TreeNode* t) {
        return;
    }
    
    
    
    
    //错误声明函数
    void Error_1() {
        printf("期望之外的基础类型");
        exit(0);
    }
    void Error_2() {
        printf("数组下标越界");
        exit(0);
    }
    void Error_3() {
        printf("期望之外的复合类型");
        exit(0);
    }
    void Error_4() {
        printf("未定义的类型");
        exit(0);
    }
    void Error_5() {
        printf("该标识符不是自定义类型");
        exit(0);
    }
    void Error_6() {
        printf("重复声明的类型");
        exit(0);
    }
    void Error_7() {
        printf("重复声明的变量");
        exit(0);
    }
    void Error_8() {
        printf("未定义的标识符");
        exit(0);
    }
    void Error_9() {
        printf("该标识符不是变量类型");
        exit(0);
    }
    void Error_10() {
        printf("该变量不是数组类型");
        exit(0);
    }
    void Error_11() {
        printf("期望之外的数组下标类型");
        exit(0);
    }
    void Error_12() {
        printf("该变量不是记录类型");
        exit(0);
    }
    void Error_13() {
        printf("记录域中无该变量");
        exit(0);
    }
    void Error_14() {
        printf("表达式左右类型不匹配");
        exit(0);
    }
    void Error_15() {
        printf("该标识符不是函数类型");
        exit(0);
    }
    void Error_16() {
        printf("形参与实参类型不匹配");
        exit(0);
    }
    void Error_17() {
        printf("形参与实参个数不匹配");
        exit(0);
    }
    void Error_18() {
        printf("条件表达式不为bool型");
        exit(0);
    }








    //语义分析的主程序，应该将语法树的根节点作为参数传入
    void SematicAnalyze(TreeNode* Program) {
        TreeNode* t = Program;
        CreateTable();
        Initialize();
        DeclarePart(t->child[1]);
        //未完成
        ProgramBody(t->child[2]);
        DropTable();
    }
};







int main()
{
    std::cout << "Hello World!\n";
}
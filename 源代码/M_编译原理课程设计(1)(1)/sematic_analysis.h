// Compilation.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <syntax_analysis.h>
#include <morpheme_analysis.h>
using namespace std;
#define initoff 7;





class Semantic {
    int level = -1;            //scopeջ���±꣬��ʾ��ǰ����,��һ�ν���ֲ�����λʱ�Ὣindex��Ϊ0
    int offset;           //��ǰ������ƫ����
    struct TypeIR;
    struct SymbolTable;
    


    enum snl_TypeKind { intTy, charTy, arraryTy, recordTy, boolTy };
    enum AccessKind { dir, indir };
    enum IdKind { typekind, varkind, prockind };
    enum Function_Class { actua, formal };
    struct intPtr { int size = 1; snl_TypeKind intTy; };//��ʵ����;
    struct charPtr { int size = 1; snl_TypeKind cahrTy; };//��ʵ����;
    struct arryPtr { int size; snl_TypeKind arryayTy; int up; int low;  snl_TypeKind elemTy; };//��ʵ����;
    class Fieldchain { public: string idname; TypeIR* unitType; int offset; Fieldchain* next; }; //��¼�����¼�ṹ���body������ṹ������ʵ����;��
    struct recordPtr { int size; snl_TypeKind recordTy; Fieldchain* body; };  //����ṹ����ʵ����;

    class ParamTable { public: string param_name; SymbolTable* location; ParamTable* next; };//������
   //struct RecordTable { string idname; TypeIR unitType; int offset; RecordTable* next; };//��¼���
    
    
    
    
        //���͵��ڲ���ʾ
    struct TypeIR {
        int size;
        snl_TypeKind snlkind_Ty;
        union {
            struct {
                int up;
                int low;
                TypeIR* elemType;
            }array;               //����������Ч
            Fieldchain* body;      //����Ϊ�ṹ�壬ö�ٵ�ʱ��Ч
        };
    };
    
    
    
    typedef struct {
        struct TypeIR* idtpye;       //�������б�ʾ��ʶ��������
        IdKind kind;        //��ʶ��������
        union {
            struct {
                AccessKind access;
                int level;
                int off;
            }VarAttr;
            struct {
                Function_Class fclass;        //��ʾ�����ǹ��̻�����ʽ����
                int off;       //�Թ��̺�����Ч����ʾƫ��
                int level;
                ParamTable* param;         //ָ����һ���β�λ�ã���ΪNULL���ʾ���β������һ������
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



    SymbolTable* entry = NULL;  //��ָ�����ڲ��ҷ��ű��е�Ԫ�أ��ҵ���ָ��ñ���
    SymbolTable* tail = NULL;   //ָ����ű�����һ��



    //�����Ƿ��ű�����ݽṹ����
    SymbolTable* Scope[100]; //Scopeջ


    void CreateTable() {             //�½���ֲ�����λ���ʼ��һ�����ű�
        level++;//������һ
        Scope[level] = NULL;
        offset = initoff;   //�ò�ķ��ű�ƫ�Ƹ���ֵ
        tail = NULL;
    }


    void DropTable() {        //�ƶ�ջָ�룬ɾ��һ�����ű�
        Scope[level] = NULL;
        level--;
    }



    bool FindSymbol();

    //�½�һ�����д�����ݣ�����Ӧ�ô���tail
    bool WriteToTable(string id_name, AttributeIR* attributeIR, SymbolTable* Tail) {
        /*
        bool flag = FindSymbol(id_name, level, Entry);
        if (flag == true) {
            printf("error����ʶ���ظ�����");
            return false;
        }                        //�ظ������Ĺ������ں���֮�����
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


    //�ڵ�ǰ������FindSymbol���Ӻ�������ʱEntryӦ�ô���entry
    bool SearchoneTable(string id, int currentlevel, SymbolTable* Entry) {
        SymbolTable* p = Scope[currentlevel];
        entry = NULL;        //�Ƚ�entry�ÿ�
        while (p != NULL) {
            if (p->symbolid == id) {
                Entry = p;
                return true;
            }
            p = p->next;
        }
        return false;
    }


    //�ݹ�����ϲ��
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


    //�ڼ�¼����в��ұ�ʶ��
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



    //��ӡ���ű������
    void PrintTable() {
        for (int plevel = 0; plevel <= level; plevel++) {
            SymbolTable* p = Scope[plevel];
            while (p != NULL) {
                cout << p->symbolid << " " << p->atribute.idtpye->snlkind_Ty << p->atribute.kind; //�ȴ�ӡ��Щ����
                p = p->next;
            }
        }
    }


    
    
    //�����ǶԷ��ű�Ĳ�������
     
    
    
    
    //��ʼ��intPtr��charPtr��boolPtr,�˺����Ƿ���ȷ���ɣ��������ô���
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
    //����һ��intPtrָ�룬ָ��int�ڲ���ʾ
    TypeIR* IntPtr() {
        TypeIR* intPtr;
        intPtr->snlkind_Ty = intTy;
        intPtr->size = 1;
        return intPtr;
    }
    //����һ��charPtrָ�룬ָ��char�ڲ���ʾ
    TypeIR* CharPtr() {
        TypeIR* charPtr;
        charPtr->snlkind_Ty = charTy;
        charPtr->size = 1;
        return charPtr;
    }
    //����һ��boolPtrָ�룬ָ��bool�ڲ���ʾ
    TypeIR* BoolPtr() {
        TypeIR* boolPtr;
        boolPtr->snlkind_Ty = boolTy;
        boolPtr->size = 1;
        return boolPtr;
    }
    //�������ʹ�����������IntPtr()��CharPtr()
    TypeIR* BaseType(TreeNode* t) {
        if (t->child[0]->name == "interger") {
            return IntPtr();
        }
        if (t->child[0]->name == "char") {
            return CharPtr();
        }
        else {
            Error_1();         //�˴����ֵ�һ����������֮��Ļ�����
        }
    }
    //������鲢�ҷ�����������ָ��,��ʱtӦ�ô���ArryType
    TypeIR* ArrayPtr(TreeNode* t) {
        int low = atoi(t->child[2]->child[0]->token->Sem.c_str());
        int up = atoi(t->child[4]->child[0]->token->Sem.c_str());
        if (low > up) {
            Error_2();            //��ʱ���ֵڶ������������±�Խ��
        }
        TypeIR* elemPtr = BaseType(t->child[7]);     //����Ԫ������
        TypeIR* arrayPtr;
        arrayPtr->size = up - low;
        arrayPtr->snlkind_Ty = arraryTy;
        arrayPtr->array.elemType = elemPtr;
        arrayPtr->array.low = low;
        arrayPtr->array.up =  up;
        return arrayPtr;
    }
    //����¼���ҷ��ؼ�¼����ָ�룬��ʱtӦ����RecType
    TypeIR* RecPtr(TreeNode* t) {
        TypeIR* recPtr;
        recPtr->snlkind_Ty = recordTy;
        t = t->child[1];     //��t�ƶ���FiledDecList
        Fieldchain* fchain = FieldDecList(t);
        recPtr->body = fchain;
        return recPtr;
    }
    //�ú�����RecPtr���Ӻ�������RecPtr�ݹ���ô���FieldDecList�ڵ�,tӦ�ô���FieldDecList
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
            }                                     //����child[0],UnitType
            if (t->child[1]->name == "IdList") {
                TreeNode* i = t->child[1];         // iָ��IdList
                while (i->name == "IdList") {
                    string id = i->child[0]->name;
                    Fieldchain* fchaintool = new Fieldchain();   //������һ��id,����һ���±���
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
                    }            //ѭ������IdList�ڵ�
                }
            }
            if (t->child[2]->child[0] == NULL) {
                break;
            }
            else if(t->child[3]->child[0]->name == "FieldDecList") {
                t = t->child[3]->child[0];
            }            //ѭ������FieldDecList�ڵ�
        }
        return fchainhead;
    }

    //���������͵ĺ��������ڵ�StructrueTyoe���뺯��
    TypeIR* StructrueType(TreeNode* t) {
        if (t->child[0]->name == "ArrayType") {
            return ArrayPtr(t->child[0]);
        }
        if (t->child[0]->name == "RecType") {
            return RecPtr(t->child[0]);
        }
        else {
            Error_3();       //�˴����ֵ�������������֮��ĸ�������
        }
    }


    //�����Զ������͵ĺ�����Ӧ��ID��Ϊ��������
    TypeIR* IDType(TreeNode* t) {
        string id_name = t->token->Sem;
        SymbolTable* e;
        bool flag = FindSymbol(id_name, level, entry);
        if (flag == false) {
            Error_4();       //��ʱ���ֵ��ĸ�����δ���������
        }
        else {
            if (entry->atribute.kind != typekind) {
                Error_5();            //��ʱ���ֵ�������󣬸ñ�ʶ�������Զ�������
            }
            else {
                TypeIR* idPtr = entry->atribute.idtpye;
                return idPtr;
            }
        }
    }



    
    

    //�������͵��ڲ���ʾ����ʱ�ڵ�tӦ�ô���TypeDef
    TypeIR* TypeProcess(TreeNode* t) {
        if (t->child[0]->name == "BaseType") {
            return BaseType(t->child[0]);   //���û����ʹ����������ڵ�BaseType���뺯��
        }
        if (t->child[0]->name == "StructrueType") {
            return StructrueType(t->child[0]);     //���ø������ʹ����������ڵ�StructrueType���뺯��
        }
        if (t->child[0]->name == "ID") {
            return IDType(t->child[0]);    //���ý�ID���뺯��
        }

    }
    

    //�����������ֵĺ���,��ʱ��TypeDecPart����
    void TypeDecPart(TreeNode* t) {
        if (t->child[0] == NULL) {
            return;
        }
        else {
            t = t->child[0];      //��ʱtΪTypeDec
            t = t->child[1];      //��ʱtΪTypeDecList
            while (t->name == "TypeDecList") {
                string ty_id = t->child[0]->child[0]->token->Sem;
                bool flag = FindSymbol(ty_id, level, entry);
                if (flag == true) {
                    Error_6();        //��ʱ���ֵ����������ظ�����������
                }
                else {
                    AttributeIR* air;
                    air->kind = typekind;
                    TypeIR* ty_tpir = TypeProcess(t->child[2]);  //��TypeDef����
                    air->idtpye = ty_tpir;
                    //VarOrProc��������
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
    
    
    
    //�����Ѿ���TypeDecPart����д��
    
    
    
    //VarDecPart����
    
    
    
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
                                Error_7();         //��ʱ���ֵ��߸������ظ������ı���
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
    
    //�����ǶԱ������������д���


    //���´����������
    
    //��������������ʱ��������ParamList�ڵ�
    ParamTable* ParamList(TreeNode* t) {
        CreateTable();           //��ʾ�½���һ���ֲ�������level++;
        ParamTable* ptable = NULL;
        if (t->child[0] == NULL) {
            return ptable;
        }
        else {
            t = t->child[0];             //��ʱtΪParamDecList
            ptable = ParamDecList(t);
            return ptable;
        }
    }
    

    //ParamList���Ӻ�������ʱ��������ParamList
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
                    WriteToTable(p_id, p_atr, tail);             //�Է��ű����
                    
                    ParamTable* p = new ParamTable();
                    p->location = tail;
                    p->param_name = p_id; 
                    if (p_tail == NULL) {
                        p_tail = p;
                    }
                    else {
                        p_tail->next = p;
                        p_tail = p;
                    }                                          //���ɲ�����
                 
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


    //����������ķ��ű���ĺ�����tӦ����ProcDec,��Ϊ��Ҫ���Ӳ�������˸ú�������ParamList����
    void ProcDec(TreeNode* t) {
        AttributeIR* pr_atr;
        string proc_id = t->child[1]->child[0]->token->Sem;
        pr_atr->idtpye = NULL;
        pr_atr->kind = prockind;
        pr_atr->VarOrPorc.ProcAttr.param = ParamList(t->child[3]);
        WriteToTable(proc_id, pr_atr, tail);
        ProcDecpart(t->child[6]);      //����ProcDecpart
        ProcBody(t->child[7]);
    }
    

    //�����������ֵĺ�����t����DeclarePart
    void DeclarePart(TreeNode* t) {
        for (int i = 0; i <= 2; i++) {
            t = t->child[i];      //��ʱ��t�ֱ�ΪTypeDecPart��VarDecPart��ProcDecPart
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

    //������DeclarePart������tΪProDecPart,ע���p��Сд����ǰһ��ProcDecPart��ͬ
    void ProcDecpart(TreeNode* t) {
        DeclarePart(t->child[0]);
    }

    //������ProgramBody������tΪProcBody
    void ProcBody(TreeNode* t) {
        ProgramBody(t->child[0]);
    }

    //�������������ֵĺ���������ProcDecPart
    void ProcDecPart(TreeNode* t) {
        if (t->child[0] == NULL) {
            return;
        }
        else if(t->child[0]->name == "ProcDec") {
            ProcDec(t->child[0]);
        }
    }




    //�Ժ����岿�ִ���ĺ�����t����ProgramBody
    void ProgramBody(TreeNode* t) {
        if (t->child[1]->name == "StmList") {
            StmList(t->child[1]);           //��ʱtΪStmList
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


    //������ʽ�ĺ���
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
            if (flag == false) {                        //δ�ҵ���ʶ��
                Error_8();                     //��ʱ���ֵڰ˸�����δ����ı�ʶ��
            }
            else {                                        //�ҵ���ʶ��
                if (entry->atribute.kind == varkind) {
                    Eptr = entry->atribute.idtpye;
                }
                else {
                    Error_9();              //��ʱ���ֵھŸ����󣬸ñ�ʶ�����Ǳ�������
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
            Error_8();          //δ����ı�ʶ��
        }
        else {
            if (entry->atribute.kind != varkind) {
                Error_9();
            }
            else {
                if (entry->atribute.idtpye->snlkind_Ty != arraryTy) {
                    Error_10();            //��ʱ���ֵ�ʮ�����󣬸ñ��������������ͱ���
                }
                else {
                    TypeIR* ty = Exp(t->child[1]);
                    if (ty->snlkind_Ty != intTy) {
                        Error_11();               //��ʱ���ֵ�ʮһ����������֮��������±�����
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
            Error_8();               //δ����ı�ʶ��
        }
        else {
            if (entry->atribute.kind != varkind) {
                Error_9();
            }
            else {
                if (entry->atribute.idtpye->snlkind_Ty != recordTy) {
                    Error_12();     //��ʱ���ֵ�ʮ�������󣬸ñ������Ǽ�¼����
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
                        Error_13();            //��ʱ���ֵ�ʮ�������󣬼�¼�����޸ñ���
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
        if (flag == false) {             //��ʾδ�ҵ����ʶ��
            Error_8();
        }
        else {
            if (entry->atribute.kind != varkind) {         //���ʶ�����Ǳ���
                Error_9();
            }
            else {
                if (t->child[0]->child[0] == NULL) {          //���ʶ���ǻ�������
                    Eptrleft = entry->atribute.idtpye;
                }
                else {                       //���ʶ����������߼�¼����
                    if (t->child[0]->child[0]->name == ".") {
                        Eptrleft = recordVar(t->child[0], id);
                    }
                    if (t->child[0]->child[0]->name == "[") {
                        Eptrleft = arrayVar(t->child[0], id);
                    }
                }
            }
        }
        //�����ұ�ʶ��
        Eptrright = Exp(t->child[3]);
        //�ж��Ƿ�����
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
                Error_15();               //��ʱ���ֵ�ʮ������󣬸ñ������Ǻ�������
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
                            Error_16();             //��ʱ���ֵ�ʮ���������β���ʵ�����Ͳ�ƥ��
                        }
                        i = i->child[1];
                        p = p->next;               //�ƶ��βκ�ʵ�ε�λ��
                        if (i->child[0] == NULL && p != NULL) {
                            Error_17();         //��ʱ���ֵ�ʮ�߸������β���ʵ�θ�����ƥ��
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
            Error_18();              //��ʱ���ֵ�ʮ�˸������������ʽ��Ϊbool����
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
    
    
    
    
    //������������
    void Error_1() {
        printf("����֮��Ļ�������");
        exit(0);
    }
    void Error_2() {
        printf("�����±�Խ��");
        exit(0);
    }
    void Error_3() {
        printf("����֮��ĸ�������");
        exit(0);
    }
    void Error_4() {
        printf("δ���������");
        exit(0);
    }
    void Error_5() {
        printf("�ñ�ʶ�������Զ�������");
        exit(0);
    }
    void Error_6() {
        printf("�ظ�����������");
        exit(0);
    }
    void Error_7() {
        printf("�ظ������ı���");
        exit(0);
    }
    void Error_8() {
        printf("δ����ı�ʶ��");
        exit(0);
    }
    void Error_9() {
        printf("�ñ�ʶ�����Ǳ�������");
        exit(0);
    }
    void Error_10() {
        printf("�ñ���������������");
        exit(0);
    }
    void Error_11() {
        printf("����֮��������±�����");
        exit(0);
    }
    void Error_12() {
        printf("�ñ������Ǽ�¼����");
        exit(0);
    }
    void Error_13() {
        printf("��¼�����޸ñ���");
        exit(0);
    }
    void Error_14() {
        printf("���ʽ�������Ͳ�ƥ��");
        exit(0);
    }
    void Error_15() {
        printf("�ñ�ʶ�����Ǻ�������");
        exit(0);
    }
    void Error_16() {
        printf("�β���ʵ�����Ͳ�ƥ��");
        exit(0);
    }
    void Error_17() {
        printf("�β���ʵ�θ�����ƥ��");
        exit(0);
    }
    void Error_18() {
        printf("�������ʽ��Ϊbool��");
        exit(0);
    }








    //���������������Ӧ�ý��﷨���ĸ��ڵ���Ϊ��������
    void SematicAnalyze(TreeNode* Program) {
        TreeNode* t = Program;
        CreateTable();
        Initialize();
        DeclarePart(t->child[1]);
        //δ���
        ProgramBody(t->child[2]);
        DropTable();
    }
};







int main()
{
    std::cout << "Hello World!\n";
}
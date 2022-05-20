// Compilation.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
//#include"morpheme_analysis.h"
#include"syntax_analysis.h"
#include"LL1_analysis.h"

using namespace std;
#define initoff 0;





class Semantic {
public:
     Semantic(string file_name)
    {
        outfile_yuyi.open(file_name.substr(0, file_name.size() - 4) + "_yuyi.txt", ios_base::out | ios_base::trunc);  //ɾ���ļ���д
        this->file_name = file_name;
    }
     ofstream outfile_yuyi;
    vector<string>yuyi;
    string file_name;
    int level = -1;            //scopeջ���±꣬��ʾ��ǰ����,��һ�ν���ֲ�����λʱ�Ὣindex��Ϊ0
    int offset;           //��ǰ������ƫ����
    struct TypeIR;
    struct SymbolTable;
    vector<string> files;


    enum snl_TypeKind { intTy, charTy, arraryTy, recordTy, boolTy };
    enum AccessKind { dir, indir };
    enum IdKind { typekind, varkind, prockind };
    enum Function_Class { actua, formal };
    struct intPtr { int size = 1; snl_TypeKind intTy; };//��ʵ����;
    struct charPtr { int size = 1; snl_TypeKind cahrTy; };//��ʵ����;
    struct arryPtr { int size; snl_TypeKind arryayTy; int up; int low;  snl_TypeKind elemTy; };//��ʵ����;
class Fieldchain { public: string idname; TypeIR* unitType; int offset; Fieldchain* next = NULL; }; //��¼�����¼�ṹ���body������ṹ������ʵ����;��
                         struct recordPtr { int size; snl_TypeKind recordTy; Fieldchain* body; };  //����ṹ����ʵ����;
                         vector<string> TypeKind = { "intTy","charTy","arrayTy","recordTy","boolTy" };
                         vector<string> IdKind_vect = { "typekind","varkind","prockind" };
class ParamTable { public: string param_name; SymbolTable* location; ParamTable* next = NULL; };//������
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
                             SymbolTable* p = Scope[level];
                             while (p->next != NULL) {
                                 p = p->next;
                             }
                             tail = p;
                         }



                         bool FindSymbol();

                         //�½�һ�����д�����ݣ�����Ӧ�ô���tail
                         void WriteToTable(string id_name, AttributeIR* attributeIR) {
                             SymbolTable* sTable = new SymbolTable();
                             sTable->atribute = *attributeIR;
                             sTable->symbolid = id_name;
                             if (tail == NULL) {
                                 Scope[level] = sTable;
                             }
                             else {
                                 tail->next = sTable;
                             }
                             tail = sTable;
                             return;
                         }


                         //�ڵ�ǰ������FindSymbol���Ӻ�������ʱEntryӦ�ô���entry
                         bool SearchoneTable(string id, int currentlevel, SymbolTable* Entry) {
                             SymbolTable* p = Scope[currentlevel];
                             entry = NULL;        //�Ƚ�entry�ÿ�
                             while (p != NULL) {
                                 //cout << "into loop" << endl;
                                 if (p->symbolid == id) {
                                     entry = p;
                                     return true;
                                 }
                                 p = p->next;
                             }
                             return false;
                         }


                         //�ݹ�����ϲ��
                         bool FindSymbol(string id, int currentlevel, SymbolTable* Entry) {
                             ////cout << "into FindSymbol" << endl;
                             // //cout << "currentlevel:" << currentlevel << endl;
                             if (currentlevel == -1) {
                                 return false;
                             }
                             bool flag = SearchoneTable(id, currentlevel, Entry);

                             if (flag) {
                                 return true;
                             }
                             else {
                                 return FindSymbol(id, currentlevel - 1, entry);
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
                         void PrintOneLevelTable() {
                             SymbolTable* p = Scope[level];
                             cout << "��ǰ������" << level << endl;
                             yuyi.push_back("��ǰ������" + to_string(level)+"\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             while (p != NULL) {
                                 cout << p->symbolid << " " << " " << IdKind_vect[p->atribute.kind];
                                 if (p->atribute.kind == varkind) {
                                     cout << " " << p->atribute.VarOrPorc.VarAttr.off;
                                 }
                                 cout << endl;
                                 if (p->atribute.kind == varkind) {
                                     yuyi.push_back(p->symbolid + "  " + IdKind_vect[p->atribute.kind] +" " +to_string( p->atribute.VarOrPorc.VarAttr.off)+"\n");
                                 }
                                 else
                                 {
                                     yuyi.push_back(p->symbolid + "  " + IdKind_vect[p->atribute.kind] + "\n");
                                 }
                                 stringstream ss;
                                 ss << yuyi[yuyi.size() - 1];
                                 outfile_yuyi << ss.str();
                                 p = p->next;
                             }
                         }



                         //�����ǶԷ��ű�Ĳ�������




                         //��ʼ��intPtr��charPtr��boolPtr,�˺����Ƿ���ȷ���ɣ��������ô���
                         void Initialize() {
                             TypeIR* intPtr = new TypeIR();
                             TypeIR* charPtr = new TypeIR();
                             TypeIR* boolPtr = new TypeIR();
                             intPtr->snlkind_Ty = intTy;
                             intPtr->size = 1;
                             charPtr->snlkind_Ty = charTy;
                             charPtr->size = 1;
                             boolPtr->snlkind_Ty = boolTy;
                             boolPtr->size = 1;
                         }
                         //����һ��intPtrָ�룬ָ��int�ڲ���ʾ
                         TypeIR* IntPtr() {
                             TypeIR* intPtr = new TypeIR();
                             intPtr->snlkind_Ty = intTy;
                             intPtr->size = 1;
                             return intPtr;
                         }
                         //����һ��charPtrָ�룬ָ��char�ڲ���ʾ
                         TypeIR* CharPtr() {
                             TypeIR* charPtr = new TypeIR();
                             charPtr->snlkind_Ty = charTy;
                             charPtr->size = 1;
                             return charPtr;
                         }
                         //����һ��boolPtrָ�룬ָ��bool�ڲ���ʾ
                         /*TypeIR* BoolPtr() {
                             TypeIR* boolPtr;
                             boolPtr->snlkind_Ty = boolTy;
                             boolPtr->size = 1;
                             return boolPtr;
                         }*/
                         //�������ʹ�����������IntPtr()��CharPtr()
                         TypeIR* BaseType(TreeNode* t) {
                             if (t->child[0]->name == "INTEGER") {
                                 return IntPtr();
                             }
                             if (t->child[0]->name == "CHAR") {
                                 return CharPtr();
                             }
                             else {
                                 Error_1();         //�˴����ֵ�һ����������֮��Ļ����ͣ��˴�����������Ĵ���������
                             }
                         }
                         //������鲢�ҷ�����������ָ��,��ʱtӦ�ô���ArryType
                         TypeIR* ArrayPtr(TreeNode* t, string id, int line) {
                             int low = atoi(t->child[2]->child[0]->token->Sem.c_str());
                             int up = atoi(t->child[4]->child[0]->token->Sem.c_str());
                             if (low > up) {
                                 Error_2(id, line);            //��ʱ���ֵڶ������������±�Խ��
                             }
                             TypeIR* elemPtr = BaseType(t->child[7]);     //����Ԫ������
                             TypeIR* arrayPtr = new TypeIR();
                             arrayPtr->size = up - low;
                             arrayPtr->snlkind_Ty = arraryTy;
                             arrayPtr->array.elemType = elemPtr;
                             arrayPtr->array.low = low;
                             arrayPtr->array.up = up;
                             return arrayPtr;
                         }
                         //����¼���ҷ��ؼ�¼����ָ�룬��ʱtӦ����RecType
                         TypeIR* RecPtr(TreeNode* t, string id) {
                             TypeIR* recPtr = new TypeIR();
                             recPtr->snlkind_Ty = recordTy;
                             t = t->child[1];     //��t�ƶ���FiledDecList
                             Fieldchain* fchain = FieldDecList(t, id);
                             Fieldchain* p = fchain;
                             int offs = 0;
                             while (p != NULL) {
                                 offs += p->unitType->size;
                                 p = p->next;
                             }
                             recPtr->size = offs;
                             recPtr->body = fchain;
                             return recPtr;
                         }
                         //�ú�����RecPtr���Ӻ�������RecPtr�ݹ���ô���FieldDecList�ڵ�,tӦ�ô���FieldDecList
                         Fieldchain* FieldDecList(TreeNode* t, string id) {
                             Fieldchain* fchaintail = NULL;
                             TypeIR* UType = new TypeIR();
                             Fieldchain* fchainhead = fchaintail;
                             int offset = 0;
                             while (t->name == "FieldDecList") {
                                 if (t->child[0]->name == "BaseType") {
                                     UType = BaseType(t->child[0]);
                                 }
                                 if (t->child[0]->name == "ArrayType") {
                                     UType = ArrayPtr(t->child[0], id, t->child[0]->child[0]->token->Line);
                                 }                                     //����child[0],UnitType
                                 if (t->child[1]->name == "IDList") {
                                     TreeNode* i = t->child[1];         // iָ��IdList
                                     while (i->name == "IDList") {
                                         string id = i->child[0]->token->Sem;
                                         Fieldchain* fchaintool = new Fieldchain();   //������һ��id,����һ���±���
                                         if (fchaintail == NULL) {
                                             fchaintail = fchaintool;
                                             fchainhead = fchaintail;
                                         }
                                         else if (fchaintail != NULL) {
                                             fchaintail->next = fchaintool;
                                             fchaintail = fchaintool;
                                         }

                                         fchaintool->idname = id;
                                         fchaintool->unitType = UType;
                                         fchaintool->offset = offset;
                                         offset += UType->size;
                                         if (i->child[1]->child.size() == 0 || i->child[1]->child[0] == NULL) {
                                             break;
                                         }
                                         else if (i->child[1]->child[1]->name == "IDList") {
                                             i = i->child[1]->child[1];
                                         }            //ѭ������IdList�ڵ�
                                     }
                                 }
                                 if (t->child[3]->child.size() == 0 || t->child[3]->child[0] == NULL) {
                                     break;
                                 }
                                 else if (t->child[3]->child[0]->name == "FieldDecList") {
                                     t = t->child[3]->child[0];
                                 }            //ѭ������FieldDecList�ڵ�
                             }
                             return fchainhead;
                         }

                         //���������͵ĺ��������ڵ�StructrueTyoe���뺯��
                         TypeIR* StructrueType(TreeNode* t, string id, int line) {
                             if (t->child[0]->name == "ArrayType") {
                                 return ArrayPtr(t->child[0], id, line);
                             }
                             if (t->child[0]->name == "RecType") {
                                 return RecPtr(t->child[0], id);
                             }
                             else {
                                 Error_3();       //�˴����ֵ�������������֮��ĸ������ͣ�ͬ��������
                             }
                         }


                         //�����Զ������͵ĺ�����Ӧ��ID��Ϊ��������
                         TypeIR* IDType(TreeNode* t, int line) {
                             string id_name = t->token->Sem;
                             //SymbolTable* e;
                             bool flag = FindSymbol(id_name, level, entry);
                             if (flag == false) {
                                 Error_4(id_name, line);       //��ʱ���ֵ��ĸ�����δ���������
                             }
                             else {
                                 if (entry->atribute.kind != typekind) {
                                     Error_5(id_name, line);            //��ʱ���ֵ�������󣬸ñ�ʶ�������Զ�������
                                 }
                                 else {
                                     TypeIR* idPtr = entry->atribute.idtpye;
                                     return idPtr;
                                 }
                             }
                         }






                         //�������͵��ڲ���ʾ����ʱ�ڵ�tӦ�ô���TypeDef
                         TypeIR* TypeProcess(TreeNode* t, string id, int line) {
                             if (t->child[0]->name == "BaseType") {
                                 return BaseType(t->child[0]);   //���û����ʹ����������ڵ�BaseType���뺯��
                             }
                             if (t->child[0]->name == "StructureType") {
                                 return StructrueType(t->child[0], id, line);     //���ø������ʹ����������ڵ�StructrueType���뺯��
                             }
                             if (t->child[0]->name == "ID") {
                                 return  IDType(t->child[0], line);    //���ý�ID���뺯��

                             }
                         }


                         //�����������ֵĺ���,��ʱ��TypeDecPart����
                         void TypeDecPart(TreeNode* t) {
                             if (t->child.size() == 0 || (t->child[0] == NULL)) {
                                 return;
                             }
                             else {
                                 t = t->child[0];      //��ʱtΪTypeDec
                                 t = t->child[1];      //��ʱtΪTypeDecList
                                 while (t->name == "TypeDecList") {
                                     string ty_id = t->child[0]->child[0]->token->Sem;
                                     bool flag = FindSymbol(ty_id, level, entry);
                                     if (flag == true) {                             
                                         Error_6(ty_id, t->child[0]->child[0]->token->Line);        //��ʱ���ֵ����������ظ�����������
                                     }
                                     else {
                                         AttributeIR* air = new AttributeIR();
                                         air->kind = typekind;
                                         TypeIR* ty_tpir = TypeProcess(t->child[2], ty_id, t->child[0]->child[0]->token->Line);  //��TypeDef����
                                         air->idtpye = ty_tpir;
                                         //VarOrProc��������
                                         WriteToTable(ty_id, air);
                                         if (t->child[4]->child.size() == 0 || t->child[4]->child[0] == NULL) {
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
                             if (t->child.size() == 0 || t->child[0] == NULL) {
                                 return;
                             }
                             else {
                                 t = t->child[0];   //t is VarDec now
                                 t = t->child[1];   //t is VarDecList now
                                 if (t->name == "VarDecList") {
                                     while (t->name == "VarDecList") {
                                         TypeIR* v_ir = TypeProcess(t->child[0], "null", -1);       //param is TypeDef �˺�������id��line����ʲô�����壬��Ϊ�����������TypeDec���ֳ��־��Ѿ�ֱ�Ӵ���
                                         TreeNode* i = t->child[1];      //i is VarIdList
                                         if (i->name == "VarIDList") {
                                             while (i->name == "VarIDList") {
                                                 string v_id = i->child[0]->token->Sem;
                                                 //cout << v_id << endl;
                                                 bool flag = SearchoneTable(v_id, level, entry);
                                                 if (flag == true) {
                                                     int line = i->child[0]->token->Line;
                                                     Error_7(entry, line);         //��ʱ���ֵ��߸������ظ������ı���
                                                 }
                                                 else {
                                                     AttributeIR* v_atr = new AttributeIR;
                                                     v_atr->idtpye = v_ir;
                                                     v_atr->kind = varkind;
                                                     v_atr->VarOrPorc.VarAttr.level = level;
                                                     v_atr->VarOrPorc.VarAttr.off = offset;
                                                     offset += v_ir->size;
                                                     WriteToTable(v_id, v_atr);
                                                 }
                                                 if (i->child[1]->child.size() == 0 || i->child[1]->child[0] == NULL) {
                                                     break;
                                                 }
                                                 else if (i->child[1]->child[1]->name == "VarIDList") {
                                                     i = i->child[1]->child[1];
                                                 }
                                             }
                                         }
                                         if (t->child[3]->child.size() == 0 || t->child[3]->child[0] == NULL) {
                                             break;
                                         }
                                         else if (t->child[3]->child[0]->name == "VarDecList") {
                                             t = t->child[3]->child[0];
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
                             if (t->child.size() == 0 || t->child[0] == NULL) {
                                 return ptable;
                             }
                             else {
                                 t = t->child[0];             //��ʱtΪParamDecList
                                 ptable = ParamDecList(t);
                                 //cout << ptable->param_name << endl;
                                 //cout << ptable->next->param_name << endl;
                                 return ptable;
                             }
                         }


                         //ParamList���Ӻ�������ʱ��������ParamList
                         ParamTable* ParamDecList(TreeNode* t) {
                             ParamTable* p_tail = NULL;
                             ParamTable* p_head = p_tail;
                             TreeNode* n = NULL;
                             int flag = 0;
                             while (t->name == "ParamDecList") {
                                 if (t->child[0]->child[0]->name == "TypeDef") {
                                     n = t->child[0]->child[0];
                                     flag = 1;
                                 }
                                 else {
                                     n = t->child[0]->child[1];
                                     flag = 2;
                                 }
                                 TypeIR* p_ir = TypeProcess(n, "null", -1);
                                 if (t->child[0]->child[0]->name == "VAR" || t->child[0]->child[0]->name == "TypeDef") {
                                     TreeNode* i = NULL;
                                     if (flag == 2) {
                                         i = t->child[0]->child[2];        //i is FormList
                                     }
                                     else {
                                         i = t->child[0]->child[1];        //i is FormList
                                     }
                                     while (i->name == "FormList") {
                                         string p_id = i->child[0]->token->Sem;
                                         AttributeIR* p_atr = new AttributeIR();
                                         p_atr->idtpye = p_ir;
                                         p_atr->kind = varkind;
                                         p_atr->VarOrPorc.VarAttr.level = level;
                                         p_atr->VarOrPorc.VarAttr.off = offset;
                                         offset += p_ir->size;
                                         WriteToTable(p_id, p_atr);             //�Է��ű����

                                         ParamTable* p = new ParamTable();
                                         p->location = tail;
                                         p->param_name = p_id;
                                         if (p_tail == NULL) {
                                             p_tail = p;
                                             p_head = p_tail;
                                         }
                                         else {
                                             p_tail->next = p;
                                             p_tail = p;
                                         }                                          //���ɲ�����

                                         if (i->child[1]->child.size() == 0 || i->child[1]->child[0] == NULL) {
                                             break;
                                         }
                                         else if (i->child[1]->child[1]->name == "FormList") {
                                             i = i->child[1]->child[1];
                                         }
                                     }
                                 }
                                 if (t->child[1]->child.size() == 0 || t->child[1]->child[0] == NULL) {
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
                             AttributeIR* pr_atr = new AttributeIR();
                             string proc_id = t->child[1]->child[0]->token->Sem;
                             pr_atr->idtpye = NULL;
                             pr_atr->kind = prockind;
                             WriteToTable(proc_id, pr_atr);
                             bool flag = FindSymbol(proc_id, level, entry);
                             if (flag) {
                                 entry->atribute.VarOrPorc.ProcAttr.param = ParamList(t->child[3]);
                             }
                             //cout << entry->atribute.VarOrPorc.ProcAttr.param->param_name<<endl;
                             //cout << entry->atribute.VarOrPorc.ProcAttr.param->location->atribute.VarOrPorc.VarAttr.level << endl;
                             ProcDecPart(t->child[6]);      //����ProcDecPart
                             ProcBody(t->child[7]);
                             ProcDecpart(t->child[8]);
                         }


                         //�����������ֵĺ�����t����DeclarePart
                         void DeclarePart(TreeNode* t) {
                             for (int i = 0; i <= 2; i++) {
                                 TreeNode* x = t->child[i];      //��ʱ��t�ֱ�ΪTypeDecPart��VarDecPart��ProcDecpart
                                 if (x->name == "TypeDecPart") {
                                     TypeDecPart(x);
                                 }
                                 if (x->name == "VarDecPart") {
                                     VarDecPart(x);
                                 }
                                 if (x->name == "ProcDecpart") {
                                     ProcDecpart(x);
                                 }
                             }
                         }

                         //������DeclarePart������tΪProDecPart,ע���p��Сд����ǰһ��ProcDecPart��ͬ
                         void ProcDecPart(TreeNode* t) {
                             DeclarePart(t->child[0]);
                         }

                         //������ProgramBody������tΪProcBody
                         void ProcBody(TreeNode* t) {
                             ProgramBody(t->child[0]);
                         }

                         //�������������ֵĺ���������ProcDecPart
                         void ProcDecpart(TreeNode* t) {
                             if (t->child.size() == 0 || t->child[0] == NULL) {
                                 return;
                             }
                             else if (t->child[0]->name == "ProcDec") {
                                 ProcDec(t->child[0]);
                             }
                         }




                         //�Ժ����岿�ִ���ĺ�����t����ProgramBody
                         void ProgramBody(TreeNode* t) {

                             if (t->child[1]->name == "StmList") {
                                 StmList(t->child[1]);           //��ʱtΪStmList
                             }
                             PrintOneLevelTable();
                             DropTable();
                         }


                         //t is StmList
                         void StmList(TreeNode* t) {
                             while (t->name == "StmList") {
                                 Stm(t->child[0]);
                                 if (t->child[1]->child.size() == 0 || t->child[1]->child[0] == NULL) {
                                     return;
                                 }
                                 else {
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
                             if (t->child[0]->name == "ID") {
                                 AssCall(t->child[1], t->child[0]->token->Sem, t->child[0]->token->Line);
                             }
                         }


                         //������ʽ�ĺ���
                         TypeIR* Exp(TreeNode* t) {
                             TypeIR* Eptr = NULL;
                             while (t->name == "Exp") {
                                 if (t->child[0]->name == "Term") {
                                     Eptr = Term(t->child[0]);
                                 }
                                 if (t->child[1]->child.size() == 0 || t->child[1]->child[0] == NULL) {
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
                             TypeIR* Eptr = new TypeIR();
                             while (t->name == "Term") {
                                 if (t->child[0]->child[0]->name == "INTC") {
                                     Eptr->snlkind_Ty = intTy;
                                     Eptr->size = 1;
                                 }
                                 if (t->child[0]->child[0]->name == "LPAREN") {
                                     Eptr = Exp(t->child[0]->child[1]);
                                 };
                                 if (t->child[0]->child[0]->name == "Variable") {
                                     Eptr = Varriable(t->child[0]->child[0]);
                                 }
                                 if (t->child[1]->child.size() == 0 || t->child[1]->child[0] == NULL) {
                                     break;
                                 }
                                 else if (t->child[1]->child[1]->name == "Term") {
                                     t = t->child[1]->child[1];
                                 }
                             }
                             return Eptr;
                         }


                         //t is Varriable
                         TypeIR* Varriable(TreeNode* t) {
                             TypeIR* Eptr = NULL;
                             string v_id = t->child[0]->token->Sem;
                             if (t->child[1]->child.size() == 0 || t->child[1]->child[0] == NULL) {              //VariMore is NULL
                                 bool flag = FindSymbol(v_id, level, entry);
                                 if (flag == false) {                        //δ�ҵ���ʶ��
                                     Error_8(v_id, t->child[0]->token->Line);                     //��ʱ���ֵڰ˸�����δ����ı�ʶ��
                                 }
                                 else {                                        //�ҵ���ʶ��
                                     if (entry->atribute.kind == varkind) {
                                         Eptr = entry->atribute.idtpye;
                                     }
                                     else {
                                         Error_9(entry, t->child[0]->token->Line);              //��ʱ���ֵھŸ����󣬸ñ�ʶ�����Ǳ�������
                                     }
                                 }
                             }
                             else {                              //VarriableMore is not null
                                 if (t->child[1]->child[0]->name == "DOT") {
                                     Eptr = recordVar(t->child[1], v_id, t->child[1]->child[0]->token->Line);         //t is VariMore
                                 }
                                 else {
                                     Eptr = arrayVar(t->child[1], v_id, t->child[1]->child[0]->token->Line);              //t is VariMore
                                 }
                             }
                             return Eptr;
                         }



                         //t is VariMore
                         TypeIR* arrayVar(TreeNode* t, string id, int line) {
                             bool flag = FindSymbol(id, level, entry);
                             TypeIR* Eptr = NULL;
                             if (flag == false) {
                                 Error_8(id, line);          //δ����ı�ʶ��
                             }
                             else {
                                 SymbolTable* sy = entry;
                                 if (sy->atribute.kind != varkind) {
                                     Error_9(sy, line);
                                 }
                                 else {
                                     //cout << entry->atribute.idtpye->array.elemType->snlkind_Ty<<endl;
                                     if (sy->atribute.idtpye->snlkind_Ty != arraryTy) {
                                         Error_10(id, line);            //��ʱ���ֵ�ʮ�����󣬸ñ��������������ͱ���
                                     }
                                     else {
                                         TypeIR* ty = Exp(t->child[1]);
                                         if (ty->snlkind_Ty != intTy) {
                                             Error_11(ty, line);               //��ʱ���ֵ�ʮһ����������֮��������±�����
                                         }
                                         else {
                                             Eptr = sy->atribute.idtpye->array.elemType;
                                         }
                                     }
                                 }
                             }
                             return Eptr;
                         }


                         //t is VariMore
                         TypeIR* recordVar(TreeNode* t, string id, int line) {
                             bool flag = FindSymbol(id, level, entry);
                             TypeIR* Eptr = NULL;
                             if (flag == false) {
                                 Error_8(id, line);               //δ����ı�ʶ��
                             }
                             else {
                                 if (entry->atribute.kind != varkind) {
                                     Error_9(entry, line);
                                 }
                                 else {
                                     if (entry->atribute.idtpye->snlkind_Ty != recordTy) {
                                         Error_12(id, line);     //��ʱ���ֵ�ʮ�������󣬸ñ������Ǽ�¼����
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
                                             Error_13(id, t->child[1]->child[0]->token->Sem, line);            //��ʱ���ֵ�ʮ�������󣬼�¼�����޸ñ���
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
                         void AssCall(TreeNode* t, string id, int line) {
                             if (t->child[0]->name == "AssignmentRest") {
                                 AssignmentRest(t->child[0], id, line);
                             }
                             if (t->child[0]->name == "CallStmRest") {
                                 CallStmRest(t->child[0], id, line);
                             }
                         }


                         //t is AssignmentRest
                         void AssignmentRest(TreeNode* t, string id, int line) {

                             TypeIR* Eptrleft = new TypeIR();
                             TypeIR* Eptrright = new TypeIR();
                             bool flag = FindSymbol(id, level, entry);
                             if (flag == false) {             //��ʾδ�ҵ����ʶ��
                                 Error_8(id, line);
                             }

                             else {
                                 if (entry->atribute.kind != varkind) {         //���ʶ�����Ǳ���
                                     Error_9(entry, line);
                                 }
                                 else {
                                     if (t->child[0]->child.size() == 0 || t->child[0]->child[0] == NULL) {          //���ʶ���ǻ�������
                                         Eptrleft = entry->atribute.idtpye;
                                     }
                                     else {                       //���ʶ����������߼�¼����
                                         if (t->child[0]->child[0]->name == "DOT") {
                                             Eptrleft = recordVar(t->child[0], id, line);
                                         }
                                         if (t->child[0]->child[0]->name == "LMIDPAREN") {
                                             Eptrleft = arrayVar(t->child[0], id, line);
                                         }
                                     }
                                 }
                             }
                             //�����ұ�ʶ��
                             Eptrright = Exp(t->child[2]);
                             //�ж��Ƿ�����
                         
                             if (Eptrleft->snlkind_Ty != Eptrright->snlkind_Ty) {
                                 //cout << Eptrleft->snlkind_Ty << endl;
                                 Error_14(line);
                             }
                             else {
                                 return;
                             }
                         }


                         //t is CallStmRest
                         void  CallStmRest(TreeNode* t, string id, int line) {
                             bool flag = FindSymbol(id, level, entry);
                             ParamTable* p = entry->atribute.VarOrPorc.ProcAttr.param;
                             if (!flag) {
                                 Error_8(id, line);
                             }
                             else {
                                 if (entry->atribute.kind != prockind) {
                                     Error_15(entry, line);               //��ʱ���ֵ�ʮ������󣬸ñ������Ǻ�������
                                 }
                                 else {
                                     TreeNode* i = t->child[1];
                                     while (i->name == "ActparamList") {
                                         if (i->child.size() == 0 || i->child[0] == NULL) {
                                             break;
                                         }
                                         else {
                                             TypeIR* Aptr = Exp(i->child[0]);
                                             TypeIR* Eptr = p->location->atribute.idtpye;
                                             //cout << p->param_name << endl;
                                             //p = p->next;
                                             //cout << p->param_name << endl;
                                             //p = p->next;
                                             //if (p == NULL) {
                                             //    cout << "NULL" << endl;
                                             //}

                                             if (Aptr->snlkind_Ty != Eptr->snlkind_Ty) {
                                                 Error_16(Aptr, Eptr, line);             //��ʱ���ֵ�ʮ���������β���ʵ�����Ͳ�ƥ��
                                             }
                                             i = i->child[1];
                                             p = p->next;               //�ƶ��βκ�ʵ�ε�λ��
                                             if ((i->child.size() == 0 || i->child[0] == NULL) && p != NULL) {
                                                 Error_17(id, line);         //��ʱ���ֵ�ʮ�߸������β���ʵ�θ�����ƥ��
                                             }
                                             if ((i->child.size() != 0 && i->child[0] != NULL) && p == NULL) {
                                                 Error_17(id, line);
                                             }
                                             if ((i->child.size() == 0 || i->child[0] == NULL) && p == NULL) {
                                                 return;
                                             }
                                         }
                                         if (i->child[1]->name == "ActparamList") {
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
                             int line = t->child[1]->child[0]->child[0]->token->Line;
                             if (t->child[1]->child[0]->child[0]->name != "LT" && t->child[1]->child[0]->child[0]->name != "EQ") {
                                 Error_18(line);              //��ʱ���ֵ�ʮ�˸������������ʽ��Ϊbool����
                                 return false;
                             }
                             if (Eptrl->snlkind_Ty == arraryTy || Eptrl->snlkind_Ty == recordTy) {
                                 Error_18(line);
                                 return false;
                             }
                             if (Eptrr->snlkind_Ty == arraryTy || Eptrr->snlkind_Ty == recordTy) {
                                 Error_18(line);
                                 return false;
                             }
                             if (Eptrl->snlkind_Ty != Eptrr->snlkind_Ty) {
                                 Error_14(t->child[1]->child[0]->child[0]->token->Line);
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
                             if (flag == false) {
                                 Error_8(id, t->child[2]->child[0]->token->Line);
                             }
                             else {
                                 if (entry->atribute.kind != varkind) {
                                     Error_9(entry, t->child[2]->child[0]->token->Line);
                                 }
                             }
                             return;
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
                         //ʵ����������������﷨���󣬲�����������������
                         void Error_1() {
                             printf("����֮��Ļ�������");
                             yuyi.push_back("����֮��Ļ�������\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size()-1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_2(string id, int l) {
                             cout << "����:" << id << "�±�Խ��" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("����:" + id + "�±�Խ��" + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_3() {
                             printf("����֮��ĸ�������");
                             yuyi.push_back("����֮��ĸ�������\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_4(string id, int l) {
                             cout << "δ���������:" << id << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("δ���������:" + id + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_5(string id, int l) {
                             cout << "��ʶ��:" << id << "�����Զ�������" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("��ʶ��:" + id + "�����Զ�������"+"\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_6(string id, int l) {
                             cout << "�ظ�����������:" << id << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("�ظ�����������:" + id + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_7(SymbolTable* t, int l) {
                             cout << "�ظ������ı���:" << t->symbolid << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("�ظ������ı���:" + t->symbolid + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_8(string id, int l) {
                             cout << "δ����ı�ʶ��:" << id << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("δ����ı�ʶ��:" + id + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_9(SymbolTable* t, int l) {
                             cout << "�ñ�ʶ�����Ǳ�������:" << t->symbolid << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("�ñ�ʶ�����Ǳ�������:" + t->symbolid+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_10(string id, int l) {
                             cout << "����" << id << "������������" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("����" + id +"������������"+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_11(TypeIR* Ty, int l) {
                             cout << "����֮��������±�����:" << TypeKind[Ty->snlkind_Ty] << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("����֮��������±�����:" + TypeKind[Ty->snlkind_Ty] + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_12(string id, int l) {
                             cout << "����:" << id << "���Ǽ�¼����" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("����" + id+"���Ǽ�¼����"+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_13(string r_id, string var_id, int l) {
                             cout << "��¼��:" << r_id << "���޸ñ���:" << var_id << endl;
                             cout << "error location is:" << l << endl;

                             yuyi.push_back("��¼��:" + r_id + "���޸ñ���:"  +var_id+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_14(int l) {
                             printf("���ʽ�������Ͳ�ƥ��\n");
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("���ʽ�������Ͳ�ƥ��  error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_15(SymbolTable* t, int l) {
                             cout << "�ñ�ʶ�����Ǻ�������:" << t->symbolid << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("�ñ�ʶ�����Ǻ�������: " + t->symbolid + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_16(TypeIR* A, TypeIR* E, int l) {
                             printf("�β���ʵ�����Ͳ�ƥ��\n");
                             cout << "�βε�����Ϊ��" << TypeKind[E->snlkind_Ty] << "  " << "ʵ�ε�����Ϊ��" << TypeKind[A->snlkind_Ty] << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("�β���ʵ�����Ͳ�ƥ��  �βε�����Ϊ��" + TypeKind[E->snlkind_Ty] + "ʵ�ε�����Ϊ��"+ TypeKind[A->snlkind_Ty] +"\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_17(string id, int l) {
                             cout << "����:" << id << " " << "�β���ʵ�θ�����ƥ��" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("����:" + id + " �β���ʵ�θ�����ƥ��"+ "\n" + "error location is : " + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_18(int l) {
                             cout << "������¼���Ͳ��������������ʽ�Ƚ�" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("������¼���Ͳ��������������ʽ�Ƚ�\n error location is:"+ to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }








                         //���������������Ӧ�ý��﷨���ĸ��ڵ���Ϊ��������
public:
    void SematicAnalyze(TreeNode* Program) {
        TreeNode* t = Program;
        CreateTable();
        // Initialize();
        DeclarePart(t->child[1]);
        ProgramBody(t->child[2]);
        printf("����������\n");
        if (error_flag == 0)
        {
            yuyi.push_back("����������\n");
            stringstream ss;
            ss << yuyi[yuyi.size() - 1];
            outfile_yuyi << ss.str();
        }
            
        DropTable();
    }



    //void init()
    //{
    //    string s = readFileIntoString("c1.txt");

    //    //�õ�token����
    //    token = getToken(s);
    //    // cout<<token.size()<<endl;
    //    for (int i = 0; i < token.size(); i++)
    //    {
    //        cout << token[i].Line << "\t" << token[i].Lex << "\t" << token[i].Sem << endl;
    //    }


    //    if (error_flag)return;//����ʷ�����������������﷨��������������һ���������⣬��ֱ�ӽ�����
    //    now_token = &token[token_index++];

    //    TreeNode* root = Program();//��ʼ�﷨�������������﷨������
    //    //SematicAnalyze(root);
    //    printTree(root, 0, false);

    //    //   if(error_flag)return;

    //}


    //void test() {
    //    string s = readFileIntoString("c5.txt");

    //    //�õ�token����
    //    token = getToken(s);
    //    if (error_flag)return;//����ʷ�����������������﷨��������������һ���������⣬��ֱ�ӽ�����
    //    now_token = &token[token_index++];
    //    TreeNode* root = Program();
    //    printTree(root, 0, false);
    //    CreateTable();
    //    DeclarePart(root->child[1]);

    //    ProgramBody(root->child[2]);
    //}
    string outstr[5000];
    char out[100];
    int strline = 0;
    char ch1[3] = "��"; // ȫ���ַ���������֦ ch1[0] ch1[1] �������ַ� ch1[2]��\0
    char ch2[3] = "��"; // ȫ���ַ���������ĺ����׳���...
    char ch3[3] = "��";

    void printTree_node()
    {
        //���ַ�����״���е���
        cout << "-------------------------------------------------------------------------------" << endl;
        for (int i = 1; i < strline; i++)
        {
            int j = 0;
            while (outstr[i][j] != '\n')
            {
                if (outstr[i - 1][j] == '\n')
                    break;
                if (outstr[i - 1][j] == ch2[0] && outstr[i - 1][j + 1] == ch2[1]) // �������Ҫ ���ӵĻ��������
                {
                    ;// ��һ����"��"
                }
                else if (outstr[i - 1][j] == ch1[0] && outstr[i - 1][j + 1] == ch1[1] && outstr[i][j] == ' ')
                {
                    // ��һ����"��" ��һ���ǿո�
                    outstr[i][j] = ch3[0];
                    outstr[i][j + 1] = ch3[1];
                }
                else if (outstr[i - 1][j] == ch3[0] && outstr[i][j] == ' ')
                {
                    // ��һ����"��" ��һ���ǿո�
                    outstr[i][j] = ch3[0];
                    outstr[i][j + 1] = ch3[1];
                }
                j++;
            }
        }

        //����ӡ�﷨��
        for (int i = 0; i < strline; i++)
        {
            files.push_back(outstr[i]);
            cout << outstr[i];
        }
        return;
    }
    void printTree(TreeNode* t, int layer, bool flag)
    {


        if (t == NULL)return;//�ݹ����
        //�ҵ����һ���ӽڵ���±�
        //cout<<strline<<"  "<<t->name<<"            "<<"layer  "<<layer<<"      child_size : "<<t->child.size()<<endl;
        int last = -1;
        for (int i = 0; i < (t->child.size()); i++)
        {
            if (t->child[i] != NULL)last = i;
        }
        // ���û�ж��� last == -1
        // ���� last ��¼���һ�����ӽڵ��index
    //       if(layer==15) cout<<"last : "<<last<<endl;

        for (int i = 0; i < layer; i++) //���ݲ�����ӡ�ո�
        {
            outstr[strline] += "   ";
        }

        if (layer == 0)
        {
            outstr[strline] += "   ";
            outstr[strline] += t->name;
        }
        else
        {

            if (flag == true)
            {
                outstr[strline] += "�� ";
                outstr[strline] += t->name;
            }
            else// ���Ǹ��ڵ�����һ�����ӽڵ�
            {
                outstr[strline] += "�� ";
                outstr[strline] += t->name;
                //cout << "�� " << t->name;
            }
        }

        outstr[strline] += "\n"; //��ӻ��з���
        strline++;

        for (int i = 0; i < t->child.size(); i++)//�ݹ�
        {
            if (i != last)// ��Ϊ���ǿ��ӽڵ� ��ô�����������ͱ��false
            {
                printTree(t->child[i], layer + 1, false);

            }
            else
            {
                printTree(t->child[i], layer + 1, true);

            }
        }
        //ȫ���ݹ�Ҫ����ʱ��ʼ��ӡ�﷨��
       // cout<<"my_layer : "<<layer<<endl;
        if (layer == 0)
        {
            printTree_node();
        }
        return;
    }


    int TestDigui() {
        string s = readFileIntoString(file_name);
        ofstream outfile_cifa;
        outfile_cifa.open(file_name.substr(0, file_name.size() - 4) + "_cifa.txt", ios_base::out | ios_base::trunc);  //ɾ���ļ���д
        ofstream outfile_digui;
        outfile_digui.open(file_name.substr(0, file_name.size() - 4) + "_yufa.txt", ios_base::out | ios_base::trunc);  //ɾ���ļ���д
        ofstream outfile_tree;
        outfile_tree.open(file_name.substr(0, file_name.size() - 4) + "_tree.txt", ios_base::out | ios_base::trunc);  //ɾ���ļ���д
        

        syntax_analysis sa;
        vector<string>cifa;
        sa.token = getToken(s,cifa);
        
        for (int i = 0; i < sa.token.size(); i++) {
            cout << sa.token[i].Line << "\t" << sa.token[i].Lex << "\t\t" << sa.token[i].Sem << endl;
            cifa.push_back(to_string(sa.token[i].Line) + "\t" + sa.token[i].Lex + "\t\t" + sa.token[i].Sem + "\n");
        }
        //�ʷ���������д�ļ�  
        
        for (int i = 0; i < cifa.size(); i++)
        {
            stringstream ss;
            ss << cifa[i];
            outfile_cifa << ss.str();
        }
        outfile_cifa.close();
        if (error_flag)return -1;




        sa.now_token = &sa.token[sa.token_index++];
        TreeNode* root = sa.Program();
        //�﷨��������д�ļ�  
        
        for (int i = 0; i < sa.digui.size(); i++)
        {
            stringstream ss;
            ss << sa.digui[i];
            outfile_digui << ss.str();
        }
        
        if (error_flag)
        {
            cout << "�ݹ��½��﷨������������ֹͣ���к�������" << endl;
            stringstream ss;
            ss << "�ݹ��½��﷨������������ֹͣ���к�������\n";
            outfile_digui << ss.str();
            outfile_digui.close();
            return 0;
        }
        outfile_digui.close();
    
        //�﷨������
        printTree(root, 0, false);


        for (int i = 0; i < files.size(); i++)
        {
            stringstream ss;
            ss << files[i];
            outfile_tree << ss.str();
        }
        outfile_tree.close();


        SematicAnalyze(root);
        outfile_yuyi.close();




    }
    int TestLL1() {
        string s = readFileIntoString(file_name);
        ofstream outfile_cifa;
        outfile_cifa.open(file_name.substr(0, file_name.size() - 4) + "_cifa.txt", ios_base::out | ios_base::trunc);  //ɾ���ļ���д
        ofstream outfile_LL1;
        outfile_LL1.open(file_name.substr(0, file_name.size() - 4) + "_yufa.txt", ios_base::out | ios_base::trunc);  //ɾ���ļ���д
        ofstream outfile_tree;
        outfile_tree.open(file_name.substr(0, file_name.size() - 4) + "_tree.txt", ios_base::out | ios_base::trunc);  //ɾ���ļ���д
  


        vector<string>cifa;
        token = getToken(s,cifa);
        for (int i = 0; i < token.size(); i++) {
            cout << token[i].Line << "\t" << token[i].Lex << "\t\t" << token[i].Sem << endl;
            cifa.push_back(to_string(token[i].Line) + "\t" + token[i].Lex + "\t\t" + token[i].Sem + "\n");
        }
     
        //�ʷ���������д�ļ�  
        
        for (int i = 0; i < cifa.size(); i++)
        {
            stringstream ss;
            ss << cifa[i];
            outfile_cifa << ss.str();
        }
        outfile_cifa.close();
        if (error_flag)return -1;




        TreeNode* root = LL1();
        //�﷨��������д�ļ�  
        
        for (int i = 0; i < ll1_file.size(); i++)
        {
            stringstream ss;
            ss << ll1_file[i];
            outfile_LL1 << ss.str();
        }
        //  cout<<"yes"<<endl;
        if (root == NULL) {
            cout << "LL1�﷨������������ֹͣ���к�������" << endl;
            stringstream ss;
            ss << "LL1�﷨������������ֹͣ���к�������\n";
            outfile_LL1 << ss.str();
            return -1;
        }
        outfile_LL1.close();
        //�﷨������
        printTree(root, 0, false);
       
        for (int i = 0; i < files.size(); i++)
        {
            stringstream ss;
            ss << files[i];
            outfile_tree << ss.str();
        }
        outfile_tree.close();



        //�����������
        SematicAnalyze(root);

        
        this->outfile_yuyi.close();




        return 0;
    }

};







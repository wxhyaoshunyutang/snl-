// Compilation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
        outfile_yuyi.open(file_name.substr(0, file_name.size() - 4) + "_yuyi.txt", ios_base::out | ios_base::trunc);  //删除文件重写
        this->file_name = file_name;
    }
     ofstream outfile_yuyi;
    vector<string>yuyi;
    string file_name;
    int level = -1;            //scope栈的下标，表示当前层数,第一次进入局部化单位时会将index置为0
    int offset;           //当前层数的偏移量
    struct TypeIR;
    struct SymbolTable;
    vector<string> files;


    enum snl_TypeKind { intTy, charTy, arraryTy, recordTy, boolTy };
    enum AccessKind { dir, indir };
    enum IdKind { typekind, varkind, prockind };
    enum Function_Class { actua, formal };
    struct intPtr { int size = 1; snl_TypeKind intTy; };//无实际用途
    struct charPtr { int size = 1; snl_TypeKind cahrTy; };//无实际用途
    struct arryPtr { int size; snl_TypeKind arryayTy; int up; int low;  snl_TypeKind elemTy; };//无实际用途
class Fieldchain { public: string idname; TypeIR* unitType; int offset; Fieldchain* next = NULL; }; //记录域表，记录结构体的body域，这个结构体是有实际用途的
                         struct recordPtr { int size; snl_TypeKind recordTy; Fieldchain* body; };  //单表结构中无实际用途
                         vector<string> TypeKind = { "intTy","charTy","arrayTy","recordTy","boolTy" };
                         vector<string> IdKind_vect = { "typekind","varkind","prockind" };
class ParamTable { public: string param_name; SymbolTable* location; ParamTable* next = NULL; };//参数表
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
                             SymbolTable* p = Scope[level];
                             while (p->next != NULL) {
                                 p = p->next;
                             }
                             tail = p;
                         }



                         bool FindSymbol();

                         //新建一个表项并写入内容，参数应该传入tail
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


                         //在当前层查表，是FindSymbol的子函数，此时Entry应该传入entry
                         bool SearchoneTable(string id, int currentlevel, SymbolTable* Entry) {
                             SymbolTable* p = Scope[currentlevel];
                             entry = NULL;        //先将entry置空
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


                         //递归地向上查表
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
                         void PrintOneLevelTable() {
                             SymbolTable* p = Scope[level];
                             cout << "当前层数：" << level << endl;
                             yuyi.push_back("当前层数：" + to_string(level)+"\n");
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



                         //以上是对符号表的操作函数




                         //初始化intPtr，charPtr，boolPtr,此函数是否正确存疑，按照无用处理
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
                         //返回一个intPtr指针，指向int内部表示
                         TypeIR* IntPtr() {
                             TypeIR* intPtr = new TypeIR();
                             intPtr->snlkind_Ty = intTy;
                             intPtr->size = 1;
                             return intPtr;
                         }
                         //返回一个charPtr指针，指向char内部表示
                         TypeIR* CharPtr() {
                             TypeIR* charPtr = new TypeIR();
                             charPtr->snlkind_Ty = charTy;
                             charPtr->size = 1;
                             return charPtr;
                         }
                         //返回一个boolPtr指针，指向bool内部表示
                         /*TypeIR* BoolPtr() {
                             TypeIR* boolPtr;
                             boolPtr->snlkind_Ty = boolTy;
                             boolPtr->size = 1;
                             return boolPtr;
                         }*/
                         //基础类型处理函数，调用IntPtr()和CharPtr()
                         TypeIR* BaseType(TreeNode* t) {
                             if (t->child[0]->name == "INTEGER") {
                                 return IntPtr();
                             }
                             if (t->child[0]->name == "CHAR") {
                                 return CharPtr();
                             }
                             else {
                                 Error_1();         //此处出现第一个错误，期望之外的基类型，此错误不是语义检查的错误，无意义
                             }
                         }
                         //检查数组并且返回数组类型指针,此时t应该传入ArryType
                         TypeIR* ArrayPtr(TreeNode* t, string id, int line) {
                             int low = atoi(t->child[2]->child[0]->token->Sem.c_str());
                             int up = atoi(t->child[4]->child[0]->token->Sem.c_str());
                             if (low > up) {
                                 Error_2(id, line);            //此时出现第二个错误，数组下标越界
                             }
                             TypeIR* elemPtr = BaseType(t->child[7]);     //处理元素类型
                             TypeIR* arrayPtr = new TypeIR();
                             arrayPtr->size = up - low;
                             arrayPtr->snlkind_Ty = arraryTy;
                             arrayPtr->array.elemType = elemPtr;
                             arrayPtr->array.low = low;
                             arrayPtr->array.up = up;
                             return arrayPtr;
                         }
                         //检查记录域并且返回记录类型指针，此时t应传入RecType
                         TypeIR* RecPtr(TreeNode* t, string id) {
                             TypeIR* recPtr = new TypeIR();
                             recPtr->snlkind_Ty = recordTy;
                             t = t->child[1];     //将t移动到FiledDecList
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
                         //该函数是RecPtr的子函数，被RecPtr递归调用处理FieldDecList节点,t应该传入FieldDecList
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
                                 }                                     //处理child[0],UnitType
                                 if (t->child[1]->name == "IDList") {
                                     TreeNode* i = t->child[1];         // i指向IdList
                                     while (i->name == "IDList") {
                                         string id = i->child[0]->token->Sem;
                                         Fieldchain* fchaintool = new Fieldchain();   //当碰到一个id,生成一个新表项
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
                                         }            //循环处理IdList节点
                                     }
                                 }
                                 if (t->child[3]->child.size() == 0 || t->child[3]->child[0] == NULL) {
                                     break;
                                 }
                                 else if (t->child[3]->child[0]->name == "FieldDecList") {
                                     t = t->child[3]->child[0];
                                 }            //循环处理FieldDecList节点
                             }
                             return fchainhead;
                         }

                         //处理复合类型的函数，将节点StructrueTyoe传入函数
                         TypeIR* StructrueType(TreeNode* t, string id, int line) {
                             if (t->child[0]->name == "ArrayType") {
                                 return ArrayPtr(t->child[0], id, line);
                             }
                             if (t->child[0]->name == "RecType") {
                                 return RecPtr(t->child[0], id);
                             }
                             else {
                                 Error_3();       //此处出现第三个错误，期望之外的复合类型，同样无意义
                             }
                         }


                         //处理自定义类型的函数，应将ID作为参数传入
                         TypeIR* IDType(TreeNode* t, int line) {
                             string id_name = t->token->Sem;
                             //SymbolTable* e;
                             bool flag = FindSymbol(id_name, level, entry);
                             if (flag == false) {
                                 Error_4(id_name, line);       //此时出现第四个错误，未定义的类型
                             }
                             else {
                                 if (entry->atribute.kind != typekind) {
                                     Error_5(id_name, line);            //此时出现的五个错误，该标识符不是自定义类型
                                 }
                                 else {
                                     TypeIR* idPtr = entry->atribute.idtpye;
                                     return idPtr;
                                 }
                             }
                         }






                         //构造类型的内部表示，此时节点t应该传入TypeDef
                         TypeIR* TypeProcess(TreeNode* t, string id, int line) {
                             if (t->child[0]->name == "BaseType") {
                                 return BaseType(t->child[0]);   //调用基类型处理函数，将节点BaseType传入函数
                             }
                             if (t->child[0]->name == "StructureType") {
                                 return StructrueType(t->child[0], id, line);     //调用复合类型处理函数，将节点StructrueType传入函数
                             }
                             if (t->child[0]->name == "ID") {
                                 return  IDType(t->child[0], line);    //调用将ID传入函数

                             }
                         }


                         //处理声明部分的函数,此时将TypeDecPart传入
                         void TypeDecPart(TreeNode* t) {
                             if (t->child.size() == 0 || (t->child[0] == NULL)) {
                                 return;
                             }
                             else {
                                 t = t->child[0];      //此时t为TypeDec
                                 t = t->child[1];      //此时t为TypeDecList
                                 while (t->name == "TypeDecList") {
                                     string ty_id = t->child[0]->child[0]->token->Sem;
                                     bool flag = FindSymbol(ty_id, level, entry);
                                     if (flag == true) {                             
                                         Error_6(ty_id, t->child[0]->child[0]->token->Line);        //此时出现第六个错误，重复声明的类型
                                     }
                                     else {
                                         AttributeIR* air = new AttributeIR();
                                         air->kind = typekind;
                                         TypeIR* ty_tpir = TypeProcess(t->child[2], ty_id, t->child[0]->child[0]->token->Line);  //将TypeDef传入
                                         air->idtpye = ty_tpir;
                                         //VarOrProc不做处理
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



                         //以上已经将TypeDecPart部分写完



                         //VarDecPart部分



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
                                         TypeIR* v_ir = TypeProcess(t->child[0], "null", -1);       //param is TypeDef 此函数参数id与line传入什么无意义，因为错误处理如果在TypeDec部分出现就已经直接处理
                                         TreeNode* i = t->child[1];      //i is VarIdList
                                         if (i->name == "VarIDList") {
                                             while (i->name == "VarIDList") {
                                                 string v_id = i->child[0]->token->Sem;
                                                 //cout << v_id << endl;
                                                 bool flag = SearchoneTable(v_id, level, entry);
                                                 if (flag == true) {
                                                     int line = i->child[0]->token->Line;
                                                     Error_7(entry, line);         //此时出现第七个错误，重复声明的变量
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

                         //以上是对变量声明的所有处理


                         //以下处理过程声明

                         //参数处理函数，此时参数传入ParamList节点
                         ParamTable* ParamList(TreeNode* t) {
                             CreateTable();           //表示新进入一个局部化区域，level++;
                             ParamTable* ptable = NULL;
                             if (t->child.size() == 0 || t->child[0] == NULL) {
                                 return ptable;
                             }
                             else {
                                 t = t->child[0];             //此时t为ParamDecList
                                 ptable = ParamDecList(t);
                                 //cout << ptable->param_name << endl;
                                 //cout << ptable->next->param_name << endl;
                                 return ptable;
                             }
                         }


                         //ParamList的子函数，此时参数传入ParamList
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
                                         WriteToTable(p_id, p_atr);             //对符号表操作

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
                                         }                                          //生成参数表

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


                         //处理函数本身的符号表项的函数，t应传入ProcDec,因为需要连接参数表，因此该函数调用ParamList函数
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
                             ProcDecPart(t->child[6]);      //传入ProcDecPart
                             ProcBody(t->child[7]);
                             ProcDecpart(t->child[8]);
                         }


                         //处理声明部分的函数，t传入DeclarePart
                         void DeclarePart(TreeNode* t) {
                             for (int i = 0; i <= 2; i++) {
                                 TreeNode* x = t->child[i];      //此时的t分别为TypeDecPart，VarDecPart，ProcDecpart
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

                         //仅调用DeclarePart函数，t为ProDecPart,注意该p是小写，与前一个ProcDecPart不同
                         void ProcDecPart(TreeNode* t) {
                             DeclarePart(t->child[0]);
                         }

                         //仅调用ProgramBody函数，t为ProcBody
                         void ProcBody(TreeNode* t) {
                             ProgramBody(t->child[0]);
                         }

                         //处理函数声明部分的函数，传入ProcDecPart
                         void ProcDecpart(TreeNode* t) {
                             if (t->child.size() == 0 || t->child[0] == NULL) {
                                 return;
                             }
                             else if (t->child[0]->name == "ProcDec") {
                                 ProcDec(t->child[0]);
                             }
                         }




                         //对函数体部分处理的函数，t传入ProgramBody
                         void ProgramBody(TreeNode* t) {

                             if (t->child[1]->name == "StmList") {
                                 StmList(t->child[1]);           //此时t为StmList
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


                         //处理表达式的函数
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
                                 if (flag == false) {                        //未找到标识符
                                     Error_8(v_id, t->child[0]->token->Line);                     //此时出现第八个错误，未定义的标识符
                                 }
                                 else {                                        //找到标识符
                                     if (entry->atribute.kind == varkind) {
                                         Eptr = entry->atribute.idtpye;
                                     }
                                     else {
                                         Error_9(entry, t->child[0]->token->Line);              //此时出现第九个错误，该标识符不是变量类型
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
                                 Error_8(id, line);          //未定义的标识符
                             }
                             else {
                                 SymbolTable* sy = entry;
                                 if (sy->atribute.kind != varkind) {
                                     Error_9(sy, line);
                                 }
                                 else {
                                     //cout << entry->atribute.idtpye->array.elemType->snlkind_Ty<<endl;
                                     if (sy->atribute.idtpye->snlkind_Ty != arraryTy) {
                                         Error_10(id, line);            //此时出现第十个错误，该变量不是数组类型变量
                                     }
                                     else {
                                         TypeIR* ty = Exp(t->child[1]);
                                         if (ty->snlkind_Ty != intTy) {
                                             Error_11(ty, line);               //此时出现第十一个错误，期望之外的数组下标类型
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
                                 Error_8(id, line);               //未定义的标识符
                             }
                             else {
                                 if (entry->atribute.kind != varkind) {
                                     Error_9(entry, line);
                                 }
                                 else {
                                     if (entry->atribute.idtpye->snlkind_Ty != recordTy) {
                                         Error_12(id, line);     //此时出现第十二个错误，该变量不是记录类型
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
                                             Error_13(id, t->child[1]->child[0]->token->Sem, line);            //此时出现第十三个错误，记录域中无该变量
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
                             if (flag == false) {             //表示未找到左标识符
                                 Error_8(id, line);
                             }

                             else {
                                 if (entry->atribute.kind != varkind) {         //左标识符不是变量
                                     Error_9(entry, line);
                                 }
                                 else {
                                     if (t->child[0]->child.size() == 0 || t->child[0]->child[0] == NULL) {          //左标识符是基础类型
                                         Eptrleft = entry->atribute.idtpye;
                                     }
                                     else {                       //左标识符是数组或者记录类型
                                         if (t->child[0]->child[0]->name == "DOT") {
                                             Eptrleft = recordVar(t->child[0], id, line);
                                         }
                                         if (t->child[0]->child[0]->name == "LMIDPAREN") {
                                             Eptrleft = arrayVar(t->child[0], id, line);
                                         }
                                     }
                                 }
                             }
                             //操作右标识符
                             Eptrright = Exp(t->child[2]);
                             //判断是否相容
                         
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
                                     Error_15(entry, line);               //此时出现第十五个错误，该变量不是函数类型
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
                                                 Error_16(Aptr, Eptr, line);             //此时出现第十六个错误，形参与实参类型不匹配
                                             }
                                             i = i->child[1];
                                             p = p->next;               //移动形参和实参的位置
                                             if ((i->child.size() == 0 || i->child[0] == NULL) && p != NULL) {
                                                 Error_17(id, line);         //此时出现第十七个错误，形参与实参个数不匹配
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
                                 Error_18(line);              //此时出现第十八个错误，条件表达式不为bool类型
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




                         //错误声明函数
                         //实际上这个错误属于语法错误，不会出现在语义分析中
                         void Error_1() {
                             printf("期望之外的基础类型");
                             yuyi.push_back("期望之外的基础类型\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size()-1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_2(string id, int l) {
                             cout << "数组:" << id << "下标越界" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("数组:" + id + "下标越界" + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_3() {
                             printf("期望之外的复合类型");
                             yuyi.push_back("期望之外的复合类型\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_4(string id, int l) {
                             cout << "未定义的类型:" << id << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("未定义的类型:" + id + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_5(string id, int l) {
                             cout << "标识符:" << id << "不是自定义类型" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("标识符:" + id + "不是自定义类型"+"\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_6(string id, int l) {
                             cout << "重复声明的类型:" << id << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("重复声明的类型:" + id + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_7(SymbolTable* t, int l) {
                             cout << "重复声明的变量:" << t->symbolid << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("重复声明的变量:" + t->symbolid + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_8(string id, int l) {
                             cout << "未定义的标识符:" << id << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("未定义的标识符:" + id + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_9(SymbolTable* t, int l) {
                             cout << "该标识符不是变量类型:" << t->symbolid << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("该标识符不是变量类型:" + t->symbolid+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_10(string id, int l) {
                             cout << "变量" << id << "不是数组类型" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("变量" + id +"不是数组类型"+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_11(TypeIR* Ty, int l) {
                             cout << "期望之外的数组下标类型:" << TypeKind[Ty->snlkind_Ty] << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("期望之外的数组下标类型:" + TypeKind[Ty->snlkind_Ty] + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_12(string id, int l) {
                             cout << "变量:" << id << "不是记录类型" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("变量" + id+"不是记录类型"+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_13(string r_id, string var_id, int l) {
                             cout << "记录域:" << r_id << "中无该变量:" << var_id << endl;
                             cout << "error location is:" << l << endl;

                             yuyi.push_back("记录域:" + r_id + "中无该变量:"  +var_id+ "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_14(int l) {
                             printf("表达式左右类型不匹配\n");
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("表达式左右类型不匹配  error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_15(SymbolTable* t, int l) {
                             cout << "该标识符不是函数类型:" << t->symbolid << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("该标识符不是函数类型: " + t->symbolid + "\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_16(TypeIR* A, TypeIR* E, int l) {
                             printf("形参与实参类型不匹配\n");
                             cout << "形参的类型为：" << TypeKind[E->snlkind_Ty] << "  " << "实参的类型为：" << TypeKind[A->snlkind_Ty] << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("形参与实参类型不匹配  形参的类型为：" + TypeKind[E->snlkind_Ty] + "实参的类型为："+ TypeKind[A->snlkind_Ty] +"\n" + "error location is:" + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_17(string id, int l) {
                             cout << "函数:" << id << " " << "形参与实参个数不匹配" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("函数:" + id + " 形参与实参个数不匹配"+ "\n" + "error location is : " + to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }
                         void Error_18(int l) {
                             cout << "数组或记录类型不能用于条件表达式比较" << endl;
                             cout << "error location is:" << l << endl;
                             yuyi.push_back("数组或记录类型不能用于条件表达式比较\n error location is:"+ to_string(l) + "\n");
                             stringstream ss;
                             ss << yuyi[yuyi.size() - 1];
                             outfile_yuyi << ss.str();
                             outfile_yuyi.close();
                             system("pause");
                             exit(0);
                         }








                         //语义分析的主程序，应该将语法树的根节点作为参数传入
public:
    void SematicAnalyze(TreeNode* Program) {
        TreeNode* t = Program;
        CreateTable();
        // Initialize();
        DeclarePart(t->child[1]);
        ProgramBody(t->child[2]);
        printf("语义分析完成\n");
        if (error_flag == 0)
        {
            yuyi.push_back("语义分析完成\n");
            stringstream ss;
            ss << yuyi[yuyi.size() - 1];
            outfile_yuyi << ss.str();
        }
            
        DropTable();
    }



    //void init()
    //{
    //    string s = readFileIntoString("c1.txt");

    //    //得到token序列
    //    token = getToken(s);
    //    // cout<<token.size()<<endl;
    //    for (int i = 0; i < token.size(); i++)
    //    {
    //        cout << token[i].Line << "\t" << token[i].Lex << "\t" << token[i].Sem << endl;
    //    }


    //    if (error_flag)return;//如果词法分析，语义分析，语法分析，三种任意一个出现问题，就直接结束。
    //    now_token = &token[token_index++];

    //    TreeNode* root = Program();//开始语法分析，并建立语法分析树
    //    //SematicAnalyze(root);
    //    printTree(root, 0, false);

    //    //   if(error_flag)return;

    //}


    //void test() {
    //    string s = readFileIntoString("c5.txt");

    //    //得到token序列
    //    token = getToken(s);
    //    if (error_flag)return;//如果词法分析，语义分析，语法分析，三种任意一个出现问题，就直接结束。
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
    char ch1[3] = "├"; // 全角字符用来画树枝 ch1[0] ch1[1] 保存了字符 ch1[2]是\0
    char ch2[3] = "└"; // 全角字符用起来真的很容易出错...
    char ch3[3] = "│";

    void printTree_node()
    {
        //对字符串形状进行调整
        cout << "-------------------------------------------------------------------------------" << endl;
        for (int i = 1; i < strline; i++)
        {
            int j = 0;
            while (outstr[i][j] != '\n')
            {
                if (outstr[i - 1][j] == '\n')
                    break;
                if (outstr[i - 1][j] == ch2[0] && outstr[i - 1][j + 1] == ch2[1]) // 这个很重要 不加的话会出问题
                {
                    ;// 上一行是"└"
                }
                else if (outstr[i - 1][j] == ch1[0] && outstr[i - 1][j + 1] == ch1[1] && outstr[i][j] == ' ')
                {
                    // 上一行是"├" 这一行是空格
                    outstr[i][j] = ch3[0];
                    outstr[i][j + 1] = ch3[1];
                }
                else if (outstr[i - 1][j] == ch3[0] && outstr[i][j] == ' ')
                {
                    // 上一行是"│" 这一行是空格
                    outstr[i][j] = ch3[0];
                    outstr[i][j + 1] = ch3[1];
                }
                j++;
            }
        }

        //最后打印语法树
        for (int i = 0; i < strline; i++)
        {
            files.push_back(outstr[i]);
            cout << outstr[i];
        }
        return;
    }
    void printTree(TreeNode* t, int layer, bool flag)
    {


        if (t == NULL)return;//递归结束
        //找到最后一个子节点的下标
        //cout<<strline<<"  "<<t->name<<"            "<<"layer  "<<layer<<"      child_size : "<<t->child.size()<<endl;
        int last = -1;
        for (int i = 0; i < (t->child.size()); i++)
        {
            if (t->child[i] != NULL)last = i;
        }
        // 如果没有儿子 last == -1
        // 否则 last 记录最后一个儿子节点的index
    //       if(layer==15) cout<<"last : "<<last<<endl;

        for (int i = 0; i < layer; i++) //根据层数打印空格
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
                outstr[strline] += "└ ";
                outstr[strline] += t->name;
            }
            else// 不是父节点的最后一个儿子节点
            {
                outstr[strline] += "├ ";
                outstr[strline] += t->name;
                //cout << "├ " << t->name;
            }
        }

        outstr[strline] += "\n"; //添加换行符号
        strline++;

        for (int i = 0; i < t->child.size(); i++)//递归
        {
            if (i != last)// 不为最后非空子节点 那么第三个参数就标成false
            {
                printTree(t->child[i], layer + 1, false);

            }
            else
            {
                printTree(t->child[i], layer + 1, true);

            }
        }
        //全部递归要结束时开始打印语法树
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
        outfile_cifa.open(file_name.substr(0, file_name.size() - 4) + "_cifa.txt", ios_base::out | ios_base::trunc);  //删除文件重写
        ofstream outfile_digui;
        outfile_digui.open(file_name.substr(0, file_name.size() - 4) + "_yufa.txt", ios_base::out | ios_base::trunc);  //删除文件重写
        ofstream outfile_tree;
        outfile_tree.open(file_name.substr(0, file_name.size() - 4) + "_tree.txt", ios_base::out | ios_base::trunc);  //删除文件重写
        

        syntax_analysis sa;
        vector<string>cifa;
        sa.token = getToken(s,cifa);
        
        for (int i = 0; i < sa.token.size(); i++) {
            cout << sa.token[i].Line << "\t" << sa.token[i].Lex << "\t\t" << sa.token[i].Sem << endl;
            cifa.push_back(to_string(sa.token[i].Line) + "\t" + sa.token[i].Lex + "\t\t" + sa.token[i].Sem + "\n");
        }
        //词法分析部分写文件  
        
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
        //语法分析部分写文件  
        
        for (int i = 0; i < sa.digui.size(); i++)
        {
            stringstream ss;
            ss << sa.digui[i];
            outfile_digui << ss.str();
        }
        
        if (error_flag)
        {
            cout << "递归下降语法分析遇到错误，停止进行后续操作" << endl;
            stringstream ss;
            ss << "递归下降语法分析遇到错误，停止进行后续操作\n";
            outfile_digui << ss.str();
            outfile_digui.close();
            return 0;
        }
        outfile_digui.close();
    
        //语法树部分
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
        outfile_cifa.open(file_name.substr(0, file_name.size() - 4) + "_cifa.txt", ios_base::out | ios_base::trunc);  //删除文件重写
        ofstream outfile_LL1;
        outfile_LL1.open(file_name.substr(0, file_name.size() - 4) + "_yufa.txt", ios_base::out | ios_base::trunc);  //删除文件重写
        ofstream outfile_tree;
        outfile_tree.open(file_name.substr(0, file_name.size() - 4) + "_tree.txt", ios_base::out | ios_base::trunc);  //删除文件重写
  


        vector<string>cifa;
        token = getToken(s,cifa);
        for (int i = 0; i < token.size(); i++) {
            cout << token[i].Line << "\t" << token[i].Lex << "\t\t" << token[i].Sem << endl;
            cifa.push_back(to_string(token[i].Line) + "\t" + token[i].Lex + "\t\t" + token[i].Sem + "\n");
        }
     
        //词法分析部分写文件  
        
        for (int i = 0; i < cifa.size(); i++)
        {
            stringstream ss;
            ss << cifa[i];
            outfile_cifa << ss.str();
        }
        outfile_cifa.close();
        if (error_flag)return -1;




        TreeNode* root = LL1();
        //语法分析部分写文件  
        
        for (int i = 0; i < ll1_file.size(); i++)
        {
            stringstream ss;
            ss << ll1_file[i];
            outfile_LL1 << ss.str();
        }
        //  cout<<"yes"<<endl;
        if (root == NULL) {
            cout << "LL1语法分析遇到错误，停止进行后续操作" << endl;
            stringstream ss;
            ss << "LL1语法分析遇到错误，停止进行后续操作\n";
            outfile_LL1 << ss.str();
            return -1;
        }
        outfile_LL1.close();
        //语法树部分
        printTree(root, 0, false);
       
        for (int i = 0; i < files.size(); i++)
        {
            stringstream ss;
            ss << files[i];
            outfile_tree << ss.str();
        }
        outfile_tree.close();



        //语义分析部分
        SematicAnalyze(root);

        
        this->outfile_yuyi.close();




        return 0;
    }

};







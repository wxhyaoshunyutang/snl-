#ifndef SYNTAX_ANALYSIS_H_INCLUDED
#define SYNTAX_ANALYSIS_H_INCLUDED
#include<vector>
#include<bits/stdc++.h>
#include"morpheme_analysis.h"
using namespace std;

//语法树类
typedef class  Syntax_tree
{
    public:

        string name;  //表示终极符或者非终极符
        Token *token;//存储该节点的Token信息
        vector<Syntax_tree*>child;
        Syntax_tree(string s)
        {
            token=NULL;
            name=s;
        }
        void add_node(Syntax_tree * t)
        {

            child.push_back(t);
        }

}TreeNode;


vector<Token>token;
Token* now_token;// readToken时使用
int token_index=0;
//递归下降子程序的函数列表
TreeNode* Program();//
TreeNode* ProgramHead();//
TreeNode* ProgramName();//
TreeNode* DeclarePart();//
TreeNode* ProgramBody();//
TreeNode* TypeDecPart();//
TreeNode* VarDecPart();//
TreeNode* ProcDecpart();//
TreeNode* TypeDec();//
TreeNode* TypeDecList();//
 TreeNode* TypeID();//
TreeNode* TypeDef();//
TreeNode* TypeDecMore();//
TreeNode* BaseType();//
TreeNode* StructureType();//
TreeNode* Low();//
TreeNode* Top();//
TreeNode* ArrayType();//
TreeNode* RecType();//
TreeNode* FieldDecList();//
TreeNode* IDList();//
TreeNode* FieldDecMore();//
TreeNode* IDMore();//
TreeNode* VarDec();//
TreeNode* VarDecList();//
TreeNode* VarIDList();//
TreeNode* VarDecMore();//
TreeNode* VarIDMore();//
TreeNode* ProcDec();//
TreeNode* ProcName();//
TreeNode* ProcDecMore();//
TreeNode* ParamList();//
TreeNode* ProcDecPart();//
TreeNode* ProcBody();//
TreeNode* ParamDecList();//
TreeNode* Param();//
TreeNode* ParamMore();//
TreeNode* FormList();//
TreeNode* FidMore();//
TreeNode* StmList();//
TreeNode* Stm();//
TreeNode* StmMore();//
TreeNode* ConditionalStm();//
TreeNode* LoopStm();//
TreeNode* InputStm();//
TreeNode* Invar();
TreeNode* OutputStm();//
TreeNode* ReturnStm();//
TreeNode* AssCall();//
TreeNode* AssignmentRest();//
TreeNode* CallStmRest();//
TreeNode* VariMore();//
TreeNode* RelExp();//
TreeNode* Exp();//
TreeNode* ActparamList();//
TreeNode* ActparamMore();//
TreeNode* Term();//
TreeNode* OtherRelE();
TreeNode* OtherTerm();
TreeNode* AddOp();
TreeNode* Factor();
TreeNode* CmpOp();
TreeNode* OtherFactor();
TreeNode* MultOp();
TreeNode* Variable();
TreeNode* FieldVar();
TreeNode* FieldVarMore();
void syntax_error(string error)//语法报错
{
    cout<<"syntax_error!! in line "<<now_token->Line<<" from "<<error<<endl;
    exit(0);//发现错误直接退出先
    error_flag=1;
}
TreeNode* match_token(string lex)//继续往后读Token序列,同时建立一个终极符的语法树节点
{

    if(now_token->Lex==lex)
    {
         TreeNode* t=new TreeNode(now_token->Lex);
        t->token=now_token;
        cout<<now_token->Line<<"行"<<"成功匹配"<<" now_token->Lex: "<<now_token->Lex<<endl;
        now_token=&token[token_index++];
        if(now_token->Lex=="EOF")cout<<"语法分析成功，无错误"<<endl;
        return t;
    }
    syntax_error(lex);//匹配不成功，报错
    return NULL;

}
TreeNode* Program()
{
    if(now_token->Lex=="PROGRAM")
    {
        TreeNode *root=new TreeNode("Program");
        root->add_node(ProgramHead());
        root->add_node(DeclarePart());
        root->add_node(ProgramBody());
        root->add_node(match_token("DOT"));
      return root;
    }
    else syntax_error("PROGRAM");
    return NULL;

}


TreeNode *ProgramHead()
{
    if(now_token->Lex=="PROGRAM")
    {
        TreeNode* root=new TreeNode("ProgramHead");
        root->add_node(match_token("PROGRAM"));
        root->add_node(ProgramName());
        return root;


    }
    else  syntax_error("ProgramHead");
    return NULL;
}



TreeNode* ProgramName()
{
    if(now_token->Lex=="ID")
    {
        TreeNode*root=new TreeNode("ProgramName");
        root->add_node(match_token("ID"));
        return root;
    }
    else syntax_error("ProgramName");
    return NULL;

}

TreeNode *DeclarePart()
{
     vector<string>TEMP={"TYPE","VAR","PROCEDURE","BEGIN"};
      for(int i=0;i<4;i++)
      {

          if(now_token->Lex==TEMP[i])
          {
              TreeNode*root=new TreeNode("DeclarePart");
              root->add_node(TypeDecPart());
              root->add_node(VarDecPart());
              root->add_node(ProcDecpart());
              return root;
          }
      }
      syntax_error("DeclarePart");
      return NULL;

}
TreeNode *ProgramBody()
{

    if(now_token->Lex=="BEGIN")
    {
        TreeNode* root=new TreeNode("ProgramBody");
        root->add_node(match_token("BEGIN"));
        root->add_node(StmList());
        root->add_node(match_token("END"));
        return root;

    }
    else syntax_error("ProgramBody");
    return NULL;
}
TreeNode* TypeDecPart()
{
    if(now_token->Lex=="TYPE")
    {
        TreeNode*root =new TreeNode("TypeDecPart");
        root->add_node(TypeDec());
        return root;

    }
    vector<string>TEMP={"VAR","PROCEDURE","BEGIN"};
    for(int i=0;i<3;i++)
    {

        if(now_token->Lex==TEMP[i])
        {

           TreeNode*root =new TreeNode("TypeDecPart");
            root->add_node(NULL);
            return root;

        }
    }
    syntax_error("TypeDecPart");
    return NULL;



}
TreeNode* VarDecPart()
{
   // cout<<now_token->Lex<<endl;
    if(now_token->Lex=="VAR")
    {
        TreeNode*root =new TreeNode("VarDecPart");
        root->add_node(VarDec());
        return root;

    }
    vector<string>TEMP={"PROCEDURE","BEGIN"};
    for(int i=0;i<2;i++)
    {

        if(now_token->Lex==TEMP[i])
        {

             TreeNode *root=new TreeNode("VarDecPart");
        root->add_node(NULL);
        return root;
        }
    }

    syntax_error("VarDecPart");
    return NULL;

}
TreeNode* ProcDecpart()
{
   // cout<<"procDecPart's lex "<<now_token->Lex<<endl;
     if(now_token->Lex=="PROCEDURE")
    {
        TreeNode*root =new TreeNode("ProcDecpart");
        root->add_node(ProcDec());
        return root;

    }
    else if(now_token->Lex=="BEGIN")
    {
         TreeNode *root=new TreeNode("ProcDecpart");
        root->add_node(NULL);
        return root;
    }
    syntax_error("ProcDecpart");
    return NULL;

}
TreeNode* ProcDecPart()
{
    if(now_token->Lex=="PROCEDURE" || now_token->Lex=="TYPE" ||now_token->Lex=="VAR" || now_token->Lex=="BEGIN" )
    {
        TreeNode*root =new TreeNode("ProcDecPart");
        root->add_node(DeclarePart());
        return root;
    }
     syntax_error("ProcDecPart");
    return NULL;

}
TreeNode* TypeDec()
{

    if(now_token->Lex=="TYPE")
    {
        TreeNode*root=new TreeNode("TypeDec");
        root->add_node(match_token("TYPE"));
        root->add_node(TypeDecList());
        return root;

    }
    else syntax_error("TypeDec");
    return NULL;
}
TreeNode*  TypeDecList()
{
    if(now_token->Lex=="ID")
    {

        TreeNode* root=new TreeNode("TypeDecList");
        root->add_node(TypeID());
        root->add_node(match_token("EQ"));
        root->add_node(TypeDef());
        root->add_node(match_token("SEMI"));
        root->add_node(TypeDecMore());
        return root;
    }
    else syntax_error("TypeDecList");
    return NULL;
}
TreeNode*  TypeID()
{
    if(now_token->Lex=="ID")
    {

        TreeNode *root=new TreeNode("TypeID");
        root->add_node(match_token("ID"));
        return root;
    }
    else syntax_error("TypeID");
    return NULL;

}
TreeNode* TypeDef()
{
    if(now_token->Lex=="INTEGER" || now_token->Lex=="CHAR")
    {
        TreeNode* root=new TreeNode("TypeDef");
        root->add_node(BaseType());
        return root;
    }
    else if(now_token->Lex=="ARRAY" || now_token->Lex=="RECORD")
    {
            TreeNode* root=new TreeNode("TypeDef");
            root->add_node(StructureType());
            return root;
    }
    else if(now_token->Lex=="ID")
    {
        TreeNode *root=new TreeNode("TypeDef");
        root->add_node(match_token("ID"));
        return root;

    }
    else syntax_error("TypeDef");
    return NULL;

}
TreeNode *TypeDecMore()
{
    if(now_token->Lex=="VAR" || now_token->Lex=="PROCEDURE" ||now_token->Lex=="BEGIN")
    {
        TreeNode *root=new TreeNode("TypeDecMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="ID")
    {
        TreeNode *root=new TreeNode("TypeDecMore");
        root->add_node(TypeDecList());
        return root;
    }
    else syntax_error("TypeDecMore");
    return NULL;

}
TreeNode* BaseType()
{
    if(now_token->Lex=="INTEGER")
    {

        TreeNode* root=new TreeNode("BaseType");
        root->add_node(match_token("INTEGER"));
        return root;
    }
    else if(now_token->Lex=="CHAR")
    {
        TreeNode* root=new TreeNode("BaseType");
        root->add_node(match_token("CHAR"));
        return root;

    }
    else syntax_error("BaseType");
    return NULL;
}
TreeNode* StructureType()
{
    if(now_token->Lex=="ARRAY")
    {
        TreeNode *root=new TreeNode("StructureType");
        root->add_node(ArrayType());
        return root;

    }
    else if(now_token->Lex=="RECORD")
    {
        TreeNode *root=new TreeNode("StructureType");
        root->add_node(RecType());
        return root;
    }
    else syntax_error("StructureType");
    return NULL;

}
TreeNode* ArrayType()
{
    if(now_token->Lex=="ARRAY")
    {
       TreeNode *root=new TreeNode("ArrayType");
       root->add_node(match_token("ARRAY"));
       root->add_node(match_token("LMIDPAREN"));
       root->add_node(Low());
       root->add_node(match_token("UNDERANGE"));

       //只用匹配一个吗？

       root->add_node(Top());
       root->add_node(match_token("RMIDPAREN"));
       root->add_node(match_token("OF"));
       root->add_node(BaseType());
       return root;

    }
    else syntax_error("ArrayType");
    return NULL;
}
TreeNode* Low()
{
    if(now_token->Lex=="INTC")
    {
        TreeNode *root=new TreeNode("Low");
        root->add_node(match_token("INTC"));
        return root;
    }
    else syntax_error("Low");
    return NULL;
}
TreeNode* Top()
{
     if(now_token->Lex=="INTC")
    {
        TreeNode *root=new TreeNode("Top");
        root->add_node(match_token("INTC"));
        return root;
    }
    else syntax_error("Top");
    return NULL;
}
TreeNode* RecType()
{
    if(now_token->Lex=="RECORD")
    {
        TreeNode *root=new TreeNode("RecType");
        root->add_node(match_token("RECORD"));
        root->add_node(FieldDecList());
        root->add_node(match_token("END"));

        return root;
    }
    else syntax_error("RecType");
    return NULL;
}
TreeNode* FieldDecList()
{
    if(now_token->Lex=="INTEGER" || now_token->Lex=="CHAR")
    {
        TreeNode *root=new TreeNode("FieldDecList");
        root->add_node(BaseType());
        root->add_node(IDList());
        root->add_node(match_token("SEMI"));
        root->add_node(FieldDecMore());
        return root;
    }
    else if(now_token->Lex=="ARRAY" )
    {
         TreeNode *root=new TreeNode("FieldDecList");
        root->add_node(ArrayType());
        root->add_node(IDList());
        root->add_node(match_token("SEMI"));
        root->add_node(FieldDecMore());
        return root;

    }
    else syntax_error("FieldDecList");
    return NULL;
}


TreeNode *StmList()
{
    vector<string>TEMP={"ID","IF","WHILE","RETURN","READ","WRITE"};
    for(int i=0;i<6;i++)
    {

        if(now_token->Lex==TEMP[i])
        {
            TreeNode* root=new TreeNode("StmList");
            root->add_node(Stm());
            root->add_node(StmMore());
            return root;
        }
    }
    syntax_error("StmList");
    return NULL;
}

TreeNode* IDList()
{
    if(now_token->Lex=="ID")
    {
        TreeNode* root=new TreeNode("IDList");
        root->add_node(match_token("ID"));
        root->add_node(IDMore());
        return NULL;
    }
    else syntax_error("IDList");
    return NULL;
}
TreeNode* FieldDecMore()
{
    if(now_token->Lex=="END")
    {
        TreeNode *root=new TreeNode("FieldDecMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="INTEGER" ||now_token->Lex=="CHAR" ||now_token->Lex=="ARRAY")
    {
        TreeNode *root=new TreeNode("FieldDecMore");
        root->add_node(FieldDecList());
        return root;
    }
    syntax_error("FieldDecMore");
    return NULL;
}
TreeNode* IDMore()
{
      if(now_token->Lex=="SEMI")
    {
        TreeNode *root=new TreeNode("IDMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="COMMA")
    {
        TreeNode *root=new TreeNode("IDMore");
        root->add_node(match_token("COMMA"));
        root->add_node(IDList());
        return root;
    }
    else syntax_error("IDMore");
    return NULL;
}
TreeNode *VarDec()
{

    if(now_token->Lex=="VAR")
    {
        TreeNode *root=new TreeNode("VarDec");
        root->add_node(match_token("VAR"));
        root->add_node(VarDecList());
        return root;
    }
    else syntax_error("VarDec");
    return NULL;
}
TreeNode *ProcDec()
{
    if(now_token->Lex=="PROCEDURE")
    {
         TreeNode *root=new TreeNode("ProcDec");
        root->add_node(match_token("PROCEDURE"));
        root->add_node(ProcName());
        root->add_node(match_token("LPAREN"));
        root->add_node(ParamList());
        root->add_node(match_token("RPAREN"));

        root->add_node(match_token("SEMI"));
        root->add_node(ProcDecPart());
        root->add_node(ProcBody());
        root->add_node(ProcDecpart());
        return root;
    }
    else syntax_error("ProcDec");
    return NULL;
}
TreeNode *VarDecList()
{
    if(now_token->Lex=="INTEGER" || now_token->Lex=="CHAR" || now_token->Lex=="ARRAY" || now_token->Lex=="RECORD" || now_token->Lex=="ID")
    {
        TreeNode *root=new TreeNode("VarDecList");
        root->add_node(TypeDef());
       root->add_node(VarIDList());
        root->add_node(match_token("SEMI"));
        root->add_node(VarDecMore());
        return root;
    }
    else syntax_error("VarDecList");
    return NULL;

}

TreeNode* VarIDList()
{
    if(now_token->Lex=="ID")
    {

        TreeNode *root=new TreeNode("VarIDList");
        root->add_node(match_token("ID"));
        root->add_node(VarIDMore());
        return root;
    }
    else syntax_error("VarIDList");
    return NULL;
}

TreeNode* VarDecMore()
{
     if(now_token->Lex=="PROCEDURE" || now_token->Lex=="BEGIN" )
    {
         TreeNode *root=new TreeNode("VarDecMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="INTEGER" || now_token->Lex=="CHAR" || now_token->Lex=="ARRAY" || now_token->Lex=="RECORD" || now_token->Lex=="ID" )
    {
          TreeNode *root=new TreeNode("VarDecMore");
        root->add_node(VarDecList());
        return root;
    }
    else syntax_error("VarDecMore");
    return NULL;

}
TreeNode* VarIDMore()
{
    if(now_token->Lex=="SEMI")
    {
         TreeNode *root=new TreeNode("VarIDMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="COMMA")
    {
        TreeNode *root=new TreeNode("VarIDMore");
        root->add_node(match_token("COMMA"));
        root->add_node(VarIDList());
        return root;
    }
    else syntax_error("VarIDMore");
    return NULL;
}

TreeNode* ProcName()
{

     if(now_token->Lex=="ID")
    {
        TreeNode *root=new TreeNode("ProcName");
        root->add_node(match_token("ID"));
        return root;
    }
    else syntax_error("ProcName");
    return NULL;
}

//  这个非终极符我看是多余的

TreeNode* ParamList()
{
      if(now_token->Lex=="RPAREN")
    {
        TreeNode *root=new TreeNode("ParamList");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="INTEGER" ||now_token->Lex=="CHAR"|| now_token->Lex=="ARRAY" ||now_token->Lex=="RECORD" || now_token->Lex=="ID" || now_token->Lex=="VAR")
    {
        TreeNode *root=new TreeNode("ParamList");
        root->add_node(ParamDecList());
        return root;
    }
    else syntax_error("ParamList");
    return NULL;
}


TreeNode* ProcBody()
{
    if(now_token->Lex=="BEGIN")
    {
       TreeNode *root=new TreeNode("ProcBody");
        root->add_node(ProgramBody());
        return root;
    }
    else syntax_error("ProcBody");
    return NULL;

}
TreeNode* ParamDecList()
{
     if(now_token->Lex=="INTEGER" || now_token->Lex=="CHAR" ||now_token->Lex=="ARRAY" || now_token->Lex=="RECORD" || now_token->Lex=="ID" || now_token->Lex=="VAR")
    {
       TreeNode *root=new TreeNode("ParamDecList");
        root->add_node(Param());
        root->add_node(ParamMore());
        return root;
    }
    else syntax_error("ParamDecList");
    return NULL;

}
TreeNode* Param()
{
     if(now_token->Lex=="INTEGER" || now_token->Lex=="CHAR" ||now_token->Lex=="ARRAY" || now_token->Lex=="RECORD" || now_token->Lex=="ID")
    {
       TreeNode *root=new TreeNode("Param");
        root->add_node(TypeDef());
        root->add_node(FormList());
        return root;
    }
    else if(now_token->Lex=="VAR")
    {
        TreeNode *root=new TreeNode("Param");
        root->add_node(match_token("VAR"));
        root->add_node(TypeDef());
        root->add_node(FormList());
        return root;
    }
    else syntax_error("Param");
    return NULL;

}

TreeNode* ParamMore()
{
      if(now_token->Lex=="RPAREN")
    {
       TreeNode *root=new TreeNode("ParamMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="SEMI")
    {
        TreeNode *root=new TreeNode("ParamMore");
        root->add_node(match_token("SEMI"));
        root->add_node(ParamDecList());
        return root;
    }
    else syntax_error("ParamMore");
    return NULL;

}
TreeNode* FormList()
{
     if(now_token->Lex=="ID")
    {
       TreeNode *root=new TreeNode("FormList");
        root->add_node(match_token("ID"));
        root->add_node(FidMore());
        return root;
    }
    else syntax_error("FormList");
    return NULL;

}
TreeNode* FidMore()
{
    if(now_token->Lex=="SEMI"|| now_token->Lex=="RPAREN")
    {
         TreeNode *root=new TreeNode("FidMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="COMMA")
    {
        TreeNode *root=new TreeNode("FidMore");
        root->add_node(match_token("COMMA"));
        root->add_node(FormList());
        return root;
    }
    else syntax_error("FidMore");
    return NULL;

}

TreeNode* Stm()
{
    if(now_token->Lex=="IF")
    {
        TreeNode *root=new TreeNode("Stm");
        root->add_node(ConditionalStm());
        return root;

    }
    else if(now_token->Lex=="WHILE")
    {
        TreeNode *root=new TreeNode("Stm");
        root->add_node(LoopStm());
        return root;

    }
    else if(now_token->Lex=="READ")
    {
        TreeNode *root=new TreeNode("Stm");
        root->add_node(InputStm());
        return root;
    }
      else if(now_token->Lex=="WRITE")
    {
           TreeNode *root=new TreeNode("Stm");
        root->add_node(OutputStm());
        return root;
    }
      else if(now_token->Lex=="RETURN")
    {
           TreeNode *root=new TreeNode("Stm");
        root->add_node(ReturnStm());
        return root;
    }
      else if(now_token->Lex=="ID")
    {
        TreeNode *root=new TreeNode("Stm");
        root->add_node(match_token("ID"));
        root->add_node(AssCall());
        return root;
    }
    else syntax_error("Stm");
    return NULL;

}
TreeNode* StmMore()
{
     if(now_token->Lex=="ELSE" || now_token->Lex=="FI" || now_token->Lex=="END" || now_token->Lex=="ENDWH")
    {
         TreeNode *root=new TreeNode("StmMore");
        root->add_node(NULL);
        return root;
    }
    else if(now_token->Lex=="SEMI")
    {
        TreeNode *root=new TreeNode("StmMore");
        root->add_node(match_token("SEMI"));
        root->add_node(StmList());
        return root;
    }
     else syntax_error("StmMore");
    return NULL;

}
TreeNode* ConditionalStm()
{

    if(now_token->Lex=="IF")
    {
        TreeNode *root=new TreeNode("ConditionalStm");
        root->add_node(match_token("IF"));
        root->add_node(RelExp());
        root->add_node(match_token("THEN"));
        root->add_node(StmList());
        root->add_node(match_token("ELSE"));
        root->add_node(StmList());
        root->add_node(match_token("FI"));
        return root;
    }
     else syntax_error("ConditionalStm");
    return NULL;

}
TreeNode* LoopStm()
{
     if(now_token->Lex=="WHILE")
    {
        TreeNode *root=new TreeNode("LoopStm");
        root->add_node(match_token("WHILE"));
        root->add_node(RelExp());
        root->add_node(match_token("DO"));
        root->add_node(StmList());
        root->add_node(match_token("ENDWH"));
        return root;
    }
     else syntax_error("LoopStm");
    return NULL;

}
TreeNode* InputStm()
{
      if(now_token->Lex=="READ")
    {
        TreeNode *root=new TreeNode("InputStm");
        root->add_node(match_token("READ"));
        root->add_node(match_token("LPAREN"));
        root->add_node(Invar());
        root->add_node(match_token("RPAREN"));
        return root;
    }
     else syntax_error("InputStm");
    return NULL;

}
TreeNode* Invar()
{
     if(now_token->Lex=="ID")
    {
        TreeNode *root=new TreeNode("Invar");
        root->add_node(match_token("ID"));
        return root;
    }
     else syntax_error("Invar");
    return NULL;


}
TreeNode* OutputStm()
{
     if(now_token->Lex=="WRITE")
    {
        TreeNode *root=new TreeNode("OutputStm");
        root->add_node(match_token("WRITE"));
        root->add_node(match_token("LPAREN"));
        root->add_node(Exp());
        root->add_node(match_token("RPAREN"));
        return root;
    }
     else syntax_error("OutputStm");
    return NULL;
}
TreeNode* ReturnStm()
{
     if(now_token->Lex=="RETURN")
    {
        TreeNode *root=new TreeNode("ReturnStm");
        root->add_node(match_token("RETURN"));
        return root;
    }
     else syntax_error("ReturnStm");
    return NULL;
}
//这一句文法有过修改
TreeNode* AssCall()
{
     if(now_token->Lex=="LMIDPAREN" || now_token->Lex=="DOT" || now_token->Lex=="ASSIG")
    {
       TreeNode *root=new TreeNode("AssCall");
        root->add_node(AssignmentRest());
        return root;
    }
    else if(now_token->Lex=="LPAREN")
    {
          TreeNode *root=new TreeNode("AssCall");
        root->add_node(CallStmRest());
        return root;

    }
     else syntax_error("AssCall");
    return NULL;
}
TreeNode* AssignmentRest()
{
      if(now_token->Lex=="LMIDPAREN" || now_token->Lex=="DOT" || now_token->Lex=="ASSIG")
    {
       TreeNode *root=new TreeNode("AssignmentRest");
        root->add_node(VariMore());
        root->add_node(match_token("ASSIG"));
        root->add_node(Exp());
        return root;
    }
     else syntax_error("AssignmentRest");
    return NULL;
}
TreeNode* CallStmRest()
{
      if(now_token->Lex=="LPAREN")
    {
       TreeNode *root=new TreeNode("CallStmRest");
       root->add_node(match_token("LPAREN"));
        root->add_node(ActparamList());
         root->add_node(match_token("RPAREN"));
        return root;
    }
     else syntax_error("CallStmRest");
    return NULL;
}
TreeNode* VariMore()
{
    if(now_token->Lex=="LMIDPAREN")
    {
         TreeNode *root=new TreeNode("VariMore");
        root->add_node(match_token("LMIDPAREN"));
         root->add_node(Exp());
         root->add_node(match_token("RMIDPAREN"));
        return root;
    }
    else if(now_token->Lex=="DOT")
    {
        TreeNode *root=new TreeNode("VariMore");
        root->add_node(match_token("DOT"));
         root->add_node(FieldVar());
        return root;
    }
    vector<string>TEMP={"ASSIG","MULT","DIV","PLUS","SUB","LT","EQ","THEN","ELSE","FI","DO","ENDWH","RPAREN","END","SEMI","COMMA","RMIDPAREN"};
    for(int i=0;i<TEMP.size();i++)
    {
        if(now_token->Lex==TEMP[i])
        {
             TreeNode *root=new TreeNode("VariMore");
        root->add_node(NULL);
        return root;
        }
    }
    syntax_error("VariMore");
    return NULL;
}
TreeNode* RelExp()
{
    if(now_token->Lex=="LPAREN"||now_token->Lex=="INTC" || now_token->Lex=="ID")
    {
         TreeNode *root=new TreeNode("RelExp");
        root->add_node(Exp());
         root->add_node(OtherRelE());
        return root;
    }
    else syntax_error("RelExp");
    return NULL;
}
TreeNode* Exp()
{
    if(now_token->Lex=="LPAREN"||now_token->Lex=="INTC" || now_token->Lex=="ID")
    {
         TreeNode *root=new TreeNode("Exp");
        root->add_node(Term());
         root->add_node(OtherTerm());
        return root;
    }
    else syntax_error("Exp");
    return NULL;

}
TreeNode* ActparamList()
{
    if(now_token->Lex=="RPAREN")
    {
        TreeNode *root=new TreeNode("ActparamList");
        root->add_node(NULL);
        return root;
    }

    else if(now_token->Lex=="LPAREN" || now_token->Lex=="INTC" || now_token->Lex=="ID")
    {
         TreeNode *root=new TreeNode("ActparamList");
        root->add_node(Exp());
         root->add_node(ActparamMore());
        return root;
    }

    else syntax_error("ActparamList");
    return NULL;


}
TreeNode* ActparamMore()
{
    if(now_token->Lex=="RPAREN")
    {
         TreeNode *root=new TreeNode("ActparamMore");
        root->add_node(NULL);
        return root;
    }

    else if(now_token->Lex=="COMMA")
    {
         TreeNode *root=new TreeNode("ActparamMore");
        root->add_node(match_token("COMMA"));
         root->add_node(ActparamList());
        return root;
    }

    else syntax_error("ActparamMore");
    return NULL;
}
TreeNode* Term()
{
    if(now_token->Lex=="LPAREN"||now_token->Lex=="INTC" || now_token->Lex=="ID")
    {
          TreeNode *root=new TreeNode("Term");
        root->add_node(Factor());
         root->add_node(OtherFactor());
        return root;
    }

    else syntax_error("Term");
    return NULL;

}
TreeNode* OtherRelE()
{
     if(now_token->Lex=="LT"||now_token->Lex=="EQ")
    {
         TreeNode *root=new TreeNode("OtherRelE");
        root->add_node(CmpOp());
         root->add_node(Exp());
        return root;
    }
    syntax_error("OtherRelE");
    return NULL;
}


TreeNode* OtherTerm()
{
    vector<string>TEMP={"LT","EQ","RMIDPAREN","THEN","ELSE","FI","DO","ENDWH","RPAREN","END","SEMI","COMMA"};
    for(int i=0;i<TEMP.size();i++)
    {
        if(now_token->Lex==TEMP[i])
        {
            TreeNode *root=new TreeNode("OtherTerm");
        root->add_node(NULL);
        return root;
        }
    }
    if(now_token->Lex=="PLUS"||now_token->Lex=="SUB")
    {
         TreeNode *root=new TreeNode("OtherTerm");
        root->add_node(AddOp());
         root->add_node(Exp());
        return root;
    }
    syntax_error("OtherTerm");
    return NULL;
}
TreeNode* AddOp()
{
    if(now_token->Lex=="PLUS")
    {
        TreeNode *root=new TreeNode("AddOp");
        root->add_node(match_token("PLUS"));
        return root;
    }
    else if(now_token->Lex=="SUB")
    {
         TreeNode *root=new TreeNode("AddOp");
        root->add_node(match_token("SUB"));
        return root;

    }
    else syntax_error("AddOp");
    return NULL;
}

TreeNode* CmpOp()
{
      if(now_token->Lex=="LT")
    {
        TreeNode *root=new TreeNode("CmpOp");
        root->add_node(match_token("LT"));
        return root;
    }
    else if(now_token->Lex=="EQ")
    {
         TreeNode *root=new TreeNode("CmpOp");
        root->add_node(match_token("EQ"));
        return root;

    }
    else syntax_error("CmpOp");
    return NULL;
}
TreeNode* Factor()
{
     if(now_token->Lex=="LPAREN")
    {
        TreeNode *root=new TreeNode("Factor");
        root->add_node(match_token("LPAREN"));
        root->add_node(Exp());
        root->add_node(match_token("RPAREN"));
        return root;
    }
    else if(now_token->Lex=="INTC")
    {
         TreeNode *root=new TreeNode("Factor");
        root->add_node(match_token("INTC"));
        return root;

    }
    else if(now_token->Lex=="ID")
    {
          TreeNode *root=new TreeNode("Factor");
        root->add_node(Variable());
        return root;
    }
    else syntax_error("Factor");
    return NULL;

}
TreeNode* OtherFactor()
{
    vector<string>TEMP={"PLUS","SUB","LT","EQ","RMIDPAREN","THEN","ELSE","FI","DO","ENDWH","RPAREN","END","SEMI","COMMA"};
    for(int i=0;i<14;i++)
    {
        if(TEMP[i]==now_token->Lex)
        {
            TreeNode *root=new TreeNode("OtherFactor");
        root->add_node(NULL);
        return root;
        }
    }
    if(now_token->Lex=="MULT" || now_token->Lex=="DIV")
    {
         TreeNode *root=new TreeNode("OtherFactor");
        root->add_node(MultOp());
        root->add_node(Term());
        return root;

    }
     else syntax_error("OtherFactor");
    return NULL;
}
TreeNode* MultOp()
{
    if(now_token->Lex=="MULT")
    {
         TreeNode *root=new TreeNode("MultOp");
        root->add_node(match_token("MULT"));
        return root;
    }
    else if(now_token->Lex=="DIV")
    {
        TreeNode *root=new TreeNode("MultOp");
        root->add_node(match_token("DIV"));
        return root;
    }
    else syntax_error("MultOp");
    return NULL;

}
TreeNode* Variable()
{
    if(now_token->Lex=="ID")
    {
         TreeNode *root=new TreeNode("Variable");
        root->add_node(match_token("ID"));
        root->add_node(VariMore());
        return root;
    }
    else syntax_error("Variable");
    return NULL;

}
TreeNode* FieldVar()
{
      if(now_token->Lex=="ID")
    {
         TreeNode *root=new TreeNode("FieldVar");
        root->add_node(match_token("ID"));
        root->add_node(FieldVarMore());
        return root;
    }
    else syntax_error("FieldVar");
    return NULL;

}
TreeNode* FieldVarMore()
{
    vector<string>TEMP={"LT","EQ","ASSIG","PLUS","SUB","MULT","DIV","THEN","ELSE","FI","DO","ENDWH","RPAREN","END","SEMI","COMMA"};
    for(int i=0;i<TEMP.size();i++)
    {
        if(now_token->Lex==TEMP[i])
        {
             TreeNode *root=new TreeNode("FieldVarMore");
             root->add_node(NULL);
             return root;
        }
    }
    if(now_token->Lex=="LMIDPAREN")
    {
         TreeNode *root=new TreeNode("FieldVarMore");
        root->add_node(match_token("LMIDPAREN"));
        root->add_node(Exp());
        root->add_node(match_token("RMIDPAREN"));
        return root;
    }
    else syntax_error("FieldVarMore");
    return NULL;
}


string outstr[5000];
char out[100];
int strline=0;
char ch1[3] = "├"; // 全角字符用来画树枝 ch1[0] ch1[1] 保存了字符 ch1[2]是\0
char ch2[3] = "└"; // 全角字符用起来真的很容易出错...
char ch3[3] = "│";

void printTree_node()
{
    //对字符串形状进行调整
    cout<<"-------------------------------------------------------------------------------"<<endl;
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
			cout << outstr[i];
		}
		return;
}
void printTree(TreeNode * t, int layer, bool flag)
{


	if (t == NULL)return;//递归结束
	//找到最后一个子节点的下标
	//cout<<strline<<"  "<<t->name<<"            "<<"layer  "<<layer<<"      child_size : "<<t->child.size()<<endl;
	int last = -1;
	for(int i=0;i<(t->child.size());i++)
    {
        if(t->child[i]!=NULL)last=i;
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

    for(int i=0;i<t->child.size();i++)//递归
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
   if(layer==0)
   {
      printTree_node();
   }
   return;
}



void init()
{
    string s = readFileIntoString("codetest.txt");

	//得到token序列
    token=getToken(s);
   // cout<<token.size()<<endl;
	for (int i = 0;i < token.size();i++)
	{
		cout <<token[i].Line <<"\t" << token[i].Lex <<"\t" <<token[i].Sem<<endl;
	}


	if(error_flag)return ;//如果词法分析，语义分析，语法分析，三种任意一个出现问题，就直接结束。
    now_token=&token[token_index++];

    TreeNode* root=Program();//开始语法分析，并建立语法分析树
    printTree(root,0,false);

 //   if(error_flag)return;




}
#endif // SYNTAX_ANALYSIS_H_INCLUDED

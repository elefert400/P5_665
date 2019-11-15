#include "ast.hpp"

namespace lake{

IRProgram * ProgramNode::to3AC(){
	IRProgram * prog = new IRProgram();
	myDeclList->to3AC(prog);
	return prog;
}

//Process local scope variables
void VarDeclListNode::to3AC(Procedure * proc){
	for (auto decl : *myDecls){
		decl->to3AC(proc);
	}
}

//Process DeclList at the global scope
void DeclListNode::to3AC(IRProgram * prog){
	for (auto decl : *myDecls){
		decl->to3AC(prog);
	}
}

void FnDeclNode::to3AC(IRProgram * prog){
	Procedure* myProc = prog->makeProc(getDeclaredName());
	//calling proc version of FnDeclNode::to3AC
	to3AC(myProc);
}

void FnDeclNode::to3AC(Procedure * proc){
	myFormals->to3AC(proc);
	EnterQuad* myEnter = new EnterQuad(proc);
	proc->addQuad(myEnter);
	myFormals->to3AC(proc);
	myBody->to3AC(proc);
}

void FormalDeclNode::to3AC(IRProgram * prog){
	TODO(Implement me)//this
}

void FormalDeclNode::to3AC(Procedure * proc){
	TODO(Implement me)//this
}

void FormalsListNode::to3AC(Procedure * proc){
	size_t argIndex = 1;
	for(auto formal : *myFormals){
		GetInQuad* currFormalQuad = new GetInQuad(argIndex, formal->getDeclaredID()->flatten(proc));
		proc->addQuad(currFormalQuad);
		formal->to3AC(proc->getProg());
		formal->to3AC(proc);
		argIndex++;
	}
}

void ExpListNode::to3AC(Procedure * proc){
	size_t argIndex = 1;
	for(auto exp : *myExps){
		SetInQuad* currArgQuad = new SetInQuad(argIndex, exp->flatten(proc));
		proc->addQuad(currArgQuad);
		argIndex++;
	}
}

void StmtListNode::to3AC(Procedure * proc){
	TODO(Implement me)//this
}

void FnBodyNode::to3AC(Procedure * proc){
	myVarDecls->to3AC(proc);
	myStmtList->to3AC(proc);
}

Opd * IntLitNode::flatten(Procedure * proc){
	return new LitOpd(std::to_string(myInt));
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myString);
	return res;
}

Opd * TrueNode::flatten(Procedure * prog){
	return new LitOpd("true");
}

Opd * FalseNode::flatten(Procedure * prog){
	return new LitOpd("false");
}

Opd * AssignNode::flatten(Procedure * proc){
	Opd* tgtOpd = myTgt->flatten(proc);
	Opd* srcOpd = mySrc->flatten(proc);
	AssignQuad* myAssignQuad = new AssignQuad(tgtOpd, srcOpd);
	proc->addQuad(myAssignQuad);
	return(tgtOpd);
}

Opd * DerefNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * CallExpNode::flatten(Procedure * proc){
	//generate the 3AC for setin's required to make the call
	myExpList->to3AC(proc);
	Opd* idOpd = myId->flatten(proc);
	CallQuad* myCallQuad = new CallQuad(myId->getSymbol());
	proc->addQuad(myCallQuad);
	return(idOpd);
}

Opd * UnaryMinusNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * NotNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * PlusNode::flatten(Procedure * proc){
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	BinOp myOp = ADD;

	Opd * tmp = proc->makeTmp();

	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	proc->addQuad(myBinOp);

	return tmp;
}

Opd * MinusNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * TimesNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * DivideNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * AndNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * OrNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * EqualsNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * LessNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * GreaterNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * LessEqNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	TODO(Implement me)
}

void AssignStmtNode::to3AC(Procedure * proc){
	myAssign->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void PostDecStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void ReadStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void WriteStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void IfStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void IfElseStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void WhileStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void CallStmtNode::to3AC(Procedure * proc){
	myCallExp->flatten(proc);
}

void ReturnStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void VarDeclNode::to3AC(Procedure * proc){
	SemSymbol * sym = getDeclaredID()->getSymbol();
	if (sym == nullptr){
		throw new InternalError("null sym");
	}
	proc->gatherLocal(sym);
}

void VarDeclNode::to3AC(IRProgram * prog){
	SemSymbol * sym = getDeclaredID()->getSymbol();
	if (sym == nullptr){
		throw new InternalError("null sym");
	}

	prog->gatherGlobal(sym);
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent)
Opd * IdNode::flatten(Procedure * proc){
	SymOpd* idOpd = proc->getSymOpd(mySymbol);
	return(idOpd);
}


}

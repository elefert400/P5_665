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
	myBody->to3AC(proc);
}

void FormalDeclNode::to3AC(IRProgram * prog){
	//what did you do differently here vs FormalDeclNode
}

void FormalDeclNode::to3AC(Procedure * proc){
	SemSymbol* idFormalSemSym = getDeclaredID()->getSymbol();
	proc->gatherFormal(idFormalSemSym);
}

void FormalsListNode::to3AC(Procedure * proc){
	size_t argIndex = 1;
	for(auto formal : *myFormals){
		//formal->to3AC(proc->getProg());
		formal->to3AC(proc);
		GetInQuad* currFormalQuad = new GetInQuad(argIndex, proc->getSymOpd(formal->getDeclaredID()->getSymbol()));
		proc->addQuad(currFormalQuad);
		argIndex++;
		// GetInQuad* currFormalQuad = new GetInQuad(argIndex, formal->getDeclaredID()->flatten(proc));

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
	for(auto stmt : *myStmts){
		stmt->to3AC(proc);
	}
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
	return new LitOpd("1");//oracle shows true -> 1
}

Opd * FalseNode::flatten(Procedure * prog){
	return new LitOpd("2");//oracle shows false -> 1
}

Opd * AssignNode::flatten(Procedure * proc){
	Opd* tgtOpd = myTgt->flatten(proc);
	Opd* srcOpd = mySrc->flatten(proc);
	AssignQuad* myAssignQuad = new AssignQuad(tgtOpd, srcOpd);//combined these two
	proc->addQuad(myAssignQuad);//this one
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
	// myId->getSymbol()->getType()->asFn()->getReturnType()->isVoid()
	if(!myId->getSymbol()->getType()->asFn()->getReturnType()->isVoid())
	{
		Opd * getOut = proc->makeTmp();
		proc->addQuad(new GetOutQuad(1, getOut));
		return getOut;
	}

	return(proc->getSymOpd(myId->getSymbol()));
}

Opd * UnaryMinusNode::flatten(Procedure * proc){
	Opd * meExp = myExp->flatten(proc);

	UnaryOp myOp = NEG;

	Opd * tmp = proc->makeTmp();

	UnaryOpQuad * myQuad = new UnaryOpQuad(tmp, myOp, meExp);

	proc->addQuad(myQuad);

	return tmp;
}

Opd * NotNode::flatten(Procedure * proc){
	Opd * meExp = myExp->flatten(proc);

	UnaryOp myOp = NOT;

	Opd * tmp = proc->makeTmp();

	UnaryOpQuad * myQuad = new UnaryOpQuad(tmp, myOp, meExp);

	proc->addQuad(myQuad);

	return tmp;
}

Opd * PlusNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = ADD;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * MinusNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = SUB;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * TimesNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = MULT;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * DivideNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = DIV;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * AndNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = AND;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * OrNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = OR;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * EqualsNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = EQ;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = NEQ;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * LessNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = LT;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * GreaterNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = GT;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * LessEqNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = LTE;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	//flatten the left and right operands
	Opd * left = myExp1->flatten(proc);
	Opd * right = myExp2->flatten(proc);

	//get the operation
	BinOp myOp = GTE;

	//generate the temp that is returned
	Opd * tmp = proc->makeTmp();

	//make the quad
	BinOpQuad * myBinOp = new BinOpQuad(tmp, myOp, left, right);

	// add the quad to the procedure
	proc->addQuad(myBinOp);

	//return the temp
	return tmp;
}

void AssignStmtNode::to3AC(Procedure * proc){
	myAssign->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
	Opd* expOpd = myExp->flatten(proc);
	BinOpQuad* myIncrQuad = new BinOpQuad(expOpd, ADD, expOpd, new LitOpd("1"));
	proc->addQuad(myIncrQuad);
}

void PostDecStmtNode::to3AC(Procedure * proc){
	Opd* expOpd = myExp->flatten(proc);
	BinOpQuad* myIncrQuad = new BinOpQuad(expOpd, SUB, expOpd, new LitOpd("1"));
	proc->addQuad(myIncrQuad);
}

void ReadStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void WriteStmtNode::to3AC(Procedure * proc){
	TODO(Implement me)
}

void IfStmtNode::to3AC(Procedure * proc){
	Opd * flatStanley = myExp->flatten(proc);
	Label * beginLbl = proc->makeLabel();
	NopQuad * beginNoOp = new NopQuad();
	beginNoOp->addLabel(beginLbl);
	JmpIfQuad * myJmp = new JmpIfQuad(flatStanley, false, beginLbl);
	proc->addQuad(myJmp);
	myDecls->to3AC(proc);
	myStmts->to3AC(proc);
	proc->addQuad(beginNoOp);
}

void IfElseStmtNode::to3AC(Procedure * proc){
	Opd * flatStanley = myExp->flatten(proc);
	Label * beginLbl = proc->makeLabel();
	NopQuad * beginNoOp = new NopQuad();
	beginNoOp->addLabel(beginLbl);
	JmpIfQuad * myJmp = new JmpIfQuad(flatStanley, false, beginLbl);
	proc->addQuad(myJmp);
	myDecls->to3AC(proc);
	myStmts->to3AC(proc);
	proc->addQuad(beginNoOp);
}

void WhileStmtNode::to3AC(Procedure * proc){
	Label * beginLbl = proc->makeLabel();
	NopQuad * beginNoOp = new NopQuad();
	beginNoOp->addLabel(beginLbl);
	proc->addQuad(beginNoOp);


	Label * endLbl = proc->makeLabel();
	NopQuad * endNoOp = new NopQuad();
	endNoOp->addLabel(endLbl);


	Opd * flattenedExp = myExp->flatten(proc);
	JmpIfQuad * myJump = new JmpIfQuad(flattenedExp, false, endLbl);
	proc->addQuad(myJump);

	myDecls->to3AC(proc);
	myStmts->to3AC(proc);


	JmpQuad * backToStartJmp = new JmpQuad(beginLbl);
	proc->addQuad(backToStartJmp);
	proc->addQuad(endNoOp);
}

void CallStmtNode::to3AC(Procedure * proc){
	myCallExp->flatten(proc);
}

void ReturnStmtNode::to3AC(Procedure * proc){
	Opd * expOpd = myExp->flatten(proc);
	size_t index = 1;
	proc->addQuad(new SetOutQuad(index, expOpd));
	proc->addQuad(new JmpQuad(proc->getLeaveLabel()));
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
	//do you need to add the node quad or something?
	//it should already be connected on the gatherGlobal and gatherLocal calls
	SymOpd* idOpd = proc->getSymOpd(mySymbol);
	return(idOpd);
}


}

#include "llvm/IR/Instructions.h"
#include "Liveness.h"

#include <unistd.h>
#include <stdio.h>

using namespace std;

void print_elem(const Value* i) {
  errs() << i->getName() << " ";
}

bool Liveness::isLiveOut(Instruction *I, Value *V){
	errs() << "bla1\n";
    return false;
}

//bb -> basic block
void Liveness::computeBBDefUse(Function &F){
	errs() << "bla2\n";
}

void Liveness::computeBBInOut(Function &F){
	errs() << "bla3\n";
}

void Liveness::computeIInOut(Function &F) {
	errs() << "bla4\n";
	for (Function::iterator b = F.begin(), e = F.end(); b != e; ++b) {
        BasicBlock::iterator i = --b->end();
        errs() << "BasicBlock (name=" << b->getName() << ") has "<< b->size() << " instructions.\n";	
        errs() << "Function (name=" << i->getName() << ")\n" ;	
      }
	
}

bool Liveness::runOnFunction(Function &F) {
	errs() << "bla0\n";
    computeBBDefUse(F);
    computeBBInOut(F);
    computeIInOut(F);
    return false;
}

char Liveness::ID = 0;

RegisterPass<Liveness> X("liveness", "Live vars analysis", false, false);






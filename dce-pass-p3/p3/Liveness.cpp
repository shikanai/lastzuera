#include "llvm/IR/Instructions.h"
#include "Liveness.h"

#include <unistd.h>
#include <stdio.h>

using namespace llvm;

namespace llvm {

void print_elem(const Value* i) {
  errs() << i->getName() << " ";
}

//Retirado do exemplo
void Liveness::addToMap(Function &F) {
    static int id = 1;
    for (inst_iterator i = inst_begin(F), E = inst_end(F); i != E; ++i, ++id){
		/*errs() << "Inserting instruction on map: Instruction (name=" << *i << ")\n" ;*/
        // Convert the iterator to a pointer, and insert the pair
        instMap.insert(std::make_pair(&*i, id));
	}
}

bool Liveness::isLiveOut(Instruction *I, Value *V){
	errs() << "isLiveOut\n";
    return false;
}

//bb -> basic block
//aqui eh onde vamos definir o Def e o Use, a partir de cada statement...
void Liveness::computeBBDefUse(Function &F){
	errs() << "DefUses\n";
    for (Function::iterator basicBlock = F.begin(), end = F.end(); basicBlock != end; ++basicBlock) {
		LivenessInfo instructionInfo;
		errs() << "Iterando dentro de Function...\n" ;
		//errs() << *basicBlock;
		for (BasicBlock::iterator instruction = basicBlock->begin(), end2 = basicBlock->end(); instruction != end2; ++instruction) {
			errs() << "Iterando dentro de BasicBlocks...\n" ;
			errs() << *instruction << "\n";
			
			//estrutura que vai conter as infos de Def e Use da instrucao analisada no momento.
			//Utilizaremos essa estrutura para armazenar as informacoes em iLivenessMap.

			/*Extraido do exemplo*/
			unsigned n = instruction->getNumOperands();
			for (unsigned j = 0; j < n; j++) {
				Value *v = instruction->getOperand(j);
				if (isa<Instruction>(v)) {
				  Instruction *op = cast<Instruction>(v);
				  if (!instructionInfo.def.count(op))
					instructionInfo.use.insert(op);
				}
			  }
			  // For the KILL set, you can use the set of all instructions
			  // that are in the block (which safely includes all of the
			  // pseudo-registers assigned to in the block).
			  instructionInfo.def.insert(&*instruction);
			}
			bbLivenessMap.insert(std::make_pair(&*basicBlock, instructionInfo));
		}

}

void Liveness::computeBBInOut(Function &F){
	errs() << "BBInOut\n";
}

//i -> instruction
void Liveness::computeIInOut(Function &F) {
	errs() << "IInOut\n";
	/*for (Function::iterator b = F.begin(), e = F.end(); b != e; ++b) {
        BasicBlock::iterator i = --b->end();
        errs() << "BasicBlock (name=" << b->getName() << ") has "<< b->size() << " instructions.\n";	
        errs() << "Instruction (name=" << *i << ")\n" ;
      }*/
	
}

bool Liveness::runOnFunction(Function &F) {

	//adiciona instrucoes ao map
	addToMap(F);
	
	errs() << "runOnFunction\n";
	//computa todos os Def e Uses de cada instrucao
    computeBBDefUse(F);
	
    computeBBInOut(F);
    computeIInOut(F);
    return false;
}

char Liveness::ID = 0;

RegisterPass<Liveness> X("liveness", "Live vars analysis", false, false);


}



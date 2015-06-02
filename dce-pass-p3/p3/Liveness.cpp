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
//Baseado em exemplo fornecido
//gen -> use
//kill -> def
//before -> in
//after -> out
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
					if (!instructionInfo.def.count(op)){
						instructionInfo.use.insert(op);
					}
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

//Baseado no exemplo fornecido
//verificar se esta ok o funcionamento
void Liveness::computeBBInOut(Function &F){
	errs() << "BBInOut\n";
	SmallVector<BasicBlock*, 32> workList;
	
	//pegando o ultimo elemento 
    workList.push_back(--F.end());

	//iterando na workList
    while (workList.empty()==0) {
	
		BasicBlock *basicBlock = workList.pop_back_val();
		
		//toda a informacao esta no LivenessInfo.
		LivenessInfo b_info = bbLivenessMap.lookup(basicBlock);
		bool shouldAddPred = !bbLivenessMap.count(basicBlock);
		
		//nao precisa desse b_genKill
		//genKill b_genKill = bbGKMap.lookup(basicBlock);
      
		// Take the union of all successors
		std::set<const Instruction*> a;
		for (succ_iterator SI = succ_begin(basicBlock), E = succ_end(basicBlock); SI != E; ++SI) {
			std::set<const Instruction*> s(bbLivenessMap.lookup(*SI).in);
			a.insert(s.begin(), s.end());
		}

        if (a != b_info.out){
          shouldAddPred = true;
          b_info.out = a;
          // in = out - def + use
          b_info.in.clear();
          std::set_difference(a.begin(), a.end(), b_info.def.begin(), b_info.def.end(),
                              std::inserter(b_info.in, b_info.in.end()));
          b_info.in.insert(b_info.use.begin(), b_info.use.end());
        }
        
        if (shouldAddPred)
          for (pred_iterator PI = pred_begin(basicBlock), E = pred_end(basicBlock); PI != E; ++PI)
            workList.push_back(*PI);
      }
	
}

//i -> instruction

void Liveness::computeIInOut(Function &F) {
	errs() << "IInOut\n";
	
	
}

bool Liveness::runOnFunction(Function &F) {

	 bool changed = false;

	//adiciona instrucoes ao map
	addToMap(F);
	
	errs() << "runOnFunction\n";
	//computa todos os Def e Uses de cada instrucao
    computeBBDefUse(F);
	
	// For each basic block in the function, compute the block's in and out sets.
    computeBBInOut(F);
	
	
    computeIInOut(F);
    return false;
}

char Liveness::ID = 0;

RegisterPass<Liveness> X("liveness", "Live vars analysis", false, false);


}



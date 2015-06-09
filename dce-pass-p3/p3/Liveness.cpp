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
//Funcao que checa se o valor V existe no out de I.
bool Liveness::isLiveOut(Instruction *I, Instruction *V){
	//errs() << "isLiveOut\n";
	//procura pela instrucao no LivenessMap
	LivenessInfo info = iLivenessMap.lookup(&*I);
	//quando encontrar, verifica se existe V no out
	if(info.out.count(V)>0){
		return true;
	}else{
		errs() << "nao encontrado V em I\n";
		return false;
	}
	
	//return (info.out.count(V));
	
    //return false;
}

//bb -> basic block
//aqui eh onde vamos definir o Def e o Use, a partir de cada statement...
//Baseado em exemplo fornecido
//gen -> use
//kill -> def
//before -> in
//after -> out
void Liveness::computeBBDefUse(Function &F){
	//errs() << "DefUses\n";
    for (Function::iterator basicBlock = F.begin(), end = F.end(); basicBlock != end; ++basicBlock) {
		LivenessInfo instructionInfo;
		//errs() << "Iterando dentro de Function...\n" ;
		//errs() << *basicBlock;
		for (BasicBlock::iterator instruction = basicBlock->begin(), end2 = basicBlock->end(); instruction != end2; ++instruction) {
			//errs() << "Iterando dentro de BasicBlocks...\n" ;
			//errs() << *instruction << "\n";
			
			/*Extraido do exemplo*/
			unsigned n = instruction->getNumOperands();
			//itera todos os operandos da instrucao atual
			for (unsigned j = 0; j < n; j++) {
				Value *v = instruction->getOperand(j);
				//verifica se o operando eh uma instrucao
				if (isa<Instruction>(v)) {
					Instruction *op = cast<Instruction>(v);
					//se nao encontrar em def, coloca em use.
					if (!instructionInfo.def.count(op)){
						instructionInfo.use.insert(op);
						//instructionInfo.def.insert(op);
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
	
		//tira o ultimo elemento, e coloca em basicBlock
		BasicBlock *basicBlock = workList.pop_back_val();
		
		//toda a informacao esta no LivenessInfo.
		LivenessInfo b_info = bbLivenessMap.lookup(basicBlock);
		bool shouldAddPred = !bbLivenessMap.count(basicBlock);
		
		//nao precisa desse b_genKill
		//genKill b_genKill = bbGKMap.lookup(basicBlock);
      
		// Take the union of all successors
		std::set<const Instruction*> a;
		
		//Itera por todos os sucessores do basic block em analise, procurando por eles no bbLivenessMap.
		//Caso exista, inserimos a instrucao em a.
		for (succ_iterator SI = succ_begin(basicBlock), E = succ_end(basicBlock); SI != E; ++SI) {
			std::set<const Instruction*> s(bbLivenessMap.lookup(*SI).in);
			a.insert(s.begin(), s.end());
		}

        if (a != b_info.out){
          shouldAddPred = true;
          b_info.out = a;
          // in = out - def + use
          b_info.in.clear();
		  
		  //out - def
          std::set_difference(a.begin(), a.end(), b_info.def.begin(), b_info.def.end(),
                              std::inserter(b_info.in, b_info.in.end()));
		  //+ use
          b_info.in.insert(b_info.use.begin(), b_info.use.end());
        }
        
        if (shouldAddPred){
			//iterando anteriores
			for (pred_iterator PI = pred_begin(basicBlock), E = pred_end(basicBlock); PI != E; ++PI){
				workList.push_back(*PI);
			}
		}
      }
	
}

//i -> instruction
//Baseado em exemplo fornecido
void Liveness::computeIInOut(Function &F) {
	errs() << "IInOut\n";
	//iterando basic blocks
	for (Function::iterator basicBlock = F.begin(), e = F.end(); basicBlock != e; ++basicBlock) {
		//Comeca da ultima instrucao
        BasicBlock::iterator instruction = --basicBlock->end();
        //armazena em instOut o set proveniente do bb - copia
        std::set<const Instruction*> instOut(bbLivenessMap.lookup(basicBlock).out);
        //copia de instOut
        std::set<const Instruction*> instIn(instOut);
        
		//LivenessInfo b_info = bbLivenessMap.lookup(basicBlock);

        while (true) {
			/*// in = out - def + use
			b_info.in.clear();
		  
			
			//out - def
			std::set_difference(b_info.out.begin(), b_info.out.end(), b_info.def.begin(), b_info.def.end(),
                              std::inserter(b_info.in, b_info.in.end()));
			//+ use
			b_info.in.insert(b_info.use.begin(), b_info.use.end());

			*/
			// in = out - def + use
			instIn.erase(instruction);
			
          unsigned n = instruction->getNumOperands();
          for (unsigned j = 0; j < n; j++) {
				Value *v = instruction->getOperand(j);
				if (isa<Instruction>(v)){
					instIn.insert(cast<Instruction>(v));
				}
          }

          LivenessInfo liveInfo;
          /*liveInfo.in = b_info.in;
          liveInfo.out = b_info.out;
          liveInfo.def = b_info.def;
          liveInfo.use = b_info.use;*/
          
          liveInfo.in = instIn;
          liveInfo.out = instOut;
          
          iLivenessMap.insert(std::make_pair(&*instruction, liveInfo));

          instOut = instIn;
          //b_info.out = b_info.in;
          
          if (instruction == basicBlock->begin()){
            break;
          }
          --instruction;
        }
      }
	
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
	/*
	for (inst_iterator i = inst_begin(F), E = inst_end(F); i != E; ++i) {
        LivenessInfo livInfo = iLivenessMap.lookup(&*i);
        errs() << "%" << instMap.lookup(&*i) << ": { ";
        std::for_each(livInfo.in.begin(), livInfo.in.end(), print_elem);
        errs() << "} { ";
        std::for_each(livInfo.out.begin(), livInfo.out.end(), print_elem);
        errs() << "}\n";
      }*/
	
    return false;
}

char Liveness::ID = 0;

RegisterPass<Liveness> X("liveness", "Live vars analysis", false, false);


}



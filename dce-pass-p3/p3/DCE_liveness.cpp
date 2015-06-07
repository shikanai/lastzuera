#include "Liveness.h"

//This allows the standard isa/dyncast/cast functionality to work with calls to intrinsic functions.

//Incluimos esse .h para conseguir fazer chamadas de isa sem o compilador reclamar.

#include <llvm/IR/IntrinsicInst.h>



using namespace llvm;

namespace {

   struct DCE : public FunctionPass {

		static char ID;

    	DCE() : FunctionPass(ID) {}



    	virtual bool runOnFunction(Function &F) {

			

			//faz analise de liveness

			Liveness &L = getAnalysis<Liveness>();

			

			bool LivenessCheck = false;

			int aux = 0;

			

			//Agora que temos L preenchido, devemos iterar por todos os basic blocks

			//e por todas as instrucoes, verificando se a instrucao eh trivialmente

			//viva.

			//iterando os basicblocks

			for (Function::iterator basicBlock = F.begin(), end = F.end(); basicBlock != end; ++basicBlock) {

				//iterando as instrucoes

				//troquei instruction por aux, apenas para nao quebrar. estou atualizando instruction dentro do for,

				//para nao destruir a lista

				for (BasicBlock::iterator instruction = basicBlock->begin(), end2 = basicBlock->end(); instruction != end2; ++aux) {

			

					//De acordo com os slides, uma instrucao eh trivialmente viva quando:

					

					//-seu comportamento gerar efeitos colaterias

					if(instruction->mayHaveSideEffects()){

						errs() << "mayHaveSideEffects\n";

						LivenessCheck = true;

					}	

					//-se for um terminador

					if(isa<TerminatorInst>(instruction)){

						errs() << "TerminatorInst\n";

						LivenessCheck = true;

					}

					//-se for uma instrucao de debugging

					if(isa<DbgInfoIntrinsic>(instruction)){

						errs() << "DbgInfoIntrinsic\n";

						LivenessCheck = true;

					}

					//-se for uma instrucao de excecao

					if(isa<LandingPadInst>(instruction)){

						errs() << "LandingPadInst\n";

						LivenessCheck = true;

					}

					

					if(L.isLiveOut(instruction,instruction)){

						errs() << "isLiveOut\n";

						LivenessCheck = true;

					}

					

					//-se for utilizada por outra instrucao viva

					//ToDo: Verificar se eh utilizada por outra instrucao viva.

					//isLiveOut...

					

					

					//Preparando I para ser incrementado e, caso nao seja

					//trivialmente viva, ser removida.

					BasicBlock::iterator itr_aux = instruction;

					instruction++;

					

					//verificando valor de LivenessCheck

					if(!LivenessCheck){

						//se a instrucao nao for trivialmente viva, removemos ela.

						//itr_aux->eraseFromParent();

						errs() << "instruction erased!\n";

						LivenessCheck = false;

					}

				}

			}

			

			//fim da iteracao

			

			errs() << "teste!\n";

			errs() << "Fim!\n";

		}



    	virtual void getAnalysisUsage(AnalysisUsage &AU) const {

			

		AU.addRequired<Liveness>();

		

    	}

   };

    char DCE::ID = 0;



	RegisterPass<DCE> X("DCE", "dead code elimination", false, false);

}


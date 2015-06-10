#include "Liveness.h"



//This allows the standard isa/dyncast/cast functionality to work with calls to intrinsic functions.

//Incluimos esse .h para conseguir fazer chamadas de isa sem o compilador reclamar.

#include <llvm/IR/IntrinsicInst.h>



using namespace llvm;

namespace {

   struct DCE_liveness : public FunctionPass {

		static char ID;

    	DCE_liveness() : FunctionPass(ID) {}



    	virtual bool runOnFunction(Function &F) {

			

			//faz analise de liveness

			Liveness &L = getAnalysis<Liveness>();

			

			//variavel que verifica se a instrucao eh trivialmente viva

			bool LivenessCheck = false;

			int aux = 0;

			bool otimizado = false;

			//a cada otimizacao temos que rodar novamente o algoritmo, para termos

			//certeza de que soh paramos quando ele estiver estabilizado.

			//para isso utilizamos a flag otimizado, que, enquanto houver otimizacoes

			//no codigo, ele fica true. Quando nao houver, ele fica false e sai do loop.

			do{

				

				otimizado = false;

				//Agora que temos L preenchido, devemos iterar por todos os basic blocks

				//e por todas as instrucoes, verificando se a instrucao eh trivialmente

				//viva.

				//iterando os basicblocks

				for (Function::iterator basicBlock = F.begin(), end = F.end(); basicBlock != end; ++basicBlock) {

					//iterando as instrucoes

					//troquei instruction por aux, apenas para nao quebrar. estou atualizando instruction dentro do for,

					//para nao destruir a lista

					for (BasicBlock::iterator instruction = basicBlock->begin(), end2 = basicBlock->end(); instruction != end2; ++aux) {

						

						//reseta LivenessCheck;

						LivenessCheck = false;

						//errs() << "instruction: " << *instruction << "\n"; 

						//De acordo com os slides, uma instrucao eh trivialmente viva quando:

						

						//-seu comportamento gerar efeitos colaterias

						//Return true if the instruction may have side effects. 

						if(instruction->mayHaveSideEffects()){

							errs() << "mayHaveSideEffects\n";

							LivenessCheck = true;

						}	

						

						//The isa<> operator works exactly like the Java “instanceof” operator. 

						//It returns true or false depending on whether a reference or pointer points 

						//to an instance of the specified class. This can be very useful for 

						//constraint checking of various sorts;

						

						//-se for uma instrucao terminator

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

						

						//isLiveOut...

						if(L.isLiveOut(instruction,instruction)){

							errs() << "isLiveOut\n";

							LivenessCheck = true;

						}

						

						//ao executar o teste, percebi que ele estava removendo

						//algumas instrucoes a mais, referentes a alloca...

						//portanto, tratamos essa parte de outra forma...

						//provavelmente isso eh devido a algum erro na analise de liveness...

						if(isa<AllocaInst>(instruction)){

							//errs() << "eh uma instrucao de alocacao!\nVerificando se eh utilizada em outras circunstancias...\n";

							if(!L.isLiveOut(instruction,instruction)){

								//todo: tratar direito isso!!!

								//tirar esse if e tratar embaixo, na parte de outra instrucao viva

								LivenessCheck = true;

								//-se for utilizada por outra instrucao viva

							}

						}

						/*

						for (Function::iterator basicBlock2 = basicBlock; basicBlock2 != end; ++basicBlock2) {

									for (BasicBlock::iterator instruction2 = instruction; instruction2 != end2; ++instruction2) {

										//LivenessCheck = true;

										if(L.isLiveOut(instruction2,instruction)){

											errs() << "encontrei uso da instrucao em algum lugar. nao posso remover\n";

											LivenessCheck = true;

											break;

										}

										//errs() << &*instruction2 << "\n";

									}

									if(LivenessCheck){

										break;

									}

									//errs() << "******" << &*end3 << "\n";

								}

						*/

						

						//verificando valor de LivenessCheck

						if(!LivenessCheck){

							//Preparando instrucao para ser incrementada e, caso nao seja

							//trivialmente viva, ser removida.

							errs() << "instruction***** \n"<< *instruction << "\n*****erasable!!!!\n";

							BasicBlock::iterator itr_aux = instruction;

							instruction++;

							otimizado = true;

							//se a instrucao nao for trivialmente viva, removemos ela.

							itr_aux->eraseFromParent();

							//LivenessCheck = false;

						}else{

							instruction++;

						}

					}

				}

				

			}while(otimizado);

			//fim da iteracao

			

			return true;

		}



    	virtual void getAnalysisUsage(AnalysisUsage &AU) const {

			

		AU.addRequired<Liveness>();

		

    	}

   };

    char DCE_liveness::ID = 0;



	RegisterPass<DCE_liveness> X("DCE_liveness", "dead code elimination", false, false);

}


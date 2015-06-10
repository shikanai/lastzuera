#include "SSA.h"

using namespace std;

/*
 * 
 * Ao implementar a funcao foi deparado com alguns exemplos que quebravam devido a forma com que era feito a remocao.
 * Primeiramente foi utilizado um break no loop apos fazer qualquer remocao, mas isso deixava errado testes onde
 * havia outra instrucao a ser removida no mesmo bloco basico. Diante disso foi criada uma flag que eh levantada
 * toda vez que ha uma remocao, fazendo entao que o loop seja executado novamente em busca de novas instrucoes ainda
 * nao removidas. Isso eh feito ateh que em uma iteracao completa nenhuma instrucao foi removida, ou seja, a flag nao
 * foi levantada.
 * 
 * Foi feito uso da estrutura pronta que a IR do Llvm proporciona, pois ja esta em SSA.
 * 
 * */
 
/*

Algoritmo

while there is some variable v with no uses
	and the statement that defines v has no other side effects
		do delete the statement that defines v

*/

bool SSA::runOnFunction(Function &F){
	
	//#DEBBUG# errs() << "Function (name=" << F.getName() << "\n";
	
	bool modificado = false; //flag de modificacao indicando que um
	
	do {
		//Para varrer do final do programa para o inicio
		Function::iterator i = F.end();
		modificado = false;
		while (i != F.begin()) {
			 --i;
			 
			/* Aqui faco o que preciso */
			/* #DEBBUG#
				errs() << "*******************************Basic block (name=" << i->getName() << ") has "
				 << i->size() << " instructions.\n";
			*/
			/* Itero o bloco basico de baixo para cima. Se uma instrucao xyz de operando KJ e PO esta no final do bloco
			 * e eh eliminada as definicoes de KJ ou de PO podem ficar no estado de tambem ser eliminadas. Como essas
			 * definicoes foram feitas antes, ao iterar do final pro inicio varro ela e a removo, se necessario. */
			for (BasicBlock::reverse_iterator bi = i->rbegin(), be = i->rend(); bi != be; ++bi){
				// #DEBBUG# errs() << "inicio do bloco\n";
				Instruction *v = cast<Instruction*>(bi);
				/* #DEBBUG#
				errs() << *bi << "\n";
				errs() << bi->use_empty()<< "\n";
				*/
				if(bi->use_empty()){	//Verifica se o uso da instrucao eh vazio, ou seja, se ela eh usada em algum lugar
					// #DEBBUG# errs() << "teste se eh usadas\n";
					if(!((isa<TerminatorInst>(&*bi))  || (isa<LandingPadInst>(&*bi)) || (bi->mayHaveSideEffects()) || (isa<DbgInfoIntrinsic>(&*bi)))){ //|| (!isa<DbgInfoIntrinsic>(&*bi))
						// #DEBBUG# errs() << "#################TEMOS UM CASO AQUI\n";
							
							// Ao encontrar uma instrucao sem uso e que nao eh das que precisam ser mantidas prepara-se pra remocao
							BasicBlock::reverse_iterator backup = bi;
							++bi;
							/* #DEBBUG#
							errs() << *bi << "\n";
							errs() << *backup << "\n";
							*/
							bool flag1 = false;
							
							//Busco na ordem normal a instrucao a ser removida e a removo
							for (Function::iterator ii = F.begin(), ei = F.end(); ii != ei; ++ii){
								for (BasicBlock::iterator bii = ii->begin(), bei = ii->end(); bii != bei; ++bii){
									if(bii == *backup){
										/* #DEBBUG#
										errs() << *bii << "\n";
										errs() << *backup << "\n";
										*/
										BasicBlock::iterator Bk_interno = *bii;
										++bii;
										/* #DEBBUG#
										errs() << *bii << "\n";
										errs() << "bkInt" << *Bk_interno << "\n";
										*/
										Bk_interno->eraseFromParent();
										// #DEBBUG# errs() << "OK, FIZ A REMOCAO\n";
										flag1 = true;
										modificado = true;
										break;
									}
								}
							}
							// #DEBBUG# errs() << "SAI DO LOOP\n";
							if(flag1){
								break;
							}
					}
				}
			}
		}
		// #DEBBUG# errs() << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
    }while(modificado);
	//return false;
}





char SSA::ID = 0;

static RegisterPass<SSA> X("ssa", "DCE para SSA", false, false);

/*
 * Refs:
 * 
 * http://stackoverflow.com/questions/3610933/iterating-c-vector-from-the-end-to-the-begin
 * 
 * http://aviral.lab.asu.edu/?p=1699
 * */


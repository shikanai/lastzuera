#include "SSA.h"

using namespace std;



bool SSA::runOnFunction(Function &F){
	//Preciso encontrar uma função que pega o ultimo bloco.
	//for (Function::reverse_iterator i = F.rbegin(); i != F.rend(); ++i ) { 
		//
	//}
	
	Function::iterator i = F.end();
	while (i != F.begin())
	{
		 --i;
		/* Aqui faco o que preciso */
		for (BasicBlock::reverse_iterator bi = i->rbegin(), be = i->rend(); bi != be; ++bi){
			
		}

	}
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


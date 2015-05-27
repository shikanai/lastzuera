#include "SSA.h"

using namespace std;



bool SSA::runOnFunction(Function &F){
	//Preciso encontrar uma função que pega o ultimo bloco.
	//for (Function::reverse_iterator i = F.rbegin(); i != F.rend(); ++i ) { 
		//
	//} 
}





char SSA::ID = 0;

RegisterPass<SSA> X("ssa", "DCE para SSA", false, false);

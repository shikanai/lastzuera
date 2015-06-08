#include "SSA.h"

using namespace std;



bool SSA::runOnFunction(Function &F){
	
	errs() << "Function (name=" << F.getName() << "\n";
	
	Function::iterator i = F.end();
	while (i != F.begin())
	{
		 --i;
		/* Aqui faco o que preciso */
		errs() << "Basic block (name=" << i->getName() << ") has "
             << i->size() << " instructions.\n";
		for (BasicBlock::reverse_iterator bi = i->rbegin(), be = i->rend(); bi != be; ++bi){
			errs() << "inicio do bloco\n";
			Instruction *v = cast<Instruction*>(bi);
			errs() << *bi << "\n";
			errs() << bi->use_empty()<< "\n";
			if(bi->use_empty()){
				errs() << "teste se eh usadas\n";
				if(!((isa<TerminatorInst>(&*bi))  || (isa<LandingPadInst>(&*bi)))){ //|| (!isa<DbgInfoIntrinsic>(&*bi))
					errs() << "TEMOS UM CASO AQUI HAHAHAHAHA\n";
					if(bi->mayHaveSideEffects()){
						errs() << "ops, era store\n";
					} else {
						BasicBlock::reverse_iterator backup = bi;
						++bi;
						for (Function::iterator ii = F.begin(), ei = F.end(); ii != ei; ++ii){
				  
						// blk is a pointer to a BasicBlock instance
							for (BasicBlock::iterator bii = ii->begin(), bei = ii->end(); bii != bei; ++bii){
								if(bii == *backup){
									BasicBlock::iterator Bk_interno = bii;
									++Bk_interno;
									Bk_interno->eraseFromParent();
									errs() << "OK, FIZ A REMOCAO\n";
								}
							}
						}
						errs() << "SAI DO LOOP\n";
					}
				}
			}
		}
	}
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


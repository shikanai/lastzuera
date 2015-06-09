#include "SSA.h"

using namespace std;



bool SSA::runOnFunction(Function &F){
	
	////errs() << "Function (name=" << F.getName() << "\n";
	
	Function::iterator i = F.end();
	while (i != F.begin())
	{
		 --i;
		/* Aqui faco o que preciso */
		errs() << "*******************************Basic block (name=" << i->getName() << ") has "
             << i->size() << " instructions.\n";
		for (BasicBlock::reverse_iterator bi = i->rbegin(), be = i->rend(); bi != be; ++bi){
			errs() << "inicio do bloco\n";
			Instruction *v = cast<Instruction*>(bi);
			errs() << *bi << "\n";
			errs() << bi->use_empty()<< "\n";
			if(bi->use_empty()){
				errs() << "teste se eh usadas\n";
				if(!((isa<TerminatorInst>(&*bi))  || (isa<LandingPadInst>(&*bi)) || (bi->mayHaveSideEffects()) || (isa<DbgInfoIntrinsic>(&*bi)))){ //|| (!isa<DbgInfoIntrinsic>(&*bi))
					errs() << "#################TEMOS UM CASO AQUI\n";
					
						BasicBlock::reverse_iterator backup = bi;
						++bi;
						errs() << *bi << "\n";
						errs() << *backup << "\n";
						bool flag1 = false;
						for (Function::iterator ii = F.begin(), ei = F.end(); ii != ei; ++ii){
				  
						// blk is a pointer to a BasicBlock instance
							for (BasicBlock::iterator bii = ii->begin(), bei = ii->end(); bii != bei; ++bii){
								if(bii == *backup){
									errs() << *bii << "\n";
									errs() << *backup << "\n";
									BasicBlock::iterator Bk_interno = *bii;
									++bii;
									errs() << *bii << "\n";
									errs() << "bkInt" << *Bk_interno << "\n";
									Bk_interno->eraseFromParent();
									errs() << "OK, FIZ A REMOCAO\n";
									flag1 = true;
									break;
								}
							}
							
						}
						errs() << "SAI DO LOOP\n";
						if(flag1){
							break;
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


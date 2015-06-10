
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Instructions.h"
#include <set>
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/IR/ValueMap.h"
//This allows the standard isa/dyncast/cast functionality to work with calls to intrinsic functions.
//Incluimos esse .h para conseguir fazer chamadas de isa sem o compilador reclamar.
#include <llvm/IR/IntrinsicInst.h>

using namespace llvm;

namespace llvm {

//alterei o tipo dos elementos de LivenessInfo, para conseguir utilizar
//com a estrutura implementada em bbInOut
struct LivenessInfo {
    std::set<const Instruction *> use;
    std::set<const Instruction *> def;
    std::set<const Instruction *> in;
    std::set<const Instruction *> out;
};

class Liveness: public FunctionPass {
private:

    DenseMap<const Instruction*, LivenessInfo> iLivenessMap;
    DenseMap<const BasicBlock*, LivenessInfo> bbLivenessMap;
    DenseMap<const Instruction*, int> instMap;


public:
	static char ID; 
	Liveness() : FunctionPass(ID) {}

	virtual bool runOnFunction(Function &F);
    void computeBBDefUse(Function &F);
    void computeBBInOut(Function &F);

    void computeIInOut(Function &F);
    bool isLiveOut(Instruction *I, Instruction *V);
    void addToMap(Function &F);

	virtual void getAnalysisUsage(AnalysisUsage &AU) const{
		AU.setPreservesAll();
	}



};

}



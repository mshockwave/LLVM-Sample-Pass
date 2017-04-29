#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Pass.h>
using namespace llvm;

namespace {

    class RedundantStore : public FunctionPass {
        BasicBlock* entry_block_;

        bool runOnFunction(Function& F) override ;
        AllocaInst* addAlloca(Type* Ty);

    public:
        static char ID;
        RedundantStore() :
                FunctionPass(ID),
                entry_block_(nullptr){}
    };

} // anonymous namespace

AllocaInst* RedundantStore::addAlloca(Type* Ty) {
    assert(entry_block_ && "Entry block not initialized");
    auto* AI = new AllocaInst(Ty);
    AI->insertBefore(&entry_block_->front());
    return AI;
}

bool RedundantStore::runOnFunction(Function &F) {
    entry_block_ = &F.getEntryBlock();
    errs() << "Working on function " << F.getName() << '\n';
    bool modify = false;

    for(auto& BB : F){
        for(auto I = BB.begin(), E = BB.end(); I != E; ++I){
            auto& Inst = *I;
            if(auto* BOp = dyn_cast<BinaryOperator>(&Inst)){
                /**
                 * Although BOp seems to be an instruction
                 * It is also extended from the llvm::Value class.
                 * Thus, BOp itself, also represents the output result
                 * of its operation!
                 *
                 * In another word, if BOp represents 'C = A + B' instruction
                 * It also represents the result, 'C'
                 * **/
                Value* output = dyn_cast<Value>(BOp);

                Value* memSpace =
                        dyn_cast<Value>(addAlloca(output->getType()));

                LoadInst* LI = new LoadInst(memSpace);
                /**
                 * LI is an instruction, but it also represents
                 * the loaded result!
                 *
                 * In another word, if LI represents 'R = load 0x123' instruction
                 * It also represents the result, 'R'
                 * **/
                Value* alsoOutput = dyn_cast<Value>(LI);
                output->replaceAllUsesWith(alsoOutput);

                StoreInst* SI = new StoreInst(output, memSpace);
                SI->insertAfter(&Inst);
                ++I;

                LI->insertAfter(SI);
                ++I;

                modify |= true;
            }
            //BB.dump();
        }
    }

    return modify;
}

char RedundantStore::ID = 0;
static RegisterPass<RedundantStore> Dummy("redundant-store",
                                          "Toy pass the stores every variable into mem and load it back",
                                          false /*Only Look At CFG*/,
                                          false /*Is Analysis Pass*/);




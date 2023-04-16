#include <llvm/IR/CFG.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/GraphWriter.h>
#include <unistd.h>
#include <deque>
#include "RefcntPass.h"
#include "common/config.h"
#include "printer/CFGDrawer.hpp"

void print_one_entry(Instruction *inst, int idx) {
    errs() << "\t\t\U0001F504 ";
    errs() << "#" << idx;
    errs() << " " << inst->getParent()->getParent()->getName();
    inst->getParent();
    errs() << " : " << *inst << "\n";
}

void RefcntPass::init_pass() {
    // TODO: initialize pass data structures and use json to configure
    Parser::JsonParser parser;
    if (parser.parse("./settings.json")) {
        outs() << "use settings.json to configure\n";
    } else {
        outs() << "use default settings";
    }
    params = parser.anaParams;

#ifdef DEBUG

    outs() << raw_fd_ostream::YELLOW
           << "initialization done\n"
           << raw_fd_ostream::RESET;
#endif
}

void RefcntPass::refcntAnalysis(Function *fun_entry) {
    // start from the entry function,
    //
    // TODO: do additional set up before traversing entry

    // preparation finished
    if (params.analysesMode == "inter") {
        interAnalysis(fun_entry);
    } else {
        intraAnalysis(fun_entry);
    }
}

void RefcntPass::intraAnalysis(Function *cur_func) {
    // TODO: do interprocedure analysis for cur func
#ifdef DEBUG
    printCFG(cur_func);
#endif
    // mainly focus on
    if (cur_func->empty())
        return;
    // prepare to traverse CFG
    using std::deque, std::set;
    deque<BasicBlock *> workList;
    set<BasicBlock *> visited;
    // we do forward analysis
    workList.emplace_back(&cur_func->getEntryBlock());
    while (!workList.empty()) {
        // currently analyzed block
        BasicBlock *block = workList.front();
        workList.pop_front();
        if (visited.find(block) != visited.end()) continue;
        visited.insert(block);

        for (const Instruction &inst: *block) {
            unsigned op = inst.getOpcode();
            switch (op) {
                // TODO: discuss which instructions we care about most
                case Instruction::Call: {
                    auto *call = dyn_cast<CallInst>(&inst);
                    llvm::Function *calledFunction = call->getCalledFunction();
#ifdef DEBUG
                    llvm::Value *receiverValue = call->getArgOperand(0);
                    std::string receiverName = receiverValue->getName().str();
                    auto name = calledFunction->getName();
                    outs() << raw_fd_ostream::YELLOW
                           << "receiver: " << receiverValue << " call method " << name << "\n"
                           << raw_fd_ostream::RESET;
                    for (Argument &arg: calledFunction->args()) {
                        outs() << "argument: " << arg.getName().data() << '\n';
                    }
#endif
                    // TODO: discuss how to report bugs found below
                    if (name == INCREF_STR) {

                    } else if (name == XINCREF_STR) {

                    } else if (name == DECREF_STR) {

                    } else if (name == XDECREF_STR) {

                    } else {
                        // TODO: check if it's a special API, aka in retStrongAPI/retWeakAPI/argBorrowAPI/argStealAPI
                        // suggest using another function to do so which processes the api's behavior
                    }
                    break;
                }
                default:
                    break;
            }
        }

        for (BasicBlock *succ: successors(block)) {
            if (/*some condition*/1) {
                workList.emplace_back(succ);
            }
        }
    }
    CheckBeforeRet();
}

void RefcntPass::interAnalysis(Function *cur_func) {
    // TODO: do intraprocedure analysis for cur func, may call intraAnalysis
}

void RefcntPass::CheckBeforeRet() {
}

bool RefcntPass::runOnModule(Module &M) {

    init_pass();
    Function *entry_func = nullptr;
    Function *main_func = nullptr;
    // looping through functions to find the main functions.
    for (auto &f: M) {
        auto f_name = f.getName();
        if (!strcmp(f_name.data(), params.entryFunction.c_str())) {
            entry_func = &f;
            break;
        } else if (strcmp(f_name.data(), "main") == 0) {
            main_func = &f;
        }
    }
    if (entry_func == nullptr) {
        if (main_func != nullptr) {
            outs() << "entry function is invalid, use default(main) as entry\n";
            entry_func = main_func;
        } else
            errs() << "no entry or main function is determined\n";
    }
    outs() << "====================analyses starts=======================\n";
    if (entry_func != nullptr) {
// Ok. we have main function now.
#ifdef DEBUG
        // FIXME: abstract debug into one function
        outs() << raw_fd_ostream::YELLOW
               << "Analysis begin, analysis mode: " << params.analysesMode << ", entry function: "
               << params.entryFunction << "\n"
               << raw_fd_ostream::RESET;
#endif
        refcntAnalysis(entry_func);
    }

    outs() << "=====================analyses ends========================\n";
    // We do not modify the code, so return false.
    return false;
}

RCFact::Fact RefcntPass::getResult() {

}

char RefcntPass::ID = 0;

static RegisterPass<RefcntPass> X("refcnt", "refcount Analysis",
                                  false, false);

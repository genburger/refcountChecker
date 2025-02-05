//
// Created by ziqi on 2023/4/17.
//

#include "Tool.h"
#include "config.h"

bool Tool::setParam() {
    JsonParser parser;
    // FIXME: settings path
    bool err = parser.parse(PROJECT_ROOT + "settings.json");
    if (!err)return false;
    params = parser.params;
    return true;
}

int Tool::run() {
    if (!setParam())return 1;
    // generate IR
    std::string ircmd(params.clangPath + ' ');
    for (const auto &arg: params.args) {
        ircmd.append(arg + ' ');
    }
    ircmd.append(params.modulePath + ' ');
    std::string ll_path = params.modulePath + ".ll";
    ircmd.append("-o " + ll_path);
    if (system(ircmd.c_str()) != 0)return 1;
    std::string opt_cmd(
            "opt -load ../lib/libRefcntAnalysis.so -refcnt --basic-aa -aa-eval -print-all-alias-modref-info ");
    //-steens-aa
    opt_cmd.append(ll_path + ' ');
    opt_cmd.append("-enable-new-pm=0");
    //change /dev/null to print out modified ir:
    // https://stackoverflow.com/questions/29758987/using-llvm-opt-with-built-in-passes
    // analysis begins
    return system(opt_cmd.c_str());
}

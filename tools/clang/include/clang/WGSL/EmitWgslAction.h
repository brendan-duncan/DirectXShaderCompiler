//===-- EmitWgslAction.h - FrontendAction for Emitting WGSL --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_CLANG_WGSL_EMITWGSLACTION_H
#define LLVM_CLANG_WGSL_EMITWGSLACTION_H

#include "clang/Frontend/FrontendAction.h"

namespace clang {

class EmitWgslAction : public ASTFrontendAction {
public:
  EmitWgslAction() {}

protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef InFile) override;
};

} // end namespace clang

#endif

//===--- EmitWgslAction.cpp - EmitWGSLAction implementation -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "clang/WGSL/EmitWgslAction.h"

#include "WgslEmitter.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/ADT/STLExtras.h"

namespace clang {

std::unique_ptr<ASTConsumer>
EmitWgslAction::CreateASTConsumer(CompilerInstance &CI, StringRef InFile) {
  return llvm::make_unique<wgsl::WgslEmitter>(CI);
}
} // end namespace clang

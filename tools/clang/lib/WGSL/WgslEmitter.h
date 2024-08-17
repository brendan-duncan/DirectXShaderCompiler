//===------- SpirvEmitter.h - SPIR-V Binary Code Emitter --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//===----------------------------------------------------------------------===//
//
//  This file defines a SPIR-V emitter class that takes in HLSL AST and emits
//  SPIR-V binary words.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_WGSL_WGSLMITTER_H
#define LLVM_CLANG_LIB_WGSL_WGSLMITTER_H

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"

namespace clang {
namespace wgsl {

class WgslEmitter : public ASTConsumer {
public:
  WgslEmitter(CompilerInstance &ci);

  void HandleTranslationUnit(ASTContext &context) override;

private:
  void doDecl(const Decl *decl);

  /// \brief Entry function name, derived from the command line
  /// and should be const.
  const llvm::StringRef hlslEntryFunctionName;
};

} // end namespace wgsl
} // end namespace clang

#endif // LLVM_CLANG_LIB_WGSL_WGSLMITTER_H

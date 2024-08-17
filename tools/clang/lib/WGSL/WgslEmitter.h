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
#include "dxc/DXIL/DxilShaderModel.h"

namespace clang {
namespace wgsl {

class WgslEmitter : public ASTConsumer {
public:
  WgslEmitter(CompilerInstance &ci);

  void HandleTranslationUnit(ASTContext &context) override;

private:
  void addFunctionToWorkQueue(hlsl::DXIL::ShaderKind shaderKind,
                              const clang::FunctionDecl *fnDecl,
                              bool isEntryFunction);

  void doDecl(const Decl *decl);

  hlsl::ShaderModel::Kind getShaderModelKind(StringRef stageName) const;

  /// \brief Structure to maintain record of all entry functions and any
  /// reachable functions.
  struct FunctionInfo {
  public:
    hlsl::ShaderModel::Kind shaderModelKind;
    const DeclaratorDecl *funcDecl;
    //WgslFunction *entryFunction;
    bool isEntryFunction;

    FunctionInfo() = default;

    FunctionInfo(hlsl::ShaderModel::Kind smk, const DeclaratorDecl *fDecl,
                 /*WgslFunction *entryFunc,*/ bool isEntryFunc)
        : shaderModelKind(smk)
        , funcDecl(fDecl)
        //, entryFunction(entryFunc)
        , isEntryFunction(isEntryFunc) {}
  };

  hlsl::ShaderModel::Kind shaderModelKind;

  /// \brief Entry function name, derived from the command line
  /// and should be const.
  const llvm::StringRef hlslEntryFunctionName;

  /// \brief A map of funcDecl to its FunctionInfo. Consists of all entry
  /// functions followed by all reachable functions from the entry functions.
  llvm::DenseMap<const DeclaratorDecl *, FunctionInfo *> functionInfoMap;

  /// A queue of FunctionInfo reachable from all the entry functions.
  std::vector<const FunctionInfo *> workQueue;
};

} // end namespace wgsl
} // end namespace clang

#endif // LLVM_CLANG_LIB_WGSL_WGSLMITTER_H

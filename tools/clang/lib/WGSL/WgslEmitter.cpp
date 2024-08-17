//===------- WgslEmitter.cpp - WGSL Code Emitter ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//===----------------------------------------------------------------------===//
//
//  This file implements a WGSL emitter class that takes in HLSL AST and emits
//  WebGPU WGSL shader code.
//
//===----------------------------------------------------------------------===//

#include "WgslEmitter.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/HlslTypes.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/Type.h"
#include "clang/Sema/Sema.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/Casting.h"

namespace clang {
namespace wgsl {

WgslEmitter::WgslEmitter(CompilerInstance &ci)
  : hlslEntryFunctionName(ci.getCodeGenOpts().HLSLEntryFunction) {
}

void WgslEmitter::HandleTranslationUnit(ASTContext &context) {
  TranslationUnitDecl *tu = context.getTranslationUnitDecl();
  uint32_t numEntryPoints = 0;

  // The entry function is the seed of the queue.
  for (auto *decl : tu->decls()) {
    if (auto *funcDecl = dyn_cast<FunctionDecl>(decl)) {
      const bool isPrototype = !funcDecl->isThisDeclarationADefinition();
      if (funcDecl->getName() == hlslEntryFunctionName && !isPrototype) {
        //addFunctionToWorkQueue(spvContext.getCurrentShaderModelKind(),
        //                       funcDecl, /*isEntryFunction*/ true);
        numEntryPoints++;
      } else {
        doDecl(decl);
      }
    }
  }
}

void WgslEmitter::doDecl(const Decl *decl) {
  if (isa<EmptyDecl>(decl) || isa<TypeAliasTemplateDecl>(decl) ||
      isa<VarTemplateDecl>(decl)) {
    return;
  }

  // Implicit decls are lazily created when needed.
  if (decl->isImplicit()) {
    return;
  }

  if (const auto *varDecl = dyn_cast<VarDecl>(decl)) {
    //doVarDecl(varDecl);
  } else if (const auto *namespaceDecl = dyn_cast<NamespaceDecl>(decl)) {
    for (auto *subDecl : namespaceDecl->decls())
      // Note: We only emit functions as they are discovered through the call
      // graph starting from the entry-point. We should not emit unused
      // functions inside namespaces.
      if (!isa<FunctionDecl>(subDecl)) {
        doDecl(subDecl);
      }
  } else if (const auto *funcDecl = dyn_cast<FunctionDecl>(decl)) {
    //doFunctionDecl(funcDecl);
  } else if (const auto *bufferDecl = dyn_cast<HLSLBufferDecl>(decl)) {
    //doHLSLBufferDecl(bufferDecl);
  } else if (const auto *recordDecl = dyn_cast<RecordDecl>(decl)) {
    //doRecordDecl(recordDecl);
  } else if (const auto *enumDecl = dyn_cast<EnumDecl>(decl)) {
    //doEnumDecl(enumDecl);
  } else if (const auto *classTemplateDecl =
                 dyn_cast<ClassTemplateDecl>(decl)) {
    //doClassTemplateDecl(classTemplateDecl);
  } else if (isa<TypedefNameDecl>(decl)) {
    //declIdMapper.recordsSpirvTypeAlias(decl);
  } else if (isa<FunctionTemplateDecl>(decl)) {
    // nothing to do.
  } else if (isa<UsingDecl>(decl)) {
    // nothing to do.
  } else if (isa<UsingDirectiveDecl>(decl)) {
    // nothing to do.
  } else {
    //emitError("decl type %0 unimplemented", decl->getLocation()) << decl->getDeclKindName();
  }
}

} // end namespace wgsl
} // end namespace clang

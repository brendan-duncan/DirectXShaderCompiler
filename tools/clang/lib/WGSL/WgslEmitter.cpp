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
  // Get ShaderModel from command line hlsl profile option.
  const hlsl::ShaderModel *shaderModel =
      hlsl::ShaderModel::GetByName(ci.getCodeGenOpts().HLSLProfile.c_str());

  shaderModelKind = shaderModel->GetKind();
}

void WgslEmitter::HandleTranslationUnit(ASTContext &context) {
  TranslationUnitDecl *tu = context.getTranslationUnitDecl();
  uint32_t numEntryPoints = 0;

  // The entry function is the seed of the queue.
  for (auto *decl : tu->decls()) {
    if (auto *funcDecl = dyn_cast<FunctionDecl>(decl)) {
      if (shaderModelKind == hlsl::ShaderModel::Kind::Library) {
        if (const auto *shaderAttr = funcDecl->getAttr<HLSLShaderAttr>()) {
          // If we are compiling as a library then add everything that has a
          // ShaderAttr.
          addFunctionToWorkQueue(getShaderModelKind(shaderAttr->getStage()),
                                 funcDecl, /*isEntryFunction*/ true);
          numEntryPoints++;
        } else if (funcDecl->getAttr<HLSLExportAttr>()) {
          addFunctionToWorkQueue(shaderModelKind, funcDecl, /*isEntryFunction*/ false);
        }
      } else {
        const bool isPrototype = !funcDecl->isThisDeclarationADefinition();
        if (funcDecl->getName() == hlslEntryFunctionName && !isPrototype) {
            addFunctionToWorkQueue(shaderModelKind,
                                funcDecl, /*isEntryFunction*/ true);
            numEntryPoints++;
        } else {
            doDecl(decl);
        }
      }
    }
  }
}

void WgslEmitter::addFunctionToWorkQueue(hlsl::DXIL::ShaderKind shaderKind,
                                         const clang::FunctionDecl *fnDecl,
                                         bool isEntryFunction) {
  // Only update the workQueue and the function info map if the given
  // FunctionDecl hasn't been added already.
  if (functionInfoMap.find(fnDecl) == functionInfoMap.end()) {
    // Note: The function is just discovered and is being added to the
    // workQueue, therefore it does not have the entryFunction SPIR-V
    // instruction yet (use nullptr).
    auto *fnInfo = new FunctionInfo(shaderKind, fnDecl, 
        ///*entryFunction*/ nullptr,
        isEntryFunction);
    functionInfoMap[fnDecl] = fnInfo;
    workQueue.push_back(fnInfo);
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

hlsl::ShaderModel::Kind WgslEmitter::getShaderModelKind(StringRef stageName) const {
  hlsl::ShaderModel::Kind SMK =
      llvm::StringSwitch<hlsl::ShaderModel::Kind>(stageName)
          .Case("pixel", hlsl::ShaderModel::Kind::Pixel)
          .Case("vertex", hlsl::ShaderModel::Kind::Vertex)
          .Case("geometry", hlsl::ShaderModel::Kind::Geometry)
          .Case("hull", hlsl::ShaderModel::Kind::Hull)
          .Case("domain", hlsl::ShaderModel::Kind::Domain)
          .Case("compute", hlsl::ShaderModel::Kind::Compute)
          .Case("raygeneration", hlsl::ShaderModel::Kind::RayGeneration)
          .Case("intersection", hlsl::ShaderModel::Kind::Intersection)
          .Case("anyhit", hlsl::ShaderModel::Kind::AnyHit)
          .Case("closesthit", hlsl::ShaderModel::Kind::ClosestHit)
          .Case("miss", hlsl::ShaderModel::Kind::Miss)
          .Case("callable", hlsl::ShaderModel::Kind::Callable)
          .Case("mesh", hlsl::ShaderModel::Kind::Mesh)
          .Case("amplification", hlsl::ShaderModel::Kind::Amplification)
          .Default(hlsl::ShaderModel::Kind::Invalid);
  assert(SMK != hlsl::ShaderModel::Kind::Invalid);
  return SMK;
}

} // end namespace wgsl
} // end namespace clang

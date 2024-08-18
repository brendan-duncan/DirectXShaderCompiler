//===-- WgslContext.h - Context holding WGSL codegen data ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_CLANG_WGSL_WGSLCONTEXT_H
#define LLVM_CLANG_WGSL_WGSLCONTEXT_H

#include "dxc/DXIL/DxilShaderModel.h"

namespace clang {
namespace wgsl {

class WgslContext {
public:
  using ShaderModelKind = hlsl::ShaderModel::Kind;

  WgslContext();

  // Forbid copy construction and assignment
  WgslContext(const WgslContext &) = delete;
  WgslContext &operator=(const WgslContext &) = delete;

  // Forbid move construction and assignment
  WgslContext(WgslContext &&) = delete;
  WgslContext &operator=(WgslContext &&) = delete;

  /// Functions to get/set current entry point ShaderModelKind.
  ShaderModelKind getCurrentShaderModelKind() { return curShaderModelKind; }
  void setCurrentShaderModelKind(ShaderModelKind smk) {
    curShaderModelKind = smk;
  }

  /// Functions to get/set hlsl profile version.
  uint32_t getMajorVersion() const { return majorVersion; }
  void setMajorVersion(uint32_t major) { majorVersion = major; }
  uint32_t getMinorVersion() const { return minorVersion; }
  void setMinorVersion(uint32_t minor) { minorVersion = minor; }

  /// Functions to query current entry point ShaderModelKind.
  bool isPS() const { return curShaderModelKind == ShaderModelKind::Pixel; }
  bool isVS() const { return curShaderModelKind == ShaderModelKind::Vertex; }
  bool isGS() const { return curShaderModelKind == ShaderModelKind::Geometry; }
  bool isHS() const { return curShaderModelKind == ShaderModelKind::Hull; }
  bool isDS() const { return curShaderModelKind == ShaderModelKind::Domain; }
  bool isCS() const { return curShaderModelKind == ShaderModelKind::Compute; }
  bool isLib() const { return curShaderModelKind == ShaderModelKind::Library; }
  bool isRay() const {
    return curShaderModelKind >= ShaderModelKind::RayGeneration &&
           curShaderModelKind <= ShaderModelKind::Callable;
  }
  bool isMS() const { return curShaderModelKind == ShaderModelKind::Mesh; }
  bool isAS() const {
    return curShaderModelKind == ShaderModelKind::Amplification;
  }

private:
  // Current ShaderModelKind for entry point.
  ShaderModelKind curShaderModelKind;
  // Major/Minor hlsl profile version.
  uint32_t majorVersion;
  uint32_t minorVersion;
};

} // end namespace wgsl
} // end namespace clang

#endif // LLVM_CLANG_WGSL_WGSLCONTEXT_H

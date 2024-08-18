//===--- WgslContext.cpp - WGSL WgslContext implementation-------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "clang/WGSL/WgslContext.h"

namespace clang {
namespace wgsl {

WgslContext::WgslContext() 
  : curShaderModelKind(hlsl::ShaderModel::Kind::Invalid) {
}

} // end namespace wgsl
} // end namespace clang

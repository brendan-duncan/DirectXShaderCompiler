set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_library(clangWGSL
  EmitWgslAction.cpp
  WgslContext.cpp
  WgslEmitter.cpp
)

#target_include_directories(clangWGSL PUBLIC ${SPIRV_HEADER_INCLUDE_DIR})
#target_include_directories(clangWGSL PRIVATE ${SPIRV_TOOLS_INCLUDE_DIR})
add_dependencies(clangWGSL TablegenHLSLOptions)

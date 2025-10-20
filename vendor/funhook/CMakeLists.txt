set(SRCS
    include/FunHook.h
    include/MemUtils.h
    include/Traits.h
	include/Typedefs.h
	include/FunctionPointer.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

add_library(funhook STATIC ${SRCS})
target_compile_features(funhook PUBLIC cxx_std_20)
set_target_properties(funhook PROPERTIES CXX_EXTENSIONS NO)
set_target_properties(funhook PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(funhook PUBLIC SUBHOOK_STATIC)

target_include_directories(funhook PUBLIC
    include
    ${CMAKE_SOURCE_DIR}/vendor/subhook
)

target_link_libraries(funhook subhook)

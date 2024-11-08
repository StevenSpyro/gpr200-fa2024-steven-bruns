file(
 GLOB_RECURSE GPR-200-FINAL_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE GPR-200-FINAL_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this gpr-200-final's asset folder to bin when it is built
add_custom_target(copyAssetsAF ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${GPR-200-FINAL_INC} DESTINATION include/gpr-200-final)
add_executable(gpr-200-final ${GPR-200-FINAL_SRC} ${GPR-200-FINAL_INC})
target_link_libraries(gpr-200-final PUBLIC core IMGUI glm)
target_include_directories(gpr-200-final PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when gpr-200-final is built
add_dependencies(gpr-200-final copyAssetsA5)
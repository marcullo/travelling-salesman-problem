function(print_header)
    message("*")
    message("* ${PROJECT_NAME}")
    message("*")
    message("*      Build type: ${CMAKE_BUILD_TYPE}")
    message("*       Generator: ${CMAKE_GENERATOR}")
    message("*")
endfunction()

function(print_footer)
    message("*")
endfunction()

function(print_configuration target flags compiler compiler_version languages standard)
    message("*          Target: ${target}")
    message("*           Flags: ${flags}")
    message("*        Compiler: ${compiler} ${compiler_version}")
    message("*     Language(s): ${languages} ${standard}")
    message("*")
endfunction()
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)


project(TemplateModule) #1
set(python_wrapper_name template.py) # 4
set(lib_name libtemplate) # .cpp file name without extension


find_package(Python REQUIRED COMPONENTS Development)


add_library(${lib_name} MODULE src/${lib_name}.cpp)
set_target_properties(${lib_name} PROPERTIES PREFIX "" OUTPUT_NAME ${lib_name})
target_link_libraries(${lib_name} PRIVATE Python::Python)

execute_process(
    COMMAND python3 -m site --user-site
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set_target_properties(${lib_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

set(pywrapper_source_dir ${CMAKE_SOURCE_DIR}/python/${python_wrapper_name})
set(pywrapper_dist_dir ${CMAKE_SOURCE_DIR}/dist)
configure_file(${pywrapper_source_dir} ${pywrapper_dist_dir}/${python_wrapper_name} COPYONLY)

#set_target_properties(template PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PYTHON_SITE_PACKAGES})



#set_target_properties(factorial PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/python)

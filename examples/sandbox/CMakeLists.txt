add_subdirectory(../third_party "${CMAKE_CURRENT_BINARY_DIR}/third_party")

add_executable(vinyasa_sandbox main.c)
target_link_libraries(vinyasa_sandbox PRIVATE vinyasa::vinyasa vinyasa_example_chitra)
target_compile_definitions(vinyasa_sandbox PRIVATE
    VINYASA_SANDBOX_FONT="$<TARGET_FILE_DIR:vinyasa_sandbox>/assets/Roboto-Regular.ttf"
)
set_target_properties(vinyasa_sandbox PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

add_custom_command(TARGET vinyasa_sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:vinyasa_sandbox>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/chitra/sandbox/Font/Roboto-Regular.ttf"
        "$<TARGET_FILE_DIR:vinyasa_sandbox>/assets/Roboto-Regular.ttf"
    VERBATIM
)

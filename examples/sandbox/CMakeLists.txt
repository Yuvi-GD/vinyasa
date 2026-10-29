add_executable(vinyasa_sandbox main.c)
target_link_libraries(vinyasa_sandbox PRIVATE vinyasa::vinyasa)
set_target_properties(vinyasa_sandbox PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

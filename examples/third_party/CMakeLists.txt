set(CHITRA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/chitra")

if(NOT EXISTS "${CHITRA_DIR}/include/chitra.h" OR
   NOT EXISTS "${CHITRA_DIR}/third_party/sokol/sokol_gfx.h" OR
   NOT EXISTS "${CHITRA_DIR}/third_party/sokol-tools/README.md")
    message(FATAL_ERROR
        "Initialize the sandbox dependencies from the Vinyasa directory: "
        "git submodule update --init --recursive -- examples/third_party/chitra"
    )
endif()

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(CHITRA_SHDC_PLATFORM win32)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(CHITRA_SHDC_PLATFORM osx)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(CHITRA_SHDC_PLATFORM linux)
else()
    message(FATAL_ERROR "Unsupported shader compiler host: ${CMAKE_HOST_SYSTEM_NAME}")
endif()
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(arm64|ARM64|aarch64)$" AND NOT CMAKE_HOST_WIN32)
    string(APPEND CHITRA_SHDC_PLATFORM "_arm64")
endif()
find_program(VINYASA_SOKOL_SHDC
    NAMES sokol-shdc sokol-shdc.exe
    PATHS "${CHITRA_DIR}/third_party/sokol-tools/bin/${CHITRA_SHDC_PLATFORM}"
    NO_DEFAULT_PATH REQUIRED
)

set(CHITRA_SHADER_HEADERS)
foreach(shader_name IN ITEMS chitra_shape chitra_text)
    set(shader_source "${CHITRA_DIR}/src/backends/shaders/${shader_name}.glsl")
    set(shader_header "${CMAKE_CURRENT_BINARY_DIR}/shaders/gen/${shader_name}.glsl.h")
    add_custom_command(
        OUTPUT "${shader_header}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders/gen"
        COMMAND "${VINYASA_SOKOL_SHDC}" -i "${shader_source}" -o "${shader_header}"
            -l glsl410:hlsl5:metal_macos
        DEPENDS "${shader_source}" "${VINYASA_SOKOL_SHDC}"
        COMMENT "Generating Chitra shader: ${shader_name}"
        VERBATIM
    )
    list(APPEND CHITRA_SHADER_HEADERS "${shader_header}")
endforeach()

add_library(vinyasa_example_chitra STATIC
    ${CHITRA_SHADER_HEADERS}
    "${CHITRA_DIR}/src/chitra.c"
    "${CHITRA_DIR}/src/chitra_table.c"
    "${CHITRA_DIR}/src/chitra_shape.c"
    "${CHITRA_DIR}/src/chitra_texture.c"
    "${CHITRA_DIR}/src/chitra_font.c"
    "${CHITRA_DIR}/src/chitra_batch.c"
    "${CHITRA_DIR}/src/chitra_clip.c"
    "${CHITRA_DIR}/src/backends/sokol_hal.c"
    "${CHITRA_DIR}/src/backends/sokol_rhi.c"
    "${CHITRA_DIR}/src/backends/sokol_impl.c"
)
target_include_directories(vinyasa_example_chitra
    PUBLIC "${CHITRA_DIR}/include" "${CHITRA_DIR}/third_party/sokol"
    PRIVATE "${CHITRA_DIR}/third_party/fontstash" "${CHITRA_DIR}/third_party/stb"
        "${CMAKE_CURRENT_BINARY_DIR}"
)
set_target_properties(vinyasa_example_chitra PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
)

if(WIN32)
    target_compile_definitions(vinyasa_example_chitra PUBLIC SOKOL_D3D11)
    target_link_libraries(vinyasa_example_chitra PRIVATE d3d11 dxgi dxguid user32 gdi32 shell32 dwmapi)
elseif(APPLE)
    enable_language(OBJC)
    set_source_files_properties("${CHITRA_DIR}/src/backends/sokol_impl.c" PROPERTIES LANGUAGE OBJC)
    target_compile_definitions(vinyasa_example_chitra PUBLIC SOKOL_METAL)
    target_link_libraries(vinyasa_example_chitra PRIVATE
        "-framework Cocoa" "-framework QuartzCore" "-framework Metal" "-framework MetalKit"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(vinyasa_example_chitra PUBLIC SOKOL_GLCORE)
    target_link_libraries(vinyasa_example_chitra PRIVATE X11 Xi Xcursor GL pthread dl m)
else()
    message(FATAL_ERROR "The Chitra sandbox supports Windows, macOS, and Linux desktop builds.")
endif()

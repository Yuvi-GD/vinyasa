# Vinyasa

Vinyasa is a standalone UI layout engine designed to calculate element sizes and positions with low CPU and memory overhead. Applications provide a hierarchy, available space, layout rules, and content measurements. Vinyasa calculates the geometry used to draw the interface.

## Layout model

Layout is built around elements, containers, and slots:

- **Element:** an item with its own sizing settings and optional measurable content, such as text or an image.
- **Container:** the behavior that measures and arranges an element's children.
- **Slot:** the settings for a child within its parent container, including any supported overrides.

An element can contain both content and children. Containers can be nested, and each container interprets its own slot settings. A row might use a slot to specify how a child shares available space; a grid might use it to specify which cells the child occupies.

## Measurement and arrangement

Measurement determines size requirements under supplied constraints. Arrangement allocates space and positions children according to the container's rules, the children's sizing settings, and their slots.

Content measurements come from the application. For text, the application measures using its own font and text system. If the available width changes, that measurement can return a different height. Vinyasa uses the result when calculating the surrounding layout.

## Integration and extension

The application owns the hierarchy and chooses which root to calculate and when. This allows the same layout model to serve immediate and retained interfaces. Rendering, input handling, and tracking changes remain with the application.

The core provides calculation facilities shared by built-in and custom containers. Container-specific behavior belongs in the container, so adding a layout rule does not require adding knowledge of that rule to the core.

## Build

Requires CMake 3.20 or newer and a C11 compiler.

```sh
git submodule update --init --recursive -- examples/third_party/chitra
cmake -S . -B build -DVINYASA_BUILD_SANDBOX=ON
cmake --build build --config Debug
```

The sandbox executable is built under `build/examples/sandbox`, with a configuration subdirectory when using a multi-configuration generator such as Visual Studio. It uses the Chitra submodule in `examples/third_party` for its window, drawing, and text. The current scene checks rectangle and text rendering; it does not execute Vinyasa layout yet.

For an existing CMake project, use `add_subdirectory` and link to `vinyasa::vinyasa`. Sandbox and test builds are disabled by default. Chitra is not required when the sandbox is disabled. `VINYASA_BUILD_TESTS` enables the test directory.

## License

[Apache License 2.0](LICENSE).

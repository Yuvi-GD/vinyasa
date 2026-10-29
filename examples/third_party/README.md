# Example dependencies

Chitra supplies the sandbox window, rendering, and text measurements. It is linked only when `VINYASA_BUILD_SANDBOX` is enabled.

`chitra` is a Git submodule of <https://github.com/Yuvi-GD/Chitra>. Vinyasa records its tested commit; Chitra records its own Sokol and shader-tool dependencies.

From the Vinyasa directory, initialize the dependencies before enabling the sandbox:

```sh
git submodule update --init --recursive -- examples/third_party/chitra
```

The local CMake file builds `vinyasa_example_chitra` without building Chitra's standalone examples. Its pinned shader compiler generates headers inside the build directory. The Chitra checkout is not modified during the build.

To upgrade, fetch Chitra, check out the desired commit, update its nested submodules, and rebuild the sandbox. Commit the changed submodule reference in Vinyasa after verification.

Licenses remain in the dependency repositories. Chitra's bundled Roboto font is copyright 2015 Google Inc., licensed under Apache License 2.0.

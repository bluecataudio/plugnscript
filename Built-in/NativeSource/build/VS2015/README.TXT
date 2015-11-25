On Windows, native scripts are shared libraries (dlls), using either the x86 or the x64 extension, depending on the target platform (win32 or x64).

The vsprops folder contains the props files that define the appropriate build settings for release and debug builds, and the Projects folder contains the projects for all samples.

It is recommended to keep these settings for your own projects. The samples use the msvc runtime as a static library so that there is no need to distribute the dlls with the scripts.
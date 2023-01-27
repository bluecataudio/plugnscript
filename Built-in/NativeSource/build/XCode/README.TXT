On Mac, native scripts are shared libraries of type mh_bundle with the "bin" extension.

The XConfig folder contains the xcconfig files that define the appropriate build settings for release and debug builds. You can reuse them for your own projects.

In order to build for ARM Macs, the base SDK used here is MacOS 11. So binaries built with these settings will run on Mac OSX 10.9 or newer. You can change build settings if you do not have this SDK (but it is recommended to install it for maximum compatibility).
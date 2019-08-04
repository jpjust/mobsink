# LARA MobSink Multiple Sinks Simulator

MobSink is developed in a Debian 10 (buster) environment. You can build it in other systems, the only dependency is wxGTK 3.0.

To install the development files for wxGTK 3.0 in a Debian-based distribution run:

```
apt-get install libwxgtk3.0-gtk3-dev
```

After installing the development files for wxGTK 3.0, use GNU make to build:

```
make
make install
```

To build a debug executable, set BUILD environment variable before make:

```
BUILD=Debug make
BUILD=Debug make install
```

You can also import this project into Eclipse CDT and build from there.

## Building for Windows

A build target for Windows is available in Eclipse project. It depends on wxWidgets for MinGW-TDM.

## More info

To get more information about this project, visit [MobSink Page](http://just.pro.br/blog/mobsink).
 
# NativeScript Runtime for the Universal Windows Platform

### To Build:

Prerequisites:

* [Windows 10](https://www.microsoft.com/windows/)
* [Visual Studio 2015](http://www.visualstudio.com/products/visual-studio-community-vs)
* [CMake 3.4](http://www.cmake.org/download/)
* [Python 2.7](https://www.python.org/downloads/windows/)
* [Ruby 2](http://rubyinstaller.org/downloads/)
* [Perl 5](http://www.activestate.com/activeperl/downloads)
* [Bison 2.4](http://gnuwin32.sourceforge.net/packages/bison.htm#download)
* [Gperf 3](http://gnuwin32.sourceforge.net/packages/gperf.htm#download)

Clone the repo (This could take several hours):
```shell
git clone --recursive git@github.com:NativeScript/windows-runtime.git
```

Create a Visual Studio project:
```shell
mkdir Build
cd Build
cmake ../Source -DCMAKE_SYSTEM_NAME="WindowsStore" -DCMAKE_SYSTEM_VERSION="10.0" -G"Visual Studio 14"
```

> Use the `"Visual Studio 14 ARM"` generator to create a project compatible with ARM devices.

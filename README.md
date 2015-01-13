# NativeScript runtime for WinRT

### To build (Windows only):

Prerequisites:

* [Microsoft Visual Studio 2013](http://www.visualstudio.com/products/visual-studio-community-vs) or newer
* [Windows Phone 8.1 SDK](https://dev.windows.com/en-us/develop/download-phone-sdk)
* [CMake 3.1](http://www.cmake.org/download/) or newer
* [Python 2.7](https://www.python.org/downloads/windows/)
* [Ruby 2.0](http://rubyinstaller.org/downloads/) or newer
* [Bison](http://gnuwin32.sourceforge.net/packages/bison.htm#download)
* [Gperf](http://gnuwin32.sourceforge.net/packages/gperf.htm#download)

```
mkdir Build && cd Build
cmake ../Source -DCMAKE_SYSTEM_NAME="WindowsPhone" -DCMAKE_SYSTEM_VERSION="8.1"
cmake --build .
```

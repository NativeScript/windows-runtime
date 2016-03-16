# NativeScript Runtime for the Universal Windows Platform

The NativeScript Runtime allows [Windows Runtime](https://en.wikipedia.org/wiki/Windows_Runtime) APIs to be called from JavaScript. It is similar to the [JavaScript bridge](https://msdn.microsoft.com/en-us/library/hh710230%28v=vs.94%29.aspx?f=255&MSPPError=-2147217396) that Microsoft provides, but also allows XAML UI controls to be instantiated. This way NativeScript UWP apps are truly native, instead of being HTML based.

> **NOTE:** At the moment, the runtime is in a **proof of concept** stage - it only allows calling of methods with primitive types. Things like inheritance, generics, exception handling, asynchronous tasks and advanced marshalling/projections are further to be developed.

## Building and Running

Make sure you meet the following requirements:
* [Windows 10](https://www.microsoft.com/windows/)
* [Visual Studio 2015](http://www.visualstudio.com/products/visual-studio-community-vs)
* [CMake 3.4](http://www.cmake.org/download/)
* [Python 2.7](https://www.python.org/downloads/windows/)
* [Ruby 2](http://rubyinstaller.org/downloads/)
* [Perl 5](http://www.activestate.com/activeperl/downloads)
* [Bison 2.4](http://gnuwin32.sourceforge.net/packages/bison.htm#download)
* [Gperf 3](http://gnuwin32.sourceforge.net/packages/gperf.htm#download)

Clone the repo (this could take several hours):
```shell
git clone --recursive git@github.com:NativeScript/windows-runtime.git
```

Generate a Visual Studio solution:
```shell
mkdir Build
cd Build
cmake ../Source -DCMAKE_SYSTEM_NAME="WindowsStore" -DCMAKE_SYSTEM_VERSION="10.0" -G"Visual Studio 14"
```
> Use the `"Visual Studio 14 ARM"` generator to create a project compatible with ARM devices.

**Open the newly created NativeScript solution and run the TestRunner project to see the ["Hello, World!"](Source/TestRunner/app/index.js) app in action.**

## Contributing

Please read through our [contributing guidelines](CONTRIBUTING.md). Included are directions for opening issues, coding standards, and notes on development.

## Related Projects

* [Android Runtime](https://github.com/NativeScript/android-runtime)
* [iOS Runtime](https://github.com/NativeScript/ios-runtime)

## License
[Apache 2.0](LICENSE)

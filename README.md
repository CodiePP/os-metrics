# os-metrics
capture operating system metrics on Windows, macOSX, Linux, BSD


## building

```
mkdir -vp build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

on Windows (MinGW64):

```
cmake -DCMAKE_BUILD_TYPE=Debug -G "MSYS Makefiles" ..
```

Fast and simple to use C++ library to handle docset bundles.

Scan a folder for docsets:
```
#!c++
DocsetGroup myDocsets = DocsetGroup::open("/home/me/Docsets", true);
```

And you already can start searching in the docsets:
```
#!c++
DocsetObjectList obs = myDocsets.find("std::cout");
for (auto o: obs)
    std::cout << o.name() << "\n";
```

Currently the library runs on Linux and OS X, Windows support is planned, but not yet implemented.

### How do I get set up? ###

You need a C++11 compliant compiler (GCC, clang), cmake, make or ninja and the SQLite3 library and headers in order to build the library.

- Clone or Download the code:
```
#!bash
# git clone https://cm0x4D@bitbucket.org/cm0x4D/libdocset.git
# cd libdocset
```
```
#!bash
# wget https://bitbucket.org/cm0x4D/libdocset/get/dcf5e98ed785.zip
# unzip dcf5e98ed785.zip
# cd cm0x4D-libdocset-dcf5e98ed785
```

- Build the library using cmake and make:
```
#!bash
# mkdir build
# cd build
# cmake -D CMAKE_BUILD_TYPE=Release ..
# make
```
... or with ninja:
```
#!bash
# mkdir build
# cd build
# cmake -G Ninja -D CMAKE_BUILD_TYPE=Release ..
# make
```

- (Optional) Install the library:
```
#!bash
# sudo make install
```
or
```
#!bash
# sudo ninja install
```

### Contact ###

If you have questions or sugestion, do not hesitate to contact me cm0x4d@codemonkey.ch
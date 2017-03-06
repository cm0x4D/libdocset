Fast and simple to use C++ library to handle docset bundles.

Scan a folder for docsets:
```c++
DocsetGroup myDocsets = DocsetGroup::open("/home/me/Docsets", true);
```

And you already can start searching in the docsets:
```c++
DocsetObjectList obs = myDocsets.find("std::cout");
for (auto o: obs)
    std::cout << o.name() << "\n";
```

Currently the library runs on Linux and OS X.

### How do I get set up? ###

You need a C++11 compliant compiler (GCC, clang), cmake, make or ninja and the SQLite3 library and headers in order to build the library.

- Clone or Download the code:
```bash
git clone https://github.com/cm0x4D/libdocset.git
cd libdocset
```
```bash
wget https://github.com/cm0x4D/libdocset/archive/master.zip
unzip libdocset-master.zip
cd libdocset-master
```

- Build the library using cmake and make:
```bash
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make
```
... or with ninja:
```bash
mkdir build
cd build
cmake -G Ninja -D CMAKE_BUILD_TYPE=Release ..
ninja
```

- (Optional) Install the library:
```bash
sudo make install
```
or
```bash
sudo ninja install
```

### Contact ###

If you have questions or sugestion, do not hesitate to contact me cm0x4d@codemonkey.ch

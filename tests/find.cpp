#include <LibDocset>
#include <iostream>
#include <chrono>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "Usage: docs <path to docsets> <search therm>" << endl;
        return 1;
    }

    DocsetGroup docsets = DocsetGroup::open(argv[1], true);
    cout << "Loading " << docsets.count() << " docsets into memory... " << flush;
    auto start = chrono::system_clock::now();
    std::size_t count = docsets.loadToMemory();
    auto end = chrono::system_clock::now();
    int elapsedMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << count << " objects loaded in " << elapsedMs << " ms." << endl;
    start = chrono::system_clock::now();
    DocsetObjectList obs = docsets.find(argv[2]);
    end = chrono::system_clock::now();
    elapsedMs = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    for (auto o: obs)
        std::cout << o.docset().name() << " : " << o.typeString()
                  << " : " << o.name() << "\n";
    std::cout << "Found " << obs.size() << " objects in " << elapsedMs << " ms" << std::endl;
}

import qbs

CppApplication {
    //Depends { name: "Qt.testlib" }
    cpp.cxxLanguageVersion: "c++17"
    consoleApplication: true
    Group{
        name: "serializatortests"
        files:["serializatortests.cpp", "serializatortests.h"]
    }
    Group{
        name: "discoveryServiceTests"
        files:["discoveryservicetests.cpp", "discoveryservicetests.h",
        "../main/discoveryService.h",
        "../main/discoveryService.cpp"]
    }

    files: [
        "main.cpp"
    ]
    Depends {
        name: "Qt"
        submodules: ["testlib", "network"]
    }

}

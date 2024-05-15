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

    Group{
        name: "mathtokenizertests"
        files:[
            "mathtokenizertests.cpp",
            "mathtokenizertests.h",
            "../main/calculator/mathtokenizer.h",
            "../main/calculator/mathtokenizer.cpp",
        ]
    }

    Group{
        name: "mathsyntaxtreetest"
        files:[
            "mathsyntaxtreetest.cpp",
            "mathsyntaxtreetest.h",
            "../main/calculator/mathsyntaxtree.h",
            "../main/calculator/mathsyntaxtree.cpp",
        ]
    }

    Group{
        name: "calculatortests"
        files:[
            "calculatortests.cpp",
            "calculatortests.h",
            "../main/calculator/calculator.h",
            "../main/calculator/calculator.cpp",
        ]
    }

    files: [
        "main.cpp",
    ]
    Depends {
        name: "Qt"
        submodules: ["testlib", "network"]
    }

}

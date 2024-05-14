import qbs

CppApplication {
    Depends { name: "Qt.testlib" }
    cpp.cxxLanguageVersion: "c++17"
    consoleApplication: true
    Group{
        name: "serializatortests"
        files:["serializatortests.cpp", "serializatortests.h"]
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

    files: [
        "main.cpp",
    ]


}

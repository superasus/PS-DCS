import qbs

CppApplication {
    Depends { name: "Qt.testlib" }
    consoleApplication: true
    Group{
        name: "serializatortests"
        files:["serializatortests.cpp", "serializatortests.h"]
    }

    files: [
        "main.cpp",
    ]


}

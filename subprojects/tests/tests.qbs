import qbs

CppApplication {
    Depends { name: "Qt.testlib" }
    consoleApplication: true
    files: [
        "tst_serializator.cpp"
    ]


}

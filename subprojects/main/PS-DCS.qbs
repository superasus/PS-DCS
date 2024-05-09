import qbs

QtApplication {
    cpp.defines: [// You can make your code fail to compile if it uses deprecated APIs.
        // In order to do so, uncomment the following line.
        //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
    ]
    cpp.cxxLanguageVersion: "c++17"
    consoleApplication: true
    install: true

    Group{
        name: "tcp"
        files: ["tcpclient.cpp",
            "tcpclient.h",
            "tcpserver.cpp",
            "tcpserver.h"]
    }

    Group{
        name: "discovery-service"
        files: ["discovery-service.h", "discovery-service.cpp"]
    }

    files: [
        "main.cpp",
        "serializator.h"]

    Depends {
        name: "Qt"
        submodules: ["core", "network"]
    }
}

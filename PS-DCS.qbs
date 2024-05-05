import qbs

QtApplication {
    cpp.defines: [// You can make your code fail to compile if it uses deprecated APIs.
        // In order to do so, uncomment the following line.
        //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
    ]
    cpp.cxxLanguageVersion: "c++17"
    consoleApplication: true
    install: true

    files: ["main.cpp", "tcpclient.cpp", "tcpclient.h", "tcpserver.cpp", "tcpserver.h", "discovery-service.h", "discovery-service.cpp", "serializator.h"]
    
    Depends {
        name: "Qt"
        submodules: ["core", "network"]
    }
}

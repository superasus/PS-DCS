import qbs

Project {
    property bool withTests: false

    SubProject {
        filePath: "subprojects/main/clasternode.qbs"
        Properties {
            name: "clasternode"
        }
        inheritProperties: true
    }

    SubProject {
        filePath: "subprojects/main/tasker.qbs"
        Properties {
            name: "tasker"
        }
        inheritProperties: true
    }

    SubProject {
        filePath: "subprojects/tests/tests.qbs"
        condition: withTests
        Properties {
            name: "tests"
        }
        inheritProperties: true
    }
}

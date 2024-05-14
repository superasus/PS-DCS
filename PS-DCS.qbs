import qbs

Project {
    property bool withTests: false

    SubProject {
        filePath: "subprojects/main/PS-DCS.qbs"
        Properties {
            name: "PS-DCS"
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

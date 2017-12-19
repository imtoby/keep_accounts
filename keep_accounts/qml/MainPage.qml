import QtQuick 2.3
import "Config"
import "Dialog"

Item {
    id: rootPage

//    Histogram {
//        model: 31
//    }

    width: Config.applicationWidth
    height: Config.applicationHeight

    BorderImage {
        id: rootBackground
        source: "qrc:/res/00.png"
        anchors.fill: parent
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
        horizontalTileMode: BorderImage.Repeat
        verticalTileMode: BorderImage.Repeat
    }

    MonthView {
        anchors.fill: parent
    }
}

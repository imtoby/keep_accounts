import QtQuick 2.3
import "Config"

Item {

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

//    EditType{
//        width: parent.width*4/5
//        height: parent.height/2
//        anchors.centerIn: parent
//    }

    MonthView{
        anchors.fill: parent
    }
}

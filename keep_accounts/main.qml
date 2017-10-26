import QtQuick 2.0
import QtQuick.Controls 2.0
import "qml"

ApplicationWindow {
    id: rootWindow
    visible: true
    width: 480
    height: 720
    title: qsTr("Keep Accounts")

    MainPage {
        anchors.fill: parent
    }

    Utils {
        id: utils
    }
}

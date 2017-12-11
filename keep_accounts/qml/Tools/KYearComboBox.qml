import QtQuick 2.3
import "../Config"

KComboBox{
    id: comboBox

    readonly property int systemYear: (new Date()).getFullYear() - 1901

    model: 200
    currentIndex: systemYear
    buttonText: unitYear(currentIndex + 1901)

    delegate: MouseArea {
        width: contentButtonWidth
        height: comboBox.height

        Text {
            anchors.centerIn: parent
            text: unitYear(index + 1901)
            color: mainColor
        }

        onClicked: {
            comboBox.clicked(index)
        }
    }

    function unitYear(year){
        return  Config.padZero(year, 2) + "年"
    }

    onClicked: {
        currentIndex = index
    }
}

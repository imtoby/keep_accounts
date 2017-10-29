import QtQuick 2.3
import "../Config"

KComboBox{
    id: comboBox

    readonly property int systemMonth: (new Date()).getMonth()

    currentIndex: systemMonth
    model: 12
    buttonText: unitMonth(currentIndex + 1)

    delegate: MouseArea {
        width: contentButtonWidth
        height: comboBox.height

        Text {
            anchors.centerIn: parent
            text: unitMonth(index + 1)
            color: mainColor
        }

        onClicked: {
            comboBox.clicked(index)
        }
    }

    function unitMonth(month){
        return  Config.padZero(month, 2) + "月"
    }

    onClicked: {
        currentIndex = index
    }
}

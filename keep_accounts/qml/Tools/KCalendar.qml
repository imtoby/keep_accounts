import QtQuick 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "../Config"

Item{
    id: calendar
    anchors.fill: parent

    property alias selectedDate: calendarContent.selectedDate

    readonly property bool popuped: yearComboBox.popuped || monthComboBox.popuped

    signal doubleClicked()

    Calendar{
        id: calendarContent
        width: calendar.width
        height: calendar.height - navigationBar.height
        anchors.bottom: parent.bottom
        onDoubleClicked:{
            calendar.doubleClicked()
        }

        style: CalendarStyle{
            background: null

            navigationBar: null

            dayOfWeekDelegate: Item{
                width: calendar.width/7
                height: 40
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: Qt.locale().standaloneDayName(
                              styleData.index,
                              Locale.ShortFormat)
                }
            }
        }
    }

    Item{
        id: navigationBar
        width: calendar.width
        height: 40

        KYearComboBox{
            id: yearComboBox
            width: 140
            height: parent.height
            borderVisible: false
            dividerVisible: false
            onClicked: {
                calendarContent.visibleYear = index + 1901
            }
            onClearOtherPop: {
                monthComboBox.hidePop()
            }

            Rectangle{
                anchors{
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                color: Config.lineColor
                implicitHeight: 20
                implicitWidth: Config.lineWidth
            }
        }

        KMonthComboBox{
            id: monthComboBox
            anchors.left: yearComboBox.right
            width: 120
            height: parent.height
            borderVisible: false
            dividerVisible: false
            onClicked: {
                calendarContent.visibleMonth = index
            }
            onClearOtherPop: {
                yearComboBox.hidePop()
            }

            Rectangle{
                anchors{
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                color: Config.lineColor
                implicitHeight: 20
                implicitWidth: Config.lineWidth
            }
        }

        KButton{
            id: rightBtn
            height: parent.height
            anchors.left: monthComboBox.right
            anchors.right: parent.right
            onClicked: {
                yearComboBox.hidePop()
                monthComboBox.hidePop()
                calendarContent.selectedDate
                        = (new Date()).toLocaleDateString(Config.locale,
                                                          Config.dateFormat)

                calendar.doubleClicked()
            }

            Text {
                anchors.centerIn: parent
                text: "今天"
                color: monthComboBox.mainColor
            }
        }

    }


}



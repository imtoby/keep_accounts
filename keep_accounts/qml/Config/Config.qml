pragma Singleton
import QtQuick 2.3

QtObject{
    readonly property int applicationWidth: 405
    readonly property int applicationHeight: 720

    readonly property int margin: 10
    readonly property int headHeight: 120
    readonly property int cellHeight: 60
    readonly property color lineColor: "lightGray"
    readonly property real lineWidth: 0.5

    readonly property color expensesColor:      "#A60000"
    readonly property color incomeColor:        "#35CC4B"
    readonly property color amountColor:        "#05296E"
    readonly property color backgroundColor:    "#CCCCCC"
    readonly property color newAccountColor:    "#FFAD00"
    readonly property color deleteColor:        "#FC635E"
    readonly property color deletePressedColor: "#80FC635E"
    readonly property color describeColor:      "#666666"

    ///////////////////// Button index ///////////////////////
    readonly property int indexIncome:      0x00
    readonly property int indexExpenses:    0x01
    readonly property int indexAmount:      0x02
    readonly property int indexMonth:       0x03
    readonly property int indexNewAccount:  0x04
    ///////////////////// Button index ///////////////////////

    ///////////////////// Move direction /////////////////////
    readonly property int moveDown: 0x00
    readonly property int moveUp:   0x01
    ///////////////////// Move direction /////////////////////

    readonly property variant locale: Qt.locale()
    readonly property string dateSeparator: "-";
    readonly property string dateFormat: infoManager.dateFormat();
    readonly property string topTypeId: infoManager.topTypeId();

    readonly property int out_type:  0x00
    readonly property int in_type:   0x01

    readonly property int typeIdLength: 36


    function padZero(num, n) {
        var len = num.toString().length;

        while(len < n){
            num = "0" + num;
            len++;
        }

        return num;
    }

    function bound(min, value, max) {
        return Math.max(min, Math.min(value, max));
    }

    function weekdayString(year, month, day) {
        var w = new Date(year, month - 1, day).getDay();
        switch(w) {
        case 1: return "周一";
        case 2: return "周二";
        case 3: return "周三";
        case 4: return "周四";
        case 5: return "周五";
        case 6: return "周六";
        default: return "周日";
        }
    }
}

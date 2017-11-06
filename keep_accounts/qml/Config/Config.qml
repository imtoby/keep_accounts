pragma Singleton
import QtQuick 2.3

QtObject{
    readonly property int applicationWidth: 405
    readonly property int applicationHeight: 720

    readonly property int margin: 10
    readonly property int headHeight: 120
    readonly property int cellHeight: 60
    readonly property color lineColor: "lightGray"
    readonly property int lineWidth: 1

    readonly property color expensesColor:      "#A60000"
    readonly property color incomeColor:        "#008500"
    readonly property color balanceColor:       "#05296E"
    readonly property color backgroundColor:    "#CCCCCC"
    readonly property color newAccountColor:    "#FFAD00"

    ///////////////////// Button index ///////////////////////
    readonly property int indexIncome:      0x00
    readonly property int indexExpenses:    0x01
    readonly property int indexBalancee:    0x02
    readonly property int indexMonth:       0x03
    readonly property int indexNewAccount:  0x04
    ///////////////////// Button index ///////////////////////

    ///////////////////// Move direction /////////////////////
    readonly property int moveDown: 0x00
    readonly property int moveUp:   0x01
    ///////////////////// Move direction /////////////////////

    readonly property variant locale: Qt.locale()
    readonly property string dateFormat: "yyyy-MM-dd"
    readonly property string topTypeId: "topTypeId_0"

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
}

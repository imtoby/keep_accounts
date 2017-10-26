import QtQuick 2.0

QtObject {

    function bound(min, value, max) {
        return Math.max(min, Math.min(value, max));
    }

}

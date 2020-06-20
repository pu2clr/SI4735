/* 
  Bluetooth remote control for VFO.
  
  This code was built using piece of example code suggested by the plug-blue-serial-plugin.
  See plug-blue-serial-plugin on: https://www.npmjs.com/package/cordova-plugin-bluetooth-serial
  
  By PU2CLR - Ricardo Lima Caratti  - April, 2019 
*/

/* jshint quotmark: false, unused: vars, browser: true */
/* global cordova, console, $, bluetoothSerial, _, refreshButton, deviceList, previewColor, red, green, blue, disconnectButton, connectionScreen, colorScreen, rgbText, messageDiv */

'use strict';

var objBand;
var objStep;

var app = {
    initialize: function () {
        this.bind();
    },
    bind: function () {
        document.addEventListener('deviceready', this.deviceready, false);
    },
    deviceready: function () {
        // wire buttons to functions
        deviceList.ontouchstart = app.connect;
        refreshButton.ontouchstart = app.list;
        sendFreq.ontouchstart = app.sendData;
        disconnectButton.ontouchstart = app.disconnect;

        app.list();
    },
    list: function (event) {
        deviceList.firstChild.innerHTML = "Discovering...";
        app.setStatus("Looking for Bluetooth Devices...");

        bluetoothSerial.list(app.ondevicelist, app.generateFailureFunction("List Failed"));
    },
    connect: function (e) {
        app.setStatus("Connecting...");
        var device = e.target.getAttribute('deviceId');
        console.log("Requesting connection to " + device);
        var deviceId = e.target.dataset.deviceId;
        if (!deviceId) { // try the parent
            deviceId = e.target.parentNode.dataset.deviceId;
        }
        bluetoothSerial.subscribe('\n', app.ondata, app.generateFailureFunction);
        bluetoothSerial.connect(device, app.onconnect, app.ondisconnect);
    },
    disconnect: function (event) {
        if (event) {
            event.preventDefault();
        }
        app.setStatus("Disconnecting...");
        bluetoothSerial.disconnect(app.ondisconnect);
    },
    onconnect: function () {

        app.setStatus("Connected.");
        connectionScreen.hidden = true;
        // Solicita informações ao VFO (Arduino)   
        app.sendToArduino('d');  // See Arduino sketch si5351_vfo_ble_atemega328.ino

    },
    ondisconnect: function () {
        connectionScreen.hidden = false;
        colorScreen.hidden = true;
        app.setStatus("Disconnected.");
    },
    sendToArduino: function (c) {
        bluetoothSerial.write(c);
    },
    ondata: function (data) { // data received from Arduino
        var protocol = data.substring(0, 1);
        var cmd
        if (protocol == "#") {
            cmd = data.substring(1, 2); // Check the command
            if (cmd == "B") { // Get Band information
                var strJson = data.substring(2, data.length);
                objBand = JSON.parse(strJson);
                // TO DO - Load band combobox
            } else if (cmd == "S") { // Get step information
                var strJson = data.substring(2, data.length);
                alert("String Step: " + strJson);
                objStep = JSON.parse(strJson);
                alert(objStep.steps[0].name + " --- " + objBand.bands[0].name);
                // TO DO - Load step combobox
            }
        } else {
            $("#freq").val(data);
        }
    },
    timeoutId: 0,
    setStatus: function (status) {
        if (app.timeoutId) {
            clearTimeout(app.timeoutId);
        }
        messageDiv.innerText = status;
        app.timeoutId = setTimeout(function () { messageDiv.innerText = ""; }, 4000);
    },
    ondevicelist: function (devices) {
        var listItem, deviceId;
        // remove existing devices
        deviceList.innerHTML = "";
        app.setStatus("");

        devices.forEach(function (device) {
            listItem = document.createElement('li');
            listItem.className = "topcoat-list__item";
            if (device.hasOwnProperty("uuid")) { // TODO https://github.com/don/BluetoothSerial/issues/5
                deviceId = device.uuid;
            } else if (device.hasOwnProperty("address")) {
                deviceId = device.address;
            } else {
                deviceId = "ERROR " + JSON.stringify(device);
            }
            listItem.setAttribute('deviceId', device.address);
            listItem.innerHTML = device.name + "<br/><i>" + deviceId + "</i>";
            deviceList.appendChild(listItem);
        });

        if (devices.length === 0) {

            if (cordova.platformId === "ios") { // BLE
                app.setStatus("No Bluetooth Peripherals Discovered.");
            } else { // Android
                app.setStatus("Please Pair a Bluetooth Device.");
            }

        } else {
            app.setStatus("Found " + devices.length + " device" + (devices.length === 1 ? "." : "s."));
        }
    },
    generateFailureFunction: function (message) {
        var func = function (reason) {
            var details = "";
            if (reason) {
                details += ": " + JSON.stringify(reason);
            }
            app.setStatus(message + details);
        };
        return func;
    }
};
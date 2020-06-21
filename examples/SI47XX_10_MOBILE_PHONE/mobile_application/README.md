# VFO Mobile Remote Control for iPhone and Android

__This application is under construction...  Please wait...__ 

## Introduction 

The reduced length of the Oled display might limit some VFO functionalities.  The goal of this mobile application is improve the VFO control and visualization. 



## Compile and deploy this mobile application

Please copy the project to a local folder and follow the steps bellow

You have to install [Android Studio](http://developer.android.com/sdk/index.html) on your computer to compile and deploy this application on your Android.

You have to install [Xcode](https://developer.apple.com/xcode/) on your Mac computer to compile and deploy this application on your iPhone or iPad.  


### Pair your phone

You have to pair your Android phone with the HM10 Bluetooth Shield.

## Cordova 

This applications was build using the [Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html). 
[Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html) is an open-source tool to develop cross-platform mobile application. Click [here](https://cordova.apache.org/docs/en/latest/guide/overview/index.html) to see more about Apache Cordova.

### Install Cordova

    Cordova runs on Node.js.  Please, before installing Cordova, you have to configure the [Node.js](https://nodejs.org/en/). 

    $ npm install cordova -g


### Install Platform and Plugin

    $ cordova platform add android
    $ cordova platform add ios
    $ cordova plugin add cordova-plugin-bluetooth-serial


### Build and Deploy

Compile and run the application


    $ cordova run --device android 

    or

    $ cordova run --device ios


## Useful commands    

cd Experiments/VFO_RADIO_CD2003GP/source/vfoMobileApplication/
~/Library/Android/sdk/tools/emulator -list-avds
~/Library/Android/sdk/tools/emulator -avd Nexus_6P_API_25 &



## References

- [Bluetooth Serial Plugin for PhoneGap](https://github.com/digistump/Digispark_Cordova_BTShieldEcho/tree/master/plugins/com.megster.cordova.bluetoothserial)
- [Bluetooth Serial Plugin for PhoneGap](https://github.com/don/BluetoothSerial)
- [UI Plugin For Seven-Segment Indicator - sevenSeg.js](https://www.jqueryscript.net/demo/jQuery-jQuery-UI-Plugin-For-Seven-Segment-Indicator-sevenSeg-js/)
- 
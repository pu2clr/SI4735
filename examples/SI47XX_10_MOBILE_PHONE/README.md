# iOS and Android Remote Control

This example shows a way to use your smartphone as a remote control. In order to follow the steps presented here, I am assuming that you have some knowledge in development for mobile devices. Also, you will need to be familiar with the Javascript programming language.

The development environment used for this example is the [Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html). Cordova provides an easy way to develop for iOS and Android.


## Compile and deploy this mobile application example

Copy this example folder to a local folder and follow the steps bellow

For Android, is recommended to install [Android Studio](http://developer.android.com/sdk/index.html) on your computer to compile and deploy this application. 

For iOS (iPhone or iPad), is recommended to install [Xcode](https://developer.apple.com/xcode/) on your Mac computer to compile and deploy this application.


### Cordova 

This applications was build using the [Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html). 
[Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html) is an open-source tool to develop cross-platform mobile application. Click [here](https://cordova.apache.org/docs/en/latest/guide/overview/index.html) to see more about Apache Cordova.

### Install Cordova

    $ npm install cordova -g


### Install Platform and Plugin

    $ cordova platform add android
    $ cordova platform add ios
    $ cordova plugin add cordova-plugin-bluetooth-serial


#### cordova-plugin-bluetooth-serial information

* [Bluetooth Serial Plugin for PhoneGap](https://www.npmjs.com/package/cordova-plugin-bluetooth-serial)
* [Bluetooth Serial Plugin for PhoneGap - github](https://github.com/don/BluetoothSerial)


### Build and Deploy

Compile and run the application

    $ cordova run android --device



## Videos 

- [VFO and BFO project test with an inexpencive radio based on CD2003GP](https://youtu.be/_KgBc6vYWLg)
- [Testing the VFO with a CD2003GM HOMEBREW FM RECEIVER](https://youtu.be/JfgRjDK8LTE)
- [VFO and BFO remote control with iPhone via Bluetooth](https://youtu.be/7gBRUmsrCus)






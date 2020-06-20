# iOS and Android Remote Control

This example shows a way to use your smartphone as a remote control via Bluetooth. In order to follow the steps presented here, I am assuming that you have some knowledge in development for mobile devices. Also, you will need to be familiar with the Javascript programming language.

The development environment used for this example is the [Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html). Cordova is a open-source mobile development framework that allows you to develop cross-platform applications. That means you can code once and deploy the application in many system, including iOS and Android. 
Cordova provides an easy way to develop for iOS and Android.


The video below shows you this example working. 

[Si4735 receiver prototype controlled by iPhone via Bluetooth](https://youtu.be/Yc9DHl7yQZ0)


### Mobile device

This example uses an old iPhone 5C. However, you can use your current iOs or Android device to run it. Thanks to Cordova development environment, you can do it with no code changes.

The screenshot below shows the mobile application interface. 

<BR> 

![Mobile Application Screenshot](https://github.com/pu2clr/SI4735/blob/master/extras/images/mobile_0001.png)

<BR> 

### Arduino Shield Bluetooth (HM10 - HMSOft TineSine)

The Bluetooth 4.0 used here is the HM10 (TinySine). It is very similar to the famous HC-05. The photos below show the HM10 Bluetooth. This kind of Bluetooth works well on iOs and Android based device. __You also can use the HC-05 Bluetooth device. However, the HC-05 will not work on iOS devices__. 

<BR>

|  Ponto 1 | Photo 2 | 
| -------- | ------- |
| ![HM10 - HMSOft TineSine 01](https://github.com/pu2clr/SI4735/blob/master/extras/images/BT01BLE.png) | ![HM10 - HMSOft TineSine 02](https://github.com/pu2clr/SI4735/blob/master/extras/images/BT02BLE.png) |

<BR>


#### HM10 Documentation

* [HM-10 Bluetooth 4 BLE Modules - Martyn Currey](http://www.martyncurrey.com/hm-10-bluetooth-4ble-modules/)
* [BLE CENTRAL - HM-10 BLE Module](http://blog.blecentral.com/2015/05/05/hm-10-peripheral/)


## Bluetooth Arduino Library 

The Arduino sketch of this example uses the [SoftwareSerial](https://github.com/PaulStoffregen/SoftwareSerial) library. You have to download and install this library on your Arduino enviroment.    


## The prototype

The photo below shows the Arduino, SI4735 and the Bluetooth devices connections. 

![HM10 - Mobile Application Screenshot](https://github.com/pu2clr/SI4735/blob/master/extras/images/iPHONE_REMOTE_CONTROL_01.png)



## Compile and deploy this mobile application example

Copy this example folder to a local folder and follow the steps bellow

For Android, is recommended to install [Android Studio](http://developer.android.com/sdk/index.html) on your computer to compile and deploy this application. 

For iOS (iPhone or iPad), is recommended to install [Xcode](https://developer.apple.com/xcode/) on your Mac computer to compile and deploy this application.


### Cordova 

This applications was build using the [Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html). 
[Apache Cordova](https://cordova.apache.org/docs/en/latest/guide/overview/index.html). Click [here](https://cordova.apache.org/docs/en/latest/guide/overview/index.html) to see more about Apache Cordova.

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

- [Si4735 receiver prototype controlled by iPhone via Bluetooth](https://youtu.be/Yc9DHl7yQZ0)






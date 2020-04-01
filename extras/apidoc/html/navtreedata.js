/*
@licstart  The following is the entire license notice for the
JavaScript code in this file.

Copyright (C) 1997-2019 by Dimitri van Heesch

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

@licend  The above is the entire license notice
for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Si4735 Arduino Library", "index.html", [
    [ "README", "md__r_e_a_d_m_e.html", [
      [ "Si4735 Library for Arduino", "md__r_e_a_d_m_e.html#autotoc_md0", [
        [ "Summary", "md__r_e_a_d_m_e.html#autotoc_md1", null ],
        [ "MIT License", "md__r_e_a_d_m_e.html#autotoc_md2", null ],
        [ "Thanks", "md__r_e_a_d_m_e.html#autotoc_md3", null ],
        [ "Your support is important.", "md__r_e_a_d_m_e.html#autotoc_md4", null ],
        [ "About the SI4735", "md__r_e_a_d_m_e.html#autotoc_md5", null ],
        [ "Radios based on Si47XX Family", "md__r_e_a_d_m_e.html#autotoc_md6", [
          [ "Functional Block Diagram", "md__r_e_a_d_m_e.html#autotoc_md7", null ],
          [ "The main Si4735 features", "md__r_e_a_d_m_e.html#autotoc_md8", null ]
        ] ],
        [ "SI4735 Terminology", "md__r_e_a_d_m_e.html#autotoc_md9", null ],
        [ "SI4735 Arduino Library Features", "md__r_e_a_d_m_e.html#autotoc_md10", null ],
        [ "Library Installation", "md__r_e_a_d_m_e.html#autotoc_md11", [
          [ "Installing via Arduino IDE", "md__r_e_a_d_m_e.html#autotoc_md12", null ],
          [ "Installing via this repository", "md__r_e_a_d_m_e.html#autotoc_md13", null ]
        ] ],
        [ "Hardware Requirements and Setup", "md__r_e_a_d_m_e.html#autotoc_md14", [
          [ "Arduino 5V and Si4735", "md__r_e_a_d_m_e.html#autotoc_md15", null ],
          [ "Schematic", "md__r_e_a_d_m_e.html#autotoc_md16", [
            [ "The picture below shows the SI4735-D60 pinout", "md__r_e_a_d_m_e.html#autotoc_md17", null ],
            [ "The image bellow shows the Slicon Labs SSOP Typical Application Schematic.", "md__r_e_a_d_m_e.html#autotoc_md18", null ]
          ] ],
          [ "Parts", "md__r_e_a_d_m_e.html#autotoc_md19", null ],
          [ "Tips to build", "md__r_e_a_d_m_e.html#autotoc_md20", null ],
          [ "Boards where this library has been successfully tested", "md__r_e_a_d_m_e.html#autotoc_md21", null ]
        ] ],
        [ "Photos (Tools and Accessories)", "md__r_e_a_d_m_e.html#autotoc_md22", [
          [ "SI4735 soldered on adapter", "md__r_e_a_d_m_e.html#autotoc_md23", [
            [ "Protoboard", "md__r_e_a_d_m_e.html#autotoc_md24", null ]
          ] ]
        ] ],
        [ "API Documentation", "md__r_e_a_d_m_e.html#autotoc_md25", [
          [ "API Index", "md__r_e_a_d_m_e.html#autotoc_md26", null ],
          [ "Defined Data Types and Structures", "md__r_e_a_d_m_e.html#autotoc_md27", null ]
        ] ],
        [ "Public Methods", "md__r_e_a_d_m_e.html#autotoc_md28", [
          [ "setDeviceI2CAddress", "md__r_e_a_d_m_e.html#autotoc_md29", null ],
          [ "getDeviceI2CAddress", "md__r_e_a_d_m_e.html#autotoc_md30", null ],
          [ "setup", "md__r_e_a_d_m_e.html#autotoc_md31", [
            [ "If you are not using interrupt use the <strong>setup</strong> method as shown below.", "md__r_e_a_d_m_e.html#autotoc_md32", null ],
            [ "Example of using setup", "md__r_e_a_d_m_e.html#autotoc_md33", null ]
          ] ],
          [ "setPowerUp", "md__r_e_a_d_m_e.html#autotoc_md34", null ],
          [ "radioPowerUp", "md__r_e_a_d_m_e.html#autotoc_md35", [
            [ "Example of using radioPowerUp", "md__r_e_a_d_m_e.html#autotoc_md36", null ]
          ] ],
          [ "powerDown", "md__r_e_a_d_m_e.html#autotoc_md37", null ],
          [ "setFrequency", "md__r_e_a_d_m_e.html#autotoc_md38", [
            [ "Example of using setFrequency", "md__r_e_a_d_m_e.html#autotoc_md39", null ]
          ] ],
          [ "frequencyUp", "md__r_e_a_d_m_e.html#autotoc_md40", null ],
          [ "frequencyDown", "md__r_e_a_d_m_e.html#autotoc_md41", null ],
          [ "setTuneFrequencyAntennaCapacitor", "md__r_e_a_d_m_e.html#autotoc_md42", null ],
          [ "setTuneFrequencyFast", "md__r_e_a_d_m_e.html#autotoc_md43", null ],
          [ "setTuneFrequencyFreeze", "md__r_e_a_d_m_e.html#autotoc_md44", null ],
          [ "seekStation", "md__r_e_a_d_m_e.html#autotoc_md45", [
            [ "Example of using seekStation", "md__r_e_a_d_m_e.html#autotoc_md46", null ]
          ] ],
          [ "seekStationUp and seekStationDown", "md__r_e_a_d_m_e.html#autotoc_md47", null ],
          [ "setSeekAmLimits", "md__r_e_a_d_m_e.html#autotoc_md48", null ],
          [ "setSeekAmSpacing", "md__r_e_a_d_m_e.html#autotoc_md49", null ],
          [ "setSeekSrnThreshold", "md__r_e_a_d_m_e.html#autotoc_md50", null ],
          [ "setSeekRssiThreshold", "md__r_e_a_d_m_e.html#autotoc_md51", null ],
          [ "setAM", "md__r_e_a_d_m_e.html#autotoc_md52", null ],
          [ "setFM", "md__r_e_a_d_m_e.html#autotoc_md54", [
            [ "Example of using setAM() and setFM()", "md__r_e_a_d_m_e.html#autotoc_md56", null ],
            [ "isCurrentTuneFM", "md__r_e_a_d_m_e.html#autotoc_md57", null ]
          ] ],
          [ "setVolume", "md__r_e_a_d_m_e.html#autotoc_md58", [
            [ "Example of using setVolume()", "md__r_e_a_d_m_e.html#autotoc_md59", null ]
          ] ],
          [ "getVolume", "md__r_e_a_d_m_e.html#autotoc_md60", null ],
          [ "volumeUp", "md__r_e_a_d_m_e.html#autotoc_md61", null ],
          [ "volumeDown", "md__r_e_a_d_m_e.html#autotoc_md62", [
            [ "Example of using volumeUp() and volumeDown()", "md__r_e_a_d_m_e.html#autotoc_md63", null ]
          ] ],
          [ "setAudioMute", "md__r_e_a_d_m_e.html#autotoc_md64", null ],
          [ "getCurrentVolume", "md__r_e_a_d_m_e.html#autotoc_md65", null ],
          [ "FM Stereo and Mono Control", "md__r_e_a_d_m_e.html#autotoc_md66", [
            [ "setFmBlendStereoThreshold", "md__r_e_a_d_m_e.html#autotoc_md67", null ],
            [ "setFmBlendMonoThreshold", "md__r_e_a_d_m_e.html#autotoc_md68", null ],
            [ "setFmBlendRssiStereoThreshold", "md__r_e_a_d_m_e.html#autotoc_md69", null ],
            [ "setFmBLendRssiMonoThreshold", "md__r_e_a_d_m_e.html#autotoc_md70", null ],
            [ "setFmBlendSnrStereoThreshold", "md__r_e_a_d_m_e.html#autotoc_md71", null ],
            [ "setFmBLendSnrMonoThreshold", "md__r_e_a_d_m_e.html#autotoc_md72", null ],
            [ "setFmBlendMultiPathStereoThreshold", "md__r_e_a_d_m_e.html#autotoc_md73", null ],
            [ "setFmBlendMultiPathMonoThreshold", "md__r_e_a_d_m_e.html#autotoc_md74", null ]
          ] ]
        ] ],
        [ "SI4735 Current Status", "md__r_e_a_d_m_e.html#autotoc_md75", [
          [ "getFrequency", "md__r_e_a_d_m_e.html#autotoc_md76", null ],
          [ "getCurrentFrequency", "md__r_e_a_d_m_e.html#autotoc_md77", null ],
          [ "getStatus", "md__r_e_a_d_m_e.html#autotoc_md78", null ],
          [ "getTuneCompleteTriggered", "md__r_e_a_d_m_e.html#autotoc_md79", null ],
          [ "getSignalQualityInterrup", "md__r_e_a_d_m_e.html#autotoc_md80", null ],
          [ "getRadioDataSystemInterrupt", "md__r_e_a_d_m_e.html#autotoc_md81", null ],
          [ "getStatusError", "md__r_e_a_d_m_e.html#autotoc_md82", null ],
          [ "getStatusCTS", "md__r_e_a_d_m_e.html#autotoc_md83", null ],
          [ "getACFIndicator", "md__r_e_a_d_m_e.html#autotoc_md84", null ],
          [ "getBandLimit", "md__r_e_a_d_m_e.html#autotoc_md85", null ],
          [ "getReceivedSignalStrengthIndicator", "md__r_e_a_d_m_e.html#autotoc_md86", null ],
          [ "getStatusSNR", "md__r_e_a_d_m_e.html#autotoc_md87", null ],
          [ "getStatusMULT", "md__r_e_a_d_m_e.html#autotoc_md88", null ],
          [ "getAntennaTuningCapacitor", "md__r_e_a_d_m_e.html#autotoc_md89", null ],
          [ "getStatusValid", "md__r_e_a_d_m_e.html#autotoc_md90", null ]
        ] ],
        [ "SI4735 Received Signal Quality", "md__r_e_a_d_m_e.html#autotoc_md91", [
          [ "getCurrentReceivedSignalQuality", "md__r_e_a_d_m_e.html#autotoc_md92", null ],
          [ "getCurrentRSSI", "md__r_e_a_d_m_e.html#autotoc_md93", null ],
          [ "getCurrentSNR", "md__r_e_a_d_m_e.html#autotoc_md94", null ],
          [ "getCurrentRssiDetectLow", "md__r_e_a_d_m_e.html#autotoc_md95", null ],
          [ "getCurrentRssiDetectHigh", "md__r_e_a_d_m_e.html#autotoc_md96", null ],
          [ "getCurrentSnrDetectLow", "md__r_e_a_d_m_e.html#autotoc_md97", null ],
          [ "getCurrentSnrDetectHigh", "md__r_e_a_d_m_e.html#autotoc_md98", null ],
          [ "getCurrentValidChannel", "md__r_e_a_d_m_e.html#autotoc_md99", null ],
          [ "getCurrentAfcRailIndicator", "md__r_e_a_d_m_e.html#autotoc_md100", null ],
          [ "getCurrentSoftMuteIndicator", "md__r_e_a_d_m_e.html#autotoc_md101", null ],
          [ "getCurrentStereoBlend", "md__r_e_a_d_m_e.html#autotoc_md102", null ],
          [ "getCurrentPilot", "md__r_e_a_d_m_e.html#autotoc_md103", null ],
          [ "getCurrentMultipath", "md__r_e_a_d_m_e.html#autotoc_md104", null ],
          [ "getCurrentSignedFrequencyOffset", "md__r_e_a_d_m_e.html#autotoc_md105", null ],
          [ "getCurrentMultipathDetectLow", "md__r_e_a_d_m_e.html#autotoc_md106", null ],
          [ "getCurrentMultipathDetectHigh", "md__r_e_a_d_m_e.html#autotoc_md107", null ],
          [ "getCurrentBlendDetectInterrupt", "md__r_e_a_d_m_e.html#autotoc_md108", null ]
        ] ],
        [ "<strong>Current AGC Status</strong>", "md__r_e_a_d_m_e.html#autotoc_md109", [
          [ "getAutomaticGainControl", "md__r_e_a_d_m_e.html#autotoc_md110", null ],
          [ "isAgcEnabled", "md__r_e_a_d_m_e.html#autotoc_md111", null ],
          [ "getAgcGainIndex", "md__r_e_a_d_m_e.html#autotoc_md112", null ],
          [ "setAutomaticGainControl", "md__r_e_a_d_m_e.html#autotoc_md113", null ],
          [ "setAvcAmMaxGain", "md__r_e_a_d_m_e.html#autotoc_md114", null ],
          [ "getCurrentAvcAmMaxGain()", "md__r_e_a_d_m_e.html#autotoc_md115", null ],
          [ "setAmSoftMuteMaxAttenuation", "md__r_e_a_d_m_e.html#autotoc_md116", null ]
        ] ],
        [ "Digital Audio", "md__r_e_a_d_m_e.html#autotoc_md117", [
          [ "digitalOutputFormat", "md__r_e_a_d_m_e.html#autotoc_md118", null ],
          [ "digitalOutputSampleRate", "md__r_e_a_d_m_e.html#autotoc_md119", null ]
        ] ],
        [ "Filters", "md__r_e_a_d_m_e.html#autotoc_md120", [
          [ "setBandwidth", "md__r_e_a_d_m_e.html#autotoc_md121", null ]
        ] ],
        [ "SI4735 Firmware Information", "md__r_e_a_d_m_e.html#autotoc_md122", [
          [ "getFirmwarePN", "md__r_e_a_d_m_e.html#autotoc_md123", null ],
          [ "getFirmwareFWMAJOR", "md__r_e_a_d_m_e.html#autotoc_md124", null ],
          [ "getFirmwareFWMINOR", "md__r_e_a_d_m_e.html#autotoc_md125", null ],
          [ "getFirmwarePATCHH", "md__r_e_a_d_m_e.html#autotoc_md126", null ],
          [ "getFirmwarePATCHL", "md__r_e_a_d_m_e.html#autotoc_md127", null ],
          [ "getFirmwareCMPMAJOR", "md__r_e_a_d_m_e.html#autotoc_md128", null ],
          [ "getFirmwareCMPMINOR", "md__r_e_a_d_m_e.html#autotoc_md129", null ],
          [ "getFirmwareCHIPREV", "md__r_e_a_d_m_e.html#autotoc_md130", null ]
        ] ],
        [ "RDS", "md__r_e_a_d_m_e.html#autotoc_md131", [
          [ "RDS Features implemented", "md__r_e_a_d_m_e.html#autotoc_md132", null ],
          [ "RDS Program type description", "md__r_e_a_d_m_e.html#autotoc_md133", null ],
          [ "RdsInit", "md__r_e_a_d_m_e.html#autotoc_md134", null ],
          [ "setRdsIntSource", "md__r_e_a_d_m_e.html#autotoc_md135", null ],
          [ "setRdsConfig", "md__r_e_a_d_m_e.html#autotoc_md136", null ],
          [ "getRdsStatus", "md__r_e_a_d_m_e.html#autotoc_md137", null ],
          [ "getRdsReceived", "md__r_e_a_d_m_e.html#autotoc_md139", null ],
          [ "getRdsSyncLost", "md__r_e_a_d_m_e.html#autotoc_md140", null ],
          [ "getRdsSyncFound", "md__r_e_a_d_m_e.html#autotoc_md141", null ],
          [ "getRdsNewBlockA", "md__r_e_a_d_m_e.html#autotoc_md142", null ],
          [ "getRdsNewBlockB", "md__r_e_a_d_m_e.html#autotoc_md143", null ],
          [ "getRdsSync", "md__r_e_a_d_m_e.html#autotoc_md144", null ],
          [ "getGroupLost", "md__r_e_a_d_m_e.html#autotoc_md145", null ],
          [ "getNumRdsFifoUsed", "md__r_e_a_d_m_e.html#autotoc_md146", null ],
          [ "getRdsPI", "md__r_e_a_d_m_e.html#autotoc_md147", null ],
          [ "getRdsGroupType", "md__r_e_a_d_m_e.html#autotoc_md148", null ],
          [ "getRdsVersionCode", "md__r_e_a_d_m_e.html#autotoc_md149", null ],
          [ "getRdsProgramType", "md__r_e_a_d_m_e.html#autotoc_md150", null ],
          [ "getRdsText", "md__r_e_a_d_m_e.html#autotoc_md151", null ],
          [ "getRdsTime", "md__r_e_a_d_m_e.html#autotoc_md152", null ]
        ] ],
        [ "SI4735 Patch Support for Single Side Band", "md__r_e_a_d_m_e.html#autotoc_md153", [
          [ "What does SSB patch means?", "md__r_e_a_d_m_e.html#autotoc_md154", null ],
          [ "queryLibraryId", "md__r_e_a_d_m_e.html#autotoc_md155", null ],
          [ "patchPowerUp", "md__r_e_a_d_m_e.html#autotoc_md156", null ],
          [ "downloadPatch", "md__r_e_a_d_m_e.html#autotoc_md157", null ]
        ] ],
        [ "Single Side Band (SSB) Methods", "md__r_e_a_d_m_e.html#autotoc_md158", [
          [ "setSSBBfo", "md__r_e_a_d_m_e.html#autotoc_md159", null ],
          [ "setSSBConfig", "md__r_e_a_d_m_e.html#autotoc_md160", null ],
          [ "setSSBDspAfc", "md__r_e_a_d_m_e.html#autotoc_md161", null ],
          [ "setSSBSoftMute", "md__r_e_a_d_m_e.html#autotoc_md162", null ],
          [ "setSsbSoftMuteMaxAttenuation", "md__r_e_a_d_m_e.html#autotoc_md163", null ],
          [ "setSSBAutomaticVolumeControl", "md__r_e_a_d_m_e.html#autotoc_md164", null ],
          [ "setSSBAvcDivider", "md__r_e_a_d_m_e.html#autotoc_md165", null ],
          [ "setSBBSidebandCutoffFilter", "md__r_e_a_d_m_e.html#autotoc_md166", null ],
          [ "setSSBAudioBandwidth", "md__r_e_a_d_m_e.html#autotoc_md167", null ],
          [ "setSSB", "md__r_e_a_d_m_e.html#autotoc_md168", null ]
        ] ],
        [ "References", "md__r_e_a_d_m_e.html#autotoc_md169", null ],
        [ "Videos", "md__r_e_a_d_m_e.html#autotoc_md170", [
          [ "Project examples made by the author", "md__r_e_a_d_m_e.html#autotoc_md171", null ],
          [ "Third-party projects using this library", "md__r_e_a_d_m_e.html#autotoc_md172", null ]
        ] ],
        [ "Join us on Facebook group \"Si47XX for Radio Experimenters\".", "md__r_e_a_d_m_e.html#autotoc_md173", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", "functions_vars" ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_s_i4735_8cpp.html",
"class_s_i4735.html#acacf1dac4c0ec170d1da629dd1fd30cb",
"md__r_e_a_d_m_e.html#autotoc_md79",
"unionsi47x__rqs__status.html#aa36d76a841b57cc87a0950cf39b6e2c1"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';
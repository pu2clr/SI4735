# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`SI4735`](#class_s_i4735) | [SI4735](#class_s_i4735) Class definition
`struct `[`si4735_digital_output_sample_rate`](#structsi4735__digital__output__sample__rate) | Digital audio output sample structure (Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE). Used to enable digital audio output and to configure the digital audio output sample rate in samples per second (sps).
`union `[`si4735_digital_output_format`](#unionsi4735__digital__output__format) | Digital audio output format data structure (Property 0x0102. DIGITAL_OUTPUT_FORMAT). Useed to configure: DCLK edge, data format, force mono, and sample precision.
`union `[`si473x_powerup`](#unionsi473x__powerup) | Power Up arguments data type
`union `[`si47x_agc_overrride`](#unionsi47x__agc__overrride) | If FM, Overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 (minimum attenuation) and 26 (maximum attenuation). If AM, overrides the AGC setting by disabling the AGC and forcing the gain index that ranges between 0
`union `[`si47x_agc_status`](#unionsi47x__agc__status) | AGC data types FM / AM and SSB structure to AGC
`union `[`si47x_antenna_capacitor`](#unionsi47x__antenna__capacitor) | Antenna Tuning Capacitor data type manupulation
`union `[`si47x_bandwidth_config`](#unionsi47x__bandwidth__config) | The bandwidth of the AM channel filter data type AMCHFLT values: 0 = 6 kHz Bandwidth 
`union `[`si47x_firmware_information`](#unionsi47x__firmware__information) | Data representation for Firmware Information (GET_REV) The part number, chip revision, firmware revision, patch revision and component revision numbers.
`union `[`si47x_firmware_query_library`](#unionsi47x__firmware__query__library) | Firmware Query Library ID response. Used to represent the response of a power up command with FUNC = 15 (patch)
`union `[`si47x_frequency`](#unionsi47x__frequency) | Represents how the frequency is stored in the si4735. It helps to convert frequency in uint16_t to two bytes (uint8_t) (FREQL and FREQH)
`union `[`si47x_property`](#unionsi47x__property) | Property Data type (help to deal with SET_PROPERTY command on si473X)
`union `[`si47x_rds_blocka`](#unionsi47x__rds__blocka) | Block A data type
`union `[`si47x_rds_blockb`](#unionsi47x__rds__blockb) | Block B data type
`union `[`si47x_rds_command`](#unionsi47x__rds__command) | FM_RDS_STATUS (0x24) command Data type for command and response information 
`union `[`si47x_rds_config`](#unionsi47x__rds__config) | Data type for FM_RDS_CONFIG Property
`union `[`si47x_rds_date_time`](#unionsi47x__rds__date__time) | Group type 4A ( RDS Date and Time) When group type 4A is used by the station, it shall be transmitted every minute according to EN 50067. This Structure uses blocks 2,3 and 5 (B,C,D)
`union `[`si47x_rds_int_source`](#unionsi47x__rds__int__source) | FM_RDS_INT_SOURCE property data type
`union `[`si47x_rds_status`](#unionsi47x__rds__status) | Response data type for current channel and reads an entry from the RDS FIFO.
`union `[`si47x_response_status`](#unionsi47x__response__status) | Response status command
`union `[`si47x_rqs_status`](#unionsi47x__rqs__status) | Data type for status information about the received signal quality FM_RSQ_STATUS and AM_RSQ_STATUS
`union `[`si47x_seek`](#unionsi47x__seek) | Represents searching for a valid frequency data type.
`union `[`si47x_set_frequency`](#unionsi47x__set__frequency) | AM_TUNE_FREQ data type command
`union `[`si47x_ssb_mode`](#unionsi47x__ssb__mode) | SSB - datatype for SSB_MODE (property 0x0101)
`union `[`si47x_tune_status`](#unionsi47x__tune__status) | Status of FM_TUNE_FREQ or FM_SEEK_START commands or Status of AM_TUNE_FREQ or AM_SEEK_START commands.

# class `SI4735` {#class_s_i4735}

[SI4735](#class_s_i4735) Class definition

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`SI4735`](#class_s_i4735_1a818c60896490631e4311452c99ff361f)`()` | This is a library for the [SI4735](#class_s_i4735), BROADCAST AM/FM/SW RADIO RECEIVER, IC from Silicon Labs for the Arduino development environment. It works with I2C protocol. This library is intended to provide an easier interface for controlling the [SI4735](#class_s_i4735). 
`public void `[`reset`](#class_s_i4735_1adc6014674bd4d1ee3aab56c605926db1)`(void)` | Reset the SI473X 
`public void `[`waitToSend`](#class_s_i4735_1a7b45c1b22c3c1a3f2326bee913d1e1e0)`(void)` | Wait for the si473x is ready (Clear to Send (CTS) status bit have to be 1). 
`public void `[`setup`](#class_s_i4735_1a944ed704e2175a6cf660ad65ca5bbad5)`(uint8_t resetPin,uint8_t defaultFunction)` | Starts the Si473X device. 
`public void `[`setup`](#class_s_i4735_1a95fd37277dbb8b16b5ae7a996a5f9eba)`(uint8_t resetPin,int interruptPin,uint8_t defaultFunction,uint8_t audioMode)` | Starts the Si473X device.
`public void `[`setPowerUp`](#class_s_i4735_1a8bc001cb28dc8fbd88ba0fa7024eca39)`(uint8_t CTSIEN,uint8_t GPO2OEN,uint8_t PATCH,uint8_t XOSCEN,uint8_t FUNC,uint8_t OPMODE)` | Set the Power Up parameters for si473X. Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()
`public void `[`radioPowerUp`](#class_s_i4735_1a3e228da65641f4dcf4253262e19bde9b)`(void)` | Powerup the Si47XX Before call this function call the setPowerUp to set up the parameters. Parameters you have to set up with setPowerUp
`public void `[`analogPowerUp`](#class_s_i4735_1a412f51386c47a17cf04fd0a3c3078726)`(void)` | Powerup in Analog Mode. It will be deprecated. Consider use radioPowerUp instead. Actually this function works fo Digital and Analog modes. You have to call setPowerUp method before.
`public void `[`powerDown`](#class_s_i4735_1aa554b3390fb6c5a4d6661b96afae73ae)`(void)` | Moves the device from powerup to powerdown mode. After Power Down command, only the Power Up command is accepted.
`public void `[`setFrequency`](#class_s_i4735_1a63d205f2d3169ba877a2bd2ee28d224f)`(uint16_t)` | Set the frequency to the corrent function of the Si4735 (FM, AM or SSB) You have to call setup or setPowerUp before call setFrequency.
`public void `[`getStatus`](#class_s_i4735_1a3cf856c44fcfffb3d81b22b1d09b881f)`()` | Gets the current status of the Si4735 (AM or FM)
`public void `[`getStatus`](#class_s_i4735_1a5c0f4036a7fbbd11a26f1f6f4579d3cd)`(uint8_t,uint8_t)` | Gets the current status of the Si4735 (AM or FM) 
`public uint16_t `[`getFrequency`](#class_s_i4735_1a7e88bb573604d465536c13554d5c61f8)`(void)` | Device Status Information Gets the current frequency of the Si4735 (AM or FM) The method status do it an more. See getStatus below.
`public uint16_t `[`getCurrentFrequency`](#class_s_i4735_1a8240278970ae8bf7283e14714b4329ad)`()` | Gets the current frequency saved in memory. Unlike getFrequency, this method gets the current frequency recorded after the last setFrequency command. This method avoids bus traffic and CI processing. However, you can not get others status information like RSSI.
`public inline bool `[`getSignalQualityInterrupt`](#class_s_i4735_1ad194137a6a712a599e3c26e5b6357594)`()` | STATUS RESPONSE Set of methods to get current status information. Call them after getStatus or getFrequency or seekStation See Si47XX PROGRAMMING GUIDE; AN332; pages 63
`public inline bool `[`getRadioDataSystemInterrupt`](#class_s_i4735_1a97dc44d9927ea58730a18fb6baa6867f)`()` | Gets Received Signal Quality Interrupt(RSQINT)
`public inline bool `[`getTuneCompleteTriggered`](#class_s_i4735_1a2cc659bddc5efc7695f2a6031c2edd4b)`()` | Gets Radio Data System (RDS) Interrupt.
`public inline bool `[`getStatusError`](#class_s_i4735_1a69723840497ed693d6f9a4e840ab07c9)`()` | Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
`public inline bool `[`getStatusCTS`](#class_s_i4735_1afcb38f53b6eb71cadcefa0bb472ca943)`()` | Return the Error flag (true or false) of status of the least Tune or Seek.
`public inline bool `[`getACFIndicator`](#class_s_i4735_1ab8a51de592fd40fdd872039b3c70716f)`()` | Gets the Error flag of status response.
`public inline bool `[`getBandLimit`](#class_s_i4735_1a19e96565a2dec7ec68469f45c8d3e084)`()` | Returns true if the AFC rails (AFC Rail Indicator).
`public inline bool `[`getStatusValid`](#class_s_i4735_1afb19da8a05320d7f38cca8325725edfd)`()` | Returns true if a seek hit the band limit (WRAP = 0 in FM_START_SEEK) or wrapped to the original frequency(WRAP = 1).
`public inline uint8_t `[`getReceivedSignalStrengthIndicator`](#class_s_i4735_1aeeefd12b49dbc764e0f45fe208441482)`()` | Returns true if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108)
`public inline uint8_t `[`getStatusSNR`](#class_s_i4735_1ac0bd4daaa321ef8956dca4d1e4e5cd3c)`()` | Returns integer Received Signal Strength Indicator (dBμV).
`public inline uint8_t `[`getStatusMULT`](#class_s_i4735_1a0f9b5438a6f92c8f2e538ee640f353ab)`()` | Returns integer containing the SNR metric when tune is complete (dB).
`public inline uint8_t `[`getAntennaTuningCapacitor`](#class_s_i4735_1a3905d6551992b4dd51d13f05978d9081)`()` | Returns integer containing the multipath metric when tune is complete.
`public void `[`getAutomaticGainControl`](#class_s_i4735_1aaae187575b9868ff427c063c3a6cca2a)`()` | Returns integer containing the current antenna tuning capacitor value.
`public void `[`setAvcAmMaxGain`](#class_s_i4735_1a98d12da4b801493a350ecf770f4f2d8b)`(uint8_t gain)` | Sets the maximum gain for automatic volume control. If no parameter is sent, it will be consider 48dB.
`public inline void `[`setAvcAmMaxGain`](#class_s_i4735_1a6e1e0e48d71e09debb07fc6dbe884829)`()` | 
`public inline uint8_t `[`getCurrentAvcAmMaxGain`](#class_s_i4735_1a27dd7dfd46583eb5be91cfc9461807ff)`()` | 
`public inline void `[`setAmSoftMuteMaxAttenuation`](#class_s_i4735_1acc57f6bde08e94462a9d75fe28a66ea8)`(uint8_t smattn)` | 
`public inline void `[`setAmSoftMuteMaxAttenuation`](#class_s_i4735_1ac1af311f799e8a007cbd048a88f1abe9)`()` | 
`public inline void `[`setSsbSoftMuteMaxAttenuation`](#class_s_i4735_1ad357ccc5f207ec11d36d9569046c098f)`(uint8_t smattn)` | 
`public inline void `[`setSsbSoftMuteMaxAttenuation`](#class_s_i4735_1afd057daf737a78a50ad1a03a6b43a111)`()` | 
`public inline bool `[`isAgcEnabled`](#class_s_i4735_1aea08b7e553212cbe01bf6f4ec9e02de7)`()` | 
`public inline uint8_t `[`getAgcGainIndex`](#class_s_i4735_1a4eceaf5745e87fcca6cf31c04c5635bd)`()` | 
`public void `[`setAutomaticGainControl`](#class_s_i4735_1a27c22f5671ce3229e925e5fbe93ba0a6)`(uint8_t AGCDIS,uint8_t AGCIDX)` | If FM, overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 (minimum attenuation) and 26 (maximum attenuation); If AM/SSB, Overrides the AM AGC setting by disabling the AGC and forcing the gain index that ranges between 0 (minimum attenuation) and 37+ATTN_BACKUP (maximum attenuation);
`public void `[`getCurrentReceivedSignalQuality`](#class_s_i4735_1abf59183da13c72384093fc238c3114cf)`(uint8_t INTACK)` | Queries the status of the Received Signal Quality (RSQ) of the current channel. This method sould be called berore call getCurrentRSSI(), [getCurrentSNR()](#class_s_i4735_1adb3fe55583dafeac7439a303b637d162) etc. Command FM_RSQ_STATUS
`public void `[`getCurrentReceivedSignalQuality`](#class_s_i4735_1a7393fa4f9d7811c8d0c0f48283ef6435)`(void)` | Queries the status of the Received Signal Quality (RSQ) of the current channel Command FM_RSQ_STATUS
`public inline uint8_t `[`getCurrentRSSI`](#class_s_i4735_1a4f5abf562ec00767a3b84f74eba5ac4d)`()` | 
`public inline uint8_t `[`getCurrentSNR`](#class_s_i4735_1adb3fe55583dafeac7439a303b637d162)`()` | current receive signal strength (0–127 dBμV).
`public inline bool `[`getCurrentRssiDetectLow`](#class_s_i4735_1a0c9ae82012cdc97c4080b84e37fa1820)`()` | current SNR metric (0–127 dB).
`public inline bool `[`getCurrentRssiDetectHigh`](#class_s_i4735_1adf11a66236392ca107b5d52a8747f838)`()` | RSSI Detect Low.
`public inline bool `[`getCurrentSnrDetectLow`](#class_s_i4735_1a1296592571c85b89ed59a4ee2324bd52)`()` | RSSI Detect High.
`public inline bool `[`getCurrentSnrDetectHigh`](#class_s_i4735_1aec3c71a3252dd6660e5bceebc0d1f988)`()` | SNR Detect Low.
`public inline bool `[`getCurrentValidChannel`](#class_s_i4735_1a5ce1abf8c9d53c027f6b0870efedda1b)`()` | SNR Detect High.
`public inline bool `[`getCurrentAfcRailIndicator`](#class_s_i4735_1affe5cee9daf44677802a94f96d893c94)`()` | Valid Channel.
`public inline bool `[`getCurrentSoftMuteIndicator`](#class_s_i4735_1a76e8485292878c1294e5910601f4575f)`()` | AFC Rail Indicator.
`public inline uint8_t `[`getCurrentStereoBlend`](#class_s_i4735_1acaafab62dd411dc9b79f9fb96e8c8832)`()` | Soft Mute Indicator. Indicates soft mute is engaged.
`public inline bool `[`getCurrentPilot`](#class_s_i4735_1a127fa3d2f9b7d57169bbef95703fb4c9)`()` | Indicates amount of stereo blend in % (100 = full stereo, 0 = full mono).
`public inline uint8_t `[`getCurrentMultipath`](#class_s_i4735_1a6d521bb5c7ef7811e57c7128cb2106b3)`()` | Indicates stereo pilot presence.
`public inline uint8_t `[`getCurrentSignedFrequencyOffset`](#class_s_i4735_1a2b3bfeca3b545f5741b1ed70a1dba077)`()` | Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
`public inline bool `[`getCurrentMultipathDetectLow`](#class_s_i4735_1ad194a5c915620c6a1b8b3d6d7ed645d9)`()` | Signed frequency offset (kHz).
`public inline bool `[`getCurrentMultipathDetectHigh`](#class_s_i4735_1a82d0865cf348deb0c9f81a2f12222fb0)`()` | Multipath Detect Low.
`public inline bool `[`getCurrentBlendDetectInterrupt`](#class_s_i4735_1a16107a2f2e51cce5a7e2b1b303425118)`()` | Multipath Detect High.
`public inline uint8_t `[`getFirmwarePN`](#class_s_i4735_1ab26fbd99452daaf18875ff1c3c35d081)`()` | Blend Detect Interrupt.
`public inline uint8_t `[`getFirmwareFWMAJOR`](#class_s_i4735_1ac2f451cb7320b66d8e08698dcacd50d0)`()` | RESP1 - Part Number (HEX)
`public inline uint8_t `[`getFirmwareFWMINOR`](#class_s_i4735_1a74b68fcd5718520fc0683977dfc18fc8)`()` | RESP2 - Returns the Firmware Major Revision (ASCII).
`public inline uint8_t `[`getFirmwarePATCHH`](#class_s_i4735_1a58b99a8d5ec2199cb5f2105675aa7a56)`()` | RESP3 - Returns the Firmware Minor Revision (ASCII).
`public inline uint8_t `[`getFirmwarePATCHL`](#class_s_i4735_1ac10b3cd3875985e2c129fea14e2bc47a)`()` | RESP4 - Returns the Patch ID High byte (HEX).
`public inline uint8_t `[`getFirmwareCMPMAJOR`](#class_s_i4735_1ab57870d69969e73265e9cffe2a73d868)`()` | RESP5 - Returns the Patch ID Low byte (HEX).
`public inline uint8_t `[`getFirmwareCMPMINOR`](#class_s_i4735_1a883645d266a612ce07d71fdb2cd81e5f)`()` | RESP6 - Returns the Component Major Revision (ASCII).
`public inline uint8_t `[`getFirmwareCHIPREV`](#class_s_i4735_1acd363d9111e4a57cf2e1167634bc1b5f)`()` | RESP7 - Returns the Component Minor Revision (ASCII).
`public void `[`setVolume`](#class_s_i4735_1a3b7543657dfd36da962267bb8692b796)`(uint8_t volume)` | RESP8 - Returns the Chip Revision (ASCII).
`public uint8_t `[`getVolume`](#class_s_i4735_1acf79be50a01fe91abd498e5d42d83317)`()` | Gets the current volume level.
`public void `[`volumeDown`](#class_s_i4735_1a23d49ab35895b76e0898405fac086c09)`()` | Set sound volume level Down 
`public void `[`volumeUp`](#class_s_i4735_1a5e3e8cf04ca99c6be2dbe15d72b4efa3)`()` | Set sound volume level Up 
`public inline uint8_t `[`getCurrentVolume`](#class_s_i4735_1a430b86ccfd4d6c89da9202a538e0cfc9)`()` | 
`public void `[`setAudioMute`](#class_s_i4735_1a12d4e173d096b1568e4209a63534d4f6)`(bool off)` | Returns the current volume level.
`public void `[`digitalOutputFormat`](#class_s_i4735_1aea652d358b4b586b6d5964ceb8eb7c81)`(uint8_t OSIZE,uint8_t OMONO,uint8_t OMODE,uint8_t OFALL)` | Digital Audio Setup Configures the digital audio output format. Options: DCLK edge, data format, force mono, and sample precision.
`public void `[`digitalOutputSampleRate`](#class_s_i4735_1a1a404ca11fb32b457be2d7cfe484dd03)`(uint16_t DOSR)` | Enables digital audio output and configures digital audio output sample rate in samples per second (sps).
`public void `[`setAM`](#class_s_i4735_1a753390952a1f2c2a98343979f1f3daa0)`()` | Sets the radio to AM function. It means: LW MW and SW.
`public void `[`setFM`](#class_s_i4735_1acd89e7b8adefcbc4e89f802acb915906)`()` | Sets the radio to FM function
`public void `[`setAM`](#class_s_i4735_1a43c5ea36995a3897820e0170d94c81f4)`(uint16_t fromFreq,uint16_t toFreq,uint16_t intialFreq,uint16_t step)` | Sets the radio to AM (LW/MW/SW) function.
`public void `[`setFM`](#class_s_i4735_1a673bc4b17d564bb6e7f3e9a61d35fbb3)`(uint16_t fromFreq,uint16_t toFreq,uint16_t initialFreq,uint16_t step)` | Sets the radio to FM function.
`public void `[`setBandwidth`](#class_s_i4735_1a8755743a36d2d4662c8db918d02ee4e1)`(uint8_t AMCHFLT,uint8_t AMPLFLT)` | Selects the bandwidth of the channel filter for AM reception. The choices are 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz). The default bandwidth is 2 kHz. Works only in AM / SSB (LW/MW/SW)
`public void `[`setFrequencyStep`](#class_s_i4735_1a794f6fe7987e146a0135b8e6fabec55c)`(uint16_t step)` | Sets the current step value.
`public inline uint8_t `[`getTuneFrequencyFast`](#class_s_i4735_1a8c54bfc1fcf1dd989e697cc9b7094f59)`()` | 
`public inline void `[`setTuneFrequencyFast`](#class_s_i4735_1a51d816112d974f10306d9d001667395a)`(uint8_t FAST)` | Returns the FAST tuning status.
`public inline uint8_t `[`getTuneFrequencyFreeze`](#class_s_i4735_1a667fead0c7b2658c2b932370ad4e4f88)`()` | FAST Tuning. If set, executes fast and invalidated tune. The tune status will not be accurate.
`public inline void `[`setTuneFrequencyFreeze`](#class_s_i4735_1aee22a5ce06d4948fad2365a7b62fbd8d)`(uint8_t FREEZE)` | Returns the FREEZE status.
`public void `[`setTuneFrequencyAntennaCapacitor`](#class_s_i4735_1aaceadaaf0835e3db124d1b36f2aebd3c)`(uint16_t capacitor)` | Only FM. Freeze Metrics During Alternate Frequency Jump.
`public void `[`frequencyUp`](#class_s_i4735_1ac0fcfd64dcf30b11d4dc219d9f7fc5d9)`()` | Increments the current frequency on current band/function by using the current step.
`public void `[`frequencyDown`](#class_s_i4735_1a93fff524a6f900ac91eb1222ce73f9ba)`()` | Decrements the current frequency on current band/function by using the current step.
`public bool `[`isCurrentTuneFM`](#class_s_i4735_1ae7ca2ffa6ad68ca69e4f35a3c40bbc3c)`()` | Returns true if the current function is FM (FM_TUNE_FREQ).
`public void `[`getFirmware`](#class_s_i4735_1a2fbc7e2187a7c645879623100dbc8502)`(void)` | Gets firmware information
`public void `[`setFunction`](#class_s_i4735_1a39f0e95a5efe5bd9add926c6f538528a)`(uint8_t FUNC)` | 
`public void `[`seekStation`](#class_s_i4735_1a3280b98b8331f00b4d0158ef1b076533)`(uint8_t SEEKUP,uint8_t WRAP)` | Look for a station
`public void `[`seekStationUp`](#class_s_i4735_1aad5211e3fff28141473442f59008e415)`()` | Search for the next station
`public void `[`seekStationDown`](#class_s_i4735_1a1eeaa09670cdbdc7ddd724c940d2506b)`()` | Search the previous station
`public void `[`setSeekAmLimits`](#class_s_i4735_1a6b901d5cd729364dbda6e5b811dc9ec2)`(uint16_t bottom,uint16_t top)` | Sets the bottom frequency and top frequency of the AM band for seek. Default is 520 to 1710.
`public void `[`setSeekAmSpacing`](#class_s_i4735_1a6f8218c0bbdcb75ea54bc8af2a60dcc6)`(uint16_t spacing)` | Selects frequency spacingfor AM seek. Default is 10 kHz spacing.
`public void `[`setSeekSrnThreshold`](#class_s_i4735_1aeef76371500acdbdc17ca8dc980e1fff)`(uint16_t value)` | Sets the SNR threshold for a valid AM Seek/Tune. If the value is zero then SNR threshold is not considered when doing a seek. Default value is 5 dB.
`public void `[`setSeekRssiThreshold`](#class_s_i4735_1a13397b273f7ab6fa7d5f48f116edac9b)`(uint16_t value)` | Sets the RSSI threshold for a valid AM Seek/Tune. If the value is zero then RSSI threshold is not considered when doing a seek. Default value is 25 dBμV.
`public void `[`setFmBlendStereoThreshold`](#class_s_i4735_1ac598865516ca09db7dbfa6a84da7f179)`(uint8_t parameter)` | Sets RSSI threshold for stereo blend (Full stereo above threshold, blend below threshold). To force stereo, set this to 0. To force mono, set this to 127.
`public void `[`setFmBlendMonoThreshold`](#class_s_i4735_1adf4c5ec4473fd8486dd2ec7a19a10d24)`(uint8_t parameter)` | Sets RSSI threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo set this to 0. To force mono set this to 127. Default value is 30 dBμV.
`public void `[`setFmBlendRssiStereoThreshold`](#class_s_i4735_1ab063630466dccea3becc11d60d122904)`(uint8_t parameter)` | Sets RSSI threshold for stereo blend. (Full stereo above threshold, blend below threshold.) To force stereo, set this to 0. To force mono, set this to 127. Default value is 49 dBμV. 
`public void `[`setFmBLendRssiMonoThreshold`](#class_s_i4735_1a1b134a94138b3bf993a5d3eb5f8ed5c6)`(uint8_t parameter)` | Sets RSSI threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo, set this to 0. To force mono, set this to 127. Default value is 30 dBμV.
`public void `[`setFmBlendSnrStereoThreshold`](#class_s_i4735_1a1d1a5eed976370d99827ba55d08b9293)`(uint8_t parameter)` | Sets SNR threshold for stereo blend (Full stereo above threshold, blend below threshold). To force stereo, set this to 0. To force mono, set this to 127. Default value is 27 dB.
`public void `[`setFmBLendSnrMonoThreshold`](#class_s_i4735_1ae2c30536e59f6691d990a2b9cf21f07e)`(uint8_t parameter)` | Sets SNR threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo, set this to 0. To force mono, set this to 127. Default value is 14 dB.
`public void `[`setFmBlendMultiPathStereoThreshold`](#class_s_i4735_1a0ed776a193fb2fcff783d4a82ff714e5)`(uint8_t parameter)` | Sets multipath threshold for stereo blend (Full stereo below threshold, blend above threshold). To force stereo, set this to 100. To force mono, set this to 0. Default value is 20.
`public void `[`setFmBlendMultiPathMonoThreshold`](#class_s_i4735_1acc9fd27b38f49f167facfcc4f6216c5e)`(uint8_t parameter)` | Sets Multipath threshold for mono blend (Full mono above threshold, blend below threshold). To force stereo, set to 100. To force mono, set to 0. The default is 60.
`public void `[`setFmStereoOn`](#class_s_i4735_1a52c7b7b1f0ed9121b0e64516b2bd2a55)`()` | Turn Off Stereo operation.
`public void `[`setFmStereoOff`](#class_s_i4735_1a59ce1008944ff9a51b6c5baeb3a3e2cb)`()` | Turn Off Stereo operation.
`public void `[`RdsInit`](#class_s_i4735_1affdba9ca9fb9bf5bb671ac3d448a7e89)`()` | RDS implementation Starts the control variables for RDS.
`public void `[`setRdsIntSource`](#class_s_i4735_1a530e64d00d95b4ceb13ce33aa74c63cb)`(uint8_t RDSNEWBLOCKB,uint8_t RDSNEWBLOCKA,uint8_t RDSSYNCFOUND,uint8_t RDSSYNCLOST,uint8_t RDSRECV)` | Configures interrupt related to RDS
`public void `[`getRdsStatus`](#class_s_i4735_1a350a2033671a7620d4b9290a2347e185)`(uint8_t INTACK,uint8_t MTFIFO,uint8_t STATUSONLY)` | Gets the RDS status. Store the status in currentRdsStatus member. RDS COMMAND FM_RDS_STATUS
`public void `[`getRdsStatus`](#class_s_i4735_1a94cce3834f85fc127e8c2dab3a03e875)`()` | Gets RDS Status. Same result of calling getRdsStatus(0,0,0);
`public inline bool `[`getRdsReceived`](#class_s_i4735_1a6e8405ea5846fab586080a7be696178e)`()` | 
`public inline bool `[`getRdsSyncLost`](#class_s_i4735_1ac283a8a89cc885c2740c65a7128ff7ef)`()` | 1 = FIFO filled to minimum number of groups
`public inline bool `[`getRdsSyncFound`](#class_s_i4735_1a78e906f12771490771c624be1777784d)`()` | 1 = Lost RDS synchronization
`public inline bool `[`getRdsNewBlockA`](#class_s_i4735_1a561a052a8c737302368274c774d8c129)`()` | 1 = Found RDS synchronization
`public inline bool `[`getRdsNewBlockB`](#class_s_i4735_1aff740d85cc04c257f53c7e910bd45fcf)`()` | 1 = Valid Block A data has been received.
`public inline bool `[`getRdsSync`](#class_s_i4735_1a6e899b37cbc3dcd0ab969724e4008f0f)`()` | 1 = Valid Block B data has been received.
`public inline bool `[`getGroupLost`](#class_s_i4735_1a8db437efc42a81466d8daf4dcc35c50c)`()` | 1 = RDS currently synchronized.
`public inline uint8_t `[`getNumRdsFifoUsed`](#class_s_i4735_1a819c370ff03c52a527fbc85f9877f526)`()` | 1 = One or more RDS groups discarded due to FIFO overrun.
`public void `[`setRdsConfig`](#class_s_i4735_1ad9718a85c79a311c26997bbda20f2351)`(uint8_t RDSEN,uint8_t BLETHA,uint8_t BLETHB,uint8_t BLETHC,uint8_t BLETHD)` | RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).
`public uint16_t `[`getRdsPI`](#class_s_i4735_1af1e67b201799a9e8d7208a32a65656b5)`(void)` | Returns the programa type. Read the Block A content
`public uint8_t `[`getRdsGroupType`](#class_s_i4735_1aa35366e5cded7b864f7268a7e4a8dbe9)`(void)` | Returns the Group Type (extracted from the Block B)
`public uint8_t `[`getRdsFlagAB`](#class_s_i4735_1a05f76a0a0ebda813483f1df24e890904)`(void)` | Returns the current Text Flag A/B 
`public uint8_t `[`getRdsVersionCode`](#class_s_i4735_1af9b6cb614d1b01b11bb23c19e6deb8b9)`(void)` | Gets the version code (extracted from the Block B)
`public uint8_t `[`getRdsProgramType`](#class_s_i4735_1ac28fa3818d4f900c4f4ba6be10c1405b)`(void)` | Returns the Program Type (extracted from the Block B)
`public uint8_t `[`getRdsTextSegmentAddress`](#class_s_i4735_1ab1ed04a109330e7e9206483c7cdf5ef9)`(void)` | Returns the address of the text segment. 2A - Each text segment in version 2A groups consists of four characters. A messages of this group can be have up to 64 characters. 2B - In version 2B groups, each text segment consists of only two characters. When the current RDS status is using this version, the maximum message length will be 32 characters.
`public char * `[`getRdsText`](#class_s_i4735_1a71cbaa3e0c103db7cee5969d162bec5d)`(void)` | Gets the RDS Text when the message is of the Group Type 2 version A
`public char * `[`getRdsText0A`](#class_s_i4735_1a12d917629b78fffebac5ecf8f2936921)`(void)` | Gets the station name and other messages.
`public char * `[`getRdsText2A`](#class_s_i4735_1ae54d0f5562ca7615c3b90bee00c29ab4)`(void)` | Gets the Text processed for the 2A group
`public char * `[`getRdsText2B`](#class_s_i4735_1a806d5cfbbeb6ca1299ce20b1220fdb18)`(void)` | Gets the Text processed for the 2B group
`public char * `[`getRdsTime`](#class_s_i4735_1aa8436bb156f3ac8321c65941be42fdba)`(void)` | Gets the RDS time and date when the Group type is 4
`public void `[`getNext2Block`](#class_s_i4735_1ab1f4f137139b40fe386ca51586999e50)`(char *)` | Process data received from group 2B
`public void `[`getNext4Block`](#class_s_i4735_1adb0a3c39aeacba0faf39aadd9465021f)`(char *)` | Process data received from group 2A
`public void `[`ssbSetup`](#class_s_i4735_1a6f045d3725a457116236ee92d41c4265)`()` | Starts the Si473X device on SSB (same AM Mode). Same [SI4735::setup](#class_s_i4735_1a944ed704e2175a6cf660ad65ca5bbad5) optimized to improve loading patch performance
`public void `[`setSSBBfo`](#class_s_i4735_1a55cf9a091d314a1aa21552f2254f756f)`(int offset)` | Single Side Band (SSB) implementation
`public void `[`setSSBConfig`](#class_s_i4735_1a9224e7d4ef14c887da96dbefd2dcefd3)`(uint8_t AUDIOBW,uint8_t SBCUTFLT,uint8_t AVC_DIVIDER,uint8_t AVCEN,uint8_t SMUTESEL,uint8_t DSP_AFCDIS)` | Set the SSB receiver mode details: 1) Enable or disable AFC track to carrier function for receiving normal AM signals; 2) Set the audio bandwidth; 3) Set the side band cutoff filter; 4) Set soft-mute based on RSSI or SNR; 5) Enable or disbable automatic volume control (AVC) function.
`public void `[`setSSB`](#class_s_i4735_1a7c48f2af416b1f89dbedbfbe9a637620)`(uint16_t fromFreq,uint16_t toFreq,uint16_t intialFreq,uint16_t step,uint8_t usblsb)` | 
`public void `[`setSSB`](#class_s_i4735_1a3e907ddf560a0c8286fa55768701b3da)`(uint8_t usblsb)` | Set the radio to AM function. It means: LW MW and SW.
`public void `[`setSSBAudioBandwidth`](#class_s_i4735_1a6b2ad4d027463262350134490d16bf32)`(uint8_t AUDIOBW)` | SSB Audio Bandwidth for SSB mode
`public void `[`setSSBAutomaticVolumeControl`](#class_s_i4735_1acacf1dac4c0ec170d1da629dd1fd30cb)`(uint8_t AVCEN)` | Sets SSB Automatic Volume Control (AVC) for SSB mode
`public void `[`setSBBSidebandCutoffFilter`](#class_s_i4735_1ae9d50b24642784943fefedbac944a8e8)`(uint8_t SBCUTFLT)` | Sets SBB Sideband Cutoff Filter for band pass and low pass filters: 0 = Band pass filter to cutoff both the unwanted side band and high frequency components > 2.0 kHz of the wanted side band. (default) 1 = Low pass filter to cutoff the unwanted side band. Other values = not allowed.
`public void `[`setSSBAvcDivider`](#class_s_i4735_1afc5e87c50a1eba6de9504c114cc1cb70)`(uint8_t AVC_DIVIDER)` | Sets AVC Divider
`public void `[`setSSBDspAfc`](#class_s_i4735_1a6cde26a5abf759a628012d1457d85c72)`(uint8_t DSP_AFCDIS)` | Sets DSP AFC disable or enable
`public void `[`setSSBSoftMute`](#class_s_i4735_1a91251dfc2ff9488aaef3aa1a528097d9)`(uint8_t SMUTESEL)` | Sets SSB Soft-mute Based on RSSI or SNR Selection:
`public `[`si47x_firmware_query_library`](#unionsi47x__firmware__query__library)` `[`queryLibraryId`](#class_s_i4735_1adfc2c82524918224cf19fec5b7fcf6d1)`()` | SI47XX PATCH RESOURCES Call it first if you are applying a patch on [SI4735](#class_s_i4735). Used to confirm if the patch is compatible with the internal device library revision. See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220.
`public void `[`patchPowerUp`](#class_s_i4735_1afaabd6c73b7ad7c20ae959d5b75c1e71)`()` | This method can be used to prepare the device to apply SSBRX patch Call queryLibraryId before call this method. Powerup the device by issuing the POWER_UP command with FUNC = 1 (AM/SW/LW Receive)
`public bool `[`downloadPatch`](#class_s_i4735_1afd1343bd8b4f0c290ef81c418222036c)`(const uint8_t * ssb_patch_content,const uint16_t ssb_patch_content_size)` | Transfers the content of a patch stored in a array of bytes to the [SI4735](#class_s_i4735) device. You must mount an array as shown below and know the size of that array as well.
`public bool `[`downloadPatch`](#class_s_i4735_1aa99cdde8dd65eb92f2ada159cc215c98)`(int eeprom_i2c_address)` | Under construction... Transfers the content of a patch stored in a eeprom to the [SI4735](#class_s_i4735) device.
`public void `[`ssbPowerUp`](#class_s_i4735_1a0fcfd8e487c594707b3f2aaa71a86d68)`()` | This function can be useful for debug and teste.
`public inline void `[`setI2CLowSpeedMode`](#class_s_i4735_1aecfcd50dbb3d95d66c8cdc77d52af63c)`(void)` | 
`public inline void `[`setI2CStandardMode`](#class_s_i4735_1a00ade87631677b8cc88098d0b55cd5a4)`(void)` | Sets I2C buss to 10KHz.
`public inline void `[`setI2CFastMode`](#class_s_i4735_1a7bb3214f70381cf26bb93f96729d641e)`(void)` | Sets I2C buss to 100KHz.
`public inline void `[`setI2CFastModeCustom`](#class_s_i4735_1a7433b4e80fd3b501efeae7973d29c985)`(long value)` | Sets I2C buss to 400KHz.
`public void `[`setDeviceI2CAddress`](#class_s_i4735_1a748a4027f317972cb2fb1aed4865973b)`(uint8_t senPin)` | Sets the I2C Bus Address
`public int16_t `[`getDeviceI2CAddress`](#class_s_i4735_1a8eddbfb3e62786aa4843d4bcd25da2b9)`(uint8_t resetPin)` | Scans for two possible addresses for the Si47XX (0x11 or 0x63 ) This function also sets the system to the found I2C bus address of Si47XX.
`public void `[`setDeviceOtherI2CAddress`](#class_s_i4735_1a668008457910e916ebadf15bda2e5b29)`(uint8_t i2cAddr)` | Sets the onther I2C Bus Address (for Si470X) You can set another I2C address different of 0x11 and 0x63 
`protected char `[`rds_buffer2A`](#class_s_i4735_1a0b40791484c47c0bf90d8dc255a20269) | 
`protected char `[`rds_buffer2B`](#class_s_i4735_1a7aa607f80e97d45ac7a80cdcbdeb09f9) | RDS Radio Text buffer - Program Information.
`protected char `[`rds_buffer0A`](#class_s_i4735_1ac24a8bbd39628c2981208cb452165a0e) | RDS Radio Text buffer - Station Informaation.
`protected char `[`rds_time`](#class_s_i4735_1a1746daa81b363880dd106fa430784bf8) | RDS Basic tuning and switching information (Type 0 groups)
`protected int `[`rdsTextAdress2A`](#class_s_i4735_1a1c7ec3559a78c5ef1bda49cb0abdf1de) | 
`protected int `[`rdsTextAdress2B`](#class_s_i4735_1adecc2478d73cd7d18e1f88fd70d1488d) | 
`protected int `[`rdsTextAdress0A`](#class_s_i4735_1a11477d1eb13efb348395a4984b67f392) | 
`protected int16_t `[`deviceAddress`](#class_s_i4735_1a243ecc7d01fc0d0e9cabb4ed00b9e47d) | 
`protected uint8_t `[`lastTextFlagAB`](#class_s_i4735_1abd965c0e49493bc2c7c37547d36dcc96) | 
`protected uint8_t `[`resetPin`](#class_s_i4735_1a6470177f747ef7eb8e7d02c76f72b19e) | 
`protected uint8_t `[`interruptPin`](#class_s_i4735_1af8f8123888060f858c74ba08a29ff38b) | 
`protected uint8_t `[`currentTune`](#class_s_i4735_1acc3d3a411d181e08846219661dbde1c4) | 
`protected uint16_t `[`currentMinimumFrequency`](#class_s_i4735_1a4663c1c92d980472556368944cb2fc68) | 
`protected uint16_t `[`currentMaximumFrequency`](#class_s_i4735_1aa3d41ad3fdb9d821a721a8acfaf47670) | 
`protected uint16_t `[`currentWorkFrequency`](#class_s_i4735_1a9da4f3df9e5e6b0448eff15e12f690dd) | 
`protected uint16_t `[`currentStep`](#class_s_i4735_1ad72759f246cceb147bf031b72173b0ca) | 
`protected uint8_t `[`lastMode`](#class_s_i4735_1a157e4ae8cd53396973584aa10888cd47) | 
`protected uint8_t `[`currentAvcAmMaxGain`](#class_s_i4735_1a650490ab792c1853c97f600661ecdaa1) | Store the last mode used.
`protected `[`si47x_frequency`](#unionsi47x__frequency)` `[`currentFrequency`](#class_s_i4735_1a4439c40db789f02371ea0fb9b22650c5) | 
`protected `[`si47x_set_frequency`](#unionsi47x__set__frequency)` `[`currentFrequencyParams`](#class_s_i4735_1a8253ae2f3c87a6943776f8a63a691ca3) | 
`protected `[`si47x_rqs_status`](#unionsi47x__rqs__status)` `[`currentRqsStatus`](#class_s_i4735_1a9edd8cc9254695631795e7efc008eb28) | 
`protected `[`si47x_response_status`](#unionsi47x__response__status)` `[`currentStatus`](#class_s_i4735_1a7ad35ba002b73b66e9ce97b304854695) | 
`protected `[`si47x_firmware_information`](#unionsi47x__firmware__information)` `[`firmwareInfo`](#class_s_i4735_1ad03d545e53a7ca2fe072a7b03d60910e) | 
`protected `[`si47x_rds_status`](#unionsi47x__rds__status)` `[`currentRdsStatus`](#class_s_i4735_1a42d5c6991d90ea1c8ae35b74f8bccc48) | 
`protected `[`si47x_agc_status`](#unionsi47x__agc__status)` `[`currentAgcStatus`](#class_s_i4735_1adf2d1f37ec9edf95e14aecb3cdc7417d) | 
`protected `[`si47x_ssb_mode`](#unionsi47x__ssb__mode)` `[`currentSSBMode`](#class_s_i4735_1a74ee9c99b1e092f7b1efa2f771f38324) | 
`protected `[`si473x_powerup`](#unionsi473x__powerup)` `[`powerUp`](#class_s_i4735_1a1331b701088bda20bf0844e2e38be142) | 
`protected uint8_t `[`volume`](#class_s_i4735_1aa5220772c468fcfb69b5d16163de73fc) | 
`protected uint8_t `[`currentSsbStatus`](#class_s_i4735_1a05730ca069db4be7c45a9c92f29d3b52) | 
`protected void `[`waitInterrupr`](#class_s_i4735_1ad2e95c88de0dfa58ff7aa36988071421)`(void)` | If you setup interrupt, this function will be called whenever the Si4735 changes.
`protected void `[`sendProperty`](#class_s_i4735_1ae351181347f8c512b625d98ef7844961)`(uint16_t propertyValue,uint16_t param)` | Sends (sets) property to the SI47XX This method is used for others to send generic properties and params to SI47XX
`protected void `[`sendSSBModeProperty`](#class_s_i4735_1ae1d866bfc4cb37c4268c8942b4b4d062)`()` | Just send the property SSB_MOD to the device. Internal use (privete method).
`protected void `[`disableFmDebug`](#class_s_i4735_1ae4683e9a9fc7a633305b18dae54fbd83)`()` | There is a debug feature that remains active in Si4704/05/3x-D60 firmware which can create periodic noise in audio. Silicon Labs recommends you disable this feature by sending the following bytes (shown here in hexadecimal form): 0x12 0x00 0xFF 0x00 0x00 0x00.
`protected void `[`clearRdsBuffer2A`](#class_s_i4735_1ac65c552004f5e5e646638e37a5a68213)`()` | Clear RDS buffer 2A (text)
`protected void `[`clearRdsBuffer2B`](#class_s_i4735_1aed0988165057aa2880f41b88ac236cd7)`()` | Clear RDS buffer 2B (text)
`protected void `[`clearRdsBuffer0A`](#class_s_i4735_1adede134b96fd5650b87673ce413babb2)`()` | Clear RDS buffer 0A (text)

## Members

#### `public  `[`SI4735`](#class_s_i4735_1a818c60896490631e4311452c99ff361f)`()` {#class_s_i4735_1a818c60896490631e4311452c99ff361f}

This is a library for the [SI4735](#class_s_i4735), BROADCAST AM/FM/SW RADIO RECEIVER, IC from Silicon Labs for the Arduino development environment. It works with I2C protocol. This library is intended to provide an easier interface for controlling the [SI4735](#class_s_i4735). 
**See also**: documentation on [https://github.com/pu2clr/SI4735](https://github.com/pu2clr/SI4735).

**See also**: also: Si47XX PROGRAMMING GUIDE; AN332 AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; AMENDMENT FOR SI4735-D60 SSB AND NBFM PATCHES

Pay attention: According to Si47XX PROGRAMMING GUIDE; AN332; page 207, "For write operations, the system controller next sends a data byte on SDIO, which is captured by the device on rising edges of SCLK. The device acknowledges each data byte by driving SDIO low for one cycle on the next falling edge of SCLK. The system controller may write up to 8 data bytes in a single 2-wire transaction. The first byte is a command, and the next seven bytes are arguments. Writing more than 8 bytes results in unpredictable device behavior". So, If you are extending this library, consider that restriction presented earlier.

ATTENTION: Some methods were implemented usin inline resource. Inline methods are implemented in [SI4735.h](#_s_i4735_8h_source)

By Ricardo Lima Caratti, Nov 2019. Construct a new [SI4735::SI4735](#class_s_i4735_1a818c60896490631e4311452c99ff361f) object

#### `public void `[`reset`](#class_s_i4735_1adc6014674bd4d1ee3aab56c605926db1)`(void)` {#class_s_i4735_1adc6014674bd4d1ee3aab56c605926db1}

Reset the SI473X 
**See also**: Si47XX PROGRAMMING GUIDE; AN332;

#### `public void `[`waitToSend`](#class_s_i4735_1a7b45c1b22c3c1a3f2326bee913d1e1e0)`(void)` {#class_s_i4735_1a7b45c1b22c3c1a3f2326bee913d1e1e0}

Wait for the si473x is ready (Clear to Send (CTS) status bit have to be 1). 
 This function should be used before sending any command to a SI47XX device. 
**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 63, 128

#### `public void `[`setup`](#class_s_i4735_1a944ed704e2175a6cf660ad65ca5bbad5)`(uint8_t resetPin,uint8_t defaultFunction)` {#class_s_i4735_1a944ed704e2175a6cf660ad65ca5bbad5}

Starts the Si473X device. 
 Use this setup if you are not using interrupt resource

#### Parameters
* `uint8_t` resetPin Digital Arduino Pin used to RESET command 

* `uint8_t` defaultFunction

#### `public void `[`setup`](#class_s_i4735_1a95fd37277dbb8b16b5ae7a996a5f9eba)`(uint8_t resetPin,int interruptPin,uint8_t defaultFunction,uint8_t audioMode)` {#class_s_i4735_1a95fd37277dbb8b16b5ae7a996a5f9eba}

Starts the Si473X device.

If the audio mode parameter is not entered, analog mode will be considered.

#### Parameters
* `uint8_t` resetPin Digital Arduino Pin used to RESET command 

* `uint8_t` interruptPin interrupt Arduino Pin (see your Arduino pinout). If less than 0, iterrupt disabled 

* `uint8_t` defaultFunction 

* `uint8_t` audioMode default SI473X_ANALOG_AUDIO (Analog Audio). Use SI473X_ANALOG_AUDIO or SI473X_DIGITAL_AUDIO

#### `public void `[`setPowerUp`](#class_s_i4735_1a8bc001cb28dc8fbd88ba0fa7024eca39)`(uint8_t CTSIEN,uint8_t GPO2OEN,uint8_t PATCH,uint8_t XOSCEN,uint8_t FUNC,uint8_t OPMODE)` {#class_s_i4735_1a8bc001cb28dc8fbd88ba0fa7024eca39}

Set the Power Up parameters for si473X. Use this method to chenge the defaul behavior of the Si473X. Use it before PowerUp()

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 65 and 129

#### Parameters
* `uint8_t` CTSIEN sets Interrupt anabled or disabled (1 = anabled and 0 = disabled ) 

* `uint8_t` GPO2OEN sets GP02 Si473X pin enabled (1 = anabled and 0 = disabled ) 

* `uint8_t` PATCH Used for firmware patch updates. Use it always 0 here. 

* `uint8_t` XOSCEN sets external Crystal enabled or disabled 

* `uint8_t` FUNC sets the receiver function have to be used [0 = FM Receive; 1 = AM (LW/MW/SW) and SSB (if SSB patch apllied)] 

* `uint8_t` OPMODE set the kind of audio mode you want to use.

#### `public void `[`radioPowerUp`](#class_s_i4735_1a3e228da65641f4dcf4253262e19bde9b)`(void)` {#class_s_i4735_1a3e228da65641f4dcf4253262e19bde9b}

Powerup the Si47XX Before call this function call the setPowerUp to set up the parameters. Parameters you have to set up with setPowerUp

CTSIEN Interrupt anabled or disabled; GPO2OEN GPO2 Output Enable or disabled; PATCH Boot normally or patch; XOSCEN Use external crystal oscillator; FUNC defaultFunction = 0 = FM Receive; 1 = AM (LW/MW/SW) Receiver. OPMODE SI473X_ANALOG_AUDIO (B00000101) or SI473X_DIGITAL_AUDIO (B00001011)

**See also**: [SI4735::setPowerUp()](#class_s_i4735_1a8bc001cb28dc8fbd88ba0fa7024eca39)

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 64, 129

#### `public void `[`analogPowerUp`](#class_s_i4735_1a412f51386c47a17cf04fd0a3c3078726)`(void)` {#class_s_i4735_1a412f51386c47a17cf04fd0a3c3078726}

Powerup in Analog Mode. It will be deprecated. Consider use radioPowerUp instead. Actually this function works fo Digital and Analog modes. You have to call setPowerUp method before.

#### `public void `[`powerDown`](#class_s_i4735_1aa554b3390fb6c5a4d6661b96afae73ae)`(void)` {#class_s_i4735_1aa554b3390fb6c5a4d6661b96afae73ae}

Moves the device from powerup to powerdown mode. After Power Down command, only the Power Up command is accepted.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 67, 132

#### `public void `[`setFrequency`](#class_s_i4735_1a63d205f2d3169ba877a2bd2ee28d224f)`(uint16_t)` {#class_s_i4735_1a63d205f2d3169ba877a2bd2ee28d224f}

Set the frequency to the corrent function of the Si4735 (FM, AM or SSB) You have to call setup or setPowerUp before call setFrequency.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 70, 135 

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 13

#### Parameters
* `uint16_t` freq Is the frequency to change. For example, FM => 10390 = 103.9 MHz; AM => 810 = 810 KHz.

#### `public void `[`getStatus`](#class_s_i4735_1a3cf856c44fcfffb3d81b22b1d09b881f)`()` {#class_s_i4735_1a3cf856c44fcfffb3d81b22b1d09b881f}

Gets the current status of the Si4735 (AM or FM)

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)

#### `public void `[`getStatus`](#class_s_i4735_1a5c0f4036a7fbbd11a26f1f6f4579d3cd)`(uint8_t,uint8_t)` {#class_s_i4735_1a5c0f4036a7fbbd11a26f1f6f4579d3cd}

Gets the current status of the Si4735 (AM or FM) 
**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)

#### Parameters
* `uint8_t` INTACK Seek/Tune Interrupt Clear. If set, clears the seek/tune complete interrupt status indicator; 

* `uint8_t` CANCEL Cancel seek. If set, aborts a seek currently in progress;

#### `public uint16_t `[`getFrequency`](#class_s_i4735_1a7e88bb573604d465536c13554d5c61f8)`(void)` {#class_s_i4735_1a7e88bb573604d465536c13554d5c61f8}

Device Status Information Gets the current frequency of the Si4735 (AM or FM) The method status do it an more. See getStatus below.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 73 (FM) and 139 (AM)

#### `public uint16_t `[`getCurrentFrequency`](#class_s_i4735_1a8240278970ae8bf7283e14714b4329ad)`()` {#class_s_i4735_1a8240278970ae8bf7283e14714b4329ad}

Gets the current frequency saved in memory. Unlike getFrequency, this method gets the current frequency recorded after the last setFrequency command. This method avoids bus traffic and CI processing. However, you can not get others status information like RSSI.

**See also**: [getFrequency()](#class_s_i4735_1a7e88bb573604d465536c13554d5c61f8)

#### `public inline bool `[`getSignalQualityInterrupt`](#class_s_i4735_1ad194137a6a712a599e3c26e5b6357594)`()` {#class_s_i4735_1ad194137a6a712a599e3c26e5b6357594}

STATUS RESPONSE Set of methods to get current status information. Call them after getStatus or getFrequency or seekStation See Si47XX PROGRAMMING GUIDE; AN332; pages 63

#### `public inline bool `[`getRadioDataSystemInterrupt`](#class_s_i4735_1a97dc44d9927ea58730a18fb6baa6867f)`()` {#class_s_i4735_1a97dc44d9927ea58730a18fb6baa6867f}

Gets Received Signal Quality Interrupt(RSQINT)

#### `public inline bool `[`getTuneCompleteTriggered`](#class_s_i4735_1a2cc659bddc5efc7695f2a6031c2edd4b)`()` {#class_s_i4735_1a2cc659bddc5efc7695f2a6031c2edd4b}

Gets Radio Data System (RDS) Interrupt.

#### `public inline bool `[`getStatusError`](#class_s_i4735_1a69723840497ed693d6f9a4e840ab07c9)`()` {#class_s_i4735_1a69723840497ed693d6f9a4e840ab07c9}

Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.

#### `public inline bool `[`getStatusCTS`](#class_s_i4735_1afcb38f53b6eb71cadcefa0bb472ca943)`()` {#class_s_i4735_1afcb38f53b6eb71cadcefa0bb472ca943}

Return the Error flag (true or false) of status of the least Tune or Seek.

#### `public inline bool `[`getACFIndicator`](#class_s_i4735_1ab8a51de592fd40fdd872039b3c70716f)`()` {#class_s_i4735_1ab8a51de592fd40fdd872039b3c70716f}

Gets the Error flag of status response.

#### `public inline bool `[`getBandLimit`](#class_s_i4735_1a19e96565a2dec7ec68469f45c8d3e084)`()` {#class_s_i4735_1a19e96565a2dec7ec68469f45c8d3e084}

Returns true if the AFC rails (AFC Rail Indicator).

#### `public inline bool `[`getStatusValid`](#class_s_i4735_1afb19da8a05320d7f38cca8325725edfd)`()` {#class_s_i4735_1afb19da8a05320d7f38cca8325725edfd}

Returns true if a seek hit the band limit (WRAP = 0 in FM_START_SEEK) or wrapped to the original frequency(WRAP = 1).

#### `public inline uint8_t `[`getReceivedSignalStrengthIndicator`](#class_s_i4735_1aeeefd12b49dbc764e0f45fe208441482)`()` {#class_s_i4735_1aeeefd12b49dbc764e0f45fe208441482}

Returns true if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108)

#### `public inline uint8_t `[`getStatusSNR`](#class_s_i4735_1ac0bd4daaa321ef8956dca4d1e4e5cd3c)`()` {#class_s_i4735_1ac0bd4daaa321ef8956dca4d1e4e5cd3c}

Returns integer Received Signal Strength Indicator (dBμV).

#### `public inline uint8_t `[`getStatusMULT`](#class_s_i4735_1a0f9b5438a6f92c8f2e538ee640f353ab)`()` {#class_s_i4735_1a0f9b5438a6f92c8f2e538ee640f353ab}

Returns integer containing the SNR metric when tune is complete (dB).

#### `public inline uint8_t `[`getAntennaTuningCapacitor`](#class_s_i4735_1a3905d6551992b4dd51d13f05978d9081)`()` {#class_s_i4735_1a3905d6551992b4dd51d13f05978d9081}

Returns integer containing the multipath metric when tune is complete.

#### `public void `[`getAutomaticGainControl`](#class_s_i4735_1aaae187575b9868ff427c063c3a6cca2a)`()` {#class_s_i4735_1aaae187575b9868ff427c063c3a6cca2a}

Returns integer containing the current antenna tuning capacitor value.

Queries AGC STATUS

**See also**: Si47XX PROGRAMMING GUIDE; AN332; For FM page 80; for AM page 142. 

**See also**: AN332 REV 0.8 Universal Programming Guide Amendment for SI4735-D60 SSB and NBFM patches; page 18.

After call this method, you can call isAgcEnabled to know the AGC status and getAgcGainIndex to know the gain index value.

#### `public void `[`setAvcAmMaxGain`](#class_s_i4735_1a98d12da4b801493a350ecf770f4f2d8b)`(uint8_t gain)` {#class_s_i4735_1a98d12da4b801493a350ecf770f4f2d8b}

Sets the maximum gain for automatic volume control. If no parameter is sent, it will be consider 48dB.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 152

#### Parameters
* `uint8_t` gain Select a value between 12 and 192. Defaul value 48dB.

#### `public inline void `[`setAvcAmMaxGain`](#class_s_i4735_1a6e1e0e48d71e09debb07fc6dbe884829)`()` {#class_s_i4735_1a6e1e0e48d71e09debb07fc6dbe884829}

#### `public inline uint8_t `[`getCurrentAvcAmMaxGain`](#class_s_i4735_1a27dd7dfd46583eb5be91cfc9461807ff)`()` {#class_s_i4735_1a27dd7dfd46583eb5be91cfc9461807ff}

#### `public inline void `[`setAmSoftMuteMaxAttenuation`](#class_s_i4735_1acc57f6bde08e94462a9d75fe28a66ea8)`(uint8_t smattn)` {#class_s_i4735_1acc57f6bde08e94462a9d75fe28a66ea8}

#### `public inline void `[`setAmSoftMuteMaxAttenuation`](#class_s_i4735_1ac1af311f799e8a007cbd048a88f1abe9)`()` {#class_s_i4735_1ac1af311f799e8a007cbd048a88f1abe9}

#### `public inline void `[`setSsbSoftMuteMaxAttenuation`](#class_s_i4735_1ad357ccc5f207ec11d36d9569046c098f)`(uint8_t smattn)` {#class_s_i4735_1ad357ccc5f207ec11d36d9569046c098f}

#### `public inline void `[`setSsbSoftMuteMaxAttenuation`](#class_s_i4735_1afd057daf737a78a50ad1a03a6b43a111)`()` {#class_s_i4735_1afd057daf737a78a50ad1a03a6b43a111}

#### `public inline bool `[`isAgcEnabled`](#class_s_i4735_1aea08b7e553212cbe01bf6f4ec9e02de7)`()` {#class_s_i4735_1aea08b7e553212cbe01bf6f4ec9e02de7}

#### `public inline uint8_t `[`getAgcGainIndex`](#class_s_i4735_1a4eceaf5745e87fcca6cf31c04c5635bd)`()` {#class_s_i4735_1a4eceaf5745e87fcca6cf31c04c5635bd}

#### `public void `[`setAutomaticGainControl`](#class_s_i4735_1a27c22f5671ce3229e925e5fbe93ba0a6)`(uint8_t AGCDIS,uint8_t AGCIDX)` {#class_s_i4735_1a27c22f5671ce3229e925e5fbe93ba0a6}

If FM, overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 (minimum attenuation) and 26 (maximum attenuation); If AM/SSB, Overrides the AM AGC setting by disabling the AGC and forcing the gain index that ranges between 0 (minimum attenuation) and 37+ATTN_BACKUP (maximum attenuation);

**See also**: Si47XX PROGRAMMING GUIDE; AN332; For FM page 81; for AM page 143

#### Parameters
* `uint8_t` AGCDIS This param selects whether the AGC is enabled or disabled (0 = AGC enabled; 1 = AGC disabled); 

* `uint8_t` AGCIDX AGC Index (0 = Minimum attenuation (max gain); 1 – 36 = Intermediate attenuation); if >greater than 36 - Maximum attenuation (min gain) ).

#### `public void `[`getCurrentReceivedSignalQuality`](#class_s_i4735_1abf59183da13c72384093fc238c3114cf)`(uint8_t INTACK)` {#class_s_i4735_1abf59183da13c72384093fc238c3114cf}

Queries the status of the Received Signal Quality (RSQ) of the current channel. This method sould be called berore call getCurrentRSSI(), [getCurrentSNR()](#class_s_i4735_1adb3fe55583dafeac7439a303b637d162) etc. Command FM_RSQ_STATUS

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 75 and 141

#### Parameters
* `INTACK` Interrupt Acknowledge. 0 = Interrupt status preserved; 1 = Clears RSQINT, BLENDINT, SNRHINT, SNRLINT, RSSIHINT, RSSILINT, MULTHINT, MULTLINT.

#### `public void `[`getCurrentReceivedSignalQuality`](#class_s_i4735_1a7393fa4f9d7811c8d0c0f48283ef6435)`(void)` {#class_s_i4735_1a7393fa4f9d7811c8d0c0f48283ef6435}

Queries the status of the Received Signal Quality (RSQ) of the current channel Command FM_RSQ_STATUS

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 75 and 141

#### Parameters
* `INTACK` Interrupt Acknowledge. 0 = Interrupt status preserved; 1 = Clears RSQINT, BLENDINT, SNRHINT, SNRLINT, RSSIHINT, RSSILINT, MULTHINT, MULTLINT.

#### `public inline uint8_t `[`getCurrentRSSI`](#class_s_i4735_1a4f5abf562ec00767a3b84f74eba5ac4d)`()` {#class_s_i4735_1a4f5abf562ec00767a3b84f74eba5ac4d}

#### `public inline uint8_t `[`getCurrentSNR`](#class_s_i4735_1adb3fe55583dafeac7439a303b637d162)`()` {#class_s_i4735_1adb3fe55583dafeac7439a303b637d162}

current receive signal strength (0–127 dBμV).

#### `public inline bool `[`getCurrentRssiDetectLow`](#class_s_i4735_1a0c9ae82012cdc97c4080b84e37fa1820)`()` {#class_s_i4735_1a0c9ae82012cdc97c4080b84e37fa1820}

current SNR metric (0–127 dB).

#### `public inline bool `[`getCurrentRssiDetectHigh`](#class_s_i4735_1adf11a66236392ca107b5d52a8747f838)`()` {#class_s_i4735_1adf11a66236392ca107b5d52a8747f838}

RSSI Detect Low.

#### `public inline bool `[`getCurrentSnrDetectLow`](#class_s_i4735_1a1296592571c85b89ed59a4ee2324bd52)`()` {#class_s_i4735_1a1296592571c85b89ed59a4ee2324bd52}

RSSI Detect High.

#### `public inline bool `[`getCurrentSnrDetectHigh`](#class_s_i4735_1aec3c71a3252dd6660e5bceebc0d1f988)`()` {#class_s_i4735_1aec3c71a3252dd6660e5bceebc0d1f988}

SNR Detect Low.

#### `public inline bool `[`getCurrentValidChannel`](#class_s_i4735_1a5ce1abf8c9d53c027f6b0870efedda1b)`()` {#class_s_i4735_1a5ce1abf8c9d53c027f6b0870efedda1b}

SNR Detect High.

#### `public inline bool `[`getCurrentAfcRailIndicator`](#class_s_i4735_1affe5cee9daf44677802a94f96d893c94)`()` {#class_s_i4735_1affe5cee9daf44677802a94f96d893c94}

Valid Channel.

#### `public inline bool `[`getCurrentSoftMuteIndicator`](#class_s_i4735_1a76e8485292878c1294e5910601f4575f)`()` {#class_s_i4735_1a76e8485292878c1294e5910601f4575f}

AFC Rail Indicator.

#### `public inline uint8_t `[`getCurrentStereoBlend`](#class_s_i4735_1acaafab62dd411dc9b79f9fb96e8c8832)`()` {#class_s_i4735_1acaafab62dd411dc9b79f9fb96e8c8832}

Soft Mute Indicator. Indicates soft mute is engaged.

#### `public inline bool `[`getCurrentPilot`](#class_s_i4735_1a127fa3d2f9b7d57169bbef95703fb4c9)`()` {#class_s_i4735_1a127fa3d2f9b7d57169bbef95703fb4c9}

Indicates amount of stereo blend in % (100 = full stereo, 0 = full mono).

#### `public inline uint8_t `[`getCurrentMultipath`](#class_s_i4735_1a6d521bb5c7ef7811e57c7128cb2106b3)`()` {#class_s_i4735_1a6d521bb5c7ef7811e57c7128cb2106b3}

Indicates stereo pilot presence.

#### `public inline uint8_t `[`getCurrentSignedFrequencyOffset`](#class_s_i4735_1a2b3bfeca3b545f5741b1ed70a1dba077)`()` {#class_s_i4735_1a2b3bfeca3b545f5741b1ed70a1dba077}

Contains the current multipath metric. (0 = no multipath; 100 = full multipath)

#### `public inline bool `[`getCurrentMultipathDetectLow`](#class_s_i4735_1ad194a5c915620c6a1b8b3d6d7ed645d9)`()` {#class_s_i4735_1ad194a5c915620c6a1b8b3d6d7ed645d9}

Signed frequency offset (kHz).

#### `public inline bool `[`getCurrentMultipathDetectHigh`](#class_s_i4735_1a82d0865cf348deb0c9f81a2f12222fb0)`()` {#class_s_i4735_1a82d0865cf348deb0c9f81a2f12222fb0}

Multipath Detect Low.

#### `public inline bool `[`getCurrentBlendDetectInterrupt`](#class_s_i4735_1a16107a2f2e51cce5a7e2b1b303425118)`()` {#class_s_i4735_1a16107a2f2e51cce5a7e2b1b303425118}

Multipath Detect High.

#### `public inline uint8_t `[`getFirmwarePN`](#class_s_i4735_1ab26fbd99452daaf18875ff1c3c35d081)`()` {#class_s_i4735_1ab26fbd99452daaf18875ff1c3c35d081}

Blend Detect Interrupt.

#### `public inline uint8_t `[`getFirmwareFWMAJOR`](#class_s_i4735_1ac2f451cb7320b66d8e08698dcacd50d0)`()` {#class_s_i4735_1ac2f451cb7320b66d8e08698dcacd50d0}

RESP1 - Part Number (HEX)

#### `public inline uint8_t `[`getFirmwareFWMINOR`](#class_s_i4735_1a74b68fcd5718520fc0683977dfc18fc8)`()` {#class_s_i4735_1a74b68fcd5718520fc0683977dfc18fc8}

RESP2 - Returns the Firmware Major Revision (ASCII).

#### `public inline uint8_t `[`getFirmwarePATCHH`](#class_s_i4735_1a58b99a8d5ec2199cb5f2105675aa7a56)`()` {#class_s_i4735_1a58b99a8d5ec2199cb5f2105675aa7a56}

RESP3 - Returns the Firmware Minor Revision (ASCII).

#### `public inline uint8_t `[`getFirmwarePATCHL`](#class_s_i4735_1ac10b3cd3875985e2c129fea14e2bc47a)`()` {#class_s_i4735_1ac10b3cd3875985e2c129fea14e2bc47a}

RESP4 - Returns the Patch ID High byte (HEX).

#### `public inline uint8_t `[`getFirmwareCMPMAJOR`](#class_s_i4735_1ab57870d69969e73265e9cffe2a73d868)`()` {#class_s_i4735_1ab57870d69969e73265e9cffe2a73d868}

RESP5 - Returns the Patch ID Low byte (HEX).

#### `public inline uint8_t `[`getFirmwareCMPMINOR`](#class_s_i4735_1a883645d266a612ce07d71fdb2cd81e5f)`()` {#class_s_i4735_1a883645d266a612ce07d71fdb2cd81e5f}

RESP6 - Returns the Component Major Revision (ASCII).

#### `public inline uint8_t `[`getFirmwareCHIPREV`](#class_s_i4735_1acd363d9111e4a57cf2e1167634bc1b5f)`()` {#class_s_i4735_1acd363d9111e4a57cf2e1167634bc1b5f}

RESP7 - Returns the Component Minor Revision (ASCII).

#### `public void `[`setVolume`](#class_s_i4735_1a3b7543657dfd36da962267bb8692b796)`(uint8_t volume)` {#class_s_i4735_1a3b7543657dfd36da962267bb8692b796}

RESP8 - Returns the Chip Revision (ASCII).

Sets volume level (0 to 63)

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 62, 123, 170, 173 and 204

#### Parameters
* `uint8_t` volume (domain: 0 - 63)

#### `public uint8_t `[`getVolume`](#class_s_i4735_1acf79be50a01fe91abd498e5d42d83317)`()` {#class_s_i4735_1acf79be50a01fe91abd498e5d42d83317}

Gets the current volume level.

**See also**: [setVolume()](#class_s_i4735_1a3b7543657dfd36da962267bb8692b796)

#### Returns
volume (domain: 0 - 63)

#### `public void `[`volumeDown`](#class_s_i4735_1a23d49ab35895b76e0898405fac086c09)`()` {#class_s_i4735_1a23d49ab35895b76e0898405fac086c09}

Set sound volume level Down 

**See also**: [setVolume()](#class_s_i4735_1a3b7543657dfd36da962267bb8692b796)

#### `public void `[`volumeUp`](#class_s_i4735_1a5e3e8cf04ca99c6be2dbe15d72b4efa3)`()` {#class_s_i4735_1a5e3e8cf04ca99c6be2dbe15d72b4efa3}

Set sound volume level Up 

**See also**: [setVolume()](#class_s_i4735_1a3b7543657dfd36da962267bb8692b796)

#### `public inline uint8_t `[`getCurrentVolume`](#class_s_i4735_1a430b86ccfd4d6c89da9202a538e0cfc9)`()` {#class_s_i4735_1a430b86ccfd4d6c89da9202a538e0cfc9}

#### `public void `[`setAudioMute`](#class_s_i4735_1a12d4e173d096b1568e4209a63534d4f6)`(bool off)` {#class_s_i4735_1a12d4e173d096b1568e4209a63534d4f6}

Returns the current volume level.

Sets the audio on or off

**See also**: See Si47XX PROGRAMMING GUIDE; AN332; pages 62, 123, 171

#### Parameters
* `value` if true, mute the audio; if false unmute the audio.

#### `public void `[`digitalOutputFormat`](#class_s_i4735_1aea652d358b4b586b6d5964ceb8eb7c81)`(uint8_t OSIZE,uint8_t OMONO,uint8_t OMODE,uint8_t OFALL)` {#class_s_i4735_1aea652d358b4b586b6d5964ceb8eb7c81}

Digital Audio Setup Configures the digital audio output format. Options: DCLK edge, data format, force mono, and sample precision.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 195.

#### Parameters
* `uint8_t` OSIZE Digital Output Audio Sample Precision (0=16 bits, 1=20 bits, 2=24 bits, 3=8bits). 

* `uint8_t` OMONO Digital Output Mono Mode (0=Use mono/stereo blend ). 

* `uint8_t` OMODE Digital Output Mode (0=I2S, 6 = Left-justified, 8 = MSB at second DCLK after DFS pulse, 12 = MSB at first DCLK after DFS pulse). 

* `uint8_t` OFALL Digital Output DCLK Edge (0 = use DCLK rising edge, 1 = use DCLK falling edge)

#### `public void `[`digitalOutputSampleRate`](#class_s_i4735_1a1a404ca11fb32b457be2d7cfe484dd03)`(uint16_t DOSR)` {#class_s_i4735_1a1a404ca11fb32b457be2d7cfe484dd03}

Enables digital audio output and configures digital audio output sample rate in samples per second (sps).

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 196.

#### Parameters
* `uint16_t` DOSR Digital Output Sample Rate(32–48 ksps .0 to disable digital audio output).

#### `public void `[`setAM`](#class_s_i4735_1a753390952a1f2c2a98343979f1f3daa0)`()` {#class_s_i4735_1a753390952a1f2c2a98343979f1f3daa0}

Sets the radio to AM function. It means: LW MW and SW.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 129.

#### `public void `[`setFM`](#class_s_i4735_1acd89e7b8adefcbc4e89f802acb915906)`()` {#class_s_i4735_1acd89e7b8adefcbc4e89f802acb915906}

Sets the radio to FM function

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 64.

#### `public void `[`setAM`](#class_s_i4735_1a43c5ea36995a3897820e0170d94c81f4)`(uint16_t fromFreq,uint16_t toFreq,uint16_t intialFreq,uint16_t step)` {#class_s_i4735_1a43c5ea36995a3897820e0170d94c81f4}

Sets the radio to AM (LW/MW/SW) function.

**See also**: [setAM()](#class_s_i4735_1a753390952a1f2c2a98343979f1f3daa0)

#### Parameters
* `fromFreq` minimum frequency for the band 

* `toFreq` maximum frequency for the band 

* `initialFreq` initial frequency 

* `step` step used to go to the next channel

#### `public void `[`setFM`](#class_s_i4735_1a673bc4b17d564bb6e7f3e9a61d35fbb3)`(uint16_t fromFreq,uint16_t toFreq,uint16_t initialFreq,uint16_t step)` {#class_s_i4735_1a673bc4b17d564bb6e7f3e9a61d35fbb3}

Sets the radio to FM function.

**See also**: [setFM()](#class_s_i4735_1acd89e7b8adefcbc4e89f802acb915906)

#### Parameters
* `fromFreq` minimum frequency for the band 

* `toFreq` maximum frequency for the band 

* `initialFreq` initial frequency (default frequency) 

* `step` step used to go to the next channel

#### `public void `[`setBandwidth`](#class_s_i4735_1a8755743a36d2d4662c8db918d02ee4e1)`(uint8_t AMCHFLT,uint8_t AMPLFLT)` {#class_s_i4735_1a8755743a36d2d4662c8db918d02ee4e1}

Selects the bandwidth of the channel filter for AM reception. The choices are 6, 4, 3, 2, 2.5, 1.8, or 1 (kHz). The default bandwidth is 2 kHz. Works only in AM / SSB (LW/MW/SW)

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 125, 151, 277, 181.

#### Parameters
* `AMCHFLT` the choices are: 0 = 6 kHz Bandwidth 
 1 = 4 kHz Bandwidth 2 = 3 kHz Bandwidth 3 = 2 kHz Bandwidth 4 = 1 kHz Bandwidth 5 = 1.8 kHz Bandwidth 6 = 2.5 kHz Bandwidth, gradual roll off 7–15 = Reserved (Do not use). 

* `AMPLFLT` Enables the AM Power Line Noise Rejection Filter.

#### `public void `[`setFrequencyStep`](#class_s_i4735_1a794f6fe7987e146a0135b8e6fabec55c)`(uint16_t step)` {#class_s_i4735_1a794f6fe7987e146a0135b8e6fabec55c}

Sets the current step value.

ATTENTION: This function does not check the limits of the current band. Please, don't take a step bigger than your legs.

#### Parameters
* `step` if you are using FM, 10 means 100KHz. If you are using AM 10 means 10KHz For AM, 1 (1KHz) to 1000 (1MHz) are valid values. For FM 5 (50KHz) and 10 (100KHz) are valid values.

#### `public inline uint8_t `[`getTuneFrequencyFast`](#class_s_i4735_1a8c54bfc1fcf1dd989e697cc9b7094f59)`()` {#class_s_i4735_1a8c54bfc1fcf1dd989e697cc9b7094f59}

#### `public inline void `[`setTuneFrequencyFast`](#class_s_i4735_1a51d816112d974f10306d9d001667395a)`(uint8_t FAST)` {#class_s_i4735_1a51d816112d974f10306d9d001667395a}

Returns the FAST tuning status.

#### `public inline uint8_t `[`getTuneFrequencyFreeze`](#class_s_i4735_1a667fead0c7b2658c2b932370ad4e4f88)`()` {#class_s_i4735_1a667fead0c7b2658c2b932370ad4e4f88}

FAST Tuning. If set, executes fast and invalidated tune. The tune status will not be accurate.

#### `public inline void `[`setTuneFrequencyFreeze`](#class_s_i4735_1aee22a5ce06d4948fad2365a7b62fbd8d)`(uint8_t FREEZE)` {#class_s_i4735_1aee22a5ce06d4948fad2365a7b62fbd8d}

Returns the FREEZE status.

#### `public void `[`setTuneFrequencyAntennaCapacitor`](#class_s_i4735_1aaceadaaf0835e3db124d1b36f2aebd3c)`(uint16_t capacitor)` {#class_s_i4735_1aaceadaaf0835e3db124d1b36f2aebd3c}

Only FM. Freeze Metrics During Alternate Frequency Jump.

Selects the tuning capacitor value.

For FM, Antenna Tuning Capacitor is valid only when using TXO/LPI pin as the antenna input.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 71 and 136

#### Parameters
* `capacitor` If zero, the tuning capacitor value is selected automatically. If the value is set to anything other than 0: AM - the tuning capacitance is manually set as 95 fF x ANTCAP + 7 pF. ANTCAP manual range is 1–6143; FM - the valid range is 0 to 191. 
 According to Silicon Labs, automatic capacitor tuning is recommended (value 0).

#### `public void `[`frequencyUp`](#class_s_i4735_1ac0fcfd64dcf30b11d4dc219d9f7fc5d9)`()` {#class_s_i4735_1ac0fcfd64dcf30b11d4dc219d9f7fc5d9}

Increments the current frequency on current band/function by using the current step.

**See also**: [setFrequencyStep()](#class_s_i4735_1a794f6fe7987e146a0135b8e6fabec55c)

#### `public void `[`frequencyDown`](#class_s_i4735_1a93fff524a6f900ac91eb1222ce73f9ba)`()` {#class_s_i4735_1a93fff524a6f900ac91eb1222ce73f9ba}

Decrements the current frequency on current band/function by using the current step.

**See also**: [setFrequencyStep](#class_s_i4735_1a794f6fe7987e146a0135b8e6fabec55c)

#### `public bool `[`isCurrentTuneFM`](#class_s_i4735_1ae7ca2ffa6ad68ca69e4f35a3c40bbc3c)`()` {#class_s_i4735_1ae7ca2ffa6ad68ca69e4f35a3c40bbc3c}

Returns true if the current function is FM (FM_TUNE_FREQ).

#### Returns
true if the current function is FM (FM_TUNE_FREQ).

#### `public void `[`getFirmware`](#class_s_i4735_1a2fbc7e2187a7c645879623100dbc8502)`(void)` {#class_s_i4735_1a2fbc7e2187a7c645879623100dbc8502}

Gets firmware information

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 66, 131

#### `public void `[`setFunction`](#class_s_i4735_1a39f0e95a5efe5bd9add926c6f538528a)`(uint8_t FUNC)` {#class_s_i4735_1a39f0e95a5efe5bd9add926c6f538528a}

#### `public void `[`seekStation`](#class_s_i4735_1a3280b98b8331f00b4d0158ef1b076533)`(uint8_t SEEKUP,uint8_t WRAP)` {#class_s_i4735_1a3280b98b8331f00b4d0158ef1b076533}

Look for a station

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 55, 72, 125 and 137

#### Parameters
* `SEEKUP` Seek Up/Down. Determines the direction of the search, either UP = 1, or DOWN = 0. 

* `Wrap/Halt.` Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.

#### `public void `[`seekStationUp`](#class_s_i4735_1aad5211e3fff28141473442f59008e415)`()` {#class_s_i4735_1aad5211e3fff28141473442f59008e415}

Search for the next station

**See also**: [seekStation(uint8_t SEEKUP, uint8_t WRAP)](#class_s_i4735_1a3280b98b8331f00b4d0158ef1b076533)

#### `public void `[`seekStationDown`](#class_s_i4735_1a1eeaa09670cdbdc7ddd724c940d2506b)`()` {#class_s_i4735_1a1eeaa09670cdbdc7ddd724c940d2506b}

Search the previous station

**See also**: [seekStation(uint8_t SEEKUP, uint8_t WRAP)](#class_s_i4735_1a3280b98b8331f00b4d0158ef1b076533)

#### `public void `[`setSeekAmLimits`](#class_s_i4735_1a6b901d5cd729364dbda6e5b811dc9ec2)`(uint16_t bottom,uint16_t top)` {#class_s_i4735_1a6b901d5cd729364dbda6e5b811dc9ec2}

Sets the bottom frequency and top frequency of the AM band for seek. Default is 520 to 1710.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 127, 161, and 162

#### Parameters
* `uint16_t` bottom - the bottom of the AM band for seek 

* `uint16_t` top - the top of the AM band for seek

#### `public void `[`setSeekAmSpacing`](#class_s_i4735_1a6f8218c0bbdcb75ea54bc8af2a60dcc6)`(uint16_t spacing)` {#class_s_i4735_1a6f8218c0bbdcb75ea54bc8af2a60dcc6}

Selects frequency spacingfor AM seek. Default is 10 kHz spacing.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 163, 229 and 283

#### Parameters
* `uint16_t` spacing - step in KHz

#### `public void `[`setSeekSrnThreshold`](#class_s_i4735_1aeef76371500acdbdc17ca8dc980e1fff)`(uint16_t value)` {#class_s_i4735_1aeef76371500acdbdc17ca8dc980e1fff}

Sets the SNR threshold for a valid AM Seek/Tune. If the value is zero then SNR threshold is not considered when doing a seek. Default value is 5 dB.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 127

#### `public void `[`setSeekRssiThreshold`](#class_s_i4735_1a13397b273f7ab6fa7d5f48f116edac9b)`(uint16_t value)` {#class_s_i4735_1a13397b273f7ab6fa7d5f48f116edac9b}

Sets the RSSI threshold for a valid AM Seek/Tune. If the value is zero then RSSI threshold is not considered when doing a seek. Default value is 25 dBμV.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 127

#### `public void `[`setFmBlendStereoThreshold`](#class_s_i4735_1ac598865516ca09db7dbfa6a84da7f179)`(uint8_t parameter)` {#class_s_i4735_1ac598865516ca09db7dbfa6a84da7f179}

Sets RSSI threshold for stereo blend (Full stereo above threshold, blend below threshold). To force stereo, set this to 0. To force mono, set this to 127.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 90.

#### Parameters
* `parameter` valid values: 0 to 127

#### `public void `[`setFmBlendMonoThreshold`](#class_s_i4735_1adf4c5ec4473fd8486dd2ec7a19a10d24)`(uint8_t parameter)` {#class_s_i4735_1adf4c5ec4473fd8486dd2ec7a19a10d24}

Sets RSSI threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo set this to 0. To force mono set this to 127. Default value is 30 dBμV.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 56.

#### Parameters
* `parameter` valid values: 0 to 127

#### `public void `[`setFmBlendRssiStereoThreshold`](#class_s_i4735_1ab063630466dccea3becc11d60d122904)`(uint8_t parameter)` {#class_s_i4735_1ab063630466dccea3becc11d60d122904}

Sets RSSI threshold for stereo blend. (Full stereo above threshold, blend below threshold.) To force stereo, set this to 0. To force mono, set this to 127. Default value is 49 dBμV. 
**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 59.

#### Parameters
* `parameter` valid values: 0 to 127

#### `public void `[`setFmBLendRssiMonoThreshold`](#class_s_i4735_1a1b134a94138b3bf993a5d3eb5f8ed5c6)`(uint8_t parameter)` {#class_s_i4735_1a1b134a94138b3bf993a5d3eb5f8ed5c6}

Sets RSSI threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo, set this to 0. To force mono, set this to 127. Default value is 30 dBμV.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 59. 

#### Parameters
* `parameter` valid values: 0 to 127

#### `public void `[`setFmBlendSnrStereoThreshold`](#class_s_i4735_1a1d1a5eed976370d99827ba55d08b9293)`(uint8_t parameter)` {#class_s_i4735_1a1d1a5eed976370d99827ba55d08b9293}

Sets SNR threshold for stereo blend (Full stereo above threshold, blend below threshold). To force stereo, set this to 0. To force mono, set this to 127. Default value is 27 dB.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 59. 

#### Parameters
* `parameter` valid values: 0 to 127

#### `public void `[`setFmBLendSnrMonoThreshold`](#class_s_i4735_1ae2c30536e59f6691d990a2b9cf21f07e)`(uint8_t parameter)` {#class_s_i4735_1ae2c30536e59f6691d990a2b9cf21f07e}

Sets SNR threshold for mono blend (Full mono below threshold, blend above threshold). To force stereo, set this to 0. To force mono, set this to 127. Default value is 14 dB.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 59.

#### Parameters
* `parameter` valid values: 0 to 127

#### `public void `[`setFmBlendMultiPathStereoThreshold`](#class_s_i4735_1a0ed776a193fb2fcff783d4a82ff714e5)`(uint8_t parameter)` {#class_s_i4735_1a0ed776a193fb2fcff783d4a82ff714e5}

Sets multipath threshold for stereo blend (Full stereo below threshold, blend above threshold). To force stereo, set this to 100. To force mono, set this to 0. Default value is 20.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 60.

#### Parameters
* `parameter` valid values: 0 to 100

#### `public void `[`setFmBlendMultiPathMonoThreshold`](#class_s_i4735_1acc9fd27b38f49f167facfcc4f6216c5e)`(uint8_t parameter)` {#class_s_i4735_1acc9fd27b38f49f167facfcc4f6216c5e}

Sets Multipath threshold for mono blend (Full mono above threshold, blend below threshold). To force stereo, set to 100. To force mono, set to 0. The default is 60.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 60.

#### Parameters
* `parameter` valid values: 0 to 100

#### `public void `[`setFmStereoOn`](#class_s_i4735_1a52c7b7b1f0ed9121b0e64516b2bd2a55)`()` {#class_s_i4735_1a52c7b7b1f0ed9121b0e64516b2bd2a55}

Turn Off Stereo operation.

#### `public void `[`setFmStereoOff`](#class_s_i4735_1a59ce1008944ff9a51b6c5baeb3a3e2cb)`()` {#class_s_i4735_1a59ce1008944ff9a51b6c5baeb3a3e2cb}

Turn Off Stereo operation.

#### `public void `[`RdsInit`](#class_s_i4735_1affdba9ca9fb9bf5bb671ac3d448a7e89)`()` {#class_s_i4735_1affdba9ca9fb9bf5bb671ac3d448a7e89}

RDS implementation Starts the control variables for RDS.

#### `public void `[`setRdsIntSource`](#class_s_i4735_1a530e64d00d95b4ceb13ce33aa74c63cb)`(uint8_t RDSNEWBLOCKB,uint8_t RDSNEWBLOCKA,uint8_t RDSSYNCFOUND,uint8_t RDSSYNCLOST,uint8_t RDSRECV)` {#class_s_i4735_1a530e64d00d95b4ceb13ce33aa74c63cb}

Configures interrupt related to RDS

Use this method if want to use interrupt

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 103

#### Parameters
* `RDSRECV` If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries. 

* `RDSSYNCLOST` If set, generate RDSINT when RDS loses synchronization. 

* `RDSSYNCFOUND` set, generate RDSINT when RDS gains synchronization. 

* `RDSNEWBLOCKA` If set, generate an interrupt when Block A data is found or subsequently changed 

* `RDSNEWBLOCKB` If set, generate an interrupt when Block B data is found or subsequently changed

#### `public void `[`getRdsStatus`](#class_s_i4735_1a350a2033671a7620d4b9290a2347e185)`(uint8_t INTACK,uint8_t MTFIFO,uint8_t STATUSONLY)` {#class_s_i4735_1a350a2033671a7620d4b9290a2347e185}

Gets the RDS status. Store the status in currentRdsStatus member. RDS COMMAND FM_RDS_STATUS

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 55 and 77

#### Parameters
* `INTACK` Interrupt Acknowledge; 0 = RDSINT status preserved. 1 = Clears RDSINT. 

* `MTFIFO` 0 = If FIFO not empty, read and remove oldest FIFO entry; 1 = Clear RDS Receive FIFO. 

* `STATUSONLY` Determines if data should be removed from the RDS FIFO.

#### `public void `[`getRdsStatus`](#class_s_i4735_1a94cce3834f85fc127e8c2dab3a03e875)`()` {#class_s_i4735_1a94cce3834f85fc127e8c2dab3a03e875}

Gets RDS Status. Same result of calling getRdsStatus(0,0,0);

**See also**: [SI4735::getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY)](#class_s_i4735_1a350a2033671a7620d4b9290a2347e185)

Please, call [getRdsStatus(uint8_t INTACK, uint8_t MTFIFO, uint8_t STATUSONLY)](#class_s_i4735_1a350a2033671a7620d4b9290a2347e185) instead [getRdsStatus()](#class_s_i4735_1a94cce3834f85fc127e8c2dab3a03e875) if you want other behaviour

#### `public inline bool `[`getRdsReceived`](#class_s_i4735_1a6e8405ea5846fab586080a7be696178e)`()` {#class_s_i4735_1a6e8405ea5846fab586080a7be696178e}

#### `public inline bool `[`getRdsSyncLost`](#class_s_i4735_1ac283a8a89cc885c2740c65a7128ff7ef)`()` {#class_s_i4735_1ac283a8a89cc885c2740c65a7128ff7ef}

1 = FIFO filled to minimum number of groups

#### `public inline bool `[`getRdsSyncFound`](#class_s_i4735_1a78e906f12771490771c624be1777784d)`()` {#class_s_i4735_1a78e906f12771490771c624be1777784d}

1 = Lost RDS synchronization

#### `public inline bool `[`getRdsNewBlockA`](#class_s_i4735_1a561a052a8c737302368274c774d8c129)`()` {#class_s_i4735_1a561a052a8c737302368274c774d8c129}

1 = Found RDS synchronization

#### `public inline bool `[`getRdsNewBlockB`](#class_s_i4735_1aff740d85cc04c257f53c7e910bd45fcf)`()` {#class_s_i4735_1aff740d85cc04c257f53c7e910bd45fcf}

1 = Valid Block A data has been received.

#### `public inline bool `[`getRdsSync`](#class_s_i4735_1a6e899b37cbc3dcd0ab969724e4008f0f)`()` {#class_s_i4735_1a6e899b37cbc3dcd0ab969724e4008f0f}

1 = Valid Block B data has been received.

#### `public inline bool `[`getGroupLost`](#class_s_i4735_1a8db437efc42a81466d8daf4dcc35c50c)`()` {#class_s_i4735_1a8db437efc42a81466d8daf4dcc35c50c}

1 = RDS currently synchronized.

#### `public inline uint8_t `[`getNumRdsFifoUsed`](#class_s_i4735_1a819c370ff03c52a527fbc85f9877f526)`()` {#class_s_i4735_1a819c370ff03c52a527fbc85f9877f526}

1 = One or more RDS groups discarded due to FIFO overrun.

#### `public void `[`setRdsConfig`](#class_s_i4735_1ad9718a85c79a311c26997bbda20f2351)`(uint8_t RDSEN,uint8_t BLETHA,uint8_t BLETHB,uint8_t BLETHC,uint8_t BLETHD)` {#class_s_i4735_1ad9718a85c79a311c26997bbda20f2351}

RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).

Sets RDS property (FM_RDS_CONFIG) Configures RDS settings to enable RDS processing (RDSEN) and set RDS block error thresholds. When a RDS Group is received, all block errors must be less than or equal the associated block error threshold for the group to be stored in the RDS FIFO.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 104

MPORTANT: All block errors must be less than or equal the associated block error threshold for the group to be stored in the RDS FIFO. 0 = No errors. 1 = 1–2 bit errors detected and corrected. 2 = 3–5 bit errors detected and corrected. 3 = Uncorrectable. Recommended Block Error Threshold options: 2,2,2,2 = No group stored if any errors are uncorrected. 3,3,3,3 = Group stored regardless of errors. 0,0,0,0 = No group stored containing corrected or uncorrected errors. 3,2,3,3 = Group stored with corrected errors on B, regardless of errors on A, C, or D.

#### Parameters
* `uint8_t` RDSEN RDS Processing Enable; 1 = RDS processing enabled. 

* `uint8_t` BLETHA Block Error Threshold BLOCKA. 

* `uint8_t` BLETHB Block Error Threshold BLOCKB. 

* `uint8_t` BLETHC Block Error Threshold BLOCKC. 

* `uint8_t` BLETHD Block Error Threshold BLOCKD.

#### `public uint16_t `[`getRdsPI`](#class_s_i4735_1af1e67b201799a9e8d7208a32a65656b5)`(void)` {#class_s_i4735_1af1e67b201799a9e8d7208a32a65656b5}

Returns the programa type. Read the Block A content

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 77 and 78

#### Returns
BLOCKAL

#### `public uint8_t `[`getRdsGroupType`](#class_s_i4735_1aa35366e5cded7b864f7268a7e4a8dbe9)`(void)` {#class_s_i4735_1aa35366e5cded7b864f7268a7e4a8dbe9}

Returns the Group Type (extracted from the Block B)

#### `public uint8_t `[`getRdsFlagAB`](#class_s_i4735_1a05f76a0a0ebda813483f1df24e890904)`(void)` {#class_s_i4735_1a05f76a0a0ebda813483f1df24e890904}

Returns the current Text Flag A/B 

#### Returns
uint8_t

#### `public uint8_t `[`getRdsVersionCode`](#class_s_i4735_1af9b6cb614d1b01b11bb23c19e6deb8b9)`(void)` {#class_s_i4735_1af9b6cb614d1b01b11bb23c19e6deb8b9}

Gets the version code (extracted from the Block B)

#### Returns
0=A or 1=B

#### `public uint8_t `[`getRdsProgramType`](#class_s_i4735_1ac28fa3818d4f900c4f4ba6be10c1405b)`(void)` {#class_s_i4735_1ac28fa3818d4f900c4f4ba6be10c1405b}

Returns the Program Type (extracted from the Block B)

**See also**: [https://en.wikipedia.org/wiki/Radio_Data_System](https://en.wikipedia.org/wiki/Radio_Data_System)

#### Returns
program type (an integer betwenn 0 and 31)

#### `public uint8_t `[`getRdsTextSegmentAddress`](#class_s_i4735_1ab1ed04a109330e7e9206483c7cdf5ef9)`(void)` {#class_s_i4735_1ab1ed04a109330e7e9206483c7cdf5ef9}

Returns the address of the text segment. 2A - Each text segment in version 2A groups consists of four characters. A messages of this group can be have up to 64 characters. 2B - In version 2B groups, each text segment consists of only two characters. When the current RDS status is using this version, the maximum message length will be 32 characters.

#### Returns
uint8_t the address of the text segment.

#### `public char * `[`getRdsText`](#class_s_i4735_1a71cbaa3e0c103db7cee5969d162bec5d)`(void)` {#class_s_i4735_1a71cbaa3e0c103db7cee5969d162bec5d}

Gets the RDS Text when the message is of the Group Type 2 version A

#### Returns
char* The string (char array) with the content (Text) received from group 2A

#### `public char * `[`getRdsText0A`](#class_s_i4735_1a12d917629b78fffebac5ecf8f2936921)`(void)` {#class_s_i4735_1a12d917629b78fffebac5ecf8f2936921}

Gets the station name and other messages.

#### Returns
char* should return a string with the station name. However, some stations send other kind of messages

#### `public char * `[`getRdsText2A`](#class_s_i4735_1ae54d0f5562ca7615c3b90bee00c29ab4)`(void)` {#class_s_i4735_1ae54d0f5562ca7615c3b90bee00c29ab4}

Gets the Text processed for the 2A group

#### Returns
char* string with the Text of the group A2

#### `public char * `[`getRdsText2B`](#class_s_i4735_1a806d5cfbbeb6ca1299ce20b1220fdb18)`(void)` {#class_s_i4735_1a806d5cfbbeb6ca1299ce20b1220fdb18}

Gets the Text processed for the 2B group

#### Returns
char* string with the Text of the group AB

#### `public char * `[`getRdsTime`](#class_s_i4735_1aa8436bb156f3ac8321c65941be42fdba)`(void)` {#class_s_i4735_1aa8436bb156f3ac8321c65941be42fdba}

Gets the RDS time and date when the Group type is 4

#### Returns
char* a string with hh:mm +/- offset

#### `public void `[`getNext2Block`](#class_s_i4735_1ab1f4f137139b40fe386ca51586999e50)`(char *)` {#class_s_i4735_1ab1f4f137139b40fe386ca51586999e50}

Process data received from group 2B

#### Parameters
* `c` char array reference to the "group 2B" text

#### `public void `[`getNext4Block`](#class_s_i4735_1adb0a3c39aeacba0faf39aadd9465021f)`(char *)` {#class_s_i4735_1adb0a3c39aeacba0faf39aadd9465021f}

Process data received from group 2A

#### Parameters
* `c` char array reference to the "group  2A" text

#### `public void `[`ssbSetup`](#class_s_i4735_1a6f045d3725a457116236ee92d41c4265)`()` {#class_s_i4735_1a6f045d3725a457116236ee92d41c4265}

Starts the Si473X device on SSB (same AM Mode). Same [SI4735::setup](#class_s_i4735_1a944ed704e2175a6cf660ad65ca5bbad5) optimized to improve loading patch performance

#### `public void `[`setSSBBfo`](#class_s_i4735_1a55cf9a091d314a1aa21552f2254f756f)`(int offset)` {#class_s_i4735_1a55cf9a091d314a1aa21552f2254f756f}

Single Side Band (SSB) implementation

This implementation was tested only on Si4735-D60 device.

SSB modulation is a refinement of amplitude modulation that one of the side band and the carrier are suppressed.

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; pages 3 and 5

First of all, it is important to say that the SSB patch content is not part of this library. The paches used here were made available by Mr. Vadim Afonkin on his Dropbox repository. It is important to note that the author of this library does not encourage anyone to use the SSB patches content for commercial purposes. In other words, this library only supports SSB patches, the patches themselves are not part of this library. 

What does SSB patch means? In this context, a patch is a piece of software used to change the behavior of the [SI4735](#class_s_i4735) device. There is little information available about patching the [SI4735](#class_s_i4735).

The following information is the understanding of the author of this project and it is not necessarily correct.

A patch is executed internally (run by internal MCU) of the device. Usually, patches are used to fixes bugs or add improvements and new features of the firmware installed in the internal ROM of the device. Patches to the [SI4735](#class_s_i4735) are distributed in binary form and have to be transferred to the internal RAM of the device by the host MCU (in this case Arduino boards). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system. Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

I would like to thank Mr Vadim Afonkin for making available the SSBRX patches for SI4735-D60 on his Dropbox repository. On this repository you have two files, amrx_6_0_1_ssbrx_patch_full_0x9D29.csg and amrx_6_0_1_ssbrx_patch_init_0xA902.csg. It is important to know that the patch content of the original files is constant hexadecimal representation used by the language C/C++. Actally, the original files are in ASCII format (not in binary format). If you are not using C/C++ or if you want to load the files directly to the [SI4735](#class_s_i4735), you must convert the values to numeric value of the hexadecimal constants. For example: 0x15 = 21 (00010101); 0x16 = 22 (00010110); 0x01 = 1 (00000001); 0xFF = 255 (11111111);

ATTENTION: The author of this project does not guarantee that procedures shown here will work in your development environment. Given this, it is at your own risk to continue with the procedures suggested here. This library works with the I²C communication protocol and it is designed to apply a SSB extension PATCH to CI SI4735-D60. Once again, the author disclaims any liability for any damage this procedure may cause to your [SI4735](#class_s_i4735) or other devices that you are using. Sets the SSB Beat Frequency Offset (BFO).

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; pages 5 and 23

#### Parameters
* `offset` 16-bit signed value (unit in Hz). The valid range is -16383 to +16383 Hz.

#### `public void `[`setSSBConfig`](#class_s_i4735_1a9224e7d4ef14c887da96dbefd2dcefd3)`(uint8_t AUDIOBW,uint8_t SBCUTFLT,uint8_t AVC_DIVIDER,uint8_t AVCEN,uint8_t SMUTESEL,uint8_t DSP_AFCDIS)` {#class_s_i4735_1a9224e7d4ef14c887da96dbefd2dcefd3}

Set the SSB receiver mode details: 1) Enable or disable AFC track to carrier function for receiving normal AM signals; 2) Set the audio bandwidth; 3) Set the side band cutoff filter; 4) Set soft-mute based on RSSI or SNR; 5) Enable or disbable automatic volume control (AVC) function.

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24

#### Parameters
* `AUDIOBW` SSB Audio bandwidth; 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz. 

* `SBCUTFLT` SSB side band cutoff filter for band passand low pass filter if 0, the band pass filter to cutoff both the unwanted side band and high frequency component > 2KHz of the wanted side band (default). 

* `AVC_DIVIDER` set 0 for SSB mode; set 3 for SYNC mode. 

* `AVCEN` SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default). 

* `SMUTESEL` SSB Soft-mute Based on RSSI or SNR. 

* `DSP_AFCDIS` DSP AFC Disable or enable; 0=SYNC MODE, AFC enable; 1=SSB MODE, AFC disable.

#### `public void `[`setSSB`](#class_s_i4735_1a7c48f2af416b1f89dbedbfbe9a637620)`(uint16_t fromFreq,uint16_t toFreq,uint16_t intialFreq,uint16_t step,uint8_t usblsb)` {#class_s_i4735_1a7c48f2af416b1f89dbedbfbe9a637620}

#### `public void `[`setSSB`](#class_s_i4735_1a3e907ddf560a0c8286fa55768701b3da)`(uint8_t usblsb)` {#class_s_i4735_1a3e907ddf560a0c8286fa55768701b3da}

Set the radio to AM function. It means: LW MW and SW.

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; pages 13 and 14 

**See also**: [setAM()](#class_s_i4735_1a753390952a1f2c2a98343979f1f3daa0)

**See also**: void [SI4735::setFrequency(uint16_t freq)](#class_s_i4735_1a63d205f2d3169ba877a2bd2ee28d224f)

#### Parameters
* `usblsb` upper or lower side band; 1 = LSB; 2 = USB

#### `public void `[`setSSBAudioBandwidth`](#class_s_i4735_1a6b2ad4d027463262350134490d16bf32)`(uint8_t AUDIOBW)` {#class_s_i4735_1a6b2ad4d027463262350134490d16bf32}

SSB Audio Bandwidth for SSB mode

0 = 1.2 kHz low-pass filter* . (default) 1 = 2.2 kHz low-pass filter* . 2 = 3.0 kHz low-pass filter. 3 = 4.0 kHz low-pass filter. 4 = 500 Hz band-pass filter for receiving CW signal, i.e. [250 Hz, 750 Hz] with center frequency at 500 Hz when USB is selected or [-250 Hz, -750 1Hz] with center frequency at -500Hz when LSB is selected* . 5 = 1 kHz band-pass filter for receiving CW signal, i.e. [500 Hz, 1500 Hz] with center frequency at 1 kHz when USB is selected or [-500 Hz, -1500 1 Hz] with center frequency at -1kHz when LSB is selected* . Other values = reserved. Note: If audio bandwidth selected is about 2 kHz or below, it is recommended to set SBCUTFLT[3:0] to 0 to enable the band pass filter for better high- cut performance on the wanted side band. Otherwise, set it to 1.

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24

#### Parameters
* `AUDIOBW` the valid values are 0, 1, 2, 3, 4 or 5; see description above

#### `public void `[`setSSBAutomaticVolumeControl`](#class_s_i4735_1acacf1dac4c0ec170d1da629dd1fd30cb)`(uint8_t AVCEN)` {#class_s_i4735_1acacf1dac4c0ec170d1da629dd1fd30cb}

Sets SSB Automatic Volume Control (AVC) for SSB mode

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24

#### Parameters
* `AVCEN` 0 = Disable AVC; 1 = Enable AVC (default).

#### `public void `[`setSBBSidebandCutoffFilter`](#class_s_i4735_1ae9d50b24642784943fefedbac944a8e8)`(uint8_t SBCUTFLT)` {#class_s_i4735_1ae9d50b24642784943fefedbac944a8e8}

Sets SBB Sideband Cutoff Filter for band pass and low pass filters: 0 = Band pass filter to cutoff both the unwanted side band and high frequency components > 2.0 kHz of the wanted side band. (default) 1 = Low pass filter to cutoff the unwanted side band. Other values = not allowed.

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24

#### Parameters
* `SBCUTFLT` 0 or 1; see above

#### `public void `[`setSSBAvcDivider`](#class_s_i4735_1afc5e87c50a1eba6de9504c114cc1cb70)`(uint8_t AVC_DIVIDER)` {#class_s_i4735_1afc5e87c50a1eba6de9504c114cc1cb70}

Sets AVC Divider

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24 

#### Parameters
* `AVC_DIVIDER` SSB mode, set divider = 0; SYNC mode, set divider = 3; Other values = not allowed.

#### `public void `[`setSSBDspAfc`](#class_s_i4735_1a6cde26a5abf759a628012d1457d85c72)`(uint8_t DSP_AFCDIS)` {#class_s_i4735_1a6cde26a5abf759a628012d1457d85c72}

Sets DSP AFC disable or enable

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24

#### Parameters
* `DSP_AFCDIS` 0 = SYNC mode, AFC enable; 1 = SSB mode, AFC disable

#### `public void `[`setSSBSoftMute`](#class_s_i4735_1a91251dfc2ff9488aaef3aa1a528097d9)`(uint8_t SMUTESEL)` {#class_s_i4735_1a91251dfc2ff9488aaef3aa1a528097d9}

Sets SSB Soft-mute Based on RSSI or SNR Selection:

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE; page 24

#### Parameters
* `SMUTESEL` 0 = Soft-mute based on RSSI (default); 1 = Soft-mute based on SNR.

#### `public `[`si47x_firmware_query_library`](#unionsi47x__firmware__query__library)` `[`queryLibraryId`](#class_s_i4735_1adfc2c82524918224cf19fec5b7fcf6d1)`()` {#class_s_i4735_1adfc2c82524918224cf19fec5b7fcf6d1}

SI47XX PATCH RESOURCES Call it first if you are applying a patch on [SI4735](#class_s_i4735). Used to confirm if the patch is compatible with the internal device library revision. See Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220.

#### Returns
a struct [si47x_firmware_query_library](#unionsi47x__firmware__query__library) (see it in [SI4735.h](#_s_i4735_8h_source)) Query the library information

You have to call this function if you are applying a patch on SI47XX (SI4735-D60)

The first command that is sent to the device is the POWER_UP command to confirm that the patch is compatible with the internal device library revision. The device moves into the powerup mode, returns the reply, and moves into the powerdown mode. The POWER_UP command is sent to the device again to configure the mode of the device and additionally is used to start the patching process. When applying the patch, the PATCH bit in ARG1 of the POWER_UP command must be set to 1 to begin the patching process. [AN332 page 219].

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 214, 215, 216, 219 

**See also**: [si47x_firmware_query_library](#unionsi47x__firmware__query__library) in [SI4735.h](#_s_i4735_8h_source)

#### Returns
[si47x_firmware_query_library](#unionsi47x__firmware__query__library) Library Identification

#### `public void `[`patchPowerUp`](#class_s_i4735_1afaabd6c73b7ad7c20ae959d5b75c1e71)`()` {#class_s_i4735_1afaabd6c73b7ad7c20ae959d5b75c1e71}

This method can be used to prepare the device to apply SSBRX patch Call queryLibraryId before call this method. Powerup the device by issuing the POWER_UP command with FUNC = 1 (AM/SW/LW Receive)

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220 and 

**See also**: AN332 REV 0.8 UNIVERSAL PROGRAMMING GUIDE AMENDMENT FOR SI4735-D60 SSB AND NBFM PATCHES; page 7.

#### `public bool `[`downloadPatch`](#class_s_i4735_1afd1343bd8b4f0c290ef81c418222036c)`(const uint8_t * ssb_patch_content,const uint16_t ssb_patch_content_size)` {#class_s_i4735_1afd1343bd8b4f0c290ef81c418222036c}

Transfers the content of a patch stored in a array of bytes to the [SI4735](#class_s_i4735) device. You must mount an array as shown below and know the size of that array as well.

It is importante to say that patches to the [SI4735](#class_s_i4735) are distributed in binary form and have to be transferred to the internal RAM of the device by the host MCU (in this case Arduino). Since the RAM is volatile memory, the patch stored into the device gets lost when you turn off the system. Consequently, the content of the patch has to be transferred again to the device each time after turn on the system or reset the device.

The disadvantage of this approach is the amount of memory used by the patch content. This may limit the use of other radio functions you want implemented in Arduino.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 64 and 215-220. 

Example of content: const PROGMEM uint8_t ssb_patch_content_full[] = { // SSB patch for whole SSBRX full download 0x15, 0x00, 0x0F, 0xE0, 0xF2, 0x73, 0x76, 0x2F, 0x16, 0x6F, 0x26, 0x1E, 0x00, 0x4B, 0x2C, 0x58, 0x16, 0xA3, 0x74, 0x0F, 0xE0, 0x4C, 0x36, 0xE4, 0x16, 0x3B, 0x1D, 0x4A, 0xEC, 0x36, 0x28, 0xB7, 0x16, 0x00, 0x3A, 0x47, 0x37, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9D, 0x29}; 

const int size_content_full = sizeof ssb_patch_content_full;

#### Parameters
* `ssb_patch_content` point to array of bytes content patch. 

* `ssb_patch_content_size` array size (number of bytes). The maximum size allowed for a patch is 15856 bytes

#### Returns
false if an error is found.

#### `public bool `[`downloadPatch`](#class_s_i4735_1aa99cdde8dd65eb92f2ada159cc215c98)`(int eeprom_i2c_address)` {#class_s_i4735_1aa99cdde8dd65eb92f2ada159cc215c98}

Under construction... Transfers the content of a patch stored in a eeprom to the [SI4735](#class_s_i4735) device.

TO USE THIS METHOD YOU HAVE TO HAVE A EEPROM WRITEN WITH THE PATCH CONTENT

**See also**: the sketch write_ssb_patch_eeprom.ino (TO DO)

#### Parameters
* `eeprom_i2c_address` 

#### Returns
false if an error is found.

#### `public void `[`ssbPowerUp`](#class_s_i4735_1a0fcfd8e487c594707b3f2aaa71a86d68)`()` {#class_s_i4735_1a0fcfd8e487c594707b3f2aaa71a86d68}

This function can be useful for debug and teste.

#### `public inline void `[`setI2CLowSpeedMode`](#class_s_i4735_1aecfcd50dbb3d95d66c8cdc77d52af63c)`(void)` {#class_s_i4735_1aecfcd50dbb3d95d66c8cdc77d52af63c}

#### `public inline void `[`setI2CStandardMode`](#class_s_i4735_1a00ade87631677b8cc88098d0b55cd5a4)`(void)` {#class_s_i4735_1a00ade87631677b8cc88098d0b55cd5a4}

Sets I2C buss to 10KHz.

#### `public inline void `[`setI2CFastMode`](#class_s_i4735_1a7bb3214f70381cf26bb93f96729d641e)`(void)` {#class_s_i4735_1a7bb3214f70381cf26bb93f96729d641e}

Sets I2C buss to 100KHz.

#### `public inline void `[`setI2CFastModeCustom`](#class_s_i4735_1a7433b4e80fd3b501efeae7973d29c985)`(long value)` {#class_s_i4735_1a7433b4e80fd3b501efeae7973d29c985}

Sets I2C buss to 400KHz.

Sets the I2C bus to a given value.

ATTENTION: use this function with cation

#### Parameters
* `value` in Hz. For example: The values 500000 sets the bus to 500KHz.

#### `public void `[`setDeviceI2CAddress`](#class_s_i4735_1a748a4027f317972cb2fb1aed4865973b)`(uint8_t senPin)` {#class_s_i4735_1a748a4027f317972cb2fb1aed4865973b}

Sets the I2C Bus Address

ATTENTION: The parameter senPin is not the I2C bus address. It is the SEN pin setup of the schematic (eletronic circuit). If it is connected to the ground, call this function with senPin = 0; else senPin = 1. You do not need to use this function if the SEN PIN configured to ground (GND).

The default value is 0x11 (senPin = 0). In this case you have to ground the pin SEN of the SI473X. If you want to change this address, call this function with senPin = 1

#### Parameters
* `senPin` 0 - when the pin SEN (16 on SSOP version or pin 6 on QFN version) is set to low (GND - 0V) 1 - when the pin SEN (16 on SSOP version or pin 6 on QFN version) is set to high (+3.3V)

#### `public int16_t `[`getDeviceI2CAddress`](#class_s_i4735_1a8eddbfb3e62786aa4843d4bcd25da2b9)`(uint8_t resetPin)` {#class_s_i4735_1a8eddbfb3e62786aa4843d4bcd25da2b9}

Scans for two possible addresses for the Si47XX (0x11 or 0x63 ) This function also sets the system to the found I2C bus address of Si47XX.

You do not need to use this function if the SEN PIN is configured to ground (GND). The default I2C address is 0x11. Use this function if you do not know how the SEN pin is configured.

#### Parameters
* `uint8_t` resetPin MCU Mater (Arduino) reset pin 

#### Returns
int16_t 0x11 if the SEN pin of the Si47XX is low or 0x63 if the SEN pin of the Si47XX is HIGH or 0x0 if error.

#### `public void `[`setDeviceOtherI2CAddress`](#class_s_i4735_1a668008457910e916ebadf15bda2e5b29)`(uint8_t i2cAddr)` {#class_s_i4735_1a668008457910e916ebadf15bda2e5b29}

Sets the onther I2C Bus Address (for Si470X) You can set another I2C address different of 0x11 and 0x63 
#### Parameters
* `uint8_t` i2cAddr (example 0x10)

#### `protected char `[`rds_buffer2A`](#class_s_i4735_1a0b40791484c47c0bf90d8dc255a20269) {#class_s_i4735_1a0b40791484c47c0bf90d8dc255a20269}

#### `protected char `[`rds_buffer2B`](#class_s_i4735_1a7aa607f80e97d45ac7a80cdcbdeb09f9) {#class_s_i4735_1a7aa607f80e97d45ac7a80cdcbdeb09f9}

RDS Radio Text buffer - Program Information.

#### `protected char `[`rds_buffer0A`](#class_s_i4735_1ac24a8bbd39628c2981208cb452165a0e) {#class_s_i4735_1ac24a8bbd39628c2981208cb452165a0e}

RDS Radio Text buffer - Station Informaation.

#### `protected char `[`rds_time`](#class_s_i4735_1a1746daa81b363880dd106fa430784bf8) {#class_s_i4735_1a1746daa81b363880dd106fa430784bf8}

RDS Basic tuning and switching information (Type 0 groups)

#### `protected int `[`rdsTextAdress2A`](#class_s_i4735_1a1c7ec3559a78c5ef1bda49cb0abdf1de) {#class_s_i4735_1a1c7ec3559a78c5ef1bda49cb0abdf1de}

#### `protected int `[`rdsTextAdress2B`](#class_s_i4735_1adecc2478d73cd7d18e1f88fd70d1488d) {#class_s_i4735_1adecc2478d73cd7d18e1f88fd70d1488d}

#### `protected int `[`rdsTextAdress0A`](#class_s_i4735_1a11477d1eb13efb348395a4984b67f392) {#class_s_i4735_1a11477d1eb13efb348395a4984b67f392}

#### `protected int16_t `[`deviceAddress`](#class_s_i4735_1a243ecc7d01fc0d0e9cabb4ed00b9e47d) {#class_s_i4735_1a243ecc7d01fc0d0e9cabb4ed00b9e47d}

#### `protected uint8_t `[`lastTextFlagAB`](#class_s_i4735_1abd965c0e49493bc2c7c37547d36dcc96) {#class_s_i4735_1abd965c0e49493bc2c7c37547d36dcc96}

#### `protected uint8_t `[`resetPin`](#class_s_i4735_1a6470177f747ef7eb8e7d02c76f72b19e) {#class_s_i4735_1a6470177f747ef7eb8e7d02c76f72b19e}

#### `protected uint8_t `[`interruptPin`](#class_s_i4735_1af8f8123888060f858c74ba08a29ff38b) {#class_s_i4735_1af8f8123888060f858c74ba08a29ff38b}

#### `protected uint8_t `[`currentTune`](#class_s_i4735_1acc3d3a411d181e08846219661dbde1c4) {#class_s_i4735_1acc3d3a411d181e08846219661dbde1c4}

#### `protected uint16_t `[`currentMinimumFrequency`](#class_s_i4735_1a4663c1c92d980472556368944cb2fc68) {#class_s_i4735_1a4663c1c92d980472556368944cb2fc68}

#### `protected uint16_t `[`currentMaximumFrequency`](#class_s_i4735_1aa3d41ad3fdb9d821a721a8acfaf47670) {#class_s_i4735_1aa3d41ad3fdb9d821a721a8acfaf47670}

#### `protected uint16_t `[`currentWorkFrequency`](#class_s_i4735_1a9da4f3df9e5e6b0448eff15e12f690dd) {#class_s_i4735_1a9da4f3df9e5e6b0448eff15e12f690dd}

#### `protected uint16_t `[`currentStep`](#class_s_i4735_1ad72759f246cceb147bf031b72173b0ca) {#class_s_i4735_1ad72759f246cceb147bf031b72173b0ca}

#### `protected uint8_t `[`lastMode`](#class_s_i4735_1a157e4ae8cd53396973584aa10888cd47) {#class_s_i4735_1a157e4ae8cd53396973584aa10888cd47}

#### `protected uint8_t `[`currentAvcAmMaxGain`](#class_s_i4735_1a650490ab792c1853c97f600661ecdaa1) {#class_s_i4735_1a650490ab792c1853c97f600661ecdaa1}

Store the last mode used.

#### `protected `[`si47x_frequency`](#unionsi47x__frequency)` `[`currentFrequency`](#class_s_i4735_1a4439c40db789f02371ea0fb9b22650c5) {#class_s_i4735_1a4439c40db789f02371ea0fb9b22650c5}

#### `protected `[`si47x_set_frequency`](#unionsi47x__set__frequency)` `[`currentFrequencyParams`](#class_s_i4735_1a8253ae2f3c87a6943776f8a63a691ca3) {#class_s_i4735_1a8253ae2f3c87a6943776f8a63a691ca3}

#### `protected `[`si47x_rqs_status`](#unionsi47x__rqs__status)` `[`currentRqsStatus`](#class_s_i4735_1a9edd8cc9254695631795e7efc008eb28) {#class_s_i4735_1a9edd8cc9254695631795e7efc008eb28}

#### `protected `[`si47x_response_status`](#unionsi47x__response__status)` `[`currentStatus`](#class_s_i4735_1a7ad35ba002b73b66e9ce97b304854695) {#class_s_i4735_1a7ad35ba002b73b66e9ce97b304854695}

#### `protected `[`si47x_firmware_information`](#unionsi47x__firmware__information)` `[`firmwareInfo`](#class_s_i4735_1ad03d545e53a7ca2fe072a7b03d60910e) {#class_s_i4735_1ad03d545e53a7ca2fe072a7b03d60910e}

#### `protected `[`si47x_rds_status`](#unionsi47x__rds__status)` `[`currentRdsStatus`](#class_s_i4735_1a42d5c6991d90ea1c8ae35b74f8bccc48) {#class_s_i4735_1a42d5c6991d90ea1c8ae35b74f8bccc48}

#### `protected `[`si47x_agc_status`](#unionsi47x__agc__status)` `[`currentAgcStatus`](#class_s_i4735_1adf2d1f37ec9edf95e14aecb3cdc7417d) {#class_s_i4735_1adf2d1f37ec9edf95e14aecb3cdc7417d}

#### `protected `[`si47x_ssb_mode`](#unionsi47x__ssb__mode)` `[`currentSSBMode`](#class_s_i4735_1a74ee9c99b1e092f7b1efa2f771f38324) {#class_s_i4735_1a74ee9c99b1e092f7b1efa2f771f38324}

#### `protected `[`si473x_powerup`](#unionsi473x__powerup)` `[`powerUp`](#class_s_i4735_1a1331b701088bda20bf0844e2e38be142) {#class_s_i4735_1a1331b701088bda20bf0844e2e38be142}

#### `protected uint8_t `[`volume`](#class_s_i4735_1aa5220772c468fcfb69b5d16163de73fc) {#class_s_i4735_1aa5220772c468fcfb69b5d16163de73fc}

#### `protected uint8_t `[`currentSsbStatus`](#class_s_i4735_1a05730ca069db4be7c45a9c92f29d3b52) {#class_s_i4735_1a05730ca069db4be7c45a9c92f29d3b52}

#### `protected void `[`waitInterrupr`](#class_s_i4735_1ad2e95c88de0dfa58ff7aa36988071421)`(void)` {#class_s_i4735_1ad2e95c88de0dfa58ff7aa36988071421}

If you setup interrupt, this function will be called whenever the Si4735 changes.

#### `protected void `[`sendProperty`](#class_s_i4735_1ae351181347f8c512b625d98ef7844961)`(uint16_t propertyValue,uint16_t param)` {#class_s_i4735_1ae351181347f8c512b625d98ef7844961}

Sends (sets) property to the SI47XX This method is used for others to send generic properties and params to SI47XX

**See also**: Si47XX PROGRAMMING GUIDE; AN332; pages 68, 124 and 133.

#### `protected void `[`sendSSBModeProperty`](#class_s_i4735_1ae1d866bfc4cb37c4268c8942b4b4d062)`()` {#class_s_i4735_1ae1d866bfc4cb37c4268c8942b4b4d062}

Just send the property SSB_MOD to the device. Internal use (privete method).

#### `protected void `[`disableFmDebug`](#class_s_i4735_1ae4683e9a9fc7a633305b18dae54fbd83)`()` {#class_s_i4735_1ae4683e9a9fc7a633305b18dae54fbd83}

There is a debug feature that remains active in Si4704/05/3x-D60 firmware which can create periodic noise in audio. Silicon Labs recommends you disable this feature by sending the following bytes (shown here in hexadecimal form): 0x12 0x00 0xFF 0x00 0x00 0x00.

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 299.

#### `protected void `[`clearRdsBuffer2A`](#class_s_i4735_1ac65c552004f5e5e646638e37a5a68213)`()` {#class_s_i4735_1ac65c552004f5e5e646638e37a5a68213}

Clear RDS buffer 2A (text)

#### `protected void `[`clearRdsBuffer2B`](#class_s_i4735_1aed0988165057aa2880f41b88ac236cd7)`()` {#class_s_i4735_1aed0988165057aa2880f41b88ac236cd7}

Clear RDS buffer 2B (text)

#### `protected void `[`clearRdsBuffer0A`](#class_s_i4735_1adede134b96fd5650b87673ce413babb2)`()` {#class_s_i4735_1adede134b96fd5650b87673ce413babb2}

Clear RDS buffer 0A (text)

# struct `si4735_digital_output_sample_rate` {#structsi4735__digital__output__sample__rate}

Digital audio output sample structure (Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE). Used to enable digital audio output and to configure the digital audio output sample rate in samples per second (sps).

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 196.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint16_t `[`DOSR`](#structsi4735__digital__output__sample__rate_1a9988ba829a46409f23d7cb9bb0f156d9) | 

## Members

#### `public uint16_t `[`DOSR`](#structsi4735__digital__output__sample__rate_1a9988ba829a46409f23d7cb9bb0f156d9) {#structsi4735__digital__output__sample__rate_1a9988ba829a46409f23d7cb9bb0f156d9}

Generated by [Moxygen](https://sourcey.com/moxygen)
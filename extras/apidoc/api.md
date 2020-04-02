# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`si4735_digital_output_format.refined`](#structsi4735__digital__output__format_8refined) | 
`struct `[`si4735_digital_output_sample_rate`](#structsi4735__digital__output__sample__rate) | Digital audio output sample structure (Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE).
`struct `[`si473x_powerup.arg`](#structsi473x__powerup_8arg) | 
`struct `[`si47x_agc_overrride.arg`](#structsi47x__agc__overrride_8arg) | 
`struct `[`si47x_agc_status.refined`](#structsi47x__agc__status_8refined) | 
`struct `[`si47x_antenna_capacitor.raw`](#structsi47x__antenna__capacitor_8raw) | 
`struct `[`si47x_bandwidth_config.param`](#structsi47x__bandwidth__config_8param) | 
`struct `[`si47x_firmware_information.resp`](#structsi47x__firmware__information_8resp) | 
`struct `[`si47x_firmware_query_library.resp`](#structsi47x__firmware__query__library_8resp) | 
`struct `[`si47x_frequency.raw`](#structsi47x__frequency_8raw) | 
`struct `[`si47x_property.raw`](#structsi47x__property_8raw) | 
`struct `[`si47x_rds_blocka.raw`](#structsi47x__rds__blocka_8raw) | 
`struct `[`si47x_rds_blocka.refined`](#structsi47x__rds__blocka_8refined) | 
`struct `[`si47x_rds_blockb.group0`](#structsi47x__rds__blockb_8group0) | 
`struct `[`si47x_rds_blockb.group2`](#structsi47x__rds__blockb_8group2) | 
`struct `[`si47x_rds_blockb.raw`](#structsi47x__rds__blockb_8raw) | 
`struct `[`si47x_rds_blockb.refined`](#structsi47x__rds__blockb_8refined) | 
`struct `[`si47x_rds_command.arg`](#structsi47x__rds__command_8arg) | 
`struct `[`si47x_rds_config.arg`](#structsi47x__rds__config_8arg) | 
`struct `[`si47x_rds_date_time.refined`](#structsi47x__rds__date__time_8refined) | 
`struct `[`si47x_rds_int_source.refined`](#structsi47x__rds__int__source_8refined) | 
`struct `[`si47x_rds_status.resp`](#structsi47x__rds__status_8resp) | 
`struct `[`si47x_response_status.resp`](#structsi47x__response__status_8resp) | 
`struct `[`si47x_rqs_status.resp`](#structsi47x__rqs__status_8resp) | 
`struct `[`si47x_seek.arg`](#structsi47x__seek_8arg) | 
`struct `[`si47x_set_frequency.arg`](#structsi47x__set__frequency_8arg) | 
`struct `[`si47x_ssb_mode.param`](#structsi47x__ssb__mode_8param) | 
`struct `[`si47x_tune_status.arg`](#structsi47x__tune__status_8arg) | 
`union `[`si4735_digital_output_format`](#unionsi4735__digital__output__format) | Digital audio output format data structure (Property 0x0102. DIGITAL_OUTPUT_FORMAT).
`union `[`si473x_powerup`](#unionsi473x__powerup) | Power Up arguments data type.
`union `[`si47x_agc_overrride`](#unionsi47x__agc__overrride) | If FM, Overrides AGC setting by disabling the AGC and forcing the LNA to have a certain gain that ranges between 0 (minimum attenuation) and 26 (maximum attenuation). If AM, overrides the AGC setting by disabling the AGC and forcing the gain index that ranges between 0
`union `[`si47x_agc_status`](#unionsi47x__agc__status) | AGC data types FM / AM and SSB structure to AGC
`union `[`si47x_antenna_capacitor`](#unionsi47x__antenna__capacitor) | Antenna Tuning Capacitor data type manupulation.
`union `[`si47x_bandwidth_config`](#unionsi47x__bandwidth__config) | The bandwidth of the AM channel filter data type AMCHFLT values: 0 = 6 kHz Bandwidth 
`union `[`si47x_firmware_information`](#unionsi47x__firmware__information) | Data representation for Firmware Information (GET_REV)
`union `[`si47x_firmware_query_library`](#unionsi47x__firmware__query__library) | Firmware Query Library ID response.
`union `[`si47x_frequency`](#unionsi47x__frequency) | Represents how the frequency is stored in the si4735.
`union `[`si47x_property`](#unionsi47x__property) | Data type to deal with SET_PROPERTY command.
`union `[`si47x_rds_blocka`](#unionsi47x__rds__blocka) | Block A data type.
`union `[`si47x_rds_blockb`](#unionsi47x__rds__blockb) | Block B data type.
`union `[`si47x_rds_command`](#unionsi47x__rds__command) | Data type for RDS Status command and response information.
`union `[`si47x_rds_config`](#unionsi47x__rds__config) | Data type for FM_RDS_CONFIG Property.
`union `[`si47x_rds_date_time`](#unionsi47x__rds__date__time) | Group type 4A ( RDS Date and Time) When group type 4A is used by the station, it shall be transmitted every minute according to EN 50067. This Structure uses blocks 2,3 and 5 (B,C,D)
`union `[`si47x_rds_int_source`](#unionsi47x__rds__int__source) | FM_RDS_INT_SOURCE property data type.
`union `[`si47x_rds_status`](#unionsi47x__rds__status) | Response data type for current channel and reads an entry from the RDS FIFO.
`union `[`si47x_response_status`](#unionsi47x__response__status) | Response status command.
`union `[`si47x_rqs_status`](#unionsi47x__rqs__status) | Radio Signal Quality data representation.
`union `[`si47x_seek`](#unionsi47x__seek) | Seek frequency (automatic tuning)
`union `[`si47x_set_frequency`](#unionsi47x__set__frequency) | AM Tune frequency data type command (AM_TUNE_FREQ command)
`union `[`si47x_ssb_mode`](#unionsi47x__ssb__mode) | SSB - datatype for SSB_MODE (property 0x0101)
`union `[`si47x_tune_status`](#unionsi47x__tune__status) | Seek station status.

# struct `si4735_digital_output_format.refined` {#structsi4735__digital__output__format_8refined}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`OSIZE`](#structsi4735__digital__output__format_8refined_1a8e0cf30fb9cddc896d94167934615d42) | 
`public uint8_t `[`OMONO`](#structsi4735__digital__output__format_8refined_1a61324adf2d131b4df8038991cbca0842) | Digital Output Audio Sample Precision (0=16 bits, 1=20 bits, 2=24 bits, 3=8bits).
`public uint8_t `[`OMODE`](#structsi4735__digital__output__format_8refined_1a870fffeed608a381a3abc7e27b22a7ee) | Digital Output Mono Mode (0=Use mono/stereo blend ).
`public uint8_t `[`OFALL`](#structsi4735__digital__output__format_8refined_1abfdc1bd5a8f46739eef7fd4fa44ad600) | Digital Output Mode (0000=I2S, 0110 = Left-justified, 1000 = MSB at second DCLK after DFS pulse, 1100 = MSB at first DCLK after DFS pulse).
`public uint8_t `[`dummy`](#structsi4735__digital__output__format_8refined_1a275876e34cf609db118f3d84b799a790) | Digital Output DCLK Edge (0 = use DCLK rising edge, 1 = use DCLK falling edge)

## Members

#### `public uint8_t `[`OSIZE`](#structsi4735__digital__output__format_8refined_1a8e0cf30fb9cddc896d94167934615d42) {#structsi4735__digital__output__format_8refined_1a8e0cf30fb9cddc896d94167934615d42}

#### `public uint8_t `[`OMONO`](#structsi4735__digital__output__format_8refined_1a61324adf2d131b4df8038991cbca0842) {#structsi4735__digital__output__format_8refined_1a61324adf2d131b4df8038991cbca0842}

Digital Output Audio Sample Precision (0=16 bits, 1=20 bits, 2=24 bits, 3=8bits).

#### `public uint8_t `[`OMODE`](#structsi4735__digital__output__format_8refined_1a870fffeed608a381a3abc7e27b22a7ee) {#structsi4735__digital__output__format_8refined_1a870fffeed608a381a3abc7e27b22a7ee}

Digital Output Mono Mode (0=Use mono/stereo blend ).

#### `public uint8_t `[`OFALL`](#structsi4735__digital__output__format_8refined_1abfdc1bd5a8f46739eef7fd4fa44ad600) {#structsi4735__digital__output__format_8refined_1abfdc1bd5a8f46739eef7fd4fa44ad600}

Digital Output Mode (0000=I2S, 0110 = Left-justified, 1000 = MSB at second DCLK after DFS pulse, 1100 = MSB at first DCLK after DFS pulse).

#### `public uint8_t `[`dummy`](#structsi4735__digital__output__format_8refined_1a275876e34cf609db118f3d84b799a790) {#structsi4735__digital__output__format_8refined_1a275876e34cf609db118f3d84b799a790}

Digital Output DCLK Edge (0 = use DCLK rising edge, 1 = use DCLK falling edge)

# struct `si4735_digital_output_sample_rate` {#structsi4735__digital__output__sample__rate}

Digital audio output sample structure (Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE).

Used to enable digital audio output and to configure the digital audio output sample rate in samples per second (sps).

**See also**: Si47XX PROGRAMMING GUIDE; AN332; page 196.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint16_t `[`DOSR`](#structsi4735__digital__output__sample__rate_1a9988ba829a46409f23d7cb9bb0f156d9) | 

## Members

#### `public uint16_t `[`DOSR`](#structsi4735__digital__output__sample__rate_1a9988ba829a46409f23d7cb9bb0f156d9) {#structsi4735__digital__output__sample__rate_1a9988ba829a46409f23d7cb9bb0f156d9}

# struct `si473x_powerup.arg` {#structsi473x__powerup_8arg}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`FUNC`](#structsi473x__powerup_8arg_1a1e49c8ad07b69571bfcafaf18ac9fa5b) | 
`public uint8_t `[`XOSCEN`](#structsi473x__powerup_8arg_1a22c9aef8785be330d7e1faf88bcfd4b1) | Function (0 = FM Receive; 1–14 = Reserved; 15 = Query Library ID)
`public uint8_t `[`PATCH`](#structsi473x__powerup_8arg_1a63bc9a3997d66d835d9f3ec29451407d) | Crystal Oscillator Enable (0 = crystal oscillator disabled; 1 = Use crystal oscillator and and OPMODE=ANALOG AUDIO) .
`public uint8_t `[`GPO2OEN`](#structsi473x__powerup_8arg_1a16e39ff0e3715799964f55ae2059869d) | Patch Enable (0 = Boot normally; 1 = Copy non-volatile memory to RAM).
`public uint8_t `[`CTSIEN`](#structsi473x__powerup_8arg_1a70cc84d478cf749951dcd0abde88b0ce) | GPO2 Output Enable (0 = GPO2 output disabled; 1 = GPO2 output enabled).
`public uint8_t `[`OPMODE`](#structsi473x__powerup_8arg_1adc7ae7360da2a9886f674ed6943a2524) | CTS Interrupt Enable (0 = CTS interrupt disabled; 1 = CTS interrupt enabled).

## Members

#### `public uint8_t `[`FUNC`](#structsi473x__powerup_8arg_1a1e49c8ad07b69571bfcafaf18ac9fa5b) {#structsi473x__powerup_8arg_1a1e49c8ad07b69571bfcafaf18ac9fa5b}

#### `public uint8_t `[`XOSCEN`](#structsi473x__powerup_8arg_1a22c9aef8785be330d7e1faf88bcfd4b1) {#structsi473x__powerup_8arg_1a22c9aef8785be330d7e1faf88bcfd4b1}

Function (0 = FM Receive; 1–14 = Reserved; 15 = Query Library ID)

#### `public uint8_t `[`PATCH`](#structsi473x__powerup_8arg_1a63bc9a3997d66d835d9f3ec29451407d) {#structsi473x__powerup_8arg_1a63bc9a3997d66d835d9f3ec29451407d}

Crystal Oscillator Enable (0 = crystal oscillator disabled; 1 = Use crystal oscillator and and OPMODE=ANALOG AUDIO) .

#### `public uint8_t `[`GPO2OEN`](#structsi473x__powerup_8arg_1a16e39ff0e3715799964f55ae2059869d) {#structsi473x__powerup_8arg_1a16e39ff0e3715799964f55ae2059869d}

Patch Enable (0 = Boot normally; 1 = Copy non-volatile memory to RAM).

#### `public uint8_t `[`CTSIEN`](#structsi473x__powerup_8arg_1a70cc84d478cf749951dcd0abde88b0ce) {#structsi473x__powerup_8arg_1a70cc84d478cf749951dcd0abde88b0ce}

GPO2 Output Enable (0 = GPO2 output disabled; 1 = GPO2 output enabled).

#### `public uint8_t `[`OPMODE`](#structsi473x__powerup_8arg_1adc7ae7360da2a9886f674ed6943a2524) {#structsi473x__powerup_8arg_1adc7ae7360da2a9886f674ed6943a2524}

CTS Interrupt Enable (0 = CTS interrupt disabled; 1 = CTS interrupt enabled).

# struct `si47x_agc_overrride.arg` {#structsi47x__agc__overrride_8arg}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`AGCDIS`](#structsi47x__agc__overrride_8arg_1aeaad2cabb6417746b2dd432eda476f55) | 
`public uint8_t `[`DUMMY`](#structsi47x__agc__overrride_8arg_1abd2103035a8021942390a78a431ba0c4) | 
`public uint8_t `[`AGCIDX`](#structsi47x__agc__overrride_8arg_1a26bd0af30c189325bd004e10593eb5cd) | 

## Members

#### `public uint8_t `[`AGCDIS`](#structsi47x__agc__overrride_8arg_1aeaad2cabb6417746b2dd432eda476f55) {#structsi47x__agc__overrride_8arg_1aeaad2cabb6417746b2dd432eda476f55}

#### `public uint8_t `[`DUMMY`](#structsi47x__agc__overrride_8arg_1abd2103035a8021942390a78a431ba0c4) {#structsi47x__agc__overrride_8arg_1abd2103035a8021942390a78a431ba0c4}

#### `public uint8_t `[`AGCIDX`](#structsi47x__agc__overrride_8arg_1a26bd0af30c189325bd004e10593eb5cd) {#structsi47x__agc__overrride_8arg_1a26bd0af30c189325bd004e10593eb5cd}

# struct `si47x_agc_status.refined` {#structsi47x__agc__status_8refined}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`STCINT`](#structsi47x__agc__status_8refined_1af8f21d801809287d578344911b65697c) | 
`public uint8_t `[`DUMMY1`](#structsi47x__agc__status_8refined_1a3651c40ccc4450f2fc89fa3139dedd5a) | 
`public uint8_t `[`RDSINT`](#structsi47x__agc__status_8refined_1a53d648e7e9d100d590e2f65ec7de079f) | 
`public uint8_t `[`RSQINT`](#structsi47x__agc__status_8refined_1a8acf6c55c97050e7abd06c104012c77a) | 
`public uint8_t `[`DUMMY2`](#structsi47x__agc__status_8refined_1abece94c62273dc7ecfabc565b76dbbe5) | 
`public uint8_t `[`ERR`](#structsi47x__agc__status_8refined_1acd22bad976363fdd1bfbf6759fede482) | 
`public uint8_t `[`CTS`](#structsi47x__agc__status_8refined_1ae16433ffd3adc248f0ce2608a95c3c76) | 
`public uint8_t `[`AGCDIS`](#structsi47x__agc__status_8refined_1aeaad2cabb6417746b2dd432eda476f55) | 
`public uint8_t `[`DUMMY`](#structsi47x__agc__status_8refined_1abd2103035a8021942390a78a431ba0c4) | 
`public uint8_t `[`AGCIDX`](#structsi47x__agc__status_8refined_1a26bd0af30c189325bd004e10593eb5cd) | 

## Members

#### `public uint8_t `[`STCINT`](#structsi47x__agc__status_8refined_1af8f21d801809287d578344911b65697c) {#structsi47x__agc__status_8refined_1af8f21d801809287d578344911b65697c}

#### `public uint8_t `[`DUMMY1`](#structsi47x__agc__status_8refined_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__agc__status_8refined_1a3651c40ccc4450f2fc89fa3139dedd5a}

#### `public uint8_t `[`RDSINT`](#structsi47x__agc__status_8refined_1a53d648e7e9d100d590e2f65ec7de079f) {#structsi47x__agc__status_8refined_1a53d648e7e9d100d590e2f65ec7de079f}

#### `public uint8_t `[`RSQINT`](#structsi47x__agc__status_8refined_1a8acf6c55c97050e7abd06c104012c77a) {#structsi47x__agc__status_8refined_1a8acf6c55c97050e7abd06c104012c77a}

#### `public uint8_t `[`DUMMY2`](#structsi47x__agc__status_8refined_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__agc__status_8refined_1abece94c62273dc7ecfabc565b76dbbe5}

#### `public uint8_t `[`ERR`](#structsi47x__agc__status_8refined_1acd22bad976363fdd1bfbf6759fede482) {#structsi47x__agc__status_8refined_1acd22bad976363fdd1bfbf6759fede482}

#### `public uint8_t `[`CTS`](#structsi47x__agc__status_8refined_1ae16433ffd3adc248f0ce2608a95c3c76) {#structsi47x__agc__status_8refined_1ae16433ffd3adc248f0ce2608a95c3c76}

#### `public uint8_t `[`AGCDIS`](#structsi47x__agc__status_8refined_1aeaad2cabb6417746b2dd432eda476f55) {#structsi47x__agc__status_8refined_1aeaad2cabb6417746b2dd432eda476f55}

#### `public uint8_t `[`DUMMY`](#structsi47x__agc__status_8refined_1abd2103035a8021942390a78a431ba0c4) {#structsi47x__agc__status_8refined_1abd2103035a8021942390a78a431ba0c4}

#### `public uint8_t `[`AGCIDX`](#structsi47x__agc__status_8refined_1a26bd0af30c189325bd004e10593eb5cd) {#structsi47x__agc__status_8refined_1a26bd0af30c189325bd004e10593eb5cd}

# struct `si47x_antenna_capacitor.raw` {#structsi47x__antenna__capacitor_8raw}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`ANTCAPL`](#structsi47x__antenna__capacitor_8raw_1a0475bf71e813922e317ef7cb7b3df9fa) | 
`public uint8_t `[`ANTCAPH`](#structsi47x__antenna__capacitor_8raw_1a5f437dbf77cdadcbb215ce5db44edbe6) | Antenna Tuning Capacitor High byte.

## Members

#### `public uint8_t `[`ANTCAPL`](#structsi47x__antenna__capacitor_8raw_1a0475bf71e813922e317ef7cb7b3df9fa) {#structsi47x__antenna__capacitor_8raw_1a0475bf71e813922e317ef7cb7b3df9fa}

#### `public uint8_t `[`ANTCAPH`](#structsi47x__antenna__capacitor_8raw_1a5f437dbf77cdadcbb215ce5db44edbe6) {#structsi47x__antenna__capacitor_8raw_1a5f437dbf77cdadcbb215ce5db44edbe6}

Antenna Tuning Capacitor High byte.

# struct `si47x_bandwidth_config.param` {#structsi47x__bandwidth__config_8param}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`AMCHFLT`](#structsi47x__bandwidth__config_8param_1ad992f11e8896f68a81b13fcd6ae2a3b1) | 
`public uint8_t `[`DUMMY1`](#structsi47x__bandwidth__config_8param_1a3651c40ccc4450f2fc89fa3139dedd5a) | Selects the bandwidth of the AM channel filter.
`public uint8_t `[`AMPLFLT`](#structsi47x__bandwidth__config_8param_1ae6741d3dc93d6502bb837ca874ee4f94) | 
`public uint8_t `[`DUMMY2`](#structsi47x__bandwidth__config_8param_1abece94c62273dc7ecfabc565b76dbbe5) | Enables the AM Power Line Noise Rejection Filter.

## Members

#### `public uint8_t `[`AMCHFLT`](#structsi47x__bandwidth__config_8param_1ad992f11e8896f68a81b13fcd6ae2a3b1) {#structsi47x__bandwidth__config_8param_1ad992f11e8896f68a81b13fcd6ae2a3b1}

#### `public uint8_t `[`DUMMY1`](#structsi47x__bandwidth__config_8param_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__bandwidth__config_8param_1a3651c40ccc4450f2fc89fa3139dedd5a}

Selects the bandwidth of the AM channel filter.

#### `public uint8_t `[`AMPLFLT`](#structsi47x__bandwidth__config_8param_1ae6741d3dc93d6502bb837ca874ee4f94) {#structsi47x__bandwidth__config_8param_1ae6741d3dc93d6502bb837ca874ee4f94}

#### `public uint8_t `[`DUMMY2`](#structsi47x__bandwidth__config_8param_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__bandwidth__config_8param_1abece94c62273dc7ecfabc565b76dbbe5}

Enables the AM Power Line Noise Rejection Filter.

# struct `si47x_firmware_information.resp` {#structsi47x__firmware__information_8resp}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`STCINT`](#structsi47x__firmware__information_8resp_1af8f21d801809287d578344911b65697c) | 
`public uint8_t `[`DUMMY1`](#structsi47x__firmware__information_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) | 
`public uint8_t `[`RDSINT`](#structsi47x__firmware__information_8resp_1a53d648e7e9d100d590e2f65ec7de079f) | 
`public uint8_t `[`RSQINT`](#structsi47x__firmware__information_8resp_1a8acf6c55c97050e7abd06c104012c77a) | 
`public uint8_t `[`DUMMY2`](#structsi47x__firmware__information_8resp_1abece94c62273dc7ecfabc565b76dbbe5) | 
`public uint8_t `[`ERR`](#structsi47x__firmware__information_8resp_1acd22bad976363fdd1bfbf6759fede482) | 
`public uint8_t `[`CTS`](#structsi47x__firmware__information_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) | 
`public uint8_t `[`PN`](#structsi47x__firmware__information_8resp_1a7b6166324dc52f374d908e03602b1daf) | 
`public uint8_t `[`FWMAJOR`](#structsi47x__firmware__information_8resp_1a67c65213847ea2a824306b665bd2851f) | RESP1 - Final 2 digits of Part Number (HEX).
`public uint8_t `[`FWMINOR`](#structsi47x__firmware__information_8resp_1ae0c243088677f14fc8dd9c9508e068ff) | RESP2 - Firmware Major Revision (ASCII).
`public uint8_t `[`PATCHH`](#structsi47x__firmware__information_8resp_1a3ab7055a80e6c432a1116bfb27181811) | RESP3 - Firmware Minor Revision (ASCII).
`public uint8_t `[`PATCHL`](#structsi47x__firmware__information_8resp_1a07cead1237e34b4696dce647443ad10e) | RESP4 - Patch ID High byte (HEX).
`public uint8_t `[`CMPMAJOR`](#structsi47x__firmware__information_8resp_1aab5038bb547e9aa4b543112de6904611) | RESP5 - Patch ID Low byte (HEX).
`public uint8_t `[`CMPMINOR`](#structsi47x__firmware__information_8resp_1a62c98580b8d5220cf351861ddd86fd07) | RESP6 - Component Major Revision (ASCII).
`public uint8_t `[`CHIPREV`](#structsi47x__firmware__information_8resp_1ab9dbe73951d365ea8a945760cec79a5e) | RESP7 - Component Minor Revision (ASCII).

## Members

#### `public uint8_t `[`STCINT`](#structsi47x__firmware__information_8resp_1af8f21d801809287d578344911b65697c) {#structsi47x__firmware__information_8resp_1af8f21d801809287d578344911b65697c}

#### `public uint8_t `[`DUMMY1`](#structsi47x__firmware__information_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__firmware__information_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a}

#### `public uint8_t `[`RDSINT`](#structsi47x__firmware__information_8resp_1a53d648e7e9d100d590e2f65ec7de079f) {#structsi47x__firmware__information_8resp_1a53d648e7e9d100d590e2f65ec7de079f}

#### `public uint8_t `[`RSQINT`](#structsi47x__firmware__information_8resp_1a8acf6c55c97050e7abd06c104012c77a) {#structsi47x__firmware__information_8resp_1a8acf6c55c97050e7abd06c104012c77a}

#### `public uint8_t `[`DUMMY2`](#structsi47x__firmware__information_8resp_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__firmware__information_8resp_1abece94c62273dc7ecfabc565b76dbbe5}

#### `public uint8_t `[`ERR`](#structsi47x__firmware__information_8resp_1acd22bad976363fdd1bfbf6759fede482) {#structsi47x__firmware__information_8resp_1acd22bad976363fdd1bfbf6759fede482}

#### `public uint8_t `[`CTS`](#structsi47x__firmware__information_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) {#structsi47x__firmware__information_8resp_1ae16433ffd3adc248f0ce2608a95c3c76}

#### `public uint8_t `[`PN`](#structsi47x__firmware__information_8resp_1a7b6166324dc52f374d908e03602b1daf) {#structsi47x__firmware__information_8resp_1a7b6166324dc52f374d908e03602b1daf}

#### `public uint8_t `[`FWMAJOR`](#structsi47x__firmware__information_8resp_1a67c65213847ea2a824306b665bd2851f) {#structsi47x__firmware__information_8resp_1a67c65213847ea2a824306b665bd2851f}

RESP1 - Final 2 digits of Part Number (HEX).

#### `public uint8_t `[`FWMINOR`](#structsi47x__firmware__information_8resp_1ae0c243088677f14fc8dd9c9508e068ff) {#structsi47x__firmware__information_8resp_1ae0c243088677f14fc8dd9c9508e068ff}

RESP2 - Firmware Major Revision (ASCII).

#### `public uint8_t `[`PATCHH`](#structsi47x__firmware__information_8resp_1a3ab7055a80e6c432a1116bfb27181811) {#structsi47x__firmware__information_8resp_1a3ab7055a80e6c432a1116bfb27181811}

RESP3 - Firmware Minor Revision (ASCII).

#### `public uint8_t `[`PATCHL`](#structsi47x__firmware__information_8resp_1a07cead1237e34b4696dce647443ad10e) {#structsi47x__firmware__information_8resp_1a07cead1237e34b4696dce647443ad10e}

RESP4 - Patch ID High byte (HEX).

#### `public uint8_t `[`CMPMAJOR`](#structsi47x__firmware__information_8resp_1aab5038bb547e9aa4b543112de6904611) {#structsi47x__firmware__information_8resp_1aab5038bb547e9aa4b543112de6904611}

RESP5 - Patch ID Low byte (HEX).

#### `public uint8_t `[`CMPMINOR`](#structsi47x__firmware__information_8resp_1a62c98580b8d5220cf351861ddd86fd07) {#structsi47x__firmware__information_8resp_1a62c98580b8d5220cf351861ddd86fd07}

RESP6 - Component Major Revision (ASCII).

#### `public uint8_t `[`CHIPREV`](#structsi47x__firmware__information_8resp_1ab9dbe73951d365ea8a945760cec79a5e) {#structsi47x__firmware__information_8resp_1ab9dbe73951d365ea8a945760cec79a5e}

RESP7 - Component Minor Revision (ASCII).

# struct `si47x_firmware_query_library.resp` {#structsi47x__firmware__query__library_8resp}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`STCINT`](#structsi47x__firmware__query__library_8resp_1af8f21d801809287d578344911b65697c) | 
`public uint8_t `[`DUMMY1`](#structsi47x__firmware__query__library_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) | 
`public uint8_t `[`RDSINT`](#structsi47x__firmware__query__library_8resp_1a53d648e7e9d100d590e2f65ec7de079f) | 
`public uint8_t `[`RSQINT`](#structsi47x__firmware__query__library_8resp_1a8acf6c55c97050e7abd06c104012c77a) | 
`public uint8_t `[`DUMMY2`](#structsi47x__firmware__query__library_8resp_1abece94c62273dc7ecfabc565b76dbbe5) | 
`public uint8_t `[`ERR`](#structsi47x__firmware__query__library_8resp_1acd22bad976363fdd1bfbf6759fede482) | 
`public uint8_t `[`CTS`](#structsi47x__firmware__query__library_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) | 
`public uint8_t `[`PN`](#structsi47x__firmware__query__library_8resp_1a7b6166324dc52f374d908e03602b1daf) | 
`public uint8_t `[`FWMAJOR`](#structsi47x__firmware__query__library_8resp_1a67c65213847ea2a824306b665bd2851f) | RESP1 - Final 2 digits of Part Number (HEX).
`public uint8_t `[`FWMINOR`](#structsi47x__firmware__query__library_8resp_1ae0c243088677f14fc8dd9c9508e068ff) | RESP2 - Firmware Major Revision (ASCII).
`public uint8_t `[`RESERVED1`](#structsi47x__firmware__query__library_8resp_1ad8179a090910d4923b044cbce813c8ea) | RESP3 - Firmware Minor Revision (ASCII).
`public uint8_t `[`RESERVED2`](#structsi47x__firmware__query__library_8resp_1a0b5885d29a74762639e4b55c51ce2d85) | RESP4 - Reserved, various values.
`public uint8_t `[`CHIPREV`](#structsi47x__firmware__query__library_8resp_1ab9dbe73951d365ea8a945760cec79a5e) | RESP5 - Reserved, various values.
`public uint8_t `[`LIBRARYID`](#structsi47x__firmware__query__library_8resp_1afb747a51a99db36a17a01f11e3cafbe5) | RESP6 - Chip Revision (ASCII).

## Members

#### `public uint8_t `[`STCINT`](#structsi47x__firmware__query__library_8resp_1af8f21d801809287d578344911b65697c) {#structsi47x__firmware__query__library_8resp_1af8f21d801809287d578344911b65697c}

#### `public uint8_t `[`DUMMY1`](#structsi47x__firmware__query__library_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__firmware__query__library_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a}

#### `public uint8_t `[`RDSINT`](#structsi47x__firmware__query__library_8resp_1a53d648e7e9d100d590e2f65ec7de079f) {#structsi47x__firmware__query__library_8resp_1a53d648e7e9d100d590e2f65ec7de079f}

#### `public uint8_t `[`RSQINT`](#structsi47x__firmware__query__library_8resp_1a8acf6c55c97050e7abd06c104012c77a) {#structsi47x__firmware__query__library_8resp_1a8acf6c55c97050e7abd06c104012c77a}

#### `public uint8_t `[`DUMMY2`](#structsi47x__firmware__query__library_8resp_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__firmware__query__library_8resp_1abece94c62273dc7ecfabc565b76dbbe5}

#### `public uint8_t `[`ERR`](#structsi47x__firmware__query__library_8resp_1acd22bad976363fdd1bfbf6759fede482) {#structsi47x__firmware__query__library_8resp_1acd22bad976363fdd1bfbf6759fede482}

#### `public uint8_t `[`CTS`](#structsi47x__firmware__query__library_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) {#structsi47x__firmware__query__library_8resp_1ae16433ffd3adc248f0ce2608a95c3c76}

#### `public uint8_t `[`PN`](#structsi47x__firmware__query__library_8resp_1a7b6166324dc52f374d908e03602b1daf) {#structsi47x__firmware__query__library_8resp_1a7b6166324dc52f374d908e03602b1daf}

#### `public uint8_t `[`FWMAJOR`](#structsi47x__firmware__query__library_8resp_1a67c65213847ea2a824306b665bd2851f) {#structsi47x__firmware__query__library_8resp_1a67c65213847ea2a824306b665bd2851f}

RESP1 - Final 2 digits of Part Number (HEX).

#### `public uint8_t `[`FWMINOR`](#structsi47x__firmware__query__library_8resp_1ae0c243088677f14fc8dd9c9508e068ff) {#structsi47x__firmware__query__library_8resp_1ae0c243088677f14fc8dd9c9508e068ff}

RESP2 - Firmware Major Revision (ASCII).

#### `public uint8_t `[`RESERVED1`](#structsi47x__firmware__query__library_8resp_1ad8179a090910d4923b044cbce813c8ea) {#structsi47x__firmware__query__library_8resp_1ad8179a090910d4923b044cbce813c8ea}

RESP3 - Firmware Minor Revision (ASCII).

#### `public uint8_t `[`RESERVED2`](#structsi47x__firmware__query__library_8resp_1a0b5885d29a74762639e4b55c51ce2d85) {#structsi47x__firmware__query__library_8resp_1a0b5885d29a74762639e4b55c51ce2d85}

RESP4 - Reserved, various values.

#### `public uint8_t `[`CHIPREV`](#structsi47x__firmware__query__library_8resp_1ab9dbe73951d365ea8a945760cec79a5e) {#structsi47x__firmware__query__library_8resp_1ab9dbe73951d365ea8a945760cec79a5e}

RESP5 - Reserved, various values.

#### `public uint8_t `[`LIBRARYID`](#structsi47x__firmware__query__library_8resp_1afb747a51a99db36a17a01f11e3cafbe5) {#structsi47x__firmware__query__library_8resp_1afb747a51a99db36a17a01f11e3cafbe5}

RESP6 - Chip Revision (ASCII).

# struct `si47x_frequency.raw` {#structsi47x__frequency_8raw}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`FREQL`](#structsi47x__frequency_8raw_1a858dfb4be81dfbd057aa63aded06bf3d) | 
`public uint8_t `[`FREQH`](#structsi47x__frequency_8raw_1a8836c6ec927c24305d2fd26a5aec2e2b) | Tune Frequency High byte.

## Members

#### `public uint8_t `[`FREQL`](#structsi47x__frequency_8raw_1a858dfb4be81dfbd057aa63aded06bf3d) {#structsi47x__frequency_8raw_1a858dfb4be81dfbd057aa63aded06bf3d}

#### `public uint8_t `[`FREQH`](#structsi47x__frequency_8raw_1a8836c6ec927c24305d2fd26a5aec2e2b) {#structsi47x__frequency_8raw_1a8836c6ec927c24305d2fd26a5aec2e2b}

Tune Frequency High byte.

# struct `si47x_property.raw` {#structsi47x__property_8raw}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`byteLow`](#structsi47x__property_8raw_1a1f3859652867ab4a967c5caa3e2ab353) | 
`public uint8_t `[`byteHigh`](#structsi47x__property_8raw_1a7b280732d6c370d505d537b0ab9f7e30) | 

## Members

#### `public uint8_t `[`byteLow`](#structsi47x__property_8raw_1a1f3859652867ab4a967c5caa3e2ab353) {#structsi47x__property_8raw_1a1f3859652867ab4a967c5caa3e2ab353}

#### `public uint8_t `[`byteHigh`](#structsi47x__property_8raw_1a7b280732d6c370d505d537b0ab9f7e30) {#structsi47x__property_8raw_1a7b280732d6c370d505d537b0ab9f7e30}

# struct `si47x_rds_blocka.raw` {#structsi47x__rds__blocka_8raw}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`highValue`](#structsi47x__rds__blocka_8raw_1aaee469db16fdff1aa5439b776b3d5860) | 
`public uint8_t `[`lowValue`](#structsi47x__rds__blocka_8raw_1a72a22270b4e47acf046fad64a8720e4f) | 

## Members

#### `public uint8_t `[`highValue`](#structsi47x__rds__blocka_8raw_1aaee469db16fdff1aa5439b776b3d5860) {#structsi47x__rds__blocka_8raw_1aaee469db16fdff1aa5439b776b3d5860}

#### `public uint8_t `[`lowValue`](#structsi47x__rds__blocka_8raw_1a72a22270b4e47acf046fad64a8720e4f) {#structsi47x__rds__blocka_8raw_1a72a22270b4e47acf046fad64a8720e4f}

# struct `si47x_rds_blocka.refined` {#structsi47x__rds__blocka_8refined}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint16_t `[`pi`](#structsi47x__rds__blocka_8refined_1a72ab8af56bddab33b269c5964b26620a) | 

## Members

#### `public uint16_t `[`pi`](#structsi47x__rds__blocka_8refined_1a72ab8af56bddab33b269c5964b26620a) {#structsi47x__rds__blocka_8refined_1a72ab8af56bddab33b269c5964b26620a}

# struct `si47x_rds_blockb.group0` {#structsi47x__rds__blockb_8group0}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint16_t `[`address`](#structsi47x__rds__blockb_8group0_1a884d9804999fc47a3c2694e49ad2536a) | 
`public uint16_t `[`DI`](#structsi47x__rds__blockb_8group0_1aa1b1c0cd777edd2e5cfcaf8e2e7dcab8) | 
`public uint16_t `[`MS`](#structsi47x__rds__blockb_8group0_1a7a663caea1b722a63dc2868158ed584d) | 
`public uint16_t `[`TA`](#structsi47x__rds__blockb_8group0_1a890a10788493e3d572586e991cd43543) | 
`public uint16_t `[`programType`](#structsi47x__rds__blockb_8group0_1a264bd2c2ca8c895803767b0d39ff4a09) | 
`public uint16_t `[`trafficProgramCode`](#structsi47x__rds__blockb_8group0_1a59e69d63ce38754ea53c4461b5cba1e2) | 
`public uint16_t `[`versionCode`](#structsi47x__rds__blockb_8group0_1a20583dcf173525a78f726ef45329c5ae) | 
`public uint16_t `[`groupType`](#structsi47x__rds__blockb_8group0_1a19223bd3731a4215ead3ba6a1eb8bbe8) | 

## Members

#### `public uint16_t `[`address`](#structsi47x__rds__blockb_8group0_1a884d9804999fc47a3c2694e49ad2536a) {#structsi47x__rds__blockb_8group0_1a884d9804999fc47a3c2694e49ad2536a}

#### `public uint16_t `[`DI`](#structsi47x__rds__blockb_8group0_1aa1b1c0cd777edd2e5cfcaf8e2e7dcab8) {#structsi47x__rds__blockb_8group0_1aa1b1c0cd777edd2e5cfcaf8e2e7dcab8}

#### `public uint16_t `[`MS`](#structsi47x__rds__blockb_8group0_1a7a663caea1b722a63dc2868158ed584d) {#structsi47x__rds__blockb_8group0_1a7a663caea1b722a63dc2868158ed584d}

#### `public uint16_t `[`TA`](#structsi47x__rds__blockb_8group0_1a890a10788493e3d572586e991cd43543) {#structsi47x__rds__blockb_8group0_1a890a10788493e3d572586e991cd43543}

#### `public uint16_t `[`programType`](#structsi47x__rds__blockb_8group0_1a264bd2c2ca8c895803767b0d39ff4a09) {#structsi47x__rds__blockb_8group0_1a264bd2c2ca8c895803767b0d39ff4a09}

#### `public uint16_t `[`trafficProgramCode`](#structsi47x__rds__blockb_8group0_1a59e69d63ce38754ea53c4461b5cba1e2) {#structsi47x__rds__blockb_8group0_1a59e69d63ce38754ea53c4461b5cba1e2}

#### `public uint16_t `[`versionCode`](#structsi47x__rds__blockb_8group0_1a20583dcf173525a78f726ef45329c5ae) {#structsi47x__rds__blockb_8group0_1a20583dcf173525a78f726ef45329c5ae}

#### `public uint16_t `[`groupType`](#structsi47x__rds__blockb_8group0_1a19223bd3731a4215ead3ba6a1eb8bbe8) {#structsi47x__rds__blockb_8group0_1a19223bd3731a4215ead3ba6a1eb8bbe8}

# struct `si47x_rds_blockb.group2` {#structsi47x__rds__blockb_8group2}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint16_t `[`address`](#structsi47x__rds__blockb_8group2_1a884d9804999fc47a3c2694e49ad2536a) | 
`public uint16_t `[`textABFlag`](#structsi47x__rds__blockb_8group2_1a652bfdc159637b708ac6e6f92d7650bc) | 
`public uint16_t `[`programType`](#structsi47x__rds__blockb_8group2_1a264bd2c2ca8c895803767b0d39ff4a09) | 
`public uint16_t `[`trafficProgramCode`](#structsi47x__rds__blockb_8group2_1a59e69d63ce38754ea53c4461b5cba1e2) | 
`public uint16_t `[`versionCode`](#structsi47x__rds__blockb_8group2_1a20583dcf173525a78f726ef45329c5ae) | 
`public uint16_t `[`groupType`](#structsi47x__rds__blockb_8group2_1a19223bd3731a4215ead3ba6a1eb8bbe8) | 

## Members

#### `public uint16_t `[`address`](#structsi47x__rds__blockb_8group2_1a884d9804999fc47a3c2694e49ad2536a) {#structsi47x__rds__blockb_8group2_1a884d9804999fc47a3c2694e49ad2536a}

#### `public uint16_t `[`textABFlag`](#structsi47x__rds__blockb_8group2_1a652bfdc159637b708ac6e6f92d7650bc) {#structsi47x__rds__blockb_8group2_1a652bfdc159637b708ac6e6f92d7650bc}

#### `public uint16_t `[`programType`](#structsi47x__rds__blockb_8group2_1a264bd2c2ca8c895803767b0d39ff4a09) {#structsi47x__rds__blockb_8group2_1a264bd2c2ca8c895803767b0d39ff4a09}

#### `public uint16_t `[`trafficProgramCode`](#structsi47x__rds__blockb_8group2_1a59e69d63ce38754ea53c4461b5cba1e2) {#structsi47x__rds__blockb_8group2_1a59e69d63ce38754ea53c4461b5cba1e2}

#### `public uint16_t `[`versionCode`](#structsi47x__rds__blockb_8group2_1a20583dcf173525a78f726ef45329c5ae) {#structsi47x__rds__blockb_8group2_1a20583dcf173525a78f726ef45329c5ae}

#### `public uint16_t `[`groupType`](#structsi47x__rds__blockb_8group2_1a19223bd3731a4215ead3ba6a1eb8bbe8) {#structsi47x__rds__blockb_8group2_1a19223bd3731a4215ead3ba6a1eb8bbe8}

# struct `si47x_rds_blockb.raw` {#structsi47x__rds__blockb_8raw}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`lowValue`](#structsi47x__rds__blockb_8raw_1a72a22270b4e47acf046fad64a8720e4f) | 
`public uint8_t `[`highValue`](#structsi47x__rds__blockb_8raw_1aaee469db16fdff1aa5439b776b3d5860) | 

## Members

#### `public uint8_t `[`lowValue`](#structsi47x__rds__blockb_8raw_1a72a22270b4e47acf046fad64a8720e4f) {#structsi47x__rds__blockb_8raw_1a72a22270b4e47acf046fad64a8720e4f}

#### `public uint8_t `[`highValue`](#structsi47x__rds__blockb_8raw_1aaee469db16fdff1aa5439b776b3d5860) {#structsi47x__rds__blockb_8raw_1aaee469db16fdff1aa5439b776b3d5860}

# struct `si47x_rds_blockb.refined` {#structsi47x__rds__blockb_8refined}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint16_t `[`content`](#structsi47x__rds__blockb_8refined_1a9a0364b9e99bb480dd25e1f0284c8555) | 
`public uint16_t `[`textABFlag`](#structsi47x__rds__blockb_8refined_1a652bfdc159637b708ac6e6f92d7650bc) | 
`public uint16_t `[`programType`](#structsi47x__rds__blockb_8refined_1a264bd2c2ca8c895803767b0d39ff4a09) | 
`public uint16_t `[`trafficProgramCode`](#structsi47x__rds__blockb_8refined_1a59e69d63ce38754ea53c4461b5cba1e2) | 
`public uint16_t `[`versionCode`](#structsi47x__rds__blockb_8refined_1a20583dcf173525a78f726ef45329c5ae) | 
`public uint16_t `[`groupType`](#structsi47x__rds__blockb_8refined_1a19223bd3731a4215ead3ba6a1eb8bbe8) | 

## Members

#### `public uint16_t `[`content`](#structsi47x__rds__blockb_8refined_1a9a0364b9e99bb480dd25e1f0284c8555) {#structsi47x__rds__blockb_8refined_1a9a0364b9e99bb480dd25e1f0284c8555}

#### `public uint16_t `[`textABFlag`](#structsi47x__rds__blockb_8refined_1a652bfdc159637b708ac6e6f92d7650bc) {#structsi47x__rds__blockb_8refined_1a652bfdc159637b708ac6e6f92d7650bc}

#### `public uint16_t `[`programType`](#structsi47x__rds__blockb_8refined_1a264bd2c2ca8c895803767b0d39ff4a09) {#structsi47x__rds__blockb_8refined_1a264bd2c2ca8c895803767b0d39ff4a09}

#### `public uint16_t `[`trafficProgramCode`](#structsi47x__rds__blockb_8refined_1a59e69d63ce38754ea53c4461b5cba1e2) {#structsi47x__rds__blockb_8refined_1a59e69d63ce38754ea53c4461b5cba1e2}

#### `public uint16_t `[`versionCode`](#structsi47x__rds__blockb_8refined_1a20583dcf173525a78f726ef45329c5ae) {#structsi47x__rds__blockb_8refined_1a20583dcf173525a78f726ef45329c5ae}

#### `public uint16_t `[`groupType`](#structsi47x__rds__blockb_8refined_1a19223bd3731a4215ead3ba6a1eb8bbe8) {#structsi47x__rds__blockb_8refined_1a19223bd3731a4215ead3ba6a1eb8bbe8}

# struct `si47x_rds_command.arg` {#structsi47x__rds__command_8arg}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`INTACK`](#structsi47x__rds__command_8arg_1aa1b4c5526f2abe4a5d02be4016a44020) | 
`public uint8_t `[`MTFIFO`](#structsi47x__rds__command_8arg_1a89ada8287a1cbe8c1cbcbaf629797e69) | 
`public uint8_t `[`STATUSONLY`](#structsi47x__rds__command_8arg_1a0ad827114d7e2367674a05bbcdc53eda) | 
`public uint8_t `[`dummy`](#structsi47x__rds__command_8arg_1a275876e34cf609db118f3d84b799a790) | 

## Members

#### `public uint8_t `[`INTACK`](#structsi47x__rds__command_8arg_1aa1b4c5526f2abe4a5d02be4016a44020) {#structsi47x__rds__command_8arg_1aa1b4c5526f2abe4a5d02be4016a44020}

#### `public uint8_t `[`MTFIFO`](#structsi47x__rds__command_8arg_1a89ada8287a1cbe8c1cbcbaf629797e69) {#structsi47x__rds__command_8arg_1a89ada8287a1cbe8c1cbcbaf629797e69}

#### `public uint8_t `[`STATUSONLY`](#structsi47x__rds__command_8arg_1a0ad827114d7e2367674a05bbcdc53eda) {#structsi47x__rds__command_8arg_1a0ad827114d7e2367674a05bbcdc53eda}

#### `public uint8_t `[`dummy`](#structsi47x__rds__command_8arg_1a275876e34cf609db118f3d84b799a790) {#structsi47x__rds__command_8arg_1a275876e34cf609db118f3d84b799a790}

# struct `si47x_rds_config.arg` {#structsi47x__rds__config_8arg}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`RDSEN`](#structsi47x__rds__config_8arg_1af06feb780d38d7dc59d42878a3353c7f) | 
`public uint8_t `[`DUMMY1`](#structsi47x__rds__config_8arg_1a3651c40ccc4450f2fc89fa3139dedd5a) | 1 = RDS Processing Enable.
`public uint8_t `[`BLETHD`](#structsi47x__rds__config_8arg_1a08e321d989e5154ff4e8a2b5d0fbb057) | 
`public uint8_t `[`BLETHC`](#structsi47x__rds__config_8arg_1a1246c30829bddc4264fe51b0abc00dbe) | Block Error Threshold BLOCKD.
`public uint8_t `[`BLETHB`](#structsi47x__rds__config_8arg_1a659adbc2d25b37801235e309fb25737f) | Block Error Threshold BLOCKC.
`public uint8_t `[`BLETHA`](#structsi47x__rds__config_8arg_1a955ffa2ef1f258071376294e01482059) | Block Error Threshold BLOCKB.

## Members

#### `public uint8_t `[`RDSEN`](#structsi47x__rds__config_8arg_1af06feb780d38d7dc59d42878a3353c7f) {#structsi47x__rds__config_8arg_1af06feb780d38d7dc59d42878a3353c7f}

#### `public uint8_t `[`DUMMY1`](#structsi47x__rds__config_8arg_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__rds__config_8arg_1a3651c40ccc4450f2fc89fa3139dedd5a}

1 = RDS Processing Enable.

#### `public uint8_t `[`BLETHD`](#structsi47x__rds__config_8arg_1a08e321d989e5154ff4e8a2b5d0fbb057) {#structsi47x__rds__config_8arg_1a08e321d989e5154ff4e8a2b5d0fbb057}

#### `public uint8_t `[`BLETHC`](#structsi47x__rds__config_8arg_1a1246c30829bddc4264fe51b0abc00dbe) {#structsi47x__rds__config_8arg_1a1246c30829bddc4264fe51b0abc00dbe}

Block Error Threshold BLOCKD.

#### `public uint8_t `[`BLETHB`](#structsi47x__rds__config_8arg_1a659adbc2d25b37801235e309fb25737f) {#structsi47x__rds__config_8arg_1a659adbc2d25b37801235e309fb25737f}

Block Error Threshold BLOCKC.

#### `public uint8_t `[`BLETHA`](#structsi47x__rds__config_8arg_1a955ffa2ef1f258071376294e01482059) {#structsi47x__rds__config_8arg_1a955ffa2ef1f258071376294e01482059}

Block Error Threshold BLOCKB.

# struct `si47x_rds_date_time.refined` {#structsi47x__rds__date__time_8refined}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`offset`](#structsi47x__rds__date__time_8refined_1a7a86c157ee9713c34fbd7a1ee40f0c5a) | 
`public uint8_t `[`offset_sense`](#structsi47x__rds__date__time_8refined_1a5ba6e404c489ff4f757e7c77cb9fa310) | 
`public uint8_t `[`minute1`](#structsi47x__rds__date__time_8refined_1a756bdae430708e155654a844c2bcc33b) | 
`public uint8_t `[`minute2`](#structsi47x__rds__date__time_8refined_1a57b42af48e9b3407c002d157d89f50ad) | 
`public uint8_t `[`hour1`](#structsi47x__rds__date__time_8refined_1a0351524e66b386b6638dd47c0d00de65) | 
`public uint8_t `[`hour2`](#structsi47x__rds__date__time_8refined_1a38034b153ffba397eebfcd07b4a50414) | 
`public uint32_t `[`mjd`](#structsi47x__rds__date__time_8refined_1a7f7685b39180278dd0fa69f523c7bd24) | 

## Members

#### `public uint8_t `[`offset`](#structsi47x__rds__date__time_8refined_1a7a86c157ee9713c34fbd7a1ee40f0c5a) {#structsi47x__rds__date__time_8refined_1a7a86c157ee9713c34fbd7a1ee40f0c5a}

#### `public uint8_t `[`offset_sense`](#structsi47x__rds__date__time_8refined_1a5ba6e404c489ff4f757e7c77cb9fa310) {#structsi47x__rds__date__time_8refined_1a5ba6e404c489ff4f757e7c77cb9fa310}

#### `public uint8_t `[`minute1`](#structsi47x__rds__date__time_8refined_1a756bdae430708e155654a844c2bcc33b) {#structsi47x__rds__date__time_8refined_1a756bdae430708e155654a844c2bcc33b}

#### `public uint8_t `[`minute2`](#structsi47x__rds__date__time_8refined_1a57b42af48e9b3407c002d157d89f50ad) {#structsi47x__rds__date__time_8refined_1a57b42af48e9b3407c002d157d89f50ad}

#### `public uint8_t `[`hour1`](#structsi47x__rds__date__time_8refined_1a0351524e66b386b6638dd47c0d00de65) {#structsi47x__rds__date__time_8refined_1a0351524e66b386b6638dd47c0d00de65}

#### `public uint8_t `[`hour2`](#structsi47x__rds__date__time_8refined_1a38034b153ffba397eebfcd07b4a50414) {#structsi47x__rds__date__time_8refined_1a38034b153ffba397eebfcd07b4a50414}

#### `public uint32_t `[`mjd`](#structsi47x__rds__date__time_8refined_1a7f7685b39180278dd0fa69f523c7bd24) {#structsi47x__rds__date__time_8refined_1a7f7685b39180278dd0fa69f523c7bd24}

# struct `si47x_rds_int_source.refined` {#structsi47x__rds__int__source_8refined}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`RDSRECV`](#structsi47x__rds__int__source_8refined_1aa75f4ec874d6a5f181b8e423950bfde0) | 
`public uint8_t `[`RDSSYNCLOST`](#structsi47x__rds__int__source_8refined_1a95a3b24fca811ec5f3f6572fe7da0bcf) | If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.
`public uint8_t `[`RDSSYNCFOUND`](#structsi47x__rds__int__source_8refined_1a1cd218d6a523d2d9120d8ab8a9dd93c7) | If set, generate RDSINT when RDS loses synchronization.
`public uint8_t `[`DUMMY1`](#structsi47x__rds__int__source_8refined_1a3651c40ccc4450f2fc89fa3139dedd5a) | f set, generate RDSINT when RDS gains synchronization.
`public uint8_t `[`RDSNEWBLOCKA`](#structsi47x__rds__int__source_8refined_1aa2d4c2b673c2842773854cdd6f28e033) | Always write to 0.
`public uint8_t `[`RDSNEWBLOCKB`](#structsi47x__rds__int__source_8refined_1a67434ed313bef4d6221df6a51971c994) | If set, generate an interrupt when Block A data is found or subsequently changed.
`public uint8_t `[`DUMMY2`](#structsi47x__rds__int__source_8refined_1abece94c62273dc7ecfabc565b76dbbe5) | If set, generate an interrupt when Block B data is found or subsequently changed.
`public uint8_t `[`DUMMY3`](#structsi47x__rds__int__source_8refined_1a613aa5c14b3ccc6304a519292c482ed1) | Reserved - Always write to 0.

## Members

#### `public uint8_t `[`RDSRECV`](#structsi47x__rds__int__source_8refined_1aa75f4ec874d6a5f181b8e423950bfde0) {#structsi47x__rds__int__source_8refined_1aa75f4ec874d6a5f181b8e423950bfde0}

#### `public uint8_t `[`RDSSYNCLOST`](#structsi47x__rds__int__source_8refined_1a95a3b24fca811ec5f3f6572fe7da0bcf) {#structsi47x__rds__int__source_8refined_1a95a3b24fca811ec5f3f6572fe7da0bcf}

If set, generate RDSINT when RDS FIFO has at least FM_RDS_INT_FIFO_COUNT entries.

#### `public uint8_t `[`RDSSYNCFOUND`](#structsi47x__rds__int__source_8refined_1a1cd218d6a523d2d9120d8ab8a9dd93c7) {#structsi47x__rds__int__source_8refined_1a1cd218d6a523d2d9120d8ab8a9dd93c7}

If set, generate RDSINT when RDS loses synchronization.

#### `public uint8_t `[`DUMMY1`](#structsi47x__rds__int__source_8refined_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__rds__int__source_8refined_1a3651c40ccc4450f2fc89fa3139dedd5a}

f set, generate RDSINT when RDS gains synchronization.

#### `public uint8_t `[`RDSNEWBLOCKA`](#structsi47x__rds__int__source_8refined_1aa2d4c2b673c2842773854cdd6f28e033) {#structsi47x__rds__int__source_8refined_1aa2d4c2b673c2842773854cdd6f28e033}

Always write to 0.

#### `public uint8_t `[`RDSNEWBLOCKB`](#structsi47x__rds__int__source_8refined_1a67434ed313bef4d6221df6a51971c994) {#structsi47x__rds__int__source_8refined_1a67434ed313bef4d6221df6a51971c994}

If set, generate an interrupt when Block A data is found or subsequently changed.

#### `public uint8_t `[`DUMMY2`](#structsi47x__rds__int__source_8refined_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__rds__int__source_8refined_1abece94c62273dc7ecfabc565b76dbbe5}

If set, generate an interrupt when Block B data is found or subsequently changed.

#### `public uint8_t `[`DUMMY3`](#structsi47x__rds__int__source_8refined_1a613aa5c14b3ccc6304a519292c482ed1) {#structsi47x__rds__int__source_8refined_1a613aa5c14b3ccc6304a519292c482ed1}

Reserved - Always write to 0.

# struct `si47x_rds_status.resp` {#structsi47x__rds__status_8resp}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`STCINT`](#structsi47x__rds__status_8resp_1af8f21d801809287d578344911b65697c) | 
`public uint8_t `[`DUMMY1`](#structsi47x__rds__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) | 
`public uint8_t `[`RDSINT`](#structsi47x__rds__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f) | 
`public uint8_t `[`RSQINT`](#structsi47x__rds__status_8resp_1a8acf6c55c97050e7abd06c104012c77a) | 
`public uint8_t `[`DUMMY2`](#structsi47x__rds__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5) | 
`public uint8_t `[`ERR`](#structsi47x__rds__status_8resp_1acd22bad976363fdd1bfbf6759fede482) | 
`public uint8_t `[`CTS`](#structsi47x__rds__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) | 
`public uint8_t `[`RDSRECV`](#structsi47x__rds__status_8resp_1aa75f4ec874d6a5f181b8e423950bfde0) | 
`public uint8_t `[`RDSSYNCLOST`](#structsi47x__rds__status_8resp_1a95a3b24fca811ec5f3f6572fe7da0bcf) | RDS Received; 1 = FIFO filled to minimum number of groups set by RDSFIFOCNT.
`public uint8_t `[`RDSSYNCFOUND`](#structsi47x__rds__status_8resp_1a1cd218d6a523d2d9120d8ab8a9dd93c7) | RDS Sync Lost; 1 = Lost RDS synchronization.
`public uint8_t `[`DUMMY3`](#structsi47x__rds__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1) | RDS Sync Found; 1 = Found RDS synchronization.
`public uint8_t `[`RDSNEWBLOCKA`](#structsi47x__rds__status_8resp_1aa2d4c2b673c2842773854cdd6f28e033) | 
`public uint8_t `[`RDSNEWBLOCKB`](#structsi47x__rds__status_8resp_1a67434ed313bef4d6221df6a51971c994) | RDS New Block A; 1 = Valid Block A data has been received.
`public uint8_t `[`DUMMY4`](#structsi47x__rds__status_8resp_1a9182569d693623e6b469099542b303e4) | RDS New Block B; 1 = Valid Block B data has been received.
`public uint8_t `[`RDSSYNC`](#structsi47x__rds__status_8resp_1a67a91dac9065974c1b69fea4ad395faf) | 
`public uint8_t `[`DUMMY5`](#structsi47x__rds__status_8resp_1a1fc4251a40ec4352f10fc3ecb20f2c79) | RDS Sync; 1 = RDS currently synchronized.
`public uint8_t `[`GRPLOST`](#structsi47x__rds__status_8resp_1a97eaf5b8af040d5cc1d1951e45846c72) | 
`public uint8_t `[`DUMMY6`](#structsi47x__rds__status_8resp_1ae47e340f9899842ec70c03024de8cf89) | Group Lost; 1 = One or more RDS groups discarded due to FIFO overrun.
`public uint8_t `[`RDSFIFOUSED`](#structsi47x__rds__status_8resp_1a4a3b791b8fe2bf1f76df87f754967f5e) | 
`public uint8_t `[`BLOCKAH`](#structsi47x__rds__status_8resp_1a2843da4b952c7be9fb480c58a29ea378) | RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).
`public uint8_t `[`BLOCKAL`](#structsi47x__rds__status_8resp_1a0f0906669ad239fff687df038925d138) | RESP4 - RDS Block A; HIGH byte.
`public uint8_t `[`BLOCKBH`](#structsi47x__rds__status_8resp_1a41992f4f83625a17ffbb4df533f4d192) | RESP5 - RDS Block A; LOW byte.
`public uint8_t `[`BLOCKBL`](#structsi47x__rds__status_8resp_1a5393be02dfd7f56d0567e5f1406fffed) | RESP6 - RDS Block B; HIGH byte.
`public uint8_t `[`BLOCKCH`](#structsi47x__rds__status_8resp_1ae464b56d44650f86be59aeef27abfbbe) | RESP7 - RDS Block B; LOW byte.
`public uint8_t `[`BLOCKCL`](#structsi47x__rds__status_8resp_1a8c78c7dd8484c36b843457e3e4f165ac) | RESP8 - RDS Block C; HIGH byte.
`public uint8_t `[`BLOCKDH`](#structsi47x__rds__status_8resp_1ab46176005c9a362139e953d3633ed8f8) | RESP9 - RDS Block C; LOW byte.
`public uint8_t `[`BLOCKDL`](#structsi47x__rds__status_8resp_1ae0fd3095e63eb37b945a03706e7fa697) | RESP10 - RDS Block D; HIGH byte.
`public uint8_t `[`BLED`](#structsi47x__rds__status_8resp_1a7d449b80d36bbc910ef88b6fcfe90364) | RESP11 - RDS Block D; LOW byte.
`public uint8_t `[`BLEC`](#structsi47x__rds__status_8resp_1a8e74ab448e91f67eaabee7a52ab05fec) | 
`public uint8_t `[`BLEB`](#structsi47x__rds__status_8resp_1aa4eb947cc07974dfc44c1da340b21c0f) | 
`public uint8_t `[`BLEA`](#structsi47x__rds__status_8resp_1abe92870ea28d50ecdfd8f987836e103f) | 

## Members

#### `public uint8_t `[`STCINT`](#structsi47x__rds__status_8resp_1af8f21d801809287d578344911b65697c) {#structsi47x__rds__status_8resp_1af8f21d801809287d578344911b65697c}

#### `public uint8_t `[`DUMMY1`](#structsi47x__rds__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__rds__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a}

#### `public uint8_t `[`RDSINT`](#structsi47x__rds__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f) {#structsi47x__rds__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f}

#### `public uint8_t `[`RSQINT`](#structsi47x__rds__status_8resp_1a8acf6c55c97050e7abd06c104012c77a) {#structsi47x__rds__status_8resp_1a8acf6c55c97050e7abd06c104012c77a}

#### `public uint8_t `[`DUMMY2`](#structsi47x__rds__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__rds__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5}

#### `public uint8_t `[`ERR`](#structsi47x__rds__status_8resp_1acd22bad976363fdd1bfbf6759fede482) {#structsi47x__rds__status_8resp_1acd22bad976363fdd1bfbf6759fede482}

#### `public uint8_t `[`CTS`](#structsi47x__rds__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) {#structsi47x__rds__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76}

#### `public uint8_t `[`RDSRECV`](#structsi47x__rds__status_8resp_1aa75f4ec874d6a5f181b8e423950bfde0) {#structsi47x__rds__status_8resp_1aa75f4ec874d6a5f181b8e423950bfde0}

#### `public uint8_t `[`RDSSYNCLOST`](#structsi47x__rds__status_8resp_1a95a3b24fca811ec5f3f6572fe7da0bcf) {#structsi47x__rds__status_8resp_1a95a3b24fca811ec5f3f6572fe7da0bcf}

RDS Received; 1 = FIFO filled to minimum number of groups set by RDSFIFOCNT.

#### `public uint8_t `[`RDSSYNCFOUND`](#structsi47x__rds__status_8resp_1a1cd218d6a523d2d9120d8ab8a9dd93c7) {#structsi47x__rds__status_8resp_1a1cd218d6a523d2d9120d8ab8a9dd93c7}

RDS Sync Lost; 1 = Lost RDS synchronization.

#### `public uint8_t `[`DUMMY3`](#structsi47x__rds__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1) {#structsi47x__rds__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1}

RDS Sync Found; 1 = Found RDS synchronization.

#### `public uint8_t `[`RDSNEWBLOCKA`](#structsi47x__rds__status_8resp_1aa2d4c2b673c2842773854cdd6f28e033) {#structsi47x__rds__status_8resp_1aa2d4c2b673c2842773854cdd6f28e033}

#### `public uint8_t `[`RDSNEWBLOCKB`](#structsi47x__rds__status_8resp_1a67434ed313bef4d6221df6a51971c994) {#structsi47x__rds__status_8resp_1a67434ed313bef4d6221df6a51971c994}

RDS New Block A; 1 = Valid Block A data has been received.

#### `public uint8_t `[`DUMMY4`](#structsi47x__rds__status_8resp_1a9182569d693623e6b469099542b303e4) {#structsi47x__rds__status_8resp_1a9182569d693623e6b469099542b303e4}

RDS New Block B; 1 = Valid Block B data has been received.

#### `public uint8_t `[`RDSSYNC`](#structsi47x__rds__status_8resp_1a67a91dac9065974c1b69fea4ad395faf) {#structsi47x__rds__status_8resp_1a67a91dac9065974c1b69fea4ad395faf}

#### `public uint8_t `[`DUMMY5`](#structsi47x__rds__status_8resp_1a1fc4251a40ec4352f10fc3ecb20f2c79) {#structsi47x__rds__status_8resp_1a1fc4251a40ec4352f10fc3ecb20f2c79}

RDS Sync; 1 = RDS currently synchronized.

#### `public uint8_t `[`GRPLOST`](#structsi47x__rds__status_8resp_1a97eaf5b8af040d5cc1d1951e45846c72) {#structsi47x__rds__status_8resp_1a97eaf5b8af040d5cc1d1951e45846c72}

#### `public uint8_t `[`DUMMY6`](#structsi47x__rds__status_8resp_1ae47e340f9899842ec70c03024de8cf89) {#structsi47x__rds__status_8resp_1ae47e340f9899842ec70c03024de8cf89}

Group Lost; 1 = One or more RDS groups discarded due to FIFO overrun.

#### `public uint8_t `[`RDSFIFOUSED`](#structsi47x__rds__status_8resp_1a4a3b791b8fe2bf1f76df87f754967f5e) {#structsi47x__rds__status_8resp_1a4a3b791b8fe2bf1f76df87f754967f5e}

#### `public uint8_t `[`BLOCKAH`](#structsi47x__rds__status_8resp_1a2843da4b952c7be9fb480c58a29ea378) {#structsi47x__rds__status_8resp_1a2843da4b952c7be9fb480c58a29ea378}

RESP3 - RDS FIFO Used; Number of groups remaining in the RDS FIFO (0 if empty).

#### `public uint8_t `[`BLOCKAL`](#structsi47x__rds__status_8resp_1a0f0906669ad239fff687df038925d138) {#structsi47x__rds__status_8resp_1a0f0906669ad239fff687df038925d138}

RESP4 - RDS Block A; HIGH byte.

#### `public uint8_t `[`BLOCKBH`](#structsi47x__rds__status_8resp_1a41992f4f83625a17ffbb4df533f4d192) {#structsi47x__rds__status_8resp_1a41992f4f83625a17ffbb4df533f4d192}

RESP5 - RDS Block A; LOW byte.

#### `public uint8_t `[`BLOCKBL`](#structsi47x__rds__status_8resp_1a5393be02dfd7f56d0567e5f1406fffed) {#structsi47x__rds__status_8resp_1a5393be02dfd7f56d0567e5f1406fffed}

RESP6 - RDS Block B; HIGH byte.

#### `public uint8_t `[`BLOCKCH`](#structsi47x__rds__status_8resp_1ae464b56d44650f86be59aeef27abfbbe) {#structsi47x__rds__status_8resp_1ae464b56d44650f86be59aeef27abfbbe}

RESP7 - RDS Block B; LOW byte.

#### `public uint8_t `[`BLOCKCL`](#structsi47x__rds__status_8resp_1a8c78c7dd8484c36b843457e3e4f165ac) {#structsi47x__rds__status_8resp_1a8c78c7dd8484c36b843457e3e4f165ac}

RESP8 - RDS Block C; HIGH byte.

#### `public uint8_t `[`BLOCKDH`](#structsi47x__rds__status_8resp_1ab46176005c9a362139e953d3633ed8f8) {#structsi47x__rds__status_8resp_1ab46176005c9a362139e953d3633ed8f8}

RESP9 - RDS Block C; LOW byte.

#### `public uint8_t `[`BLOCKDL`](#structsi47x__rds__status_8resp_1ae0fd3095e63eb37b945a03706e7fa697) {#structsi47x__rds__status_8resp_1ae0fd3095e63eb37b945a03706e7fa697}

RESP10 - RDS Block D; HIGH byte.

#### `public uint8_t `[`BLED`](#structsi47x__rds__status_8resp_1a7d449b80d36bbc910ef88b6fcfe90364) {#structsi47x__rds__status_8resp_1a7d449b80d36bbc910ef88b6fcfe90364}

RESP11 - RDS Block D; LOW byte.

#### `public uint8_t `[`BLEC`](#structsi47x__rds__status_8resp_1a8e74ab448e91f67eaabee7a52ab05fec) {#structsi47x__rds__status_8resp_1a8e74ab448e91f67eaabee7a52ab05fec}

#### `public uint8_t `[`BLEB`](#structsi47x__rds__status_8resp_1aa4eb947cc07974dfc44c1da340b21c0f) {#structsi47x__rds__status_8resp_1aa4eb947cc07974dfc44c1da340b21c0f}

#### `public uint8_t `[`BLEA`](#structsi47x__rds__status_8resp_1abe92870ea28d50ecdfd8f987836e103f) {#structsi47x__rds__status_8resp_1abe92870ea28d50ecdfd8f987836e103f}

# struct `si47x_response_status.resp` {#structsi47x__response__status_8resp}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`STCINT`](#structsi47x__response__status_8resp_1af8f21d801809287d578344911b65697c) | 
`public uint8_t `[`DUMMY1`](#structsi47x__response__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) | Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
`public uint8_t `[`RDSINT`](#structsi47x__response__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f) | 
`public uint8_t `[`RSQINT`](#structsi47x__response__status_8resp_1a8acf6c55c97050e7abd06c104012c77a) | Radio Data System (RDS) Interrup; 0 = interrupt has not been triggered.
`public uint8_t `[`DUMMY2`](#structsi47x__response__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5) | Received Signal Quality Interrupt; 0 = interrupt has not been triggered.
`public uint8_t `[`ERR`](#structsi47x__response__status_8resp_1acd22bad976363fdd1bfbf6759fede482) | 
`public uint8_t `[`CTS`](#structsi47x__response__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) | Error. 0 = No error 1 = Error.
`public uint8_t `[`VALID`](#structsi47x__response__status_8resp_1ac9f1a6384b1c466d4612f513bd8e13ea) | Clear to Send.
`public uint8_t `[`AFCRL`](#structsi47x__response__status_8resp_1a4f37d8e1a484e1ac152858e71923f86c) | Valid Channel.
`public uint8_t `[`DUMMY3`](#structsi47x__response__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1) | AFC Rail Indicator.
`public uint8_t `[`BLTF`](#structsi47x__response__status_8resp_1a4163d039102268d1b6ac272a25006d7f) | 
`public uint8_t `[`READFREQH`](#structsi47x__response__status_8resp_1a0d093ad5e587811b66f0047303ec86a9) | Reports if a seek hit the band limit.
`public uint8_t `[`READFREQL`](#structsi47x__response__status_8resp_1ad667880f1209fa0da1d2c87c794329c7) | Read Frequency High byte.
`public uint8_t `[`RSSI`](#structsi47x__response__status_8resp_1a6833f5d3374c1679bea428b50dbad9cc) | Read Frequency Low byte.
`public uint8_t `[`SNR`](#structsi47x__response__status_8resp_1a8dc5367d65459f69d78eb03e93f7703f) | Received Signal Strength Indicator (dBμV)
`public uint8_t `[`MULT`](#structsi47x__response__status_8resp_1a36b0ca295026d76ee15df50dc4991e90) | This byte contains the SNR metric when tune is complete (dB).
`public uint8_t `[`READANTCAP`](#structsi47x__response__status_8resp_1aa4c3e6fac8670fa0b36ba68ff185aa24) | Contains the multipath metric when tune is complete.

## Members

#### `public uint8_t `[`STCINT`](#structsi47x__response__status_8resp_1af8f21d801809287d578344911b65697c) {#structsi47x__response__status_8resp_1af8f21d801809287d578344911b65697c}

#### `public uint8_t `[`DUMMY1`](#structsi47x__response__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__response__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a}

Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.

#### `public uint8_t `[`RDSINT`](#structsi47x__response__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f) {#structsi47x__response__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f}

#### `public uint8_t `[`RSQINT`](#structsi47x__response__status_8resp_1a8acf6c55c97050e7abd06c104012c77a) {#structsi47x__response__status_8resp_1a8acf6c55c97050e7abd06c104012c77a}

Radio Data System (RDS) Interrup; 0 = interrupt has not been triggered.

#### `public uint8_t `[`DUMMY2`](#structsi47x__response__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__response__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5}

Received Signal Quality Interrupt; 0 = interrupt has not been triggered.

#### `public uint8_t `[`ERR`](#structsi47x__response__status_8resp_1acd22bad976363fdd1bfbf6759fede482) {#structsi47x__response__status_8resp_1acd22bad976363fdd1bfbf6759fede482}

#### `public uint8_t `[`CTS`](#structsi47x__response__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) {#structsi47x__response__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76}

Error. 0 = No error 1 = Error.

#### `public uint8_t `[`VALID`](#structsi47x__response__status_8resp_1ac9f1a6384b1c466d4612f513bd8e13ea) {#structsi47x__response__status_8resp_1ac9f1a6384b1c466d4612f513bd8e13ea}

Clear to Send.

#### `public uint8_t `[`AFCRL`](#structsi47x__response__status_8resp_1a4f37d8e1a484e1ac152858e71923f86c) {#structsi47x__response__status_8resp_1a4f37d8e1a484e1ac152858e71923f86c}

Valid Channel.

#### `public uint8_t `[`DUMMY3`](#structsi47x__response__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1) {#structsi47x__response__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1}

AFC Rail Indicator.

#### `public uint8_t `[`BLTF`](#structsi47x__response__status_8resp_1a4163d039102268d1b6ac272a25006d7f) {#structsi47x__response__status_8resp_1a4163d039102268d1b6ac272a25006d7f}

#### `public uint8_t `[`READFREQH`](#structsi47x__response__status_8resp_1a0d093ad5e587811b66f0047303ec86a9) {#structsi47x__response__status_8resp_1a0d093ad5e587811b66f0047303ec86a9}

Reports if a seek hit the band limit.

#### `public uint8_t `[`READFREQL`](#structsi47x__response__status_8resp_1ad667880f1209fa0da1d2c87c794329c7) {#structsi47x__response__status_8resp_1ad667880f1209fa0da1d2c87c794329c7}

Read Frequency High byte.

#### `public uint8_t `[`RSSI`](#structsi47x__response__status_8resp_1a6833f5d3374c1679bea428b50dbad9cc) {#structsi47x__response__status_8resp_1a6833f5d3374c1679bea428b50dbad9cc}

Read Frequency Low byte.

#### `public uint8_t `[`SNR`](#structsi47x__response__status_8resp_1a8dc5367d65459f69d78eb03e93f7703f) {#structsi47x__response__status_8resp_1a8dc5367d65459f69d78eb03e93f7703f}

Received Signal Strength Indicator (dBμV)

#### `public uint8_t `[`MULT`](#structsi47x__response__status_8resp_1a36b0ca295026d76ee15df50dc4991e90) {#structsi47x__response__status_8resp_1a36b0ca295026d76ee15df50dc4991e90}

This byte contains the SNR metric when tune is complete (dB).

#### `public uint8_t `[`READANTCAP`](#structsi47x__response__status_8resp_1aa4c3e6fac8670fa0b36ba68ff185aa24) {#structsi47x__response__status_8resp_1aa4c3e6fac8670fa0b36ba68ff185aa24}

Contains the multipath metric when tune is complete.

# struct `si47x_rqs_status.resp` {#structsi47x__rqs__status_8resp}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`STCINT`](#structsi47x__rqs__status_8resp_1af8f21d801809287d578344911b65697c) | 
`public uint8_t `[`DUMMY1`](#structsi47x__rqs__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) | 
`public uint8_t `[`RDSINT`](#structsi47x__rqs__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f) | 
`public uint8_t `[`RSQINT`](#structsi47x__rqs__status_8resp_1a8acf6c55c97050e7abd06c104012c77a) | 
`public uint8_t `[`DUMMY2`](#structsi47x__rqs__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5) | 
`public uint8_t `[`ERR`](#structsi47x__rqs__status_8resp_1acd22bad976363fdd1bfbf6759fede482) | 
`public uint8_t `[`CTS`](#structsi47x__rqs__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) | 
`public uint8_t `[`RSSIILINT`](#structsi47x__rqs__status_8resp_1a78e3f2f0b0c5b1dfd51e3c0d48ae18e3) | 
`public uint8_t `[`RSSIHINT`](#structsi47x__rqs__status_8resp_1a1c2886a1d02e462622c810dec48a7a8d) | RSSI Detect Low.
`public uint8_t `[`SNRLINT`](#structsi47x__rqs__status_8resp_1a97383bc737ce99a121985160e6ffa344) | RSSI Detect High.
`public uint8_t `[`SNRHINT`](#structsi47x__rqs__status_8resp_1ae2e739596a7340b62844e12390115e6a) | SNR Detect Low.
`public uint8_t `[`MULTLINT`](#structsi47x__rqs__status_8resp_1af07f7e10faec4b062b389868532a8146) | SNR Detect High.
`public uint8_t `[`MULTHINT`](#structsi47x__rqs__status_8resp_1a6b4ed08de1237c36d005669ead7d8a50) | Multipath Detect Low.
`public uint8_t `[`DUMMY3`](#structsi47x__rqs__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1) | Multipath Detect High.
`public uint8_t `[`BLENDINT`](#structsi47x__rqs__status_8resp_1a78a25db6cd73b193ba6c941df8584d35) | 
`public uint8_t `[`VALID`](#structsi47x__rqs__status_8resp_1ac9f1a6384b1c466d4612f513bd8e13ea) | Blend Detect Interrupt.
`public uint8_t `[`AFCRL`](#structsi47x__rqs__status_8resp_1a4f37d8e1a484e1ac152858e71923f86c) | Valid Channel.
`public uint8_t `[`DUMMY4`](#structsi47x__rqs__status_8resp_1a9182569d693623e6b469099542b303e4) | AFC Rail Indicator.
`public uint8_t `[`SMUTE`](#structsi47x__rqs__status_8resp_1a018a3d212081bb86b34e6b8fbd88c4e2) | 
`public uint8_t `[`DUMMY5`](#structsi47x__rqs__status_8resp_1a1fc4251a40ec4352f10fc3ecb20f2c79) | Soft Mute Indicator. Indicates soft mute is engaged.
`public uint8_t `[`STBLEND`](#structsi47x__rqs__status_8resp_1aedcdb3b2f6ac6daf3bacf8bb54a92d28) | 
`public uint8_t `[`PILOT`](#structsi47x__rqs__status_8resp_1aece6117f0bc60ec824fe5d441abdb288) | Indicates amount of stereo blend in% (100 = full stereo, 0 = full mono).
`public uint8_t `[`RSSI`](#structsi47x__rqs__status_8resp_1a6833f5d3374c1679bea428b50dbad9cc) | Indicates stereo pilot presence.
`public uint8_t `[`SNR`](#structsi47x__rqs__status_8resp_1a8dc5367d65459f69d78eb03e93f7703f) | RESP4 - Contains the current receive signal strength (0–127 dBμV).
`public uint8_t `[`MULT`](#structsi47x__rqs__status_8resp_1a36b0ca295026d76ee15df50dc4991e90) | RESP5 - Contains the current SNR metric (0–127 dB).
`public uint8_t `[`FREQOFF`](#structsi47x__rqs__status_8resp_1a0c144971635328e95870811948c9071a) | RESP6 - Contains the current multipath metric. (0 = no multipath; 100 = full multipath)

## Members

#### `public uint8_t `[`STCINT`](#structsi47x__rqs__status_8resp_1af8f21d801809287d578344911b65697c) {#structsi47x__rqs__status_8resp_1af8f21d801809287d578344911b65697c}

#### `public uint8_t `[`DUMMY1`](#structsi47x__rqs__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__rqs__status_8resp_1a3651c40ccc4450f2fc89fa3139dedd5a}

#### `public uint8_t `[`RDSINT`](#structsi47x__rqs__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f) {#structsi47x__rqs__status_8resp_1a53d648e7e9d100d590e2f65ec7de079f}

#### `public uint8_t `[`RSQINT`](#structsi47x__rqs__status_8resp_1a8acf6c55c97050e7abd06c104012c77a) {#structsi47x__rqs__status_8resp_1a8acf6c55c97050e7abd06c104012c77a}

#### `public uint8_t `[`DUMMY2`](#structsi47x__rqs__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5) {#structsi47x__rqs__status_8resp_1abece94c62273dc7ecfabc565b76dbbe5}

#### `public uint8_t `[`ERR`](#structsi47x__rqs__status_8resp_1acd22bad976363fdd1bfbf6759fede482) {#structsi47x__rqs__status_8resp_1acd22bad976363fdd1bfbf6759fede482}

#### `public uint8_t `[`CTS`](#structsi47x__rqs__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76) {#structsi47x__rqs__status_8resp_1ae16433ffd3adc248f0ce2608a95c3c76}

#### `public uint8_t `[`RSSIILINT`](#structsi47x__rqs__status_8resp_1a78e3f2f0b0c5b1dfd51e3c0d48ae18e3) {#structsi47x__rqs__status_8resp_1a78e3f2f0b0c5b1dfd51e3c0d48ae18e3}

#### `public uint8_t `[`RSSIHINT`](#structsi47x__rqs__status_8resp_1a1c2886a1d02e462622c810dec48a7a8d) {#structsi47x__rqs__status_8resp_1a1c2886a1d02e462622c810dec48a7a8d}

RSSI Detect Low.

#### `public uint8_t `[`SNRLINT`](#structsi47x__rqs__status_8resp_1a97383bc737ce99a121985160e6ffa344) {#structsi47x__rqs__status_8resp_1a97383bc737ce99a121985160e6ffa344}

RSSI Detect High.

#### `public uint8_t `[`SNRHINT`](#structsi47x__rqs__status_8resp_1ae2e739596a7340b62844e12390115e6a) {#structsi47x__rqs__status_8resp_1ae2e739596a7340b62844e12390115e6a}

SNR Detect Low.

#### `public uint8_t `[`MULTLINT`](#structsi47x__rqs__status_8resp_1af07f7e10faec4b062b389868532a8146) {#structsi47x__rqs__status_8resp_1af07f7e10faec4b062b389868532a8146}

SNR Detect High.

#### `public uint8_t `[`MULTHINT`](#structsi47x__rqs__status_8resp_1a6b4ed08de1237c36d005669ead7d8a50) {#structsi47x__rqs__status_8resp_1a6b4ed08de1237c36d005669ead7d8a50}

Multipath Detect Low.

#### `public uint8_t `[`DUMMY3`](#structsi47x__rqs__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1) {#structsi47x__rqs__status_8resp_1a613aa5c14b3ccc6304a519292c482ed1}

Multipath Detect High.

#### `public uint8_t `[`BLENDINT`](#structsi47x__rqs__status_8resp_1a78a25db6cd73b193ba6c941df8584d35) {#structsi47x__rqs__status_8resp_1a78a25db6cd73b193ba6c941df8584d35}

#### `public uint8_t `[`VALID`](#structsi47x__rqs__status_8resp_1ac9f1a6384b1c466d4612f513bd8e13ea) {#structsi47x__rqs__status_8resp_1ac9f1a6384b1c466d4612f513bd8e13ea}

Blend Detect Interrupt.

#### `public uint8_t `[`AFCRL`](#structsi47x__rqs__status_8resp_1a4f37d8e1a484e1ac152858e71923f86c) {#structsi47x__rqs__status_8resp_1a4f37d8e1a484e1ac152858e71923f86c}

Valid Channel.

#### `public uint8_t `[`DUMMY4`](#structsi47x__rqs__status_8resp_1a9182569d693623e6b469099542b303e4) {#structsi47x__rqs__status_8resp_1a9182569d693623e6b469099542b303e4}

AFC Rail Indicator.

#### `public uint8_t `[`SMUTE`](#structsi47x__rqs__status_8resp_1a018a3d212081bb86b34e6b8fbd88c4e2) {#structsi47x__rqs__status_8resp_1a018a3d212081bb86b34e6b8fbd88c4e2}

#### `public uint8_t `[`DUMMY5`](#structsi47x__rqs__status_8resp_1a1fc4251a40ec4352f10fc3ecb20f2c79) {#structsi47x__rqs__status_8resp_1a1fc4251a40ec4352f10fc3ecb20f2c79}

Soft Mute Indicator. Indicates soft mute is engaged.

#### `public uint8_t `[`STBLEND`](#structsi47x__rqs__status_8resp_1aedcdb3b2f6ac6daf3bacf8bb54a92d28) {#structsi47x__rqs__status_8resp_1aedcdb3b2f6ac6daf3bacf8bb54a92d28}

#### `public uint8_t `[`PILOT`](#structsi47x__rqs__status_8resp_1aece6117f0bc60ec824fe5d441abdb288) {#structsi47x__rqs__status_8resp_1aece6117f0bc60ec824fe5d441abdb288}

Indicates amount of stereo blend in% (100 = full stereo, 0 = full mono).

#### `public uint8_t `[`RSSI`](#structsi47x__rqs__status_8resp_1a6833f5d3374c1679bea428b50dbad9cc) {#structsi47x__rqs__status_8resp_1a6833f5d3374c1679bea428b50dbad9cc}

Indicates stereo pilot presence.

#### `public uint8_t `[`SNR`](#structsi47x__rqs__status_8resp_1a8dc5367d65459f69d78eb03e93f7703f) {#structsi47x__rqs__status_8resp_1a8dc5367d65459f69d78eb03e93f7703f}

RESP4 - Contains the current receive signal strength (0–127 dBμV).

#### `public uint8_t `[`MULT`](#structsi47x__rqs__status_8resp_1a36b0ca295026d76ee15df50dc4991e90) {#structsi47x__rqs__status_8resp_1a36b0ca295026d76ee15df50dc4991e90}

RESP5 - Contains the current SNR metric (0–127 dB).

#### `public uint8_t `[`FREQOFF`](#structsi47x__rqs__status_8resp_1a0c144971635328e95870811948c9071a) {#structsi47x__rqs__status_8resp_1a0c144971635328e95870811948c9071a}

RESP6 - Contains the current multipath metric. (0 = no multipath; 100 = full multipath)

# struct `si47x_seek.arg` {#structsi47x__seek_8arg}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`RESERVED1`](#structsi47x__seek_8arg_1ad8179a090910d4923b044cbce813c8ea) | 
`public uint8_t `[`WRAP`](#structsi47x__seek_8arg_1ae1c8555fcf0ea2bb648a6fd527d658c0) | 
`public uint8_t `[`SEEKUP`](#structsi47x__seek_8arg_1aed8a1fd68dec9364930b51d020ba07d6) | Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.
`public uint8_t `[`RESERVED2`](#structsi47x__seek_8arg_1a0b5885d29a74762639e4b55c51ce2d85) | Determines the direction of the search, either UP = 1, or DOWN = 0.

## Members

#### `public uint8_t `[`RESERVED1`](#structsi47x__seek_8arg_1ad8179a090910d4923b044cbce813c8ea) {#structsi47x__seek_8arg_1ad8179a090910d4923b044cbce813c8ea}

#### `public uint8_t `[`WRAP`](#structsi47x__seek_8arg_1ae1c8555fcf0ea2bb648a6fd527d658c0) {#structsi47x__seek_8arg_1ae1c8555fcf0ea2bb648a6fd527d658c0}

#### `public uint8_t `[`SEEKUP`](#structsi47x__seek_8arg_1aed8a1fd68dec9364930b51d020ba07d6) {#structsi47x__seek_8arg_1aed8a1fd68dec9364930b51d020ba07d6}

Determines whether the seek should Wrap = 1, or Halt = 0 when it hits the band limit.

#### `public uint8_t `[`RESERVED2`](#structsi47x__seek_8arg_1a0b5885d29a74762639e4b55c51ce2d85) {#structsi47x__seek_8arg_1a0b5885d29a74762639e4b55c51ce2d85}

Determines the direction of the search, either UP = 1, or DOWN = 0.

# struct `si47x_set_frequency.arg` {#structsi47x__set__frequency_8arg}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`FAST`](#structsi47x__set__frequency_8arg_1adca6e617f6fb54033deb311e7e7c93cc) | 
`public uint8_t `[`FREEZE`](#structsi47x__set__frequency_8arg_1a1da4c7af6c0e106e2aec1c483252b329) | ARG1 - FAST Tuning. If set, executes fast and invalidated tune. The tune status will not be accurate.
`public uint8_t `[`DUMMY1`](#structsi47x__set__frequency_8arg_1a3651c40ccc4450f2fc89fa3139dedd5a) | Valid only for FM (Must be 0 to AM)
`public uint8_t `[`USBLSB`](#structsi47x__set__frequency_8arg_1a46ba89190297ccd0df7c8f8b5f72497f) | Always set 0.
`public uint8_t `[`FREQH`](#structsi47x__set__frequency_8arg_1a8836c6ec927c24305d2fd26a5aec2e2b) | SSB Upper Side Band (USB) and Lower Side Band (LSB) Selection. 10 = USB is selected; 01 = LSB is selected.
`public uint8_t `[`FREQL`](#structsi47x__set__frequency_8arg_1a858dfb4be81dfbd057aa63aded06bf3d) | ARG2 - Tune Frequency High byte.
`public uint8_t `[`ANTCAPH`](#structsi47x__set__frequency_8arg_1a5f437dbf77cdadcbb215ce5db44edbe6) | ARG3 - Tune Frequency Low byte.
`public uint8_t `[`ANTCAPL`](#structsi47x__set__frequency_8arg_1a0475bf71e813922e317ef7cb7b3df9fa) | ARG4 - Antenna Tuning Capacitor High byte.

## Members

#### `public uint8_t `[`FAST`](#structsi47x__set__frequency_8arg_1adca6e617f6fb54033deb311e7e7c93cc) {#structsi47x__set__frequency_8arg_1adca6e617f6fb54033deb311e7e7c93cc}

#### `public uint8_t `[`FREEZE`](#structsi47x__set__frequency_8arg_1a1da4c7af6c0e106e2aec1c483252b329) {#structsi47x__set__frequency_8arg_1a1da4c7af6c0e106e2aec1c483252b329}

ARG1 - FAST Tuning. If set, executes fast and invalidated tune. The tune status will not be accurate.

#### `public uint8_t `[`DUMMY1`](#structsi47x__set__frequency_8arg_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__set__frequency_8arg_1a3651c40ccc4450f2fc89fa3139dedd5a}

Valid only for FM (Must be 0 to AM)

#### `public uint8_t `[`USBLSB`](#structsi47x__set__frequency_8arg_1a46ba89190297ccd0df7c8f8b5f72497f) {#structsi47x__set__frequency_8arg_1a46ba89190297ccd0df7c8f8b5f72497f}

Always set 0.

#### `public uint8_t `[`FREQH`](#structsi47x__set__frequency_8arg_1a8836c6ec927c24305d2fd26a5aec2e2b) {#structsi47x__set__frequency_8arg_1a8836c6ec927c24305d2fd26a5aec2e2b}

SSB Upper Side Band (USB) and Lower Side Band (LSB) Selection. 10 = USB is selected; 01 = LSB is selected.

#### `public uint8_t `[`FREQL`](#structsi47x__set__frequency_8arg_1a858dfb4be81dfbd057aa63aded06bf3d) {#structsi47x__set__frequency_8arg_1a858dfb4be81dfbd057aa63aded06bf3d}

ARG2 - Tune Frequency High byte.

#### `public uint8_t `[`ANTCAPH`](#structsi47x__set__frequency_8arg_1a5f437dbf77cdadcbb215ce5db44edbe6) {#structsi47x__set__frequency_8arg_1a5f437dbf77cdadcbb215ce5db44edbe6}

ARG3 - Tune Frequency Low byte.

#### `public uint8_t `[`ANTCAPL`](#structsi47x__set__frequency_8arg_1a0475bf71e813922e317ef7cb7b3df9fa) {#structsi47x__set__frequency_8arg_1a0475bf71e813922e317ef7cb7b3df9fa}

ARG4 - Antenna Tuning Capacitor High byte.

# struct `si47x_ssb_mode.param` {#structsi47x__ssb__mode_8param}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`AUDIOBW`](#structsi47x__ssb__mode_8param_1a38eda9266347ac0c0f09182baa2fefd0) | 
`public uint8_t `[`SBCUTFLT`](#structsi47x__ssb__mode_8param_1a5fd112c4d977a89cf8b7717ef0a12a9b) | 0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz
`public uint8_t `[`AVC_DIVIDER`](#structsi47x__ssb__mode_8param_1a911ed778d046d0d22bf2df2938c1f084) | SSB side band cutoff filter for band passand low pass filter.
`public uint8_t `[`AVCEN`](#structsi47x__ssb__mode_8param_1a55576f7d4412f08280e49fba44ef707a) | set 0 for SSB mode; set 3 for SYNC mode;
`public uint8_t `[`SMUTESEL`](#structsi47x__ssb__mode_8param_1aeb7815cb295d531065f797c39cf36da5) | SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default);.
`public uint8_t `[`DUMMY1`](#structsi47x__ssb__mode_8param_1a3651c40ccc4450f2fc89fa3139dedd5a) | SSB Soft-mute Based on RSSI or SNR.
`public uint8_t `[`DSP_AFCDIS`](#structsi47x__ssb__mode_8param_1a5ff4a830553e8db4d36f8b3677e3d412) | Always write 0;.

## Members

#### `public uint8_t `[`AUDIOBW`](#structsi47x__ssb__mode_8param_1a38eda9266347ac0c0f09182baa2fefd0) {#structsi47x__ssb__mode_8param_1a38eda9266347ac0c0f09182baa2fefd0}

#### `public uint8_t `[`SBCUTFLT`](#structsi47x__ssb__mode_8param_1a5fd112c4d977a89cf8b7717ef0a12a9b) {#structsi47x__ssb__mode_8param_1a5fd112c4d977a89cf8b7717ef0a12a9b}

0 = 1.2KHz (default); 1=2.2KHz; 2=3KHz; 3=4KHz; 4=500Hz; 5=1KHz

#### `public uint8_t `[`AVC_DIVIDER`](#structsi47x__ssb__mode_8param_1a911ed778d046d0d22bf2df2938c1f084) {#structsi47x__ssb__mode_8param_1a911ed778d046d0d22bf2df2938c1f084}

SSB side band cutoff filter for band passand low pass filter.

#### `public uint8_t `[`AVCEN`](#structsi47x__ssb__mode_8param_1a55576f7d4412f08280e49fba44ef707a) {#structsi47x__ssb__mode_8param_1a55576f7d4412f08280e49fba44ef707a}

set 0 for SSB mode; set 3 for SYNC mode;

#### `public uint8_t `[`SMUTESEL`](#structsi47x__ssb__mode_8param_1aeb7815cb295d531065f797c39cf36da5) {#structsi47x__ssb__mode_8param_1aeb7815cb295d531065f797c39cf36da5}

SSB Automatic Volume Control (AVC) enable; 0=disable; 1=enable (default);.

#### `public uint8_t `[`DUMMY1`](#structsi47x__ssb__mode_8param_1a3651c40ccc4450f2fc89fa3139dedd5a) {#structsi47x__ssb__mode_8param_1a3651c40ccc4450f2fc89fa3139dedd5a}

SSB Soft-mute Based on RSSI or SNR.

#### `public uint8_t `[`DSP_AFCDIS`](#structsi47x__ssb__mode_8param_1a5ff4a830553e8db4d36f8b3677e3d412) {#structsi47x__ssb__mode_8param_1a5ff4a830553e8db4d36f8b3677e3d412}

Always write 0;.

# struct `si47x_tune_status.arg` {#structsi47x__tune__status_8arg}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`INTACK`](#structsi47x__tune__status_8arg_1aa1b4c5526f2abe4a5d02be4016a44020) | 
`public uint8_t `[`CANCEL`](#structsi47x__tune__status_8arg_1a2027c027133e22c8929e2874dc44dd36) | If set, clears the seek/tune complete interrupt status indicator.
`public uint8_t `[`RESERVED2`](#structsi47x__tune__status_8arg_1a0b5885d29a74762639e4b55c51ce2d85) | If set, aborts a seek currently in progress.

## Members

#### `public uint8_t `[`INTACK`](#structsi47x__tune__status_8arg_1aa1b4c5526f2abe4a5d02be4016a44020) {#structsi47x__tune__status_8arg_1aa1b4c5526f2abe4a5d02be4016a44020}

#### `public uint8_t `[`CANCEL`](#structsi47x__tune__status_8arg_1a2027c027133e22c8929e2874dc44dd36) {#structsi47x__tune__status_8arg_1a2027c027133e22c8929e2874dc44dd36}

If set, clears the seek/tune complete interrupt status indicator.

#### `public uint8_t `[`RESERVED2`](#structsi47x__tune__status_8arg_1a0b5885d29a74762639e4b55c51ce2d85) {#structsi47x__tune__status_8arg_1a0b5885d29a74762639e4b55c51ce2d85}

If set, aborts a seek currently in progress.

Generated by [Moxygen](https://sourcey.com/moxygen)
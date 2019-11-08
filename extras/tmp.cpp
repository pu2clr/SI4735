
#define DEBUG 1 // Should be commented when released. Uncomment it if you want to debug

#if defined(DEBUG)
void SI4735::debugStatus()
{
    Serial.print("Tune complete (STCINT).: ");
    Serial.println(currentStatus.resp.STCINT); // Seek/Tune Complete Interrupt; 1 = Tune complete has been triggered.
    Serial.print("(RDS) Interrupt(RDSINT): ");
    Serial.println(currentStatus.resp.RDSINT); // Radio Data System (RDS) Interrup; 0 = interrupt has not been triggered.
    Serial.print("Receiv.Sig.Quality INT: ");
    Serial.println(currentStatus.resp.RSQINT); // Received Signal Quality Interrupt; 0 = interrupt has not been triggered.
    Serial.print("ERR...................: ");
    Serial.println(currentStatus.resp.ERR); // Error. 0 = No error 1 = Error
    Serial.print("CTS...................: ");
    Serial.println(currentStatus.resp.CTS); // Clear to Send.
    // RESP1
    Serial.print("Valid Channel..........: ");
    Serial.println(currentStatus.resp.VALID); // Valid Channel
    Serial.print("AFC Rail Indicator.....: ");
    Serial.println(currentStatus.resp.AFCRL); // AFC Rail Indicator
    Serial.print("Reports if a seek limit: ");
    Serial.println(currentStatus.resp.BLTF); // Reports if a seek hit the band limit
    // RESP4
    Serial.print("Receiv.Signal Ind.dBμV.: ");
    Serial.println(currentStatus.resp.RSSI); // Received Signal Strength Indicator (dBμV)
    // RESP5
    Serial.print("SNR (dB)...............: ");
    Serial.println(currentStatus.resp.SNR); // This Serial.println(currentStatus.resp.contains the SNR metric when tune is complete (dB).
    // RESP6
    Serial.print("multipath metric.......: ");
    Serial.println(currentStatus.resp.MULT); // Contains the multipath metric when tune is complete
    // RESP7
    Serial.print("Antenna Tuning cap......: ");
    Serial.println(currentStatus.resp.READANTCAP); // Contains the current antenna tuning capacitor value

    si47x_frequency freq;
    freq.raw.FREQL = currentStatus.resp.READFREQL;
    freq.raw.FREQH = currentStatus.resp.READFREQH;

    Serial.print("Palatable frequency.....: ");
    Serial.println(freq.value);
}
#endif
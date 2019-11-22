# SI473X RDS  



## RDS Terminology

| Term | Description |
| ---- | ----- |
| RDS  | Radio Data System | 
| AF   | alternative frequencies list |
| CT   | clock time and date |
| EON  | enhanced other networks information |
| PI   | programme identification |
| PS   | programme service name |
| PTY  | programme type | 
| REG  | regional |
| RT   | radio text |
| RT+  | radio text plus |
| TA, TP | traffic announcement, traffic programme |
| TMC  | (traffic message channel) |




## RDS Overview

The RDS is communication protocol used to send small messages to regular FM radio receivers.  It is capable to send to your receiver messages about the program type, traffic information, local time, alternative frequencies and text. 

RDS runs at 1187.5 bps. It is based on 26-bit information where 16 bits are the information data and 10 bits are used to error detection. Each four data blocks has 104 bits that are sent continuously. 

That blocks can be identified by the transmitted data. You can receive 16 group types of information where each group has two versions, A or B. The group type and the version (A or B) can be known by reading block B. The table below shows each group type with its respective version and meaning.  


| RDS group type | A Version | B Version| 
| ---------- | ----------| ---------| 
|0|	Basic tuning and switching information only |	Basic tuning and switching information only|
|1|	Program Item Number and slow labeling codes only |	Program Item Number |
|2|	Radiotext |	Radiotext |
|3|	Applications Identification for ODA only |	Open Data Applications |
|4|	Clock-time and date only |	Open Data Applications |
|5|	Transparent Data Channels (32 channels) or ODA | Transparent Data Channels (32 channels) or ODA
|6|	In House applications or ODA |	In House applications or ODA |
|7|	Radio Paging or ODA	 | Open Data Applications |
|8|	Traffic Message Channel (TMC) or ODA | Open Data Applications |
|9|	Emergency Warning System (EWS)or ODA | Open Data Applications |
|10| Program Type Name | Open Data Applications |
|11| Open Data Applications |	Open Data Applications |
|12| Open Data Applications	| Open Data Applications |
|13| Enhanced Radio Paging or ODA |	Open Data Applications |
|14| Enhanced Other Networks information only |	Enhanced Other Networks information onlys |
|15| Defined in RBDS only (Fast switching information) | Fast switching information only |
__Source: http://www.g.laroche.free.fr/english/rds/groupes/listeGroupesRDS.htm__


The Bloco B has the data structure shown below.  

| Information | Bits | Description |
| ----------- | ---- | ----------- |
| groupType   |  4   | Group Type code |
| versionCode |  1   | 0=A; 1=B |
| trafficProgramCode | 1 | |  0 = No Traffic Alerts; 1 = Station gives Traffic Alerts |
| programType  | 5   | PTY (Program Type) code |
| content | 5 | Depends on Group Type and Version codes |


|               | Block 1  	                    | Block 2 |    |    |     |     |
|-------------- | ----------------------------- | ------- | -- | -- | --- | --- |
| Block Meaning            |Program Identification Code |GTYPE |B0  | TP    |PTY  | ????|
| bit notation per block   | b15-b0                     |b15-b12 |b11   |b10 |b9-b5  |b4-b0|
| Fixed Meaning Per Group? | Yes                        |Yes|Yes   |Yes | Yes   |No    |

Source: [Wikipedia, Radio Data System](https://en.wikipedia.org/wiki/Radio_Data_System)



## References

1. [Radio Data System](https://en.wikipedia.org/wiki/Radio_Data_System)
2. [Specification of the radio data system (RDS) for VHF/FM sound broadcasting in the frequency range from 87,5 to 108,0 MHz](http://www.interactive-radio-system.com/docs/EN50067_RDS_Standard.pdf)
3. [https://github.com/FlyingLotus1983/Si4735](https://github.com/FlyingLotus1983/Si4735)
# SI473X RDS features 


## RDS Overview

The RDS is communication protocol used to send small messages to regular FM radio receivers.  It is capable to send to your receiver messages about the program type, traffic information, local time, alternative frequencies and text. 

RDS runs at 1187.5 bps. It is based on 26-bit information where 16 bits are the information data and 10 bits are used to error detection. Each four data blocks has 104 bits that are divided by four groups.  

That groups can be identified by the transmitted data. You can receive 16 group types where each group has two versions, A or B.
The table below shows each group type with its respective version and meaning.  


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


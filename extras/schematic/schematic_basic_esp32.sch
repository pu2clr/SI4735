<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="yes"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="16" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="14" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="53" name="tGND_GNDA" color="7" fill="9" visible="no" active="no"/>
<layer number="54" name="bGND_GNDA" color="1" fill="9" visible="no" active="no"/>
<layer number="56" name="wert" color="7" fill="1" visible="no" active="no"/>
<layer number="57" name="tCAD" color="7" fill="1" visible="no" active="no"/>
<layer number="59" name="tCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="60" name="bCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="7" fill="1" visible="no" active="no"/>
<layer number="100" name="Muster" color="7" fill="1" visible="no" active="no"/>
<layer number="101" name="Patch_Top" color="12" fill="4" visible="yes" active="yes"/>
<layer number="102" name="Vscore" color="7" fill="1" visible="yes" active="yes"/>
<layer number="103" name="tMap" color="7" fill="1" visible="yes" active="yes"/>
<layer number="104" name="Name" color="16" fill="1" visible="yes" active="yes"/>
<layer number="105" name="tPlate" color="7" fill="1" visible="yes" active="yes"/>
<layer number="106" name="bPlate" color="7" fill="1" visible="yes" active="yes"/>
<layer number="107" name="Crop" color="7" fill="1" visible="yes" active="yes"/>
<layer number="108" name="tplace-old" color="10" fill="1" visible="yes" active="yes"/>
<layer number="109" name="ref-old" color="11" fill="1" visible="yes" active="yes"/>
<layer number="110" name="fp0" color="7" fill="1" visible="yes" active="yes"/>
<layer number="111" name="LPC17xx" color="7" fill="1" visible="yes" active="yes"/>
<layer number="112" name="tSilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="113" name="IDFDebug" color="7" fill="1" visible="yes" active="yes"/>
<layer number="114" name="Badge_Outline" color="11" fill="1" visible="no" active="no"/>
<layer number="115" name="ReferenceISLANDS" color="7" fill="1" visible="yes" active="yes"/>
<layer number="116" name="Patch_BOT" color="9" fill="4" visible="yes" active="yes"/>
<layer number="118" name="Rect_Pads" color="7" fill="1" visible="yes" active="yes"/>
<layer number="121" name="_tsilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="122" name="_bsilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="123" name="tTestmark" color="7" fill="1" visible="yes" active="yes"/>
<layer number="124" name="bTestmark" color="7" fill="1" visible="yes" active="yes"/>
<layer number="125" name="_tNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="126" name="_bNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="127" name="_tValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="128" name="_bValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="129" name="Mask" color="7" fill="1" visible="yes" active="yes"/>
<layer number="131" name="tAdjust" color="7" fill="1" visible="yes" active="yes"/>
<layer number="132" name="bAdjust" color="7" fill="1" visible="yes" active="yes"/>
<layer number="144" name="Drill_legend" color="7" fill="1" visible="yes" active="yes"/>
<layer number="150" name="Notes" color="7" fill="1" visible="yes" active="yes"/>
<layer number="151" name="HeatSink" color="7" fill="1" visible="yes" active="yes"/>
<layer number="152" name="_bDocu" color="7" fill="1" visible="yes" active="yes"/>
<layer number="153" name="FabDoc1" color="7" fill="1" visible="yes" active="yes"/>
<layer number="154" name="FabDoc2" color="7" fill="1" visible="yes" active="yes"/>
<layer number="155" name="FabDoc3" color="7" fill="1" visible="yes" active="yes"/>
<layer number="199" name="Contour" color="7" fill="1" visible="yes" active="yes"/>
<layer number="200" name="200bmp" color="1" fill="10" visible="yes" active="yes"/>
<layer number="201" name="201bmp" color="2" fill="10" visible="yes" active="yes"/>
<layer number="202" name="202bmp" color="3" fill="10" visible="yes" active="yes"/>
<layer number="203" name="203bmp" color="4" fill="10" visible="yes" active="yes"/>
<layer number="204" name="204bmp" color="5" fill="10" visible="yes" active="yes"/>
<layer number="205" name="205bmp" color="6" fill="10" visible="yes" active="yes"/>
<layer number="206" name="206bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="207" name="207bmp" color="8" fill="10" visible="yes" active="yes"/>
<layer number="208" name="208bmp" color="9" fill="10" visible="yes" active="yes"/>
<layer number="209" name="209bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="210" name="210bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="211" name="211bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="212" name="212bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="213" name="213bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="214" name="214bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="215" name="215bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="216" name="216bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="217" name="217bmp" color="18" fill="1" visible="no" active="no"/>
<layer number="218" name="218bmp" color="19" fill="1" visible="no" active="no"/>
<layer number="219" name="219bmp" color="20" fill="1" visible="no" active="no"/>
<layer number="220" name="220bmp" color="21" fill="1" visible="no" active="no"/>
<layer number="221" name="221bmp" color="22" fill="1" visible="no" active="no"/>
<layer number="222" name="222bmp" color="23" fill="1" visible="no" active="no"/>
<layer number="223" name="223bmp" color="24" fill="1" visible="no" active="no"/>
<layer number="224" name="224bmp" color="25" fill="1" visible="no" active="no"/>
<layer number="225" name="225bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="226" name="226bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="227" name="227bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="228" name="228bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="229" name="229bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="230" name="230bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="231" name="231bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="232" name="Eagle3D_PG2" color="7" fill="1" visible="yes" active="yes"/>
<layer number="233" name="Eagle3D_PG3" color="7" fill="1" visible="yes" active="yes"/>
<layer number="248" name="Housing" color="7" fill="1" visible="yes" active="yes"/>
<layer number="249" name="Edge" color="7" fill="1" visible="yes" active="yes"/>
<layer number="250" name="Descript" color="3" fill="1" visible="no" active="no"/>
<layer number="251" name="SMDround" color="12" fill="11" visible="no" active="no"/>
<layer number="254" name="cooling" color="7" fill="1" visible="yes" active="yes"/>
<layer number="255" name="routoute" color="7" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="si4735">
<packages>
<package name="SSOP24_SIL">
<smd name="1" x="-2.6543" y="3.4925" dx="1.397" dy="0.3556" layer="1"/>
<smd name="2" x="-2.6543" y="2.8575" dx="1.397" dy="0.3556" layer="1"/>
<smd name="3" x="-2.6543" y="2.2225" dx="1.397" dy="0.3556" layer="1"/>
<smd name="4" x="-2.6543" y="1.5875" dx="1.397" dy="0.3556" layer="1"/>
<smd name="5" x="-2.6543" y="0.9525" dx="1.397" dy="0.3556" layer="1"/>
<smd name="6" x="-2.6543" y="0.3175" dx="1.397" dy="0.3556" layer="1"/>
<smd name="7" x="-2.6543" y="-0.3175" dx="1.397" dy="0.3556" layer="1"/>
<smd name="8" x="-2.6543" y="-0.9525" dx="1.397" dy="0.3556" layer="1"/>
<smd name="9" x="-2.6543" y="-1.5875" dx="1.397" dy="0.3556" layer="1"/>
<smd name="10" x="-2.6543" y="-2.2225" dx="1.397" dy="0.3556" layer="1"/>
<smd name="11" x="-2.6543" y="-2.8575" dx="1.397" dy="0.3556" layer="1"/>
<smd name="12" x="-2.6543" y="-3.4925" dx="1.397" dy="0.3556" layer="1"/>
<smd name="13" x="2.6543" y="-3.4925" dx="1.397" dy="0.3556" layer="1"/>
<smd name="14" x="2.6543" y="-2.8575" dx="1.397" dy="0.3556" layer="1"/>
<smd name="15" x="2.6543" y="-2.2225" dx="1.397" dy="0.3556" layer="1"/>
<smd name="16" x="2.6543" y="-1.5875" dx="1.397" dy="0.3556" layer="1"/>
<smd name="17" x="2.6543" y="-0.9525" dx="1.397" dy="0.3556" layer="1"/>
<smd name="18" x="2.6543" y="-0.3175" dx="1.397" dy="0.3556" layer="1"/>
<smd name="19" x="2.6543" y="0.3175" dx="1.397" dy="0.3556" layer="1"/>
<smd name="20" x="2.6543" y="0.9525" dx="1.397" dy="0.3556" layer="1"/>
<smd name="21" x="2.6543" y="1.5875" dx="1.397" dy="0.3556" layer="1"/>
<smd name="22" x="2.6543" y="2.2225" dx="1.397" dy="0.3556" layer="1"/>
<smd name="23" x="2.6543" y="2.8575" dx="1.397" dy="0.3556" layer="1"/>
<smd name="24" x="2.6543" y="3.4925" dx="1.397" dy="0.3556" layer="1"/>
<wire x1="-2.0828" y1="-4.4704" x2="2.0828" y2="-4.4704" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="-4.4704" x2="2.0828" y2="-4.0132" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="4.4704" x2="-2.0828" y2="4.4704" width="0.1524" layer="21"/>
<wire x1="-2.0828" y1="4.4704" x2="-2.0828" y2="4.0132" width="0.1524" layer="21"/>
<wire x1="-2.0828" y1="-4.0132" x2="-2.0828" y2="-4.4704" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="4.0132" x2="2.0828" y2="4.4704" width="0.1524" layer="21"/>
<polygon width="0.0254" layer="21">
<vertex x="-3.8608" y="-2.032"/>
<vertex x="-3.8608" y="-2.413"/>
<vertex x="-3.6068" y="-2.413"/>
<vertex x="-3.6068" y="-2.032"/>
</polygon>
<polygon width="0.0254" layer="21">
<vertex x="3.8608" y="1.143"/>
<vertex x="3.8608" y="0.762"/>
<vertex x="3.6068" y="0.762"/>
<vertex x="3.6068" y="1.143"/>
</polygon>
<text x="-3.4798" y="3.7084" size="1.27" layer="21" ratio="6" rot="SR0">*</text>
<text x="-1.7272" y="-0.635" size="1.27" layer="21" ratio="6" rot="SR0">&gt;Value</text>
<wire x1="-1.9558" y1="3.3528" x2="-1.9558" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="3.6576" x2="-2.9972" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="3.6576" x2="-2.9972" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="3.3528" x2="-1.9558" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.6924" x2="-1.9558" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.9972" x2="-2.9972" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.9972" x2="-2.9972" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.6924" x2="-1.9558" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.0828" x2="-1.9558" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.3876" x2="-2.9972" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.3876" x2="-2.9972" y2="2.0574" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.0574" x2="-1.9558" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.4224" x2="-1.9558" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.7272" x2="-2.9972" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.7272" x2="-2.9972" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.4224" x2="-1.9558" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.8128" x2="-1.9558" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.1176" x2="-2.9972" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.1176" x2="-2.9972" y2="0.7874" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.7874" x2="-1.9558" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.1524" x2="-1.9558" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.4572" x2="-2.9972" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.4572" x2="-2.9972" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.1524" x2="-1.9558" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.4572" x2="-1.9558" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.1524" x2="-2.9972" y2="-0.1778" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.1778" x2="-2.9972" y2="-0.4826" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.4826" x2="-1.9558" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.1176" x2="-1.9558" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.8128" x2="-2.9972" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.8128" x2="-2.9972" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.1176" x2="-1.9558" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.7272" x2="-1.9558" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.4224" x2="-2.9972" y2="-1.4478" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.4478" x2="-2.9972" y2="-1.7526" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.7526" x2="-1.9558" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.3876" x2="-1.9558" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.0828" x2="-2.9972" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.0828" x2="-2.9972" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.3876" x2="-1.9558" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.9972" x2="-1.9558" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.6924" x2="-2.9972" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.6924" x2="-2.9972" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.9972" x2="-1.9558" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-3.6576" x2="-1.9558" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-3.3528" x2="-2.9972" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.3528" x2="-2.9972" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.6576" x2="-1.9558" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-3.3528" x2="1.9558" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-3.6576" x2="2.9972" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-3.6576" x2="2.9972" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-3.3528" x2="1.9558" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.6924" x2="1.9558" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.9972" x2="2.9972" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.9972" x2="2.9972" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.6924" x2="1.9558" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.0828" x2="1.9558" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.3876" x2="2.9972" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.3876" x2="2.9972" y2="-2.0574" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.0574" x2="1.9558" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.4224" x2="1.9558" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.7272" x2="2.9972" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.7272" x2="2.9972" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.4224" x2="1.9558" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.8128" x2="1.9558" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.1176" x2="2.9972" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.1176" x2="2.9972" y2="-0.7874" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.7874" x2="1.9558" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.1524" x2="1.9558" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.4572" x2="2.9972" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.4572" x2="2.9972" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.1524" x2="1.9558" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.4572" x2="1.9558" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.1524" x2="2.9972" y2="0.1778" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.1778" x2="2.9972" y2="0.4826" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.4826" x2="1.9558" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.1176" x2="1.9558" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.8128" x2="2.9972" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.8128" x2="2.9972" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.1176" x2="1.9558" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.7272" x2="1.9558" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.4224" x2="2.9972" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.4224" x2="2.9972" y2="1.7526" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.7526" x2="1.9558" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.3876" x2="1.9558" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.0828" x2="2.9972" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.0828" x2="2.9972" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.3876" x2="1.9558" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.9972" x2="1.9558" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.6924" x2="2.9972" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.6924" x2="2.9972" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.9972" x2="1.9558" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="3.6576" x2="1.9558" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="3.3528" x2="2.9972" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.3528" x2="2.9972" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.6576" x2="1.9558" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-4.318" x2="1.9558" y2="-4.318" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-4.318" x2="1.9558" y2="4.318" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="4.318" x2="-0.3048" y2="4.318" width="0.1524" layer="51"/>
<wire x1="-0.3048" y1="4.318" x2="-1.9558" y2="4.318" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="4.318" x2="-1.9558" y2="-4.318" width="0.1524" layer="51"/>
<wire x1="0.3048" y1="4.3434" x2="-0.3048" y2="4.318" width="0.1524" layer="51" curve="-180"/>
<text x="-2.159" y="2.9972" size="1.27" layer="51" ratio="6" rot="SR0">*</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
</package>
<package name="SSOP24_SIL-M">
<smd name="1" x="-2.7559" y="3.4925" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="2" x="-2.7559" y="2.8575" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="3" x="-2.7559" y="2.2225" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="4" x="-2.7559" y="1.5875" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="5" x="-2.7559" y="0.9525" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="6" x="-2.7559" y="0.3175" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="7" x="-2.7559" y="-0.3175" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="8" x="-2.7559" y="-0.9525" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="9" x="-2.7559" y="-1.5875" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="10" x="-2.7559" y="-2.2225" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="11" x="-2.7559" y="-2.8575" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="12" x="-2.7559" y="-3.4925" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="13" x="2.7559" y="-3.4925" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="14" x="2.7559" y="-2.8575" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="15" x="2.7559" y="-2.2225" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="16" x="2.7559" y="-1.5875" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="17" x="2.7559" y="-0.9525" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="18" x="2.7559" y="-0.3175" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="19" x="2.7559" y="0.3175" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="20" x="2.7559" y="0.9525" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="21" x="2.7559" y="1.5875" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="22" x="2.7559" y="2.2225" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="23" x="2.7559" y="2.8575" dx="1.6002" dy="0.4064" layer="1"/>
<smd name="24" x="2.7559" y="3.4925" dx="1.6002" dy="0.4064" layer="1"/>
<wire x1="-2.0828" y1="-4.4704" x2="2.0828" y2="-4.4704" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="-4.4704" x2="2.0828" y2="-4.0386" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="4.4704" x2="-2.0828" y2="4.4704" width="0.1524" layer="21"/>
<wire x1="-2.0828" y1="4.4704" x2="-2.0828" y2="4.0386" width="0.1524" layer="21"/>
<wire x1="-2.0828" y1="-4.0386" x2="-2.0828" y2="-4.4704" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="4.0386" x2="2.0828" y2="4.4704" width="0.1524" layer="21"/>
<polygon width="0.0254" layer="21">
<vertex x="-4.064" y="-2.032"/>
<vertex x="-4.064" y="-2.413"/>
<vertex x="-3.81" y="-2.413"/>
<vertex x="-3.81" y="-2.032"/>
</polygon>
<polygon width="0.0254" layer="21">
<vertex x="4.064" y="1.143"/>
<vertex x="4.064" y="0.762"/>
<vertex x="3.81" y="0.762"/>
<vertex x="3.81" y="1.143"/>
</polygon>
<text x="-3.5814" y="3.7592" size="1.27" layer="21" ratio="6" rot="SR0">*</text>
<text x="-1.7272" y="-0.635" size="1.27" layer="21" ratio="6" rot="SR0">&gt;Value</text>
<wire x1="-1.9558" y1="3.3528" x2="-1.9558" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="3.6576" x2="-2.9972" y2="3.6322" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="3.6322" x2="-2.9972" y2="3.3274" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="3.3274" x2="-1.9558" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.6924" x2="-1.9558" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.9972" x2="-2.9972" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.9972" x2="-2.9972" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.6924" x2="-1.9558" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.0828" x2="-1.9558" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.3876" x2="-2.9972" y2="2.3622" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.3622" x2="-2.9972" y2="2.0574" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.0574" x2="-1.9558" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.4224" x2="-1.9558" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.7272" x2="-2.9972" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.7272" x2="-2.9972" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.4224" x2="-1.9558" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.8128" x2="-1.9558" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.1176" x2="-2.9972" y2="1.0922" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.0922" x2="-2.9972" y2="0.7874" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.7874" x2="-1.9558" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.1524" x2="-1.9558" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.4572" x2="-2.9972" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.4572" x2="-2.9972" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.1524" x2="-1.9558" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.4572" x2="-1.9558" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.1524" x2="-2.9972" y2="-0.1778" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.1778" x2="-2.9972" y2="-0.4826" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.4826" x2="-1.9558" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.1176" x2="-1.9558" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.8128" x2="-2.9972" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.8128" x2="-2.9972" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.1176" x2="-1.9558" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.7272" x2="-1.9558" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.4224" x2="-2.9972" y2="-1.4478" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.4478" x2="-2.9972" y2="-1.7526" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.7526" x2="-1.9558" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.3876" x2="-1.9558" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.0828" x2="-2.9972" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.0828" x2="-2.9972" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.3876" x2="-1.9558" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.9972" x2="-1.9558" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.6924" x2="-2.9972" y2="-2.7178" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.7178" x2="-2.9972" y2="-3.0226" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.0226" x2="-1.9558" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-3.6576" x2="-1.9558" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-3.3528" x2="-2.9972" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.3528" x2="-2.9972" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.6576" x2="-1.9558" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-3.3528" x2="1.9558" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-3.6576" x2="2.9972" y2="-3.6322" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-3.6322" x2="2.9972" y2="-3.3274" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-3.3274" x2="1.9558" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.6924" x2="1.9558" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.9972" x2="2.9972" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.9972" x2="2.9972" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.6924" x2="1.9558" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.0828" x2="1.9558" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.3876" x2="2.9972" y2="-2.3622" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.3622" x2="2.9972" y2="-2.0574" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.0574" x2="1.9558" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.4224" x2="1.9558" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.7272" x2="2.9972" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.7272" x2="2.9972" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.4224" x2="1.9558" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.8128" x2="1.9558" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.1176" x2="2.9972" y2="-1.0922" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.0922" x2="2.9972" y2="-0.7874" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.7874" x2="1.9558" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.1524" x2="1.9558" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.4572" x2="2.9972" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.4572" x2="2.9972" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.1524" x2="1.9558" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.4572" x2="1.9558" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.1524" x2="2.9972" y2="0.1778" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.1778" x2="2.9972" y2="0.4826" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.4826" x2="1.9558" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.1176" x2="1.9558" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.8128" x2="2.9972" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.8128" x2="2.9972" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.1176" x2="1.9558" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.7272" x2="1.9558" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.4224" x2="2.9972" y2="1.4478" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.4478" x2="2.9972" y2="1.7526" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.7526" x2="1.9558" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.3876" x2="1.9558" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.0828" x2="2.9972" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.0828" x2="2.9972" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.3876" x2="1.9558" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.9972" x2="1.9558" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.6924" x2="2.9972" y2="2.7178" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.7178" x2="2.9972" y2="3.0226" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.0226" x2="1.9558" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="3.6576" x2="1.9558" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="3.3528" x2="2.9972" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.3528" x2="2.9972" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.6576" x2="1.9558" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-4.318" x2="1.9558" y2="-4.318" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-4.318" x2="1.9558" y2="4.318" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="4.318" x2="-0.3048" y2="4.318" width="0.1524" layer="51"/>
<wire x1="-0.3048" y1="4.318" x2="-1.9558" y2="4.318" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="4.318" x2="-1.9558" y2="-4.318" width="0.1524" layer="51"/>
<wire x1="0.3048" y1="4.3434" x2="-0.3048" y2="4.318" width="0.1524" layer="51" curve="-180"/>
<text x="-2.159" y="2.9972" size="1.27" layer="51" ratio="6" rot="SR0">*</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
</package>
<package name="SSOP24_SIL-L">
<smd name="1" x="-2.5527" y="3.4925" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="2" x="-2.5527" y="2.8575" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="3" x="-2.5527" y="2.2225" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="4" x="-2.5527" y="1.5875" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="5" x="-2.5527" y="0.9525" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="6" x="-2.5527" y="0.3175" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="7" x="-2.5527" y="-0.3175" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="8" x="-2.5527" y="-0.9525" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="9" x="-2.5527" y="-1.5875" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="10" x="-2.5527" y="-2.2225" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="11" x="-2.5527" y="-2.8575" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="12" x="-2.5527" y="-3.4925" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="13" x="2.5527" y="-3.4925" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="14" x="2.5527" y="-2.8575" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="15" x="2.5527" y="-2.2225" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="16" x="2.5527" y="-1.5875" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="17" x="2.5527" y="-0.9525" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="18" x="2.5527" y="-0.3175" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="19" x="2.5527" y="0.3175" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="20" x="2.5527" y="0.9525" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="21" x="2.5527" y="1.5875" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="22" x="2.5527" y="2.2225" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="23" x="2.5527" y="2.8575" dx="1.1938" dy="0.3048" layer="1"/>
<smd name="24" x="2.5527" y="3.4925" dx="1.1938" dy="0.3048" layer="1"/>
<wire x1="-2.0828" y1="-4.4704" x2="2.0828" y2="-4.4704" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="-4.4704" x2="2.0828" y2="-3.9878" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="4.4704" x2="-2.0828" y2="4.4704" width="0.1524" layer="21"/>
<wire x1="-2.0828" y1="4.4704" x2="-2.0828" y2="3.9878" width="0.1524" layer="21"/>
<wire x1="-2.0828" y1="-3.9878" x2="-2.0828" y2="-4.4704" width="0.1524" layer="21"/>
<wire x1="2.0828" y1="3.9878" x2="2.0828" y2="4.4704" width="0.1524" layer="21"/>
<polygon width="0.0254" layer="21">
<vertex x="-3.6576" y="-2.032"/>
<vertex x="-3.6576" y="-2.413"/>
<vertex x="-3.4036" y="-2.413"/>
<vertex x="-3.4036" y="-2.032"/>
</polygon>
<polygon width="0.0254" layer="21">
<vertex x="3.6576" y="1.143"/>
<vertex x="3.6576" y="0.762"/>
<vertex x="3.4036" y="0.762"/>
<vertex x="3.4036" y="1.143"/>
</polygon>
<text x="-3.3782" y="3.6576" size="1.27" layer="21" ratio="6" rot="SR0">*</text>
<text x="-1.7272" y="-0.635" size="1.27" layer="21" ratio="6" rot="SR0">&gt;Value</text>
<wire x1="-1.9558" y1="3.3528" x2="-1.9558" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="3.6576" x2="-2.9972" y2="3.6322" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="3.6322" x2="-2.9972" y2="3.3274" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="3.3274" x2="-1.9558" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.6924" x2="-1.9558" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.9972" x2="-2.9972" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.9972" x2="-2.9972" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.6924" x2="-1.9558" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.0828" x2="-1.9558" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="2.3876" x2="-2.9972" y2="2.3622" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.3622" x2="-2.9972" y2="2.0574" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="2.0574" x2="-1.9558" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.4224" x2="-1.9558" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.7272" x2="-2.9972" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.7272" x2="-2.9972" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.4224" x2="-1.9558" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.8128" x2="-1.9558" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="1.1176" x2="-2.9972" y2="1.0922" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="1.0922" x2="-2.9972" y2="0.7874" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.7874" x2="-1.9558" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.1524" x2="-1.9558" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="0.4572" x2="-2.9972" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.4572" x2="-2.9972" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="0.1524" x2="-1.9558" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.4572" x2="-1.9558" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.1524" x2="-2.9972" y2="-0.1778" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.1778" x2="-2.9972" y2="-0.4826" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.4826" x2="-1.9558" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.1176" x2="-1.9558" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-0.8128" x2="-2.9972" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-0.8128" x2="-2.9972" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.1176" x2="-1.9558" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.7272" x2="-1.9558" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-1.4224" x2="-2.9972" y2="-1.4478" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.4478" x2="-2.9972" y2="-1.7526" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-1.7526" x2="-1.9558" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.3876" x2="-1.9558" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.0828" x2="-2.9972" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.0828" x2="-2.9972" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.3876" x2="-1.9558" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.9972" x2="-1.9558" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-2.6924" x2="-2.9972" y2="-2.7178" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-2.7178" x2="-2.9972" y2="-3.0226" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.0226" x2="-1.9558" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-3.6576" x2="-1.9558" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-3.3528" x2="-2.9972" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.3528" x2="-2.9972" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="-2.9972" y1="-3.6576" x2="-1.9558" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-3.3528" x2="1.9558" y2="-3.6576" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-3.6576" x2="2.9972" y2="-3.6322" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-3.6322" x2="2.9972" y2="-3.3274" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-3.3274" x2="1.9558" y2="-3.3528" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.6924" x2="1.9558" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.9972" x2="2.9972" y2="-2.9972" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.9972" x2="2.9972" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.6924" x2="1.9558" y2="-2.6924" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.0828" x2="1.9558" y2="-2.3876" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-2.3876" x2="2.9972" y2="-2.3622" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.3622" x2="2.9972" y2="-2.0574" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-2.0574" x2="1.9558" y2="-2.0828" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.4224" x2="1.9558" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.7272" x2="2.9972" y2="-1.7272" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.7272" x2="2.9972" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.4224" x2="1.9558" y2="-1.4224" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.8128" x2="1.9558" y2="-1.1176" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-1.1176" x2="2.9972" y2="-1.0922" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-1.0922" x2="2.9972" y2="-0.7874" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.7874" x2="1.9558" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.1524" x2="1.9558" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-0.4572" x2="2.9972" y2="-0.4572" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.4572" x2="2.9972" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="-0.1524" x2="1.9558" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.4572" x2="1.9558" y2="0.1524" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.1524" x2="2.9972" y2="0.1778" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.1778" x2="2.9972" y2="0.4826" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.4826" x2="1.9558" y2="0.4572" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.1176" x2="1.9558" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="0.8128" x2="2.9972" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="0.8128" x2="2.9972" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.1176" x2="1.9558" y2="1.1176" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.7272" x2="1.9558" y2="1.4224" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="1.4224" x2="2.9972" y2="1.4478" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.4478" x2="2.9972" y2="1.7526" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="1.7526" x2="1.9558" y2="1.7272" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.3876" x2="1.9558" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.0828" x2="2.9972" y2="2.0828" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.0828" x2="2.9972" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.3876" x2="1.9558" y2="2.3876" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.9972" x2="1.9558" y2="2.6924" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="2.6924" x2="2.9972" y2="2.7178" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="2.7178" x2="2.9972" y2="3.0226" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.0226" x2="1.9558" y2="2.9972" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="3.6576" x2="1.9558" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="3.3528" x2="2.9972" y2="3.3528" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.3528" x2="2.9972" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="2.9972" y1="3.6576" x2="1.9558" y2="3.6576" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="-4.318" x2="1.9558" y2="-4.318" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="-4.318" x2="1.9558" y2="4.318" width="0.1524" layer="51"/>
<wire x1="1.9558" y1="4.318" x2="-0.3048" y2="4.318" width="0.1524" layer="51"/>
<wire x1="-0.3048" y1="4.318" x2="-1.9558" y2="4.318" width="0.1524" layer="51"/>
<wire x1="-1.9558" y1="4.318" x2="-1.9558" y2="-4.318" width="0.1524" layer="51"/>
<wire x1="0.3048" y1="4.3434" x2="-0.3048" y2="4.318" width="0.1524" layer="51" curve="-180"/>
<text x="-2.159" y="2.9972" size="1.27" layer="51" ratio="6" rot="SR0">*</text>
<text x="-3.2766" y="-0.635" size="1.27" layer="25" ratio="6" rot="SR0">&gt;Name</text>
</package>
</packages>
<symbols>
<symbol name="SI4735-D60-GU">
<pin name="DOUT" x="2.54" y="0" length="middle" direction="out"/>
<pin name="DFS" x="2.54" y="-2.54" length="middle" direction="in"/>
<pin name="GPO3/[DCLK]" x="2.54" y="-5.08" length="middle"/>
<pin name="GPO2/[INT]" x="2.54" y="-7.62" length="middle"/>
<pin name="GPO1" x="2.54" y="-10.16" length="middle" direction="out"/>
<pin name="NC_2" x="2.54" y="-12.7" length="middle" direction="nc"/>
<pin name="NC_3" x="2.54" y="-15.24" length="middle" direction="nc"/>
<pin name="FMI" x="2.54" y="-17.78" length="middle" direction="in"/>
<pin name="RFGND" x="2.54" y="-20.32" length="middle" direction="pas"/>
<pin name="NC_4" x="2.54" y="-22.86" length="middle" direction="nc"/>
<pin name="NC" x="2.54" y="-25.4" length="middle" direction="nc"/>
<pin name="AMI" x="2.54" y="-27.94" length="middle" direction="in"/>
<pin name="GND_2" x="68.58" y="-27.94" length="middle" direction="pas" rot="R180"/>
<pin name="GND" x="68.58" y="-25.4" length="middle" direction="pas" rot="R180"/>
<pin name="!RST" x="68.58" y="-22.86" length="middle" direction="in" rot="R180"/>
<pin name="!SEN" x="68.58" y="-20.32" length="middle" direction="in" rot="R180"/>
<pin name="SCLK" x="68.58" y="-17.78" length="middle" direction="in" rot="R180"/>
<pin name="SDIO" x="68.58" y="-15.24" length="middle" rot="R180"/>
<pin name="RCLK" x="68.58" y="-12.7" length="middle" direction="in" rot="R180"/>
<pin name="VD" x="68.58" y="-10.16" length="middle" direction="pwr" rot="R180"/>
<pin name="VA" x="68.58" y="-7.62" length="middle" direction="pwr" rot="R180"/>
<pin name="DBYP" x="68.58" y="-5.08" length="middle" direction="pas" rot="R180"/>
<pin name="ROUT/[DOUT]" x="68.58" y="-2.54" length="middle" direction="out" rot="R180"/>
<pin name="LOUT/[DFS]" x="68.58" y="0" length="middle" direction="out" rot="R180"/>
<wire x1="7.62" y1="5.08" x2="7.62" y2="-33.02" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-33.02" x2="63.5" y2="-33.02" width="0.1524" layer="94"/>
<wire x1="63.5" y1="-33.02" x2="63.5" y2="5.08" width="0.1524" layer="94"/>
<wire x1="63.5" y1="5.08" x2="7.62" y2="5.08" width="0.1524" layer="94"/>
<text x="30.8356" y="9.1186" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="30.2006" y="6.5786" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="SI4735-D60-GU" prefix="U">
<gates>
<gate name="A" symbol="SI4735-D60-GU" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SSOP24_SIL">
<connects>
<connect gate="A" pin="!RST" pad="15"/>
<connect gate="A" pin="!SEN" pad="16"/>
<connect gate="A" pin="AMI" pad="12"/>
<connect gate="A" pin="DBYP" pad="22"/>
<connect gate="A" pin="DFS" pad="2"/>
<connect gate="A" pin="DOUT" pad="1"/>
<connect gate="A" pin="FMI" pad="8"/>
<connect gate="A" pin="GND" pad="14"/>
<connect gate="A" pin="GND_2" pad="13"/>
<connect gate="A" pin="GPO1" pad="5"/>
<connect gate="A" pin="GPO2/[INT]" pad="4"/>
<connect gate="A" pin="GPO3/[DCLK]" pad="3"/>
<connect gate="A" pin="LOUT/[DFS]" pad="24"/>
<connect gate="A" pin="NC" pad="11"/>
<connect gate="A" pin="NC_2" pad="6"/>
<connect gate="A" pin="NC_3" pad="7"/>
<connect gate="A" pin="NC_4" pad="10"/>
<connect gate="A" pin="RCLK" pad="19"/>
<connect gate="A" pin="RFGND" pad="9"/>
<connect gate="A" pin="ROUT/[DOUT]" pad="23"/>
<connect gate="A" pin="SCLK" pad="17"/>
<connect gate="A" pin="SDIO" pad="18"/>
<connect gate="A" pin="VA" pad="21"/>
<connect gate="A" pin="VD" pad="20"/>
</connects>
<technologies>
<technology name="">
<attribute name="BUILT_BY" value="EMA_Cory" constant="no"/>
<attribute name="COPYRIGHT" value="Copyright (C) 2018 Accelerated Designs. All rights reserved" constant="no"/>
<attribute name="DATASHEET" value="https://www.silabs.com/documents/public/data-sheets/Si4730-31-34-35-D60.pdf" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="SI4735-D60-GU" constant="no"/>
<attribute name="SOURCELIBRARY" value="Silicon_Labs_2020-04-23" constant="no"/>
<attribute name="VENDOR" value="Silicon Labs" constant="no"/>
</technology>
</technologies>
</device>
<device name="SSOP24_SIL-M" package="SSOP24_SIL-M">
<connects>
<connect gate="A" pin="!RST" pad="15"/>
<connect gate="A" pin="!SEN" pad="16"/>
<connect gate="A" pin="AMI" pad="12"/>
<connect gate="A" pin="DBYP" pad="22"/>
<connect gate="A" pin="DFS" pad="2"/>
<connect gate="A" pin="DOUT" pad="1"/>
<connect gate="A" pin="FMI" pad="8"/>
<connect gate="A" pin="GND" pad="14"/>
<connect gate="A" pin="GND_2" pad="13"/>
<connect gate="A" pin="GPO1" pad="5"/>
<connect gate="A" pin="GPO2/[INT]" pad="4"/>
<connect gate="A" pin="GPO3/[DCLK]" pad="3"/>
<connect gate="A" pin="LOUT/[DFS]" pad="24"/>
<connect gate="A" pin="NC" pad="11"/>
<connect gate="A" pin="NC_2" pad="6"/>
<connect gate="A" pin="NC_3" pad="7"/>
<connect gate="A" pin="NC_4" pad="10"/>
<connect gate="A" pin="RCLK" pad="19"/>
<connect gate="A" pin="RFGND" pad="9"/>
<connect gate="A" pin="ROUT/[DOUT]" pad="23"/>
<connect gate="A" pin="SCLK" pad="17"/>
<connect gate="A" pin="SDIO" pad="18"/>
<connect gate="A" pin="VA" pad="21"/>
<connect gate="A" pin="VD" pad="20"/>
</connects>
<technologies>
<technology name="">
<attribute name="BUILT_BY" value="EMA_Cory" constant="no"/>
<attribute name="COPYRIGHT" value="Copyright (C) 2018 Accelerated Designs. All rights reserved" constant="no"/>
<attribute name="DATASHEET" value="https://www.silabs.com/documents/public/data-sheets/Si4730-31-34-35-D60.pdf" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="SI4735-D60-GU" constant="no"/>
<attribute name="SOURCELIBRARY" value="Silicon_Labs_2020-04-23" constant="no"/>
<attribute name="VENDOR" value="Silicon Labs" constant="no"/>
</technology>
</technologies>
</device>
<device name="SSOP24_SIL-L" package="SSOP24_SIL-L">
<connects>
<connect gate="A" pin="!RST" pad="15"/>
<connect gate="A" pin="!SEN" pad="16"/>
<connect gate="A" pin="AMI" pad="12"/>
<connect gate="A" pin="DBYP" pad="22"/>
<connect gate="A" pin="DFS" pad="2"/>
<connect gate="A" pin="DOUT" pad="1"/>
<connect gate="A" pin="FMI" pad="8"/>
<connect gate="A" pin="GND" pad="14"/>
<connect gate="A" pin="GND_2" pad="13"/>
<connect gate="A" pin="GPO1" pad="5"/>
<connect gate="A" pin="GPO2/[INT]" pad="4"/>
<connect gate="A" pin="GPO3/[DCLK]" pad="3"/>
<connect gate="A" pin="LOUT/[DFS]" pad="24"/>
<connect gate="A" pin="NC" pad="11"/>
<connect gate="A" pin="NC_2" pad="6"/>
<connect gate="A" pin="NC_3" pad="7"/>
<connect gate="A" pin="NC_4" pad="10"/>
<connect gate="A" pin="RCLK" pad="19"/>
<connect gate="A" pin="RFGND" pad="9"/>
<connect gate="A" pin="ROUT/[DOUT]" pad="23"/>
<connect gate="A" pin="SCLK" pad="17"/>
<connect gate="A" pin="SDIO" pad="18"/>
<connect gate="A" pin="VA" pad="21"/>
<connect gate="A" pin="VD" pad="20"/>
</connects>
<technologies>
<technology name="">
<attribute name="BUILT_BY" value="EMA_Cory" constant="no"/>
<attribute name="COPYRIGHT" value="Copyright (C) 2018 Accelerated Designs. All rights reserved" constant="no"/>
<attribute name="DATASHEET" value="https://www.silabs.com/documents/public/data-sheets/Si4730-31-34-35-D60.pdf" constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="SI4735-D60-GU" constant="no"/>
<attribute name="SOURCELIBRARY" value="Silicon_Labs_2020-04-23" constant="no"/>
<attribute name="VENDOR" value="Silicon Labs" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1" urn="urn:adsk.eagle:library:371">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND" urn="urn:adsk.eagle:symbol:26925/1" library_version="1">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" urn="urn:adsk.eagle:component:26954/1" prefix="GND" library_version="1">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Resistors" urn="urn:adsk.eagle:library:532">
<description>&lt;h3&gt;SparkFun Resistors&lt;/h3&gt;
This library contains resistors. Reference designator:R. 
&lt;br&gt;
&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is &lt;b&gt; the end user's responsibility&lt;/b&gt; to ensure correctness and suitablity for a given componet or application. 
&lt;br&gt;
&lt;br&gt;If you enjoy using this library, please buy one of our products at &lt;a href=" www.sparkfun.com"&gt;SparkFun.com&lt;/a&gt;.
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;
&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="0603" urn="urn:adsk.eagle:footprint:39615/1" library_version="1">
<description>&lt;p&gt;&lt;b&gt;Generic 1608 (0603) package&lt;/b&gt;&lt;/p&gt;
&lt;p&gt;0.2mm courtyard excess rounded to nearest 0.05mm.&lt;/p&gt;</description>
<wire x1="-1.6" y1="0.7" x2="1.6" y2="0.7" width="0.0508" layer="39"/>
<wire x1="1.6" y1="0.7" x2="1.6" y2="-0.7" width="0.0508" layer="39"/>
<wire x1="1.6" y1="-0.7" x2="-1.6" y2="-0.7" width="0.0508" layer="39"/>
<wire x1="-1.6" y1="-0.7" x2="-1.6" y2="0.7" width="0.0508" layer="39"/>
<wire x1="-0.356" y1="0.432" x2="0.356" y2="0.432" width="0.1016" layer="51"/>
<wire x1="-0.356" y1="-0.419" x2="0.356" y2="-0.419" width="0.1016" layer="51"/>
<smd name="1" x="-0.85" y="0" dx="1.1" dy="1" layer="1"/>
<smd name="2" x="0.85" y="0" dx="1.1" dy="1" layer="1"/>
<text x="0" y="0.762" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-0.762" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
<rectangle x1="-0.8382" y1="-0.4699" x2="-0.3381" y2="0.4801" layer="51"/>
<rectangle x1="0.3302" y1="-0.4699" x2="0.8303" y2="0.4801" layer="51"/>
<rectangle x1="-0.1999" y1="-0.3" x2="0.1999" y2="0.3" layer="35"/>
</package>
</packages>
<packages3d>
<package3d name="0603" urn="urn:adsk.eagle:package:39650/1" type="box" library_version="1">
<description>Generic 1608 (0603) package
0.2mm courtyard excess rounded to nearest 0.05mm.</description>
<packageinstances>
<packageinstance name="0603"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="RESISTOR" urn="urn:adsk.eagle:symbol:39614/1" library_version="1">
<wire x1="-2.54" y1="0" x2="-2.159" y2="1.016" width="0.1524" layer="94"/>
<wire x1="-2.159" y1="1.016" x2="-1.524" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="-1.524" y1="-1.016" x2="-0.889" y2="1.016" width="0.1524" layer="94"/>
<wire x1="-0.889" y1="1.016" x2="-0.254" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="-0.254" y1="-1.016" x2="0.381" y2="1.016" width="0.1524" layer="94"/>
<wire x1="0.381" y1="1.016" x2="1.016" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="1.016" y1="-1.016" x2="1.651" y2="1.016" width="0.1524" layer="94"/>
<wire x1="1.651" y1="1.016" x2="2.286" y2="-1.016" width="0.1524" layer="94"/>
<wire x1="2.286" y1="-1.016" x2="2.54" y2="0" width="0.1524" layer="94"/>
<text x="0" y="1.524" size="1.778" layer="95" font="vector" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.524" size="1.778" layer="96" font="vector" align="top-center">&gt;VALUE</text>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="4.7KOHM" urn="urn:adsk.eagle:component:39723/1" prefix="R" library_version="1">
<description>&lt;h3&gt;4.7kΩ resistor&lt;/h3&gt;
&lt;p&gt;A resistor is a passive two-terminal electrical component that implements electrical resistance as a circuit element. Resistors act to reduce current flow, and, at the same time, act to lower voltage levels within circuits. - Wikipedia&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="-0603-1/10W-1%" package="0603">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:39650/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="RES-07857"/>
<attribute name="VALUE" value="4.7k"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="2.2KOHM" urn="urn:adsk.eagle:component:39698/1" prefix="R" library_version="1">
<description>&lt;h3&gt;2.2kΩ resistor&lt;/h3&gt;
&lt;p&gt;A resistor is a passive two-terminal electrical component that implements electrical resistance as a circuit element. Resistors act to reduce current flow, and, at the same time, act to lower voltage levels within circuits. - Wikipedia&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="-0603-1/10W-1%" package="0603">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:39650/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="RES-08272"/>
<attribute name="VALUE" value="2.2k"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply2" urn="urn:adsk.eagle:library:372">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
Please keep in mind, that these devices are necessary for the
automatic wiring of the supply signals.&lt;p&gt;
The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="VCC" urn="urn:adsk.eagle:symbol:26997/1" library_version="2">
<circle x="0" y="1.27" radius="1.27" width="0.254" layer="94"/>
<text x="-1.905" y="3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="VCC" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="VCC" urn="urn:adsk.eagle:component:27060/1" prefix="SUPPLY" library_version="2">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="VCC" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Capacitors" urn="urn:adsk.eagle:library:510">
<description>&lt;h3&gt;SparkFun Capacitors&lt;/h3&gt;
This library contains capacitors. 
&lt;br&gt;
&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is &lt;b&gt; the end user's responsibility&lt;/b&gt; to ensure correctness and suitablity for a given componet or application. 
&lt;br&gt;
&lt;br&gt;If you enjoy using this library, please buy one of our products at &lt;a href=" www.sparkfun.com"&gt;SparkFun.com&lt;/a&gt;.
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;
&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="0603" urn="urn:adsk.eagle:footprint:37386/1" library_version="1">
<description>&lt;p&gt;&lt;b&gt;Generic 1608 (0603) package&lt;/b&gt;&lt;/p&gt;
&lt;p&gt;0.2mm courtyard excess rounded to nearest 0.05mm.&lt;/p&gt;</description>
<wire x1="-1.6" y1="0.7" x2="1.6" y2="0.7" width="0.0508" layer="39"/>
<wire x1="1.6" y1="0.7" x2="1.6" y2="-0.7" width="0.0508" layer="39"/>
<wire x1="1.6" y1="-0.7" x2="-1.6" y2="-0.7" width="0.0508" layer="39"/>
<wire x1="-1.6" y1="-0.7" x2="-1.6" y2="0.7" width="0.0508" layer="39"/>
<wire x1="-0.356" y1="0.432" x2="0.356" y2="0.432" width="0.1016" layer="51"/>
<wire x1="-0.356" y1="-0.419" x2="0.356" y2="-0.419" width="0.1016" layer="51"/>
<smd name="1" x="-0.85" y="0" dx="1.1" dy="1" layer="1"/>
<smd name="2" x="0.85" y="0" dx="1.1" dy="1" layer="1"/>
<text x="0" y="0.762" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-0.762" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
<rectangle x1="-0.8382" y1="-0.4699" x2="-0.3381" y2="0.4801" layer="51"/>
<rectangle x1="0.3302" y1="-0.4699" x2="0.8303" y2="0.4801" layer="51"/>
<rectangle x1="-0.1999" y1="-0.3" x2="0.1999" y2="0.3" layer="35"/>
</package>
<package name="CAP-PTH-2.54" urn="urn:adsk.eagle:footprint:37402/1" library_version="1">
<description>2 PTH spaced 0.1" apart</description>
<wire x1="0" y1="0.635" x2="0" y2="-0.635" width="0.2032" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="0.7" diameter="1.651"/>
<pad name="2" x="1.27" y="0" drill="0.7" diameter="1.651"/>
<text x="0" y="1.016" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;Name</text>
<text x="0" y="-1.016" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;Value</text>
</package>
<package name="1206" urn="urn:adsk.eagle:footprint:37399/1" library_version="1">
<description>&lt;p&gt;&lt;b&gt;Generic 3216 (1206) package&lt;/b&gt;&lt;/p&gt;
&lt;p&gt;0.2mm courtyard excess rounded to nearest 0.05mm.&lt;/p&gt;</description>
<wire x1="-2.4" y1="1.1" x2="2.4" y2="1.1" width="0.0508" layer="39"/>
<wire x1="2.4" y1="-1.1" x2="-2.4" y2="-1.1" width="0.0508" layer="39"/>
<wire x1="-2.4" y1="-1.1" x2="-2.4" y2="1.1" width="0.0508" layer="39"/>
<wire x1="2.4" y1="1.1" x2="2.4" y2="-1.1" width="0.0508" layer="39"/>
<wire x1="-0.965" y1="0.787" x2="0.965" y2="0.787" width="0.1016" layer="51"/>
<wire x1="-0.965" y1="-0.787" x2="0.965" y2="-0.787" width="0.1016" layer="51"/>
<smd name="1" x="-1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<smd name="2" x="1.4" y="0" dx="1.6" dy="1.8" layer="1"/>
<text x="0" y="1.143" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.143" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
<rectangle x1="-1.7018" y1="-0.8509" x2="-0.9517" y2="0.8491" layer="51"/>
<rectangle x1="0.9517" y1="-0.8491" x2="1.7018" y2="0.8509" layer="51"/>
<rectangle x1="-0.1999" y1="-0.4001" x2="0.1999" y2="0.4001" layer="35"/>
</package>
</packages>
<packages3d>
<package3d name="0603" urn="urn:adsk.eagle:package:37414/1" type="box" library_version="1">
<description>Generic 1608 (0603) package
0.2mm courtyard excess rounded to nearest 0.05mm.</description>
<packageinstances>
<packageinstance name="0603"/>
</packageinstances>
</package3d>
<package3d name="CAP-PTH-2.54" urn="urn:adsk.eagle:package:37435/1" type="box" library_version="1">
<description>2 PTH spaced 0.1" apart</description>
<packageinstances>
<packageinstance name="CAP-PTH-2.54"/>
</packageinstances>
</package3d>
<package3d name="1206" urn="urn:adsk.eagle:package:37426/1" type="box" library_version="1">
<description>Generic 3216 (1206) package
0.2mm courtyard excess rounded to nearest 0.05mm.</description>
<packageinstances>
<packageinstance name="1206"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="CAP" urn="urn:adsk.eagle:symbol:37385/1" library_version="1">
<wire x1="0" y1="2.54" x2="0" y2="2.032" width="0.1524" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="0.508" width="0.1524" layer="94"/>
<text x="1.524" y="2.921" size="1.778" layer="95" font="vector">&gt;NAME</text>
<text x="1.524" y="-2.159" size="1.778" layer="96" font="vector">&gt;VALUE</text>
<rectangle x1="-2.032" y1="0.508" x2="2.032" y2="1.016" layer="94"/>
<rectangle x1="-2.032" y1="1.524" x2="2.032" y2="2.032" layer="94"/>
<pin name="1" x="0" y="5.08" visible="off" length="short" direction="pas" swaplevel="1" rot="R270"/>
<pin name="2" x="0" y="-2.54" visible="off" length="short" direction="pas" swaplevel="1" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="22NF/22,000PF" urn="urn:adsk.eagle:component:37445/1" prefix="C" library_version="1">
<description>&lt;h3&gt;0.022uF/22nF/22,000pF ceramic capacitors&lt;/h3&gt;
&lt;p&gt;A capacitor is a passive two-terminal electrical component used to store electrical energy temporarily in an electric field.&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="CAP" x="0" y="0"/>
</gates>
<devices>
<device name="-0603-50V-10%" package="0603">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37414/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="CAP-07885" constant="no"/>
<attribute name="VALUE" value="22nF" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="22PF" urn="urn:adsk.eagle:component:37477/1" prefix="C" library_version="1">
<description>&lt;h3&gt;22pF ceramic capacitors&lt;/h3&gt;
&lt;p&gt;A capacitor is a passive two-terminal electrical component used to store electrical energy temporarily in an electric field.&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="CAP" x="0" y="0"/>
</gates>
<devices>
<device name="-0603-50V-5%" package="0603">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37414/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="CAP-07876"/>
<attribute name="VALUE" value="22pF"/>
</technology>
</technologies>
</device>
<device name="-PTH-2.54MM-200V-5%" package="CAP-PTH-2.54">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37435/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="CAP-09128"/>
<attribute name="VALUE" value="22pF"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="4.7UF" urn="urn:adsk.eagle:component:37469/1" prefix="C" library_version="1">
<description>&lt;h3&gt;4.7µF ceramic capacitors&lt;/h3&gt;
&lt;p&gt;A capacitor is a passive two-terminal electrical component used to store electrical energy temporarily in an electric field.&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="CAP" x="0" y="-2.54"/>
</gates>
<devices>
<device name="0603" package="0603">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37414/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="CAP-08280"/>
<attribute name="VALUE" value="4.7uF"/>
</technology>
</technologies>
</device>
<device name="-1206-16V-(+80/-20%)" package="1206">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37426/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="CAP-10300" constant="no"/>
<attribute name="VALUE" value="4.7uF" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Clocks" urn="urn:adsk.eagle:library:511">
<description>&lt;h3&gt;SparkFun Clocks, Oscillators and Resonators&lt;/h3&gt;
This library contains the real-time clocks, oscillators, resonators, and crystals we use. 
&lt;br&gt;
&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is &lt;b&gt; the end user's responsibility&lt;/b&gt; to ensure correctness and suitablity for a given componet or application. 
&lt;br&gt;
&lt;br&gt;If you enjoy using this library, please buy one of our products at &lt;a href=" www.sparkfun.com"&gt;SparkFun.com&lt;/a&gt;.
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;
&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="HC49U" urn="urn:adsk.eagle:footprint:37481/1" library_version="1">
<description>&lt;h3&gt;HC49/U 11.6x4.6mm PTH Crystal (13.46mm height)&lt;/h3&gt;
&lt;p&gt;&lt;a href="https://www.digikey.com/Web%20Export/Supplier%20Content/Citizen_300/PDF/Citizen_HC49US.pdf?redirected=1"&gt;Example Datasheet&lt;/a&gt;&lt;/p&gt;</description>
<wire x1="-2.921" y1="-2.286" x2="2.921" y2="-2.286" width="0.4064" layer="21"/>
<wire x1="-2.921" y1="2.286" x2="2.921" y2="2.286" width="0.4064" layer="21"/>
<wire x1="-2.921" y1="-1.778" x2="2.921" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.778" x2="-2.921" y2="1.778" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.778" x2="2.921" y2="-1.778" width="0.1524" layer="21" curve="-180"/>
<wire x1="2.921" y1="2.286" x2="2.921" y2="-2.286" width="0.4064" layer="21" curve="-180"/>
<wire x1="-2.921" y1="2.286" x2="-2.921" y2="-2.286" width="0.4064" layer="21" curve="180"/>
<wire x1="-2.921" y1="1.778" x2="-2.921" y2="-1.778" width="0.1524" layer="21" curve="180"/>
<wire x1="-0.254" y1="0.889" x2="0.254" y2="0.889" width="0.1524" layer="21"/>
<wire x1="0.254" y1="0.889" x2="0.254" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-0.889" x2="-0.254" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-0.889" x2="-0.254" y2="0.889" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0.889" x2="0.635" y2="0" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0" x2="0.635" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0.889" x2="-0.635" y2="0" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0" x2="-0.635" y2="-0.889" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0" x2="1.27" y2="0" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0" x2="-1.27" y2="0" width="0.1524" layer="21"/>
<pad name="1" x="-2.413" y="0" drill="0.8128"/>
<pad name="2" x="2.413" y="0" drill="0.8128"/>
<text x="0" y="2.667" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.667" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
<rectangle x1="-3.81" y1="-2.794" x2="3.81" y2="2.794" layer="43"/>
<rectangle x1="-4.318" y1="-2.54" x2="-3.81" y2="2.54" layer="43"/>
<rectangle x1="-4.826" y1="-2.286" x2="-4.318" y2="2.286" layer="43"/>
<rectangle x1="-5.334" y1="-1.778" x2="-4.826" y2="1.778" layer="43"/>
<rectangle x1="-5.588" y1="-1.27" x2="-5.334" y2="1.016" layer="43"/>
<rectangle x1="3.81" y1="-2.54" x2="4.318" y2="2.54" layer="43"/>
<rectangle x1="4.318" y1="-2.286" x2="4.826" y2="2.286" layer="43"/>
<rectangle x1="4.826" y1="-1.778" x2="5.334" y2="1.778" layer="43"/>
<rectangle x1="5.334" y1="-1.016" x2="5.588" y2="1.016" layer="43"/>
</package>
<package name="CRYSTAL-PTH-3X8-CYL" urn="urn:adsk.eagle:footprint:37482/1" library_version="1">
<description>&lt;h3&gt;3x8mm Cylindrical Can (Radial) PTH Crystal&lt;/h3&gt;

This is the "KIT" version, which has limited top masking for improved ease of assembly.

&lt;p&gt;Example product: &lt;a href="https://www.sparkfun.com/products/540"&gt;32kHz crystal&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href="http://www.ecsxtal.com/store/pdf/ECS-3x8.pdf"&gt;Example datasheet&lt;/a&gt; (ECS-3X8)&lt;/p&gt;</description>
<wire x1="-1.397" y1="1.651" x2="1.397" y2="1.651" width="0.1524" layer="21"/>
<wire x1="1.27" y1="9.906" x2="1.524" y2="9.652" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.524" y1="9.652" x2="-1.27" y2="9.906" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.27" y1="9.906" x2="1.27" y2="9.906" width="0.1524" layer="21"/>
<wire x1="1.397" y1="1.651" x2="1.397" y2="2.032" width="0.1524" layer="21"/>
<wire x1="1.524" y1="2.032" x2="1.397" y2="2.032" width="0.1524" layer="21"/>
<wire x1="1.524" y1="2.032" x2="1.524" y2="9.652" width="0.1524" layer="21"/>
<wire x1="-1.397" y1="1.651" x2="-1.397" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="2.032" x2="-1.397" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-1.524" y1="2.032" x2="-1.524" y2="9.652" width="0.1524" layer="21"/>
<wire x1="1.397" y1="2.032" x2="-1.397" y2="2.032" width="0.1524" layer="21"/>
<wire x1="0.5588" y1="0.7112" x2="0.508" y2="0.762" width="0.4064" layer="21"/>
<wire x1="0.508" y1="0.762" x2="0.508" y2="1.143" width="0.4064" layer="21"/>
<wire x1="-0.508" y1="0.762" x2="-0.508" y2="1.016" width="0.4064" layer="21"/>
<wire x1="-0.5588" y1="0.7112" x2="-0.508" y2="0.762" width="0.4064" layer="21"/>
<wire x1="0.635" y1="0.635" x2="1.27" y2="0" width="0.1524" layer="51"/>
<wire x1="-0.635" y1="0.635" x2="-1.27" y2="0" width="0.1524" layer="51"/>
<wire x1="-0.762" y1="5.588" x2="-0.762" y2="5.207" width="0.1524" layer="21"/>
<wire x1="0.762" y1="5.207" x2="-0.762" y2="5.207" width="0.1524" layer="21"/>
<wire x1="0.762" y1="5.207" x2="0.762" y2="5.588" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="5.588" x2="0.762" y2="5.588" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="5.969" x2="0" y2="5.969" width="0.1524" layer="21"/>
<wire x1="-0.762" y1="4.826" x2="0" y2="4.826" width="0.1524" layer="21"/>
<wire x1="0" y1="4.826" x2="0" y2="4.318" width="0.1524" layer="21"/>
<wire x1="0" y1="4.826" x2="0.762" y2="4.826" width="0.1524" layer="21"/>
<wire x1="0" y1="5.969" x2="0" y2="6.477" width="0.1524" layer="21"/>
<wire x1="0" y1="5.969" x2="0.762" y2="5.969" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" diameter="1.6764"/>
<pad name="2" x="1.27" y="0" drill="0.8128" diameter="1.6764"/>
<rectangle x1="0.3048" y1="1.016" x2="0.7112" y2="1.6002" layer="21"/>
<rectangle x1="-0.7112" y1="1.016" x2="-0.3048" y2="1.6002" layer="21"/>
<rectangle x1="-1.778" y1="1.016" x2="1.778" y2="10.414" layer="43"/>
<text x="-1.778" y="5.334" size="0.6096" layer="25" font="vector" ratio="20" rot="R90" align="bottom-center">&gt;NAME</text>
<text x="1.778" y="5.334" size="0.6096" layer="27" font="vector" ratio="20" rot="R90" align="top-center">&gt;VALUE</text>
</package>
<package name="CRYSTAL-PTH-2X6-CYL" urn="urn:adsk.eagle:footprint:37483/1" library_version="1">
<description>&lt;h3&gt;2x6mm Cylindrical Can (Radial) PTH Crystal&lt;/h3&gt;

&lt;p&gt;Example product: &lt;a href="https://www.sparkfun.com/products/540"&gt;32kHz crystal&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href="http://www.ecsxtal.com/store/pdf/ECS-3x8.pdf"&gt;Example datasheet&lt;/a&gt; (ECS-2X6)&lt;/p&gt;</description>
<wire x1="-0.889" y1="1.651" x2="0.889" y2="1.651" width="0.1524" layer="21"/>
<wire x1="0.762" y1="7.747" x2="1.016" y2="7.493" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.016" y1="7.493" x2="-0.762" y2="7.747" width="0.1524" layer="21" curve="-90"/>
<wire x1="-0.762" y1="7.747" x2="0.762" y2="7.747" width="0.1524" layer="21"/>
<wire x1="0.889" y1="1.651" x2="0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="1.016" y1="2.032" x2="1.016" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="1.651" x2="-0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-1.016" y1="2.032" x2="-0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-1.016" y1="2.032" x2="-1.016" y2="7.493" width="0.1524" layer="21"/>
<wire x1="0.508" y1="0.762" x2="0.508" y2="1.143" width="0.4064" layer="21"/>
<wire x1="-0.508" y1="0.762" x2="-0.508" y2="1.27" width="0.4064" layer="21"/>
<wire x1="0.635" y1="0.635" x2="1.27" y2="0" width="0.4064" layer="51"/>
<wire x1="-0.635" y1="0.635" x2="-1.27" y2="0" width="0.4064" layer="51"/>
<wire x1="-0.508" y1="4.953" x2="-0.508" y2="4.572" width="0.1524" layer="21"/>
<wire x1="0.508" y1="4.572" x2="-0.508" y2="4.572" width="0.1524" layer="21"/>
<wire x1="0.508" y1="4.572" x2="0.508" y2="4.953" width="0.1524" layer="21"/>
<wire x1="-0.508" y1="4.953" x2="0.508" y2="4.953" width="0.1524" layer="21"/>
<wire x1="-0.508" y1="5.334" x2="0" y2="5.334" width="0.1524" layer="21"/>
<wire x1="-0.508" y1="4.191" x2="0" y2="4.191" width="0.1524" layer="21"/>
<wire x1="0" y1="4.191" x2="0" y2="3.683" width="0.1524" layer="21"/>
<wire x1="0" y1="4.191" x2="0.508" y2="4.191" width="0.1524" layer="21"/>
<wire x1="0" y1="5.334" x2="0" y2="5.842" width="0.1524" layer="21"/>
<wire x1="0" y1="5.334" x2="0.508" y2="5.334" width="0.1524" layer="21"/>
<wire x1="1.016" y1="2.032" x2="0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="0.889" y1="2.032" x2="-0.889" y2="2.032" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" diameter="1.6764"/>
<pad name="2" x="1.27" y="0" drill="0.8128" diameter="1.6764"/>
<rectangle x1="0.3048" y1="1.016" x2="0.7112" y2="1.6002" layer="21"/>
<rectangle x1="-0.7112" y1="1.016" x2="-0.3048" y2="1.6002" layer="21"/>
<rectangle x1="-1.778" y1="0.762" x2="1.778" y2="8.382" layer="43"/>
<text x="-1.27" y="4.572" size="0.6096" layer="25" font="vector" ratio="20" rot="R90" align="bottom-center">&gt;NAME</text>
<text x="1.27" y="4.572" size="0.6096" layer="27" font="vector" ratio="20" rot="R90" align="top-center">&gt;VALUE</text>
</package>
<package name="HC49UP" urn="urn:adsk.eagle:footprint:37484/1" library_version="1">
<description>&lt;h3&gt;HC-49/UP 11.4x4.8mm SMD Crystal&lt;/h3&gt;
&lt;p&gt;&lt;a href="http://www.standardcrystalcorp.com/pdf%5Cc-3.pdf"&gt;Example Datasheet&lt;/a&gt;&lt;/p&gt;</description>
<wire x1="-5.1091" y1="1.143" x2="-3.429" y2="2.0321" width="0.0508" layer="21" curve="-55.770993" cap="flat"/>
<wire x1="-5.715" y1="1.143" x2="-5.715" y2="2.159" width="0.1524" layer="21"/>
<wire x1="3.429" y1="2.032" x2="5.1091" y2="1.143" width="0.0508" layer="21" curve="-55.772485" cap="flat"/>
<wire x1="5.715" y1="1.143" x2="5.715" y2="2.159" width="0.1524" layer="21"/>
<wire x1="3.429" y1="-1.27" x2="-3.429" y2="-1.27" width="0.0508" layer="21"/>
<wire x1="3.429" y1="-2.032" x2="-3.429" y2="-2.032" width="0.0508" layer="21"/>
<wire x1="-3.429" y1="1.27" x2="3.429" y2="1.27" width="0.0508" layer="21"/>
<wire x1="5.461" y1="-2.413" x2="-5.461" y2="-2.413" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-0.381" x2="6.477" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="5.715" y1="0.381" x2="6.477" y2="0.381" width="0.1524" layer="51"/>
<wire x1="6.477" y1="-0.381" x2="6.477" y2="0.381" width="0.1524" layer="51"/>
<wire x1="5.461" y1="-2.413" x2="5.715" y2="-2.159" width="0.1524" layer="21" curve="90"/>
<wire x1="5.715" y1="-1.143" x2="5.715" y2="1.143" width="0.1524" layer="51"/>
<wire x1="5.715" y1="-2.159" x2="5.715" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="3.429" y1="-1.27" x2="3.9826" y2="-1.143" width="0.0508" layer="21" curve="25.842828" cap="flat"/>
<wire x1="3.429" y1="1.27" x2="3.9826" y2="1.143" width="0.0508" layer="21" curve="-25.842828" cap="flat"/>
<wire x1="3.429" y1="-2.032" x2="5.109" y2="-1.1429" width="0.0508" layer="21" curve="55.771157" cap="flat"/>
<wire x1="3.9826" y1="-1.143" x2="3.9826" y2="1.143" width="0.0508" layer="51" curve="128.314524" cap="flat"/>
<wire x1="5.1091" y1="-1.143" x2="5.1091" y2="1.143" width="0.0508" layer="51" curve="68.456213" cap="flat"/>
<wire x1="-5.1091" y1="-1.143" x2="-3.429" y2="-2.032" width="0.0508" layer="21" curve="55.772485" cap="flat"/>
<wire x1="-3.9826" y1="-1.143" x2="-3.9826" y2="1.143" width="0.0508" layer="51" curve="-128.314524" cap="flat"/>
<wire x1="-3.9826" y1="-1.143" x2="-3.429" y2="-1.27" width="0.0508" layer="21" curve="25.842828" cap="flat"/>
<wire x1="-3.9826" y1="1.143" x2="-3.429" y2="1.27" width="0.0508" layer="21" curve="-25.842828" cap="flat"/>
<wire x1="-6.477" y1="-0.381" x2="-6.477" y2="0.381" width="0.1524" layer="51"/>
<wire x1="-5.1091" y1="-1.143" x2="-5.1091" y2="1.143" width="0.0508" layer="51" curve="-68.456213" cap="flat"/>
<wire x1="-5.715" y1="-1.143" x2="-5.715" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="-0.381" x2="-5.715" y2="0.381" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="0.381" x2="-5.715" y2="1.143" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="-2.159" x2="-5.715" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-2.159" x2="-5.461" y2="-2.413" width="0.1524" layer="21" curve="90"/>
<wire x1="-5.715" y1="-0.381" x2="-6.477" y2="-0.381" width="0.1524" layer="51"/>
<wire x1="-5.715" y1="0.381" x2="-6.477" y2="0.381" width="0.1524" layer="51"/>
<wire x1="-3.429" y1="2.032" x2="3.429" y2="2.032" width="0.0508" layer="21"/>
<wire x1="5.461" y1="2.413" x2="-5.461" y2="2.413" width="0.1524" layer="21"/>
<wire x1="5.461" y1="2.413" x2="5.715" y2="2.159" width="0.1524" layer="21" curve="-90"/>
<wire x1="-5.715" y1="2.159" x2="-5.461" y2="2.413" width="0.1524" layer="21" curve="-90"/>
<wire x1="-0.254" y1="0.635" x2="-0.254" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-0.635" x2="0.254" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-0.635" x2="0.254" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0.254" y1="0.635" x2="-0.254" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0.635" x2="-0.635" y2="0" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0" x2="-0.635" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0" x2="-1.016" y2="0" width="0.0508" layer="21"/>
<wire x1="0.635" y1="0.635" x2="0.635" y2="0" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0" x2="0.635" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0" x2="1.016" y2="0" width="0.0508" layer="21"/>
<smd name="1" x="-4.826" y="0" dx="5.334" dy="1.9304" layer="1"/>
<smd name="2" x="4.826" y="0" dx="5.334" dy="1.9304" layer="1"/>
<rectangle x1="-6.604" y1="-3.048" x2="6.604" y2="3.048" layer="43"/>
<text x="-0.254" y="2.667" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.667" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
</package>
<package name="HC49US" urn="urn:adsk.eagle:footprint:37485/1" library_version="1">
<description>&lt;h3&gt;HC49/US 11.6x4.6mm PTH Crystal&lt;/h3&gt;
&lt;p&gt;&lt;a href="https://www.digikey.com/Web%20Export/Supplier%20Content/Citizen_300/PDF/Citizen_HC49US.pdf?redirected=1"&gt;Example Datasheet&lt;/a&gt;&lt;/p&gt;</description>
<wire x1="-3.429" y1="-2.286" x2="3.429" y2="-2.286" width="0.2032" layer="21"/>
<wire x1="3.429" y1="2.286" x2="-3.429" y2="2.286" width="0.2032" layer="21"/>
<wire x1="3.429" y1="2.286" x2="3.429" y2="-2.286" width="0.2032" layer="21" curve="-180"/>
<wire x1="-3.429" y1="2.286" x2="-3.429" y2="-2.286" width="0.2032" layer="21" curve="180"/>
<pad name="1" x="-2.54" y="0" drill="0.7" diameter="1.651" rot="R90"/>
<pad name="2" x="2.54" y="0" drill="0.7" diameter="1.651" rot="R90"/>
<text x="0" y="2.54" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.54" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
</package>
<package name="CRYSTAL-SMD-2X6-CYL" urn="urn:adsk.eagle:footprint:37486/1" library_version="1">
<description>&lt;h3&gt;6.0x2.0mm Cylindrical Can (Radial) SMD Crystal&lt;/h3&gt;
&lt;p&gt;&lt;a href="http://cfm.citizen.co.jp/english/product/pdf/CMR200T.pdf"&gt;Example&lt;/a&gt;&lt;/p&gt;</description>
<smd name="X1" x="-1.27" y="0" dx="1" dy="2.5" layer="1"/>
<smd name="X2" x="1.27" y="0" dx="1" dy="2.5" layer="1"/>
<smd name="SHEILD" x="0" y="5.08" dx="2.5" dy="6" layer="1"/>
<text x="0" y="8.255" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;Name</text>
<text x="0" y="-1.524" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;Value</text>
<wire x1="-1.27" y1="-1" x2="-1.27" y2="1" width="0.3" layer="51"/>
<wire x1="-1.27" y1="1" x2="-0.6" y2="1" width="0.3" layer="51"/>
<wire x1="-0.6" y1="1" x2="-0.6" y2="2" width="0.3" layer="51"/>
<wire x1="-0.6" y1="2" x2="-0.5" y2="2" width="0.127" layer="51"/>
<wire x1="1.27" y1="-1" x2="1.27" y2="1" width="0.3" layer="51"/>
<wire x1="1.27" y1="1" x2="0.6" y2="1" width="0.3" layer="51"/>
<wire x1="0.6" y1="1" x2="0.6" y2="2" width="0.3" layer="51"/>
<wire x1="0.6" y1="2" x2="0.5" y2="2" width="0.127" layer="51"/>
<polygon width="0.1" layer="51">
<vertex x="-1" y="8"/>
<vertex x="-1" y="2"/>
<vertex x="1" y="2"/>
<vertex x="1" y="8"/>
</polygon>
</package>
<package name="CRYSTAL-SMD-5X3.2-4PAD" urn="urn:adsk.eagle:footprint:37487/1" library_version="1">
<description>&lt;h3&gt;5x3.2mm SMD Crystal&lt;/h3&gt;
&lt;p&gt;Example: &lt;a href="https://www.sparkfun.com/products/94"&gt;16MHz SMD Crystal&lt;/a&gt; (&lt;a href="https://www.sparkfun.com/datasheets/Components/SPK-5032-16MHZ.pdf"&gt;Datasheet&lt;/a&gt;)&lt;/p&gt;</description>
<wire x1="-0.6" y1="1.7" x2="0.6" y2="1.7" width="0.2032" layer="21"/>
<wire x1="2.6" y1="0.3" x2="2.6" y2="-0.3" width="0.2032" layer="21"/>
<wire x1="0.6" y1="-1.7" x2="-0.6" y2="-1.7" width="0.2032" layer="21"/>
<wire x1="-2.6" y1="0.3" x2="-2.6" y2="-0.3" width="0.2032" layer="21"/>
<wire x1="-2.5" y1="1.6" x2="-2.5" y2="-1.6" width="0.127" layer="51"/>
<wire x1="-2.5" y1="-1.6" x2="2.5" y2="-1.6" width="0.127" layer="51"/>
<wire x1="2.5" y1="-1.6" x2="2.5" y2="1.6" width="0.127" layer="51"/>
<wire x1="2.5" y1="1.6" x2="-2.5" y2="1.6" width="0.127" layer="51"/>
<smd name="1" x="-1.85" y="-1.15" dx="1.9" dy="1.1" layer="1"/>
<smd name="3" x="1.85" y="1.15" dx="1.9" dy="1.1" layer="1"/>
<smd name="4" x="-1.85" y="1.15" dx="1.9" dy="1.1" layer="1"/>
<smd name="2" x="1.85" y="-1.15" dx="1.9" dy="1.1" layer="1"/>
<text x="0" y="1.905" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.905" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
<polygon width="0.127" layer="51">
<vertex x="-2.5" y="1.6"/>
<vertex x="-2.5" y="0.8"/>
<vertex x="-1.3" y="0.8"/>
<vertex x="-1.3" y="1.6"/>
</polygon>
<polygon width="0.127" layer="51">
<vertex x="2.5" y="-1.6"/>
<vertex x="2.5" y="-0.8"/>
<vertex x="1.3" y="-0.8"/>
<vertex x="1.3" y="-1.6"/>
</polygon>
<polygon width="0.127" layer="51">
<vertex x="1.3" y="1.6"/>
<vertex x="1.3" y="0.8"/>
<vertex x="2.5" y="0.8"/>
<vertex x="2.5" y="1.6"/>
</polygon>
<polygon width="0.127" layer="51">
<vertex x="-1.3" y="-1.6"/>
<vertex x="-1.3" y="-0.8"/>
<vertex x="-2.5" y="-0.8"/>
<vertex x="-2.5" y="-1.6"/>
</polygon>
</package>
<package name="CRYSTAL-SMD-MC-146" urn="urn:adsk.eagle:footprint:37488/1" library_version="1">
<description>&lt;h3&gt;7x1.5mm MC-146 Flat Lead SMD Crystal&lt;/h3&gt;

&lt;p&gt;&lt;a href="https://support.epson.biz/td/api/doc_check.php?dl=brief_MC-156_en.pdf"&gt;Example Datasheet&lt;/a&gt;&lt;/p&gt;</description>
<wire x1="0.35" y1="0" x2="7.05" y2="0" width="0.127" layer="51"/>
<wire x1="7.05" y1="0" x2="7.05" y2="1.5" width="0.127" layer="51"/>
<wire x1="7.05" y1="1.5" x2="0.35" y2="1.5" width="0.127" layer="51"/>
<wire x1="0.35" y1="1.5" x2="0.35" y2="0" width="0.127" layer="51"/>
<wire x1="7.05" y1="-0.2" x2="0.35" y2="-0.2" width="0.2032" layer="21"/>
<wire x1="0.35" y1="1.7" x2="7.05" y2="1.7" width="0.2032" layer="21"/>
<smd name="P$1" x="0.6" y="0.3" dx="1.2" dy="0.6" layer="1"/>
<smd name="P$2" x="0.6" y="1.2" dx="1.2" dy="0.6" layer="1"/>
<smd name="NC2" x="6.9" y="0.3" dx="1.2" dy="0.6" layer="1"/>
<smd name="NC1" x="6.9" y="1.2" dx="1.2" dy="0.6" layer="1"/>
<text x="3.81" y="1.851" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;Name</text>
<text x="3.81" y="-0.327" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;Value</text>
</package>
<package name="CRYSTAL-PTH-2X6-CYL-KIT" urn="urn:adsk.eagle:footprint:37489/1" library_version="1">
<description>&lt;h3&gt;2x6mm Cylindrical Can (Radial) PTH Crystal&lt;/h3&gt;

This is the "KIT" version, which has limited top masking for improved ease of assembly.

&lt;p&gt;Example product: &lt;a href="https://www.sparkfun.com/products/540"&gt;32kHz crystal&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href="http://www.ecsxtal.com/store/pdf/ECS-3x8.pdf"&gt;Example datasheet&lt;/a&gt; (ECS-2X6)&lt;/p&gt;</description>
<wire x1="-0.889" y1="1.651" x2="0.889" y2="1.651" width="0.1524" layer="21"/>
<wire x1="0.762" y1="7.747" x2="1.016" y2="7.493" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.016" y1="7.493" x2="-0.762" y2="7.747" width="0.1524" layer="21" curve="-90"/>
<wire x1="-0.762" y1="7.747" x2="0.762" y2="7.747" width="0.1524" layer="21"/>
<wire x1="0.889" y1="1.651" x2="0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="1.016" y1="2.032" x2="1.016" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-0.889" y1="1.651" x2="-0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-1.016" y1="2.032" x2="-0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-1.016" y1="2.032" x2="-1.016" y2="7.493" width="0.1524" layer="21"/>
<wire x1="0.508" y1="0.762" x2="0.508" y2="1.143" width="0.4064" layer="21"/>
<wire x1="-0.508" y1="0.762" x2="-0.508" y2="1.27" width="0.4064" layer="21"/>
<wire x1="0.635" y1="0.635" x2="1.27" y2="0" width="0.4064" layer="51"/>
<wire x1="-0.635" y1="0.635" x2="-1.27" y2="0" width="0.4064" layer="51"/>
<wire x1="-0.508" y1="4.953" x2="-0.508" y2="4.572" width="0.1524" layer="21"/>
<wire x1="0.508" y1="4.572" x2="-0.508" y2="4.572" width="0.1524" layer="21"/>
<wire x1="0.508" y1="4.572" x2="0.508" y2="4.953" width="0.1524" layer="21"/>
<wire x1="-0.508" y1="4.953" x2="0.508" y2="4.953" width="0.1524" layer="21"/>
<wire x1="-0.508" y1="5.334" x2="0" y2="5.334" width="0.1524" layer="21"/>
<wire x1="-0.508" y1="4.191" x2="0" y2="4.191" width="0.1524" layer="21"/>
<wire x1="0" y1="4.191" x2="0" y2="3.683" width="0.1524" layer="21"/>
<wire x1="0" y1="4.191" x2="0.508" y2="4.191" width="0.1524" layer="21"/>
<wire x1="0" y1="5.334" x2="0" y2="5.842" width="0.1524" layer="21"/>
<wire x1="0" y1="5.334" x2="0.508" y2="5.334" width="0.1524" layer="21"/>
<wire x1="1.016" y1="2.032" x2="0.889" y2="2.032" width="0.1524" layer="21"/>
<wire x1="0.889" y1="2.032" x2="-0.889" y2="2.032" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="0.8128" diameter="1.6764" stop="no"/>
<pad name="2" x="1.27" y="0" drill="0.8128" diameter="1.6764" stop="no"/>
<rectangle x1="0.3048" y1="1.016" x2="0.7112" y2="1.6002" layer="21"/>
<rectangle x1="-0.7112" y1="1.016" x2="-0.3048" y2="1.6002" layer="21"/>
<rectangle x1="-1.778" y1="0.762" x2="1.778" y2="8.382" layer="43"/>
<circle x="-1.27" y="0" radius="0.508" width="0" layer="29"/>
<circle x="1.27" y="0" radius="0.508" width="0" layer="29"/>
<circle x="-1.27" y="0" radius="0.924571875" width="0" layer="30"/>
<circle x="1.27" y="0" radius="0.915809375" width="0" layer="30"/>
<text x="-1.27" y="4.572" size="0.6096" layer="25" font="vector" ratio="20" rot="R90" align="bottom-center">&gt;NAME</text>
<text x="1.27" y="4.572" size="0.6096" layer="27" font="vector" ratio="20" rot="R90" align="top-center">&gt;VALUE</text>
</package>
<package name="CRYSTAL-SMD-5X3.2-2PAD" urn="urn:adsk.eagle:footprint:37490/1" library_version="1">
<description>&lt;h3&gt;5x3.2mm 2-pad SMD Crystal&lt;/h3&gt;
&lt;p&gt;&lt;a href="http://www.txccrystal.com/images/pdf/7a.pdf"&gt;Example Datasheet&lt;/a&gt;&lt;/p&gt;</description>
<smd name="P$1" x="1.85" y="0" dx="1.7" dy="2.4" layer="1"/>
<smd name="P$2" x="-1.85" y="0" dx="1.7" dy="2.4" layer="1"/>
<wire x1="-2.6" y1="1.7" x2="2.6" y2="1.7" width="0.2032" layer="21"/>
<wire x1="2.6" y1="-1.7" x2="-2.6" y2="-1.7" width="0.2032" layer="21"/>
<wire x1="-2.5" y1="-1.6" x2="2.5" y2="-1.6" width="0.127" layer="51"/>
<wire x1="2.5" y1="-1.6" x2="2.5" y2="1.6" width="0.127" layer="51"/>
<wire x1="2.5" y1="1.6" x2="-2.5" y2="1.6" width="0.127" layer="51"/>
<wire x1="-2.5" y1="1.6" x2="-2.5" y2="-1.6" width="0.127" layer="51"/>
<wire x1="-2.6" y1="1.7" x2="-2.6" y2="1.4" width="0.2032" layer="21"/>
<wire x1="2.6" y1="-1.7" x2="2.6" y2="-1.4" width="0.2032" layer="21"/>
<wire x1="-2.6" y1="-1.7" x2="-2.6" y2="-1.4" width="0.2032" layer="21"/>
<wire x1="2.6" y1="1.7" x2="2.6" y2="1.4" width="0.2032" layer="21"/>
<text x="0" y="1.878" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;NAME</text>
<text x="0" y="-1.878" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;VALUE</text>
<polygon width="0.127" layer="51">
<vertex x="-2.5" y="1"/>
<vertex x="-1.2" y="1"/>
<vertex x="-1.2" y="-1"/>
<vertex x="-2.5" y="-1"/>
</polygon>
<polygon width="0.127" layer="51">
<vertex x="2.5" y="-1"/>
<vertex x="1.2" y="-1"/>
<vertex x="1.2" y="1"/>
<vertex x="2.5" y="1"/>
</polygon>
</package>
<package name="CRYSTAL-SMD-3.2X1.5MM" urn="urn:adsk.eagle:footprint:37491/1" library_version="1">
<description>&lt;h3&gt;3.2 x 1.5mm SMD Crystal Package&lt;/h3&gt;
&lt;p&gt;Example: &lt;a href="http://www.sii.co.jp/en/quartz/files/2013/03/file_PRODUCT_MASTER_50812_GRAPHIC03.pdf"&gt;SX-32S&lt;/a&gt;&lt;/p&gt;</description>
<smd name="P$1" x="-1.25" y="0" dx="1" dy="1.8" layer="1"/>
<smd name="P$2" x="1.25" y="0" dx="1" dy="1.8" layer="1"/>
<wire x1="-1.6" y1="0.75" x2="1.6" y2="0.75" width="0.127" layer="51"/>
<wire x1="1.6" y1="0.75" x2="1.6" y2="-0.75" width="0.127" layer="51"/>
<wire x1="1.6" y1="-0.75" x2="-1.6" y2="-0.75" width="0.127" layer="51"/>
<wire x1="-1.6" y1="-0.75" x2="-1.6" y2="0.75" width="0.127" layer="51"/>
<wire x1="-0.5" y1="0.85" x2="0.5" y2="0.85" width="0.2032" layer="21"/>
<wire x1="0.5" y1="-0.85" x2="-0.5" y2="-0.85" width="0.2032" layer="21"/>
<text x="0" y="1.043" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;Name</text>
<text x="0" y="-1.043" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;Value</text>
</package>
<package name="CRYSTAL-SMD-3.2X2.5MM" urn="urn:adsk.eagle:footprint:37492/1" library_version="1">
<description>&lt;h3&gt;3.2 x 2.5mm SMD Crystal Package&lt;/h3&gt;
&lt;p&gt;Example: &lt;a href="http://www.digikey.com/product-search/en?keywords=SER3627TR-ND"&gt;SX-32S&lt;/a&gt;&lt;/p&gt;</description>
<wire x1="-1.6" y1="-1.25" x2="-1.6" y2="1.25" width="0.127" layer="51"/>
<wire x1="-1.6" y1="1.25" x2="1.6" y2="1.25" width="0.127" layer="51"/>
<wire x1="1.6" y1="1.25" x2="1.6" y2="-1.25" width="0.127" layer="51"/>
<wire x1="1.6" y1="-1.25" x2="-1.6" y2="-1.25" width="0.127" layer="51"/>
<wire x1="-0.4" y1="1.377" x2="0.4" y2="1.377" width="0.2032" layer="21"/>
<wire x1="-1.727" y1="-0.15" x2="-1.727" y2="0.15" width="0.2032" layer="21"/>
<wire x1="1.727" y1="0.15" x2="1.727" y2="-0.15" width="0.2032" layer="21"/>
<wire x1="0.4" y1="-1.377" x2="-0.4" y2="-1.377" width="0.2032" layer="21"/>
<rectangle x1="-1.6" y1="0.35" x2="-0.6" y2="1.15" layer="51"/>
<rectangle x1="0.6" y1="-1.15" x2="1.6" y2="-0.35" layer="51" rot="R180"/>
<rectangle x1="-1.6" y1="-1.15" x2="-0.6" y2="-0.35" layer="51"/>
<rectangle x1="0.6" y1="0.35" x2="1.6" y2="1.15" layer="51" rot="R180"/>
<smd name="1" x="-1.175" y="-0.875" dx="1.2" dy="1.1" layer="1" rot="R180"/>
<smd name="2" x="1.175" y="-0.875" dx="1.2" dy="1.1" layer="1"/>
<smd name="3" x="1.175" y="0.875" dx="1.2" dy="1.1" layer="1"/>
<smd name="4" x="-1.175" y="0.875" dx="1.2" dy="1.1" layer="1" rot="R180"/>
<text x="0" y="1.524" size="0.6096" layer="25" font="vector" ratio="20" align="bottom-center">&gt;Name</text>
<text x="0" y="-1.524" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;Value</text>
</package>
</packages>
<packages3d>
<package3d name="HC49U" urn="urn:adsk.eagle:package:37511/1" type="box" library_version="1">
<description>HC49/U 11.6x4.6mm PTH Crystal (13.46mm height)
Example Datasheet</description>
<packageinstances>
<packageinstance name="HC49U"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-PTH-3X8-CYL" urn="urn:adsk.eagle:package:37512/1" type="box" library_version="1">
<description>3x8mm Cylindrical Can (Radial) PTH Crystal

This is the "KIT" version, which has limited top masking for improved ease of assembly.

Example product: 32kHz crystal
Example datasheet (ECS-3X8)</description>
<packageinstances>
<packageinstance name="CRYSTAL-PTH-3X8-CYL"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-PTH-2X6-CYL" urn="urn:adsk.eagle:package:37514/1" type="box" library_version="1">
<description>2x6mm Cylindrical Can (Radial) PTH Crystal

Example product: 32kHz crystal
Example datasheet (ECS-2X6)</description>
<packageinstances>
<packageinstance name="CRYSTAL-PTH-2X6-CYL"/>
</packageinstances>
</package3d>
<package3d name="HC49UP" urn="urn:adsk.eagle:package:37513/1" type="box" library_version="1">
<description>HC-49/UP 11.4x4.8mm SMD Crystal
Example Datasheet</description>
<packageinstances>
<packageinstance name="HC49UP"/>
</packageinstances>
</package3d>
<package3d name="HC49US" urn="urn:adsk.eagle:package:37515/1" type="box" library_version="1">
<description>HC49/US 11.6x4.6mm PTH Crystal
Example Datasheet</description>
<packageinstances>
<packageinstance name="HC49US"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-SMD-2X6-CYL" urn="urn:adsk.eagle:package:37516/1" type="box" library_version="1">
<description>6.0x2.0mm Cylindrical Can (Radial) SMD Crystal
Example</description>
<packageinstances>
<packageinstance name="CRYSTAL-SMD-2X6-CYL"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-SMD-5X3.2-4PAD" urn="urn:adsk.eagle:package:37518/1" type="box" library_version="1">
<description>5x3.2mm SMD Crystal
Example: 16MHz SMD Crystal (Datasheet)</description>
<packageinstances>
<packageinstance name="CRYSTAL-SMD-5X3.2-4PAD"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-SMD-MC-146" urn="urn:adsk.eagle:package:37517/1" type="box" library_version="1">
<description>7x1.5mm MC-146 Flat Lead SMD Crystal

Example Datasheet</description>
<packageinstances>
<packageinstance name="CRYSTAL-SMD-MC-146"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-PTH-2X6-CYL-KIT" urn="urn:adsk.eagle:package:37519/1" type="box" library_version="1">
<description>2x6mm Cylindrical Can (Radial) PTH Crystal

This is the "KIT" version, which has limited top masking for improved ease of assembly.

Example product: 32kHz crystal
Example datasheet (ECS-2X6)</description>
<packageinstances>
<packageinstance name="CRYSTAL-PTH-2X6-CYL-KIT"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-SMD-5X3.2-2PAD" urn="urn:adsk.eagle:package:37520/1" type="box" library_version="1">
<description>5x3.2mm 2-pad SMD Crystal
Example Datasheet</description>
<packageinstances>
<packageinstance name="CRYSTAL-SMD-5X3.2-2PAD"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-SMD-3.2X1.5MM" urn="urn:adsk.eagle:package:37548/1" type="box" library_version="1">
<description>3.2 x 1.5mm SMD Crystal Package
Example: SX-32S</description>
<packageinstances>
<packageinstance name="CRYSTAL-SMD-3.2X1.5MM"/>
</packageinstances>
</package3d>
<package3d name="CRYSTAL-SMD-3.2X2.5MM" urn="urn:adsk.eagle:package:37521/1" type="box" library_version="1">
<description>3.2 x 2.5mm SMD Crystal Package
Example: SX-32S</description>
<packageinstances>
<packageinstance name="CRYSTAL-SMD-3.2X2.5MM"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="CRYSTAL" urn="urn:adsk.eagle:symbol:37480/1" library_version="1">
<description>&lt;h3&gt;Crystal (no ground pin)&lt;/h3&gt;</description>
<wire x1="1.016" y1="0" x2="2.54" y2="0" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.016" y2="0" width="0.1524" layer="94"/>
<wire x1="-0.381" y1="1.524" x2="-0.381" y2="-1.524" width="0.254" layer="94"/>
<wire x1="-0.381" y1="-1.524" x2="0.381" y2="-1.524" width="0.254" layer="94"/>
<wire x1="0.381" y1="-1.524" x2="0.381" y2="1.524" width="0.254" layer="94"/>
<wire x1="0.381" y1="1.524" x2="-0.381" y2="1.524" width="0.254" layer="94"/>
<wire x1="1.016" y1="1.778" x2="1.016" y2="-1.778" width="0.254" layer="94"/>
<wire x1="-1.016" y1="1.778" x2="-1.016" y2="-1.778" width="0.254" layer="94"/>
<text x="0" y="2.032" size="1.778" layer="95" font="vector" align="bottom-center">&gt;NAME</text>
<text x="0" y="-2.032" size="1.778" layer="96" font="vector" align="top-center">&gt;VALUE</text>
<text x="-2.159" y="-1.143" size="0.8636" layer="93">1</text>
<text x="1.524" y="-1.143" size="0.8636" layer="93">2</text>
<pin name="2" x="2.54" y="0" visible="off" length="point" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-2.54" y="0" visible="off" length="point" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="CRYSTAL" urn="urn:adsk.eagle:component:37549/1" prefix="Y" uservalue="yes" library_version="1">
<description>&lt;h3&gt;Crystals (Generic)&lt;/h3&gt;
&lt;p&gt;These are &lt;b&gt;passive&lt;/b&gt; quartz crystals, which can be used as a clock source for a microcontroller.&lt;/p&gt;
&lt;p&gt;Crystal's are two-terminal devices. They require external "load" capacitors to generate an oscillating signal.&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="CRYSTAL" x="0" y="0"/>
</gates>
<devices>
<device name="PTH-HC49UV" package="HC49U">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37511/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-3X8" package="CRYSTAL-PTH-3X8-CYL">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37512/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-2X6" package="CRYSTAL-PTH-2X6-CYL">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37514/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-HC49UP" package="HC49UP">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37513/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-HC49US" package="HC49US">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37515/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-2X6" package="CRYSTAL-SMD-2X6-CYL">
<connects>
<connect gate="G$1" pin="1" pad="X1"/>
<connect gate="G$1" pin="2" pad="X2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37516/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-5X3.2-4PAD" package="CRYSTAL-SMD-5X3.2-4PAD">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37518/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-MC146" package="CRYSTAL-SMD-MC-146">
<connects>
<connect gate="G$1" pin="1" pad="P$2"/>
<connect gate="G$1" pin="2" pad="P$1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37517/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="PTH-2X6-KIT" package="CRYSTAL-PTH-2X6-CYL-KIT">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37519/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-5X3.2-2PAD" package="CRYSTAL-SMD-5X3.2-2PAD">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37520/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-3.2X1.5" package="CRYSTAL-SMD-3.2X1.5MM">
<connects>
<connect gate="G$1" pin="1" pad="P$1"/>
<connect gate="G$1" pin="2" pad="P$2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37548/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="SMD-3.2X2.5" package="CRYSTAL-SMD-3.2X2.5MM">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37521/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Connectors" urn="urn:adsk.eagle:library:513">
<description>&lt;h3&gt;SparkFun Connectors&lt;/h3&gt;
This library contains electrically-functional connectors. 
&lt;br&gt;
&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is &lt;b&gt; the end user's responsibility&lt;/b&gt; to ensure correctness and suitablity for a given componet or application. 
&lt;br&gt;
&lt;br&gt;If you enjoy using this library, please buy one of our products at &lt;a href=" www.sparkfun.com"&gt;SparkFun.com&lt;/a&gt;.
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;
&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="STEREOJACK2.5MM_STEREOJACK2.5MM" urn="urn:adsk.eagle:footprint:37599/1" library_version="1">
<description>&lt;h3&gt;2.5mm Stereo Audio Jack - PTH&lt;/h3&gt;
&lt;p&gt;Specifications:
&lt;ul&gt;&lt;li&gt;Pin count:4&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href=”http://www.4uconnector.com/online/object/4udrawing/19726.pdf”&gt;Datasheet referenced for footprint&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;Example device(s):
&lt;ul&gt;&lt;li&gt;Audio_Jack_2.5MM&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;</description>
<wire x1="4.6" y1="2.5" x2="-3.3" y2="2.5" width="0.127" layer="21"/>
<wire x1="-3.3" y1="2.5" x2="-3.3" y2="1.7" width="0.127" layer="21"/>
<wire x1="-3.3" y1="1.7" x2="-3.3" y2="-1.9" width="0.127" layer="21"/>
<wire x1="-3.3" y1="-1.9" x2="-3.3" y2="-2.6" width="0.127" layer="21"/>
<wire x1="-3.3" y1="-2.6" x2="4.6" y2="-2.6" width="0.127" layer="21"/>
<wire x1="4.6" y1="-2.6" x2="4.6" y2="-1.5" width="0.127" layer="21"/>
<wire x1="4.6" y1="1.5" x2="4.6" y2="2.5" width="0.127" layer="21"/>
<wire x1="-3.3" y1="1.7" x2="-6.3" y2="1.7" width="0.127" layer="21"/>
<wire x1="-6.3" y1="1.7" x2="-6.3" y2="-1.9" width="0.127" layer="21"/>
<wire x1="-6.3" y1="-1.9" x2="-3.3" y2="-1.9" width="0.127" layer="21"/>
<pad name="RING1" x="2.5" y="-1.85" drill="1.1" diameter="1.9304"/>
<pad name="RING2" x="2.5" y="1.85" drill="1.1" diameter="1.9304"/>
<pad name="SHIELD" x="-2.4" y="0" drill="1" diameter="1.9304"/>
<pad name="TIP" x="4.5" y="0" drill="1.1" diameter="1.9304"/>
<hole x="0" y="0" drill="1.2"/>
<text x="-2.54" y="2.794" size="0.6096" layer="25" font="vector" ratio="20">&gt;NAME</text>
<text x="-2.54" y="-3.556" size="0.6096" layer="27" font="vector" ratio="20">&gt;VALUE</text>
</package>
<package name="STEREOJACK2.5MM_SPECIAL_POGOPINS" urn="urn:adsk.eagle:footprint:37600/1" library_version="1">
<description>&lt;h3&gt;2.5mm Stereo Audio Jack - PTH Pogo Pin Compatible&lt;/h3&gt;
Long pads to enable testing with pogo pins.
&lt;p&gt;Specifications:
&lt;ul&gt;&lt;li&gt;Pin count:4&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href=”http://www.4uconnector.com/online/object/4udrawing/19726.pdf”&gt;Datasheet referenced for footprint&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;Example device(s):
&lt;ul&gt;&lt;li&gt;Audio_Jack_2.5MM&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;</description>
<wire x1="4.6" y1="2.5" x2="-3.3" y2="2.5" width="0.127" layer="21"/>
<wire x1="-3.3" y1="2.5" x2="-3.3" y2="1.7" width="0.127" layer="21"/>
<wire x1="-3.3" y1="1.7" x2="-3.3" y2="-1.9" width="0.127" layer="21"/>
<wire x1="-3.3" y1="-1.9" x2="-3.3" y2="-2.6" width="0.127" layer="21"/>
<wire x1="-3.3" y1="-2.6" x2="4.6" y2="-2.6" width="0.127" layer="21"/>
<wire x1="4.6" y1="-2.6" x2="4.6" y2="-1.5" width="0.127" layer="21"/>
<wire x1="4.6" y1="1.5" x2="4.6" y2="2.5" width="0.127" layer="21"/>
<wire x1="-3.3" y1="1.7" x2="-6.3" y2="1.7" width="0.127" layer="21"/>
<wire x1="-6.3" y1="1.7" x2="-6.3" y2="-1.9" width="0.127" layer="21"/>
<wire x1="-6.3" y1="-1.9" x2="-3.3" y2="-1.9" width="0.127" layer="21"/>
<pad name="RING1" x="2.5" y="-1.85" drill="1.1176" diameter="1.9304" shape="offset" rot="R270"/>
<pad name="RING2" x="2.5" y="1.85" drill="1.1176" diameter="1.9304" shape="offset" rot="R90"/>
<pad name="SHIELD" x="-2.4" y="0" drill="1.1176" diameter="1.9304" shape="offset" rot="R180"/>
<pad name="TIP" x="4.5" y="0" drill="1.1176" diameter="1.9304" shape="offset"/>
<hole x="0" y="0" drill="1.2"/>
</package>
<package name="STEREOJACK2.5MM_SPECIAL_HOLES-ONLY" urn="urn:adsk.eagle:footprint:37601/1" library_version="1">
<description>&lt;h3&gt;2.5mm Stereo Audio Jack - PTH Holes Only&lt;/h3&gt;
No electrical connections. 
&lt;p&gt;Specifications:
&lt;ul&gt;&lt;li&gt;Pin count:4&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href=”http://www.4uconnector.com/online/object/4udrawing/19726.pdf”&gt;Datasheet referenced for footprint&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;Example device(s):
&lt;ul&gt;&lt;li&gt;Audio_Jack_2.5MM&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;</description>
<wire x1="4.6" y1="2.5" x2="-3.3" y2="2.5" width="0.127" layer="21"/>
<wire x1="-3.3" y1="2.5" x2="-3.3" y2="1.7" width="0.127" layer="21"/>
<wire x1="-3.3" y1="1.7" x2="-3.3" y2="-1.9" width="0.127" layer="21"/>
<wire x1="-3.3" y1="-1.9" x2="-3.3" y2="-2.6" width="0.127" layer="21"/>
<wire x1="-3.3" y1="-2.6" x2="4.6" y2="-2.6" width="0.127" layer="21"/>
<wire x1="4.6" y1="-2.6" x2="4.6" y2="-1.5" width="0.127" layer="21"/>
<wire x1="4.6" y1="1.5" x2="4.6" y2="2.5" width="0.127" layer="21"/>
<wire x1="-3.3" y1="1.7" x2="-6.3" y2="1.7" width="0.127" layer="21"/>
<wire x1="-6.3" y1="1.7" x2="-6.3" y2="-1.9" width="0.127" layer="21"/>
<wire x1="-6.3" y1="-1.9" x2="-3.3" y2="-1.9" width="0.127" layer="21"/>
<pad name="RING1" x="2.5" y="-1.85" drill="0.889" diameter="0.8128" rot="R270"/>
<pad name="RING2" x="2.5" y="1.85" drill="0.889" diameter="0.8128" rot="R90"/>
<pad name="SHIELD" x="-2.4" y="0" drill="0.889" diameter="0.8128" rot="R180"/>
<pad name="TIP" x="4.5" y="0" drill="0.889" diameter="0.8128"/>
<hole x="0" y="0" drill="0.889"/>
<hole x="-2.4" y="0" drill="1.4732"/>
<hole x="2.5" y="1.85" drill="1.4732"/>
<hole x="4.5" y="0" drill="1.4732"/>
<hole x="2.5" y="-1.85" drill="1.4732"/>
</package>
<package name="STEREOJACK2.5MM" urn="urn:adsk.eagle:footprint:37602/1" library_version="1">
<description>&lt;h3&gt;2.5mm Stereo Audio Jack - SMT&lt;/h3&gt;
&lt;p&gt;Specifications:
&lt;ul&gt;&lt;li&gt;Pin count:5&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;
&lt;p&gt;&lt;a href=”http://www.4uconnector.com/online/object/4udrawing/19726.pdf”&gt;Datasheet referenced for footprint&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;Example device(s):
&lt;ul&gt;&lt;li&gt;Audio_Jack_2.5MM&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;</description>
<wire x1="-1.5" y1="1.3" x2="-1.5" y2="2.5" width="0.2032" layer="21"/>
<wire x1="-1.5" y1="2.5" x2="-0.096" y2="2.5" width="0.2032" layer="21"/>
<wire x1="-1.5" y1="1.3" x2="-1.5" y2="-1.3" width="0.2032" layer="21"/>
<wire x1="-1.5" y1="-1.3" x2="-1.5" y2="-2.5" width="0.2032" layer="21"/>
<wire x1="-1.5" y1="-2.5" x2="-0.1206" y2="-2.5" width="0.2032" layer="21"/>
<wire x1="1.996" y1="2.5" x2="5.0516" y2="2.5" width="0.2032" layer="21"/>
<wire x1="2.073" y1="-2.5" x2="5.077" y2="-2.5" width="0.2032" layer="21"/>
<wire x1="-2.5" y1="1.3" x2="-1.5" y2="1.3" width="0.2032" layer="21"/>
<wire x1="-2.5" y1="1.3" x2="-2.5" y2="-1.3" width="0.2032" layer="21"/>
<wire x1="-2.5" y1="-1.3" x2="-1.5" y2="-1.3" width="0.2032" layer="21"/>
<wire x1="6.773" y1="2.5" x2="7.9" y2="2.5" width="0.2032" layer="21"/>
<wire x1="7.9" y1="2.5" x2="7.9" y2="0.9738" width="0.2032" layer="21"/>
<wire x1="6.8484" y1="-2.5" x2="7.9" y2="-2.5" width="0.2032" layer="21"/>
<wire x1="7.9" y1="-2.5" x2="7.9" y2="-0.873" width="0.2032" layer="21"/>
<smd name="5" x="1" y="2.9516" dx="1.4666" dy="2.3" layer="1"/>
<smd name="A" x="1.016" y="-2.9516" dx="1.4666" dy="2.3" layer="1"/>
<smd name="4" x="5.9492" y="-2.54" dx="2.1" dy="1.119" layer="1" rot="R90"/>
<smd name="1" x="5.8984" y="2.54" dx="2.1" dy="1.1444" layer="1" rot="R90"/>
<smd name="3" x="8.55" y="0" dx="1.9984" dy="1.1698" layer="1" rot="R180"/>
<hole x="0" y="0" drill="1.2"/>
<hole x="4" y="0" drill="1.2"/>
<text x="2.54" y="3.937" size="0.6096" layer="25" font="vector" ratio="20">&gt;NAME</text>
<text x="2.54" y="-4.572" size="0.6096" layer="27" font="vector" ratio="20">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="STEREOJACK2.5MM_STEREOJACK2.5MM" urn="urn:adsk.eagle:package:37996/1" type="box" library_version="1">
<description>2.5mm Stereo Audio Jack - PTH
Specifications:
Pin count:4

Datasheet referenced for footprint
Example device(s):
Audio_Jack_2.5MM
</description>
<packageinstances>
<packageinstance name="STEREOJACK2.5MM_STEREOJACK2.5MM"/>
</packageinstances>
</package3d>
<package3d name="STEREOJACK2.5MM_SPECIAL_POGOPINS" urn="urn:adsk.eagle:package:38005/1" type="box" library_version="1">
<description>2.5mm Stereo Audio Jack - PTH Pogo Pin Compatible
Long pads to enable testing with pogo pins.
Specifications:
Pin count:4

Datasheet referenced for footprint
Example device(s):
Audio_Jack_2.5MM
</description>
<packageinstances>
<packageinstance name="STEREOJACK2.5MM_SPECIAL_POGOPINS"/>
</packageinstances>
</package3d>
<package3d name="STEREOJACK2.5MM_SPECIAL_HOLES-ONLY" urn="urn:adsk.eagle:package:37993/1" type="box" library_version="1">
<description>2.5mm Stereo Audio Jack - PTH Holes Only
No electrical connections. 
Specifications:
Pin count:4

Datasheet referenced for footprint
Example device(s):
Audio_Jack_2.5MM
</description>
<packageinstances>
<packageinstance name="STEREOJACK2.5MM_SPECIAL_HOLES-ONLY"/>
</packageinstances>
</package3d>
<package3d name="STEREOJACK2.5MM" urn="urn:adsk.eagle:package:37998/1" type="box" library_version="1">
<description>2.5mm Stereo Audio Jack - SMT
Specifications:
Pin count:5

Datasheet referenced for footprint
Example device(s):
Audio_Jack_2.5MM
</description>
<packageinstances>
<packageinstance name="STEREOJACK2.5MM"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="AUDIO-JACK" urn="urn:adsk.eagle:symbol:37598/1" library_version="1">
<description>&lt;h3&gt; Audio Jack &lt;/h3&gt;
&lt;p&gt; 3 pin Basic Audio Jack&lt;/p&gt;</description>
<wire x1="-1.27" y1="-2.54" x2="0" y2="-1.27" width="0.1524" layer="94"/>
<wire x1="0" y1="-1.27" x2="1.27" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="0" x2="-1.27" y2="0" width="0.1524" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="-1.27" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="-1.27" x2="-3.81" y2="0" width="0.1524" layer="94"/>
<wire x1="2.54" y1="2.54" x2="-5.08" y2="2.54" width="0.1524" layer="94"/>
<text x="-5.08" y="3.048" size="1.778" layer="95" font="vector">&gt;NAME</text>
<text x="-5.08" y="-5.08" size="1.778" layer="96" font="vector">&gt;VALUE</text>
<text x="2.286" y="-2.286" size="1.016" layer="94" ratio="15">L</text>
<text x="2.286" y="0.254" size="1.016" layer="94" ratio="15">R</text>
<rectangle x1="-7.62" y1="-0.762" x2="-2.54" y2="0.762" layer="94" rot="R90"/>
<pin name="RIGHT" x="5.08" y="0" visible="off" length="short" rot="R180"/>
<pin name="LEFT" x="5.08" y="-2.54" visible="off" length="short" rot="R180"/>
<pin name="SLEEVE" x="5.08" y="2.54" visible="off" length="short" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="AUDIO_JACK_2.5MM" urn="urn:adsk.eagle:component:38306/1" prefix="J" uservalue="yes" library_version="1">
<description>&lt;h3&gt;Audio Jack&lt;/h3&gt;
&lt;p&gt;2.5mm variants of PTH and SMT audio jacks.&lt;/p&gt;
&lt;p&gt;SparkFun Products:
&lt;li&gt;&lt;a href=”https://www.sparkfun.com/products/11888”&gt;SparkFun PicoBoard&lt;/a&gt;- 2.5mm SMD&lt;/li&gt;
&lt;/ul&gt;&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="AUDIO-JACK" x="0" y="0"/>
</gates>
<devices>
<device name="_2.5MM" package="STEREOJACK2.5MM_STEREOJACK2.5MM">
<connects>
<connect gate="G$1" pin="LEFT" pad="SHIELD"/>
<connect gate="G$1" pin="RIGHT" pad="TIP"/>
<connect gate="G$1" pin="SLEEVE" pad="RING1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37996/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="CONN-09878" constant="no"/>
</technology>
</technologies>
</device>
<device name="SPECIAL_POGO_PINS" package="STEREOJACK2.5MM_SPECIAL_POGOPINS">
<connects>
<connect gate="G$1" pin="LEFT" pad="SHIELD"/>
<connect gate="G$1" pin="RIGHT" pad="TIP"/>
<connect gate="G$1" pin="SLEEVE" pad="RING1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:38005/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="_HOLESONLY" package="STEREOJACK2.5MM_SPECIAL_HOLES-ONLY">
<connects>
<connect gate="G$1" pin="LEFT" pad="SHIELD"/>
<connect gate="G$1" pin="RIGHT" pad="TIP"/>
<connect gate="G$1" pin="SLEEVE" pad="RING1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37993/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="_2.5MM_SMD" package="STEREOJACK2.5MM">
<connects>
<connect gate="G$1" pin="LEFT" pad="1"/>
<connect gate="G$1" pin="RIGHT" pad="4"/>
<connect gate="G$1" pin="SLEEVE" pad="5"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37998/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="PROD_ID" value="CONN-10062" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="rcl" urn="urn:adsk.eagle:library:334">
<description>&lt;b&gt;Resistors, Capacitors, Inductors&lt;/b&gt;&lt;p&gt;
Based on the previous libraries:
&lt;ul&gt;
&lt;li&gt;r.lbr
&lt;li&gt;cap.lbr 
&lt;li&gt;cap-fe.lbr
&lt;li&gt;captant.lbr
&lt;li&gt;polcap.lbr
&lt;li&gt;ipc-smd.lbr
&lt;/ul&gt;
All SMD packages are defined according to the IPC specifications and  CECC&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;&lt;p&gt;
&lt;p&gt;
for Electrolyt Capacitors see also :&lt;p&gt;
www.bccomponents.com &lt;p&gt;
www.panasonic.com&lt;p&gt;
www.kemet.com&lt;p&gt;
http://www.secc.co.jp/pdf/os_e/2004/e_os_all.pdf &lt;b&gt;(SANYO)&lt;/b&gt;
&lt;p&gt;
for trimmer refence see : &lt;u&gt;www.electrospec-inc.com/cross_references/trimpotcrossref.asp&lt;/u&gt;&lt;p&gt;

&lt;table border=0 cellspacing=0 cellpadding=0 width="100%" cellpaddding=0&gt;
&lt;tr valign="top"&gt;

&lt;! &lt;td width="10"&gt;&amp;nbsp;&lt;/td&gt;
&lt;td width="90%"&gt;

&lt;b&gt;&lt;font color="#0000FF" size="4"&gt;TRIM-POT CROSS REFERENCE&lt;/font&gt;&lt;/b&gt;
&lt;P&gt;
&lt;TABLE BORDER=0 CELLSPACING=1 CELLPADDING=2&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;RECTANGULAR MULTI-TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;BOURNS&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;BI&amp;nbsp;TECH&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;DALE-VISHAY&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;PHILIPS/MEPCO&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;MURATA&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;PANASONIC&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;SPECTROL&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;B&gt;
      &lt;FONT SIZE=3 FACE=ARIAL color="#FF0000"&gt;MILSPEC&lt;/FONT&gt;
      &lt;/B&gt;
    &lt;/TD&gt;&lt;TD&gt;&amp;nbsp;&lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3 &gt;
      3005P&lt;BR&gt;
      3006P&lt;BR&gt;
      3006W&lt;BR&gt;
      3006Y&lt;BR&gt;
      3009P&lt;BR&gt;
      3009W&lt;BR&gt;
      3009Y&lt;BR&gt;
      3057J&lt;BR&gt;
      3057L&lt;BR&gt;
      3057P&lt;BR&gt;
      3057Y&lt;BR&gt;
      3059J&lt;BR&gt;
      3059L&lt;BR&gt;
      3059P&lt;BR&gt;
      3059Y&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      89P&lt;BR&gt;
      89W&lt;BR&gt;
      89X&lt;BR&gt;
      89PH&lt;BR&gt;
      76P&lt;BR&gt;
      89XH&lt;BR&gt;
      78SLT&lt;BR&gt;
      78L&amp;nbsp;ALT&lt;BR&gt;
      56P&amp;nbsp;ALT&lt;BR&gt;
      78P&amp;nbsp;ALT&lt;BR&gt;
      T8S&lt;BR&gt;
      78L&lt;BR&gt;
      56P&lt;BR&gt;
      78P&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      T18/784&lt;BR&gt;
      783&lt;BR&gt;
      781&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      2199&lt;BR&gt;
      1697/1897&lt;BR&gt;
      1680/1880&lt;BR&gt;
      2187&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      8035EKP/CT20/RJ-20P&lt;BR&gt;
      -&lt;BR&gt;
      RJ-20X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      1211L&lt;BR&gt;
      8012EKQ&amp;nbsp;ALT&lt;BR&gt;
      8012EKR&amp;nbsp;ALT&lt;BR&gt;
      1211P&lt;BR&gt;
      8012EKJ&lt;BR&gt;
      8012EKL&lt;BR&gt;
      8012EKQ&lt;BR&gt;
      8012EKR&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      2101P&lt;BR&gt;
      2101W&lt;BR&gt;
      2101Y&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      2102L&lt;BR&gt;
      2102S&lt;BR&gt;
      2102Y&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      EVMCOG&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      43P&lt;BR&gt;
      43W&lt;BR&gt;
      43Y&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      40L&lt;BR&gt;
      40P&lt;BR&gt;
      40Y&lt;BR&gt;
      70Y-T602&lt;BR&gt;
      70L&lt;BR&gt;
      70P&lt;BR&gt;
      70Y&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      RT/RTR12&lt;BR&gt;
      RT/RTR12&lt;BR&gt;
      RT/RTR12&lt;BR&gt;
      -&lt;BR&gt;
      RJ/RJR12&lt;BR&gt;
      RJ/RJR12&lt;BR&gt;
      RJ/RJR12&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;&amp;nbsp;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SQUARE MULTI-TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
   &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MURATA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;SPECTROL&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MILSPEC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3250L&lt;BR&gt;
      3250P&lt;BR&gt;
      3250W&lt;BR&gt;
      3250X&lt;BR&gt;
      3252P&lt;BR&gt;
      3252W&lt;BR&gt;
      3252X&lt;BR&gt;
      3260P&lt;BR&gt;
      3260W&lt;BR&gt;
      3260X&lt;BR&gt;
      3262P&lt;BR&gt;
      3262W&lt;BR&gt;
      3262X&lt;BR&gt;
      3266P&lt;BR&gt;
      3266W&lt;BR&gt;
      3266X&lt;BR&gt;
      3290H&lt;BR&gt;
      3290P&lt;BR&gt;
      3290W&lt;BR&gt;
      3292P&lt;BR&gt;
      3292W&lt;BR&gt;
      3292X&lt;BR&gt;
      3296P&lt;BR&gt;
      3296W&lt;BR&gt;
      3296X&lt;BR&gt;
      3296Y&lt;BR&gt;
      3296Z&lt;BR&gt;
      3299P&lt;BR&gt;
      3299W&lt;BR&gt;
      3299X&lt;BR&gt;
      3299Y&lt;BR&gt;
      3299Z&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      66P&amp;nbsp;ALT&lt;BR&gt;
      66W&amp;nbsp;ALT&lt;BR&gt;
      66X&amp;nbsp;ALT&lt;BR&gt;
      66P&amp;nbsp;ALT&lt;BR&gt;
      66W&amp;nbsp;ALT&lt;BR&gt;
      66X&amp;nbsp;ALT&lt;BR&gt;
      -&lt;BR&gt;
      64W&amp;nbsp;ALT&lt;BR&gt;
      -&lt;BR&gt;
      64P&amp;nbsp;ALT&lt;BR&gt;
      64W&amp;nbsp;ALT&lt;BR&gt;
      64X&amp;nbsp;ALT&lt;BR&gt;
      64P&lt;BR&gt;
      64W&lt;BR&gt;
      64X&lt;BR&gt;
      66X&amp;nbsp;ALT&lt;BR&gt;
      66P&amp;nbsp;ALT&lt;BR&gt;
      66W&amp;nbsp;ALT&lt;BR&gt;
      66P&lt;BR&gt;
      66W&lt;BR&gt;
      66X&lt;BR&gt;
      67P&lt;BR&gt;
      67W&lt;BR&gt;
      67X&lt;BR&gt;
      67Y&lt;BR&gt;
      67Z&lt;BR&gt;
      68P&lt;BR&gt;
      68W&lt;BR&gt;
      68X&lt;BR&gt;
      67Y&amp;nbsp;ALT&lt;BR&gt;
      67Z&amp;nbsp;ALT&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      5050&lt;BR&gt;
      5091&lt;BR&gt;
      5080&lt;BR&gt;
      5087&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      T63YB&lt;BR&gt;
      T63XB&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      5887&lt;BR&gt;
      5891&lt;BR&gt;
      5880&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      T93Z&lt;BR&gt;
      T93YA&lt;BR&gt;
      T93XA&lt;BR&gt;
      T93YB&lt;BR&gt;
      T93XB&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      8026EKP&lt;BR&gt;
      8026EKW&lt;BR&gt;
      8026EKM&lt;BR&gt;
      8026EKP&lt;BR&gt;
      8026EKB&lt;BR&gt;
      8026EKM&lt;BR&gt;
      1309X&lt;BR&gt;
      1309P&lt;BR&gt;
      1309W&lt;BR&gt;
      8024EKP&lt;BR&gt;
      8024EKW&lt;BR&gt;
      8024EKN&lt;BR&gt;
      RJ-9P/CT9P&lt;BR&gt;
      RJ-9W&lt;BR&gt;
      RJ-9X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3103P&lt;BR&gt;
      3103Y&lt;BR&gt;
      3103Z&lt;BR&gt;
      3103P&lt;BR&gt;
      3103Y&lt;BR&gt;
      3103Z&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3105P/3106P&lt;BR&gt;
      3105W/3106W&lt;BR&gt;
      3105X/3106X&lt;BR&gt;
      3105Y/3106Y&lt;BR&gt;
      3105Z/3105Z&lt;BR&gt;
      3102P&lt;BR&gt;
      3102W&lt;BR&gt;
      3102X&lt;BR&gt;
      3102Y&lt;BR&gt;
      3102Z&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMCBG&lt;BR&gt;
      EVMCCG&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      55-1-X&lt;BR&gt;
      55-4-X&lt;BR&gt;
      55-3-X&lt;BR&gt;
      55-2-X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      50-2-X&lt;BR&gt;
      50-4-X&lt;BR&gt;
      50-3-X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      64P&lt;BR&gt;
      64W&lt;BR&gt;
      64X&lt;BR&gt;
      64Y&lt;BR&gt;
      64Z&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      RT/RTR22&lt;BR&gt;
      RT/RTR22&lt;BR&gt;
      RT/RTR22&lt;BR&gt;
      RT/RTR22&lt;BR&gt;
      RJ/RJR22&lt;BR&gt;
      RJ/RJR22&lt;BR&gt;
      RJ/RJR22&lt;BR&gt;
      RT/RTR26&lt;BR&gt;
      RT/RTR26&lt;BR&gt;
      RT/RTR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RJ/RJR26&lt;BR&gt;
      RT/RTR24&lt;BR&gt;
      RT/RTR24&lt;BR&gt;
      RT/RTR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      RJ/RJR24&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;&amp;nbsp;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=8&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SINGLE TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MURATA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;SPECTROL&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD ALIGN=CENTER&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;MILSPEC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3323P&lt;BR&gt;
      3323S&lt;BR&gt;
      3323W&lt;BR&gt;
      3329H&lt;BR&gt;
      3329P&lt;BR&gt;
      3329W&lt;BR&gt;
      3339H&lt;BR&gt;
      3339P&lt;BR&gt;
      3339W&lt;BR&gt;
      3352E&lt;BR&gt;
      3352H&lt;BR&gt;
      3352K&lt;BR&gt;
      3352P&lt;BR&gt;
      3352T&lt;BR&gt;
      3352V&lt;BR&gt;
      3352W&lt;BR&gt;
      3362H&lt;BR&gt;
      3362M&lt;BR&gt;
      3362P&lt;BR&gt;
      3362R&lt;BR&gt;
      3362S&lt;BR&gt;
      3362U&lt;BR&gt;
      3362W&lt;BR&gt;
      3362X&lt;BR&gt;
      3386B&lt;BR&gt;
      3386C&lt;BR&gt;
      3386F&lt;BR&gt;
      3386H&lt;BR&gt;
      3386K&lt;BR&gt;
      3386M&lt;BR&gt;
      3386P&lt;BR&gt;
      3386S&lt;BR&gt;
      3386W&lt;BR&gt;
      3386X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      25P&lt;BR&gt;
      25S&lt;BR&gt;
      25RX&lt;BR&gt;
      82P&lt;BR&gt;
      82M&lt;BR&gt;
      82PA&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      91E&lt;BR&gt;
      91X&lt;BR&gt;
      91T&lt;BR&gt;
      91B&lt;BR&gt;
      91A&lt;BR&gt;
      91V&lt;BR&gt;
      91W&lt;BR&gt;
      25W&lt;BR&gt;
      25V&lt;BR&gt;
      25P&lt;BR&gt;
      -&lt;BR&gt;
      25S&lt;BR&gt;
      25U&lt;BR&gt;
      25RX&lt;BR&gt;
      25X&lt;BR&gt;
      72XW&lt;BR&gt;
      72XL&lt;BR&gt;
      72PM&lt;BR&gt;
      72RX&lt;BR&gt;
      -&lt;BR&gt;
      72PX&lt;BR&gt;
      72P&lt;BR&gt;
      72RXW&lt;BR&gt;
      72RXL&lt;BR&gt;
      72X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      T7YB&lt;BR&gt;
      T7YA&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      TXD&lt;BR&gt;
      TYA&lt;BR&gt;
      TYP&lt;BR&gt;
      -&lt;BR&gt;
      TYD&lt;BR&gt;
      TX&lt;BR&gt;
      -&lt;BR&gt;
      150SX&lt;BR&gt;
      100SX&lt;BR&gt;
      102T&lt;BR&gt;
      101S&lt;BR&gt;
      190T&lt;BR&gt;
      150TX&lt;BR&gt;
      101&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      101SX&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      ET6P&lt;BR&gt;
      ET6S&lt;BR&gt;
      ET6X&lt;BR&gt;
      RJ-6W/8014EMW&lt;BR&gt;
      RJ-6P/8014EMP&lt;BR&gt;
      RJ-6X/8014EMX&lt;BR&gt;
      TM7W&lt;BR&gt;
      TM7P&lt;BR&gt;
      TM7X&lt;BR&gt;
      -&lt;BR&gt;
      8017SMS&lt;BR&gt;
      -&lt;BR&gt;
      8017SMB&lt;BR&gt;
      8017SMA&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      CT-6W&lt;BR&gt;
      CT-6H&lt;BR&gt;
      CT-6P&lt;BR&gt;
      CT-6R&lt;BR&gt;
      -&lt;BR&gt;
      CT-6V&lt;BR&gt;
      CT-6X&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      8038EKV&lt;BR&gt;
      -&lt;BR&gt;
      8038EKX&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      8038EKP&lt;BR&gt;
      8038EKZ&lt;BR&gt;
      8038EKW&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3321H&lt;BR&gt;
      3321P&lt;BR&gt;
      3321N&lt;BR&gt;
      1102H&lt;BR&gt;
      1102P&lt;BR&gt;
      1102T&lt;BR&gt;
      RVA0911V304A&lt;BR&gt;
      -&lt;BR&gt;
      RVA0911H413A&lt;BR&gt;
      RVG0707V100A&lt;BR&gt;
      RVA0607V(H)306A&lt;BR&gt;
      RVA1214H213A&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      3104B&lt;BR&gt;
      3104C&lt;BR&gt;
      3104F&lt;BR&gt;
      3104H&lt;BR&gt;
      -&lt;BR&gt;
      3104M&lt;BR&gt;
      3104P&lt;BR&gt;
      3104S&lt;BR&gt;
      3104W&lt;BR&gt;
      3104X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      EVMQ0G&lt;BR&gt;
      EVMQIG&lt;BR&gt;
      EVMQ3G&lt;BR&gt;
      EVMS0G&lt;BR&gt;
      EVMQ0G&lt;BR&gt;
      EVMG0G&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMK4GA00B&lt;BR&gt;
      EVM30GA00B&lt;BR&gt;
      EVMK0GA00B&lt;BR&gt;
      EVM38GA00B&lt;BR&gt;
      EVMB6&lt;BR&gt;
      EVLQ0&lt;BR&gt;
      -&lt;BR&gt;
      EVMMSG&lt;BR&gt;
      EVMMBG&lt;BR&gt;
      EVMMAG&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMMCS&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMM1&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMM0&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      EVMM3&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      62-3-1&lt;BR&gt;
      62-1-2&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      67R&lt;BR&gt;
      -&lt;BR&gt;
      67P&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      67X&lt;BR&gt;
      63V&lt;BR&gt;
      63S&lt;BR&gt;
      63M&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      63H&lt;BR&gt;
      63P&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      63X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      RJ/RJR50&lt;BR&gt;
      RJ/RJR50&lt;BR&gt;
      RJ/RJR50&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
&lt;/TABLE&gt;
&lt;P&gt;&amp;nbsp;&lt;P&gt;
&lt;TABLE BORDER=0 CELLSPACING=1 CELLPADDING=3&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=7&gt;
      &lt;FONT color="#0000FF" SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SMD TRIM-POT CROSS REFERENCE&lt;/B&gt;&lt;/FONT&gt;
      &lt;P&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;MULTI-TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURNS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;TOCOS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;AUX/KYOCERA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3224G&lt;BR&gt;
      3224J&lt;BR&gt;
      3224W&lt;BR&gt;
      3269P&lt;BR&gt;
      3269W&lt;BR&gt;
      3269X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      44G&lt;BR&gt;
      44J&lt;BR&gt;
      44W&lt;BR&gt;
      84P&lt;BR&gt;
      84W&lt;BR&gt;
      84X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      ST63Z&lt;BR&gt;
      ST63Y&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      ST5P&lt;BR&gt;
      ST5W&lt;BR&gt;
      ST5X&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=7&gt;&amp;nbsp;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD COLSPAN=7&gt;
      &lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;SINGLE TURN&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BOURNS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;BI&amp;nbsp;TECH&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;DALE-VISHAY&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PHILIPS/MEPCO&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;PANASONIC&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;TOCOS&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD&gt;
      &lt;FONT SIZE=3 FACE=ARIAL&gt;&lt;B&gt;AUX/KYOCERA&lt;/B&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
  &lt;TR&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      3314G&lt;BR&gt;
      3314J&lt;BR&gt;
      3364A/B&lt;BR&gt;
      3364C/D&lt;BR&gt;
      3364W/X&lt;BR&gt;
      3313G&lt;BR&gt;
      3313J&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      23B&lt;BR&gt;
      23A&lt;BR&gt;
      21X&lt;BR&gt;
      21W&lt;BR&gt;
      -&lt;BR&gt;
      22B&lt;BR&gt;
      22A&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      ST5YL/ST53YL&lt;BR&gt;
      ST5YJ/5T53YJ&lt;BR&gt;
      ST-23A&lt;BR&gt;
      ST-22B&lt;BR&gt;
      ST-22&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      ST-4B&lt;BR&gt;
      ST-4A&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      ST-3B&lt;BR&gt;
      ST-3A&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      EVM-6YS&lt;BR&gt;
      EVM-1E&lt;BR&gt;
      EVM-1G&lt;BR&gt;
      EVM-1D&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      G4B&lt;BR&gt;
      G4A&lt;BR&gt;
      TR04-3S1&lt;BR&gt;
      TRG04-2S1&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
    &lt;TD BGCOLOR="#cccccc" ALIGN=CENTER&gt;&lt;FONT FACE=ARIAL SIZE=3&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;
      DVR-43A&lt;BR&gt;
      CVR-42C&lt;BR&gt;
      CVR-42A/C&lt;BR&gt;
      -&lt;BR&gt;
      -&lt;BR&gt;&lt;/FONT&gt;
    &lt;/TD&gt;
  &lt;/TR&gt;
&lt;/TABLE&gt;
&lt;P&gt;
&lt;FONT SIZE=4 FACE=ARIAL&gt;&lt;B&gt;ALT =&amp;nbsp;ALTERNATE&lt;/B&gt;&lt;/FONT&gt;
&lt;P&gt;

&amp;nbsp;
&lt;P&gt;
&lt;/td&gt;
&lt;/tr&gt;
&lt;/table&gt;</description>
<packages>
<package name="XC10B5" urn="urn:adsk.eagle:footprint:23203/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 13 x 5 mm, grid 10.16 mm</description>
<wire x1="-2.54" y1="1.27" x2="-2.54" y2="0" width="0.4064" layer="21"/>
<wire x1="-1.651" y1="1.27" x2="-1.651" y2="0" width="0.4064" layer="21"/>
<wire x1="-3.81" y1="0" x2="-2.54" y2="0" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="3.81" y1="0" x2="-1.651" y2="0" width="0.1524" layer="21"/>
<wire x1="-1.651" y1="0" x2="-1.651" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="-5.969" y1="-2.54" x2="5.969" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="6.477" y1="2.032" x2="6.477" y2="-2.032" width="0.1524" layer="21"/>
<wire x1="5.969" y1="2.54" x2="-5.969" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-6.477" y1="-2.032" x2="-6.477" y2="2.032" width="0.1524" layer="21"/>
<wire x1="-3.6068" y1="1.3208" x2="-3.048" y2="0.5842" width="0.1524" layer="21"/>
<wire x1="-3.6068" y1="0.5842" x2="-3.048" y2="1.3208" width="0.1524" layer="21"/>
<wire x1="5.969" y1="2.54" x2="6.477" y2="2.032" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.477" y1="2.032" x2="-5.969" y2="2.54" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.477" y1="-2.032" x2="-5.969" y2="-2.54" width="0.1524" layer="21" curve="90"/>
<wire x1="5.969" y1="-2.54" x2="6.477" y2="-2.032" width="0.1524" layer="21" curve="90"/>
<pad name="1" x="-5.08" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="5.08" y="0" drill="1.016" shape="octagon"/>
<text x="-6.223" y="2.921" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.889" y="-1.905" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC10B6" urn="urn:adsk.eagle:footprint:23204/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 13 x 6 mm, grid 10.16 mm</description>
<wire x1="-2.54" y1="1.27" x2="-2.54" y2="0" width="0.4064" layer="21"/>
<wire x1="-1.651" y1="1.27" x2="-1.651" y2="0" width="0.4064" layer="21"/>
<wire x1="-3.81" y1="0" x2="-2.54" y2="0" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="3.81" y1="0" x2="-1.651" y2="0" width="0.1524" layer="21"/>
<wire x1="-1.651" y1="0" x2="-1.651" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="-5.969" y1="-3.048" x2="5.969" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="6.477" y1="2.54" x2="6.477" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="5.969" y1="3.048" x2="-5.969" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-6.477" y1="-2.54" x2="-6.477" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-3.6068" y1="1.3208" x2="-3.048" y2="0.5842" width="0.1524" layer="21"/>
<wire x1="-3.6068" y1="0.5842" x2="-3.048" y2="1.3208" width="0.1524" layer="21"/>
<wire x1="5.969" y1="3.048" x2="6.477" y2="2.54" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.477" y1="2.54" x2="-5.969" y2="3.048" width="0.1524" layer="21" curve="-90"/>
<wire x1="-6.477" y1="-2.54" x2="-5.969" y2="-3.048" width="0.1524" layer="21" curve="90"/>
<wire x1="5.969" y1="-3.048" x2="6.477" y2="-2.54" width="0.1524" layer="21" curve="90"/>
<pad name="1" x="-5.08" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="5.08" y="0" drill="1.016" shape="octagon"/>
<text x="-6.096" y="3.429" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-0.889" y="-1.905" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC15B10" urn="urn:adsk.eagle:footprint:23205/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 17.5 x 10 mm, grid 15.24 mm</description>
<wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
<wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="6.35" y1="0" x2="-4.191" y2="0" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="-9.525" y1="-5.08" x2="9.525" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="9.525" y1="5.08" x2="9.525" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="9.525" y1="5.08" x2="-9.525" y2="5.08" width="0.1524" layer="21"/>
<wire x1="-9.525" y1="-5.08" x2="-9.525" y2="5.08" width="0.1524" layer="21"/>
<wire x1="-6.6548" y1="2.5908" x2="-5.842" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-6.6548" y1="1.3462" x2="-5.842" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="1.016" shape="octagon"/>
<text x="-9.525" y="5.461" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-2.921" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC15B5" urn="urn:adsk.eagle:footprint:23206/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 17.5 x 5 mm, grid 15.24 mm</description>
<wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
<wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="6.35" y1="0" x2="-4.191" y2="0" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="-9.017" y1="-2.54" x2="9.017" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="9.017" y1="2.54" x2="9.017" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="9.017" y1="2.54" x2="-9.017" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-9.017" y1="-2.54" x2="-9.017" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-6.5278" y1="1.9558" x2="-5.715" y2="0.7112" width="0.1524" layer="21"/>
<wire x1="-6.5278" y1="0.7112" x2="-5.715" y2="1.9558" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="1.016" shape="octagon"/>
<text x="-9.017" y="2.921" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.429" y="-2.032" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC15B6" urn="urn:adsk.eagle:footprint:23207/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 17.5 x 6 mm, grid 15.24 mm</description>
<wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
<wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
<wire x1="6.35" y1="0" x2="-4.191" y2="0" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="-9.017" y1="-3.048" x2="9.017" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="9.017" y1="3.048" x2="9.017" y2="-3.048" width="0.1524" layer="21"/>
<wire x1="9.017" y1="3.048" x2="-9.017" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-9.017" y1="-3.048" x2="-9.017" y2="3.048" width="0.1524" layer="21"/>
<wire x1="-6.5278" y1="1.9558" x2="-5.715" y2="0.7112" width="0.1524" layer="21"/>
<wire x1="-6.5278" y1="0.7112" x2="-5.715" y2="1.9558" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="1.016" shape="octagon"/>
<text x="-9.017" y="3.429" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.429" y="-1.905" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC15B8" urn="urn:adsk.eagle:footprint:23208/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 17.5 x 8 mm, grid 15.24 mm</description>
<wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
<wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
<wire x1="-6.35" y1="0" x2="-5.08" y2="0" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="6.35" y1="0" x2="-4.191" y2="0" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
<wire x1="-9.017" y1="-4.318" x2="9.017" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="9.017" y1="4.318" x2="9.017" y2="-4.318" width="0.1524" layer="21"/>
<wire x1="9.017" y1="4.318" x2="-9.017" y2="4.318" width="0.1524" layer="21"/>
<wire x1="-9.017" y1="-4.318" x2="-9.017" y2="4.318" width="0.1524" layer="21"/>
<wire x1="-6.5278" y1="1.9558" x2="-5.715" y2="0.7112" width="0.1524" layer="21"/>
<wire x1="-6.5278" y1="0.7112" x2="-5.715" y2="1.9558" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="7.62" y="0" drill="1.016" shape="octagon"/>
<text x="-9.017" y="4.699" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-2.54" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC22B10" urn="urn:adsk.eagle:footprint:23209/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 26.6 x 10 mm, grid 22.86 mm</description>
<wire x1="-9.525" y1="0.0254" x2="-7.62" y2="0.0254" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="1.9304" x2="-7.62" y2="0.0254" width="0.4064" layer="21"/>
<wire x1="-6.731" y1="1.9304" x2="-6.731" y2="0.0254" width="0.4064" layer="21"/>
<wire x1="-6.731" y1="0.0254" x2="9.525" y2="0.0254" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="0.0254" x2="-6.731" y2="-1.8796" width="0.4064" layer="21"/>
<wire x1="-7.62" y1="0.0254" x2="-7.62" y2="-1.8796" width="0.4064" layer="21"/>
<wire x1="-13.335" y1="-5.08" x2="13.335" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="13.335" y1="5.08" x2="13.335" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="13.335" y1="5.08" x2="-13.335" y2="5.08" width="0.1524" layer="21"/>
<wire x1="-13.335" y1="-5.08" x2="-13.335" y2="5.08" width="0.1524" layer="21"/>
<wire x1="-9.8298" y1="2.5908" x2="-9.017" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-9.8298" y1="1.3462" x2="-9.017" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-11.43" y="0.0254" drill="1.016" shape="octagon"/>
<pad name="2" x="11.43" y="0.0254" drill="1.016" shape="octagon"/>
<text x="-13.2842" y="5.4356" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.7658" y="-2.8448" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC22B7" urn="urn:adsk.eagle:footprint:23210/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 26.6 x 7.5 mm, grid 22.86 mm</description>
<wire x1="-13.335" y1="3.81" x2="13.335" y2="3.81" width="0.1524" layer="21"/>
<wire x1="13.335" y1="3.81" x2="13.335" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="13.335" y1="-3.81" x2="-13.335" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-13.335" y1="-3.81" x2="-13.335" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-9.525" y1="0" x2="-7.62" y2="0" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="1.905" x2="-7.62" y2="0" width="0.4064" layer="21"/>
<wire x1="-6.731" y1="1.905" x2="-6.731" y2="0" width="0.4064" layer="21"/>
<wire x1="-6.731" y1="0" x2="9.525" y2="0" width="0.1524" layer="21"/>
<wire x1="-6.731" y1="0" x2="-6.731" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-7.62" y1="0" x2="-7.62" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-9.8298" y1="2.5908" x2="-9.017" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-9.8298" y1="1.3462" x2="-9.017" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-11.43" y="0" drill="1.016" shape="octagon"/>
<pad name="2" x="11.43" y="0" drill="1.016" shape="octagon"/>
<text x="-13.2842" y="4.2672" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-5.7658" y="-2.8702" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC27B11" urn="urn:adsk.eagle:footprint:23211/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 32 x 11 mm, grid 27.9 mm</description>
<wire x1="-11.7348" y1="0" x2="-10.16" y2="0" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="1.905" x2="-10.16" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="1.905" x2="-9.271" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="0" x2="11.7348" y2="0" width="0.1524" layer="21"/>
<wire x1="-9.271" y1="0" x2="-9.271" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-10.16" y1="0" x2="-10.16" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-16.002" y1="-5.588" x2="16.002" y2="-5.588" width="0.1524" layer="21"/>
<wire x1="16.002" y1="5.588" x2="16.002" y2="-5.588" width="0.1524" layer="21"/>
<wire x1="16.002" y1="5.588" x2="-16.002" y2="5.588" width="0.1524" layer="21"/>
<wire x1="-16.002" y1="-5.588" x2="-16.002" y2="5.588" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="2.5908" x2="-11.557" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="1.3462" x2="-11.557" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="0" drill="1.1938" shape="octagon"/>
<pad name="2" x="13.97" y="0" drill="1.1938" shape="octagon"/>
<text x="-15.9512" y="5.9182" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.3058" y="-2.8702" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC27B13" urn="urn:adsk.eagle:footprint:25541/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 32 x 13 mm, grid 27.9 mm</description>
<wire x1="-11.7348" y1="0" x2="-10.16" y2="0" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="1.905" x2="-10.16" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="1.905" x2="-9.271" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="0" x2="11.7348" y2="0" width="0.1524" layer="21"/>
<wire x1="-9.271" y1="0" x2="-9.271" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-10.16" y1="0" x2="-10.16" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-16.002" y1="-6.477" x2="16.002" y2="-6.477" width="0.4064" layer="21"/>
<wire x1="16.002" y1="6.477" x2="16.002" y2="-6.477" width="0.1524" layer="21"/>
<wire x1="16.002" y1="6.477" x2="-16.002" y2="6.477" width="0.1524" layer="21"/>
<wire x1="-16.002" y1="-6.477" x2="-16.002" y2="6.477" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="2.5908" x2="-11.557" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="1.3462" x2="-11.557" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="0" drill="1.1938" shape="octagon"/>
<pad name="2" x="13.97" y="0" drill="1.1938" shape="octagon"/>
<text x="-15.9512" y="6.8072" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.3058" y="-2.8702" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC27B15" urn="urn:adsk.eagle:footprint:23213/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 32 x 15 mm, grid 27.9 mm</description>
<wire x1="-11.7602" y1="0" x2="-10.16" y2="0" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="1.905" x2="-10.16" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="1.905" x2="-9.271" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="0" x2="11.7348" y2="0" width="0.1524" layer="21"/>
<wire x1="-9.271" y1="0" x2="-9.271" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-10.16" y1="0" x2="-10.16" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-16.002" y1="-7.493" x2="16.002" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="16.002" y1="7.493" x2="16.002" y2="-7.493" width="0.1524" layer="21"/>
<wire x1="16.002" y1="7.493" x2="-16.002" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-16.002" y1="-7.493" x2="-16.002" y2="7.493" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="2.5908" x2="-11.557" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="1.3462" x2="-11.557" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="0" drill="1.1938" shape="octagon"/>
<pad name="2" x="13.97" y="0" drill="1.1938" shape="octagon"/>
<text x="-15.9512" y="7.8232" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.3058" y="-2.8702" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC27B18" urn="urn:adsk.eagle:footprint:23214/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 32 x 18 mm, grid 27.9 mm</description>
<wire x1="-11.7602" y1="0" x2="-10.16" y2="0" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="1.905" x2="-10.16" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="1.905" x2="-9.271" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="0" x2="11.7348" y2="0" width="0.1524" layer="21"/>
<wire x1="-9.271" y1="0" x2="-9.271" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-10.16" y1="0" x2="-10.16" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-16.002" y1="-9.017" x2="16.002" y2="-9.017" width="0.1524" layer="21"/>
<wire x1="16.002" y1="9.017" x2="16.002" y2="-9.017" width="0.1524" layer="21"/>
<wire x1="16.002" y1="9.017" x2="-16.002" y2="9.017" width="0.1524" layer="21"/>
<wire x1="-16.002" y1="-9.017" x2="-16.002" y2="9.017" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="2.5908" x2="-11.557" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="1.3462" x2="-11.557" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="0" drill="1.1938" shape="octagon"/>
<pad name="2" x="13.97" y="0" drill="1.1938" shape="octagon"/>
<text x="-15.9512" y="9.3472" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.3058" y="-2.8702" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="XC27B21" urn="urn:adsk.eagle:footprint:23215/1" library_version="11">
<description>&lt;b&gt;X CAPACITOR&lt;/b&gt;&lt;p&gt;
body 32 x 21 mm, grid 27.9 mm</description>
<wire x1="-11.7602" y1="0" x2="-10.16" y2="0" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="1.905" x2="-10.16" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="1.905" x2="-9.271" y2="0" width="0.4064" layer="21"/>
<wire x1="-9.271" y1="0" x2="11.7348" y2="0" width="0.1524" layer="21"/>
<wire x1="-9.271" y1="0" x2="-9.271" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-10.16" y1="0" x2="-10.16" y2="-1.905" width="0.4064" layer="21"/>
<wire x1="-16.002" y1="-10.541" x2="16.002" y2="-10.541" width="0.1524" layer="21"/>
<wire x1="16.002" y1="10.541" x2="16.002" y2="-10.541" width="0.1524" layer="21"/>
<wire x1="16.002" y1="10.541" x2="-16.002" y2="10.541" width="0.1524" layer="21"/>
<wire x1="-16.002" y1="-10.541" x2="-16.002" y2="10.541" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="2.5908" x2="-11.557" y2="1.3462" width="0.1524" layer="21"/>
<wire x1="-12.3698" y1="1.3462" x2="-11.557" y2="2.5908" width="0.1524" layer="21"/>
<pad name="1" x="-13.97" y="0" drill="1.1938" shape="octagon"/>
<pad name="2" x="13.97" y="0" drill="1.1938" shape="octagon"/>
<text x="-15.9512" y="10.8712" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-8.3058" y="-2.8702" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
</package>
</packages>
<packages3d>
<package3d name="XC10B5" urn="urn:adsk.eagle:package:23696/1" type="box" library_version="11">
<description>X CAPACITOR
body 13 x 5 mm, grid 10.16 mm</description>
<packageinstances>
<packageinstance name="XC10B5"/>
</packageinstances>
</package3d>
<package3d name="XC10B6" urn="urn:adsk.eagle:package:23700/1" type="box" library_version="11">
<description>X CAPACITOR
body 13 x 6 mm, grid 10.16 mm</description>
<packageinstances>
<packageinstance name="XC10B6"/>
</packageinstances>
</package3d>
<package3d name="XC15B10" urn="urn:adsk.eagle:package:23694/1" type="box" library_version="11">
<description>X CAPACITOR
body 17.5 x 10 mm, grid 15.24 mm</description>
<packageinstances>
<packageinstance name="XC15B10"/>
</packageinstances>
</package3d>
<package3d name="XC15B5" urn="urn:adsk.eagle:package:23695/1" type="box" library_version="11">
<description>X CAPACITOR
body 17.5 x 5 mm, grid 15.24 mm</description>
<packageinstances>
<packageinstance name="XC15B5"/>
</packageinstances>
</package3d>
<package3d name="XC15B6" urn="urn:adsk.eagle:package:23697/1" type="box" library_version="11">
<description>X CAPACITOR
body 17.5 x 6 mm, grid 15.24 mm</description>
<packageinstances>
<packageinstance name="XC15B6"/>
</packageinstances>
</package3d>
<package3d name="XC15B8" urn="urn:adsk.eagle:package:23698/1" type="box" library_version="11">
<description>X CAPACITOR
body 17.5 x 8 mm, grid 15.24 mm</description>
<packageinstances>
<packageinstance name="XC15B8"/>
</packageinstances>
</package3d>
<package3d name="XC22B10" urn="urn:adsk.eagle:package:23699/1" type="box" library_version="11">
<description>X CAPACITOR
body 26.6 x 10 mm, grid 22.86 mm</description>
<packageinstances>
<packageinstance name="XC22B10"/>
</packageinstances>
</package3d>
<package3d name="XC22B7" urn="urn:adsk.eagle:package:23706/1" type="box" library_version="11">
<description>X CAPACITOR
body 26.6 x 7.5 mm, grid 22.86 mm</description>
<packageinstances>
<packageinstance name="XC22B7"/>
</packageinstances>
</package3d>
<package3d name="XC27B11" urn="urn:adsk.eagle:package:23702/1" type="box" library_version="11">
<description>X CAPACITOR
body 32 x 11 mm, grid 27.9 mm</description>
<packageinstances>
<packageinstance name="XC27B11"/>
</packageinstances>
</package3d>
<package3d name="XC27B13" urn="urn:adsk.eagle:package:25977/1" type="box" library_version="11">
<description>X CAPACITOR
body 32 x 13 mm, grid 27.9 mm</description>
<packageinstances>
<packageinstance name="XC27B13"/>
</packageinstances>
</package3d>
<package3d name="XC27B15" urn="urn:adsk.eagle:package:23703/1" type="box" library_version="11">
<description>X CAPACITOR
body 32 x 15 mm, grid 27.9 mm</description>
<packageinstances>
<packageinstance name="XC27B15"/>
</packageinstances>
</package3d>
<package3d name="XC27B18" urn="urn:adsk.eagle:package:23707/1" type="box" library_version="11">
<description>X CAPACITOR
body 32 x 18 mm, grid 27.9 mm</description>
<packageinstances>
<packageinstance name="XC27B18"/>
</packageinstances>
</package3d>
<package3d name="XC27B21" urn="urn:adsk.eagle:package:23709/1" type="box" library_version="11">
<description>X CAPACITOR
body 32 x 21 mm, grid 27.9 mm</description>
<packageinstances>
<packageinstance name="XC27B21"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="C-EU" urn="urn:adsk.eagle:symbol:23120/1" library_version="11">
<wire x1="0" y1="0" x2="0" y2="-0.508" width="0.1524" layer="94"/>
<wire x1="0" y1="-2.54" x2="0" y2="-2.032" width="0.1524" layer="94"/>
<text x="1.524" y="0.381" size="1.778" layer="95">&gt;NAME</text>
<text x="1.524" y="-4.699" size="1.778" layer="96">&gt;VALUE</text>
<rectangle x1="-2.032" y1="-2.032" x2="2.032" y2="-1.524" layer="94"/>
<rectangle x1="-2.032" y1="-1.016" x2="2.032" y2="-0.508" layer="94"/>
<pin name="1" x="0" y="2.54" visible="off" length="short" direction="pas" swaplevel="1" rot="R270"/>
<pin name="2" x="0" y="-5.08" visible="off" length="short" direction="pas" swaplevel="1" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="CX" urn="urn:adsk.eagle:component:23795/5" prefix="CX" uservalue="yes" library_version="11">
<description>&lt;B&gt;X CAPACITOR&lt;/B&gt;&lt;p&gt;
for power line</description>
<gates>
<gate name="G$1" symbol="C-EU" x="0" y="0"/>
</gates>
<devices>
<device name="XC10B5" package="XC10B5">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23696/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC10B6" package="XC10B6">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23700/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="1" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC15B10" package="XC15B10">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23694/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC15B5" package="XC15B5">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23695/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC15B6" package="XC15B6">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23697/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="1" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC15B8" package="XC15B8">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23698/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="3" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC22B10" package="XC22B10">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23699/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC22B7" package="XC22B7">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23706/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC27B11" package="XC27B11">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23702/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC27B13" package="XC27B13">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:25977/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC27B15" package="XC27B15">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23703/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC27B18" package="XC27B18">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23707/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
<device name="XC27B21" package="XC27B21">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:23709/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
<attribute name="SPICEPREFIX" value="C" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="ngspice-simulation" urn="urn:adsk.eagle:library:527439">
<description>SPICE compatible library parts</description>
<packages>
</packages>
<symbols>
<symbol name="L" urn="urn:adsk.eagle:symbol:527456/4" library_version="18">
<description>INDUCTOR</description>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
<pin name="2" x="7.62" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<text x="-2.54" y="2.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-5.08" size="1.778" layer="97">&gt;SPICEMODEL</text>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<text x="-2.54" y="-7.62" size="1.778" layer="97">&gt;SPICEEXTRA</text>
<wire x1="-2.54" y1="0" x2="0" y2="0" width="0.254" layer="94" curve="-180"/>
<wire x1="0" y1="0" x2="2.54" y2="0" width="0.254" layer="94" curve="-180"/>
<wire x1="2.54" y1="0" x2="5.08" y2="0" width="0.254" layer="94" curve="-180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="L" urn="urn:adsk.eagle:component:527476/7" prefix="L" uservalue="yes" library_version="18">
<description>INDUCTOR</description>
<gates>
<gate name="G$1" symbol="L" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name="">
<attribute name="_EXTERNAL_" value="" constant="no"/>
</technology>
</technologies>
</device>
</devices>
<spice>
<pinmapping spiceprefix="L">
<pinmap gate="G$1" pin="1" pinorder="1"/>
<pinmap gate="G$1" pin="2" pinorder="2"/>
</pinmapping>
</spice>
</deviceset>
</devicesets>
</library>
<library name="discrete" urn="urn:adsk.eagle:library:211">
<description>Discrete devices (Antenna, Arrrester, Thermistor)</description>
<packages>
<package name="PAD-01" urn="urn:adsk.eagle:footprint:12919/1" library_version="2">
<description>&lt;b&gt;PAD&lt;/b&gt;</description>
<circle x="0" y="0" radius="0.889" width="0.762" layer="51"/>
<pad name="1" x="0" y="0" drill="1.3208" diameter="2.54" shape="octagon"/>
<text x="-1.27" y="1.524" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-1.27" y="3.048" size="1.27" layer="25" ratio="10">&gt;NAME</text>
</package>
</packages>
<packages3d>
<package3d name="PAD-01" urn="urn:adsk.eagle:package:12928/1" type="box" library_version="2">
<description>PAD</description>
<packageinstances>
<packageinstance name="PAD-01"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="ANTENNA" urn="urn:adsk.eagle:symbol:12918/1" library_version="2">
<wire x1="-2.54" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="2.54" y1="0" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="-2.54" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="2.54" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="1.27" size="1.778" layer="95">&gt;NAME</text>
<text x="2.54" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="0" y="-5.08" visible="off" length="short" direction="out" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ANTENNA" urn="urn:adsk.eagle:component:12934/2" prefix="ANT" uservalue="yes" library_version="2">
<description>&lt;b&gt;Antenna&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="ANTENNA" x="0" y="10.16"/>
</gates>
<devices>
<device name="" package="PAD-01">
<connects>
<connect gate="A" pin="1" pad="1"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:12928/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="11" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="SparkFun-Boards" urn="urn:adsk.eagle:library:509">
<description>&lt;h3&gt;SparkFun Electronics' preferred foot prints&lt;/h3&gt;
This library contains footprints for SparkFun breakout boards, microcontrollers (Arduino, Particle, Teensy, etc.),  breadboards, non-RF modules, etc.
&lt;br&gt;
&lt;br&gt;
We've spent an enormous amount of time creating and checking these footprints and parts, but it is &lt;b&gt; the end user's responsibility&lt;/b&gt; to ensure correctness and suitablity for a given componet or application. 
&lt;br&gt;
&lt;br&gt;If you enjoy using this library, please buy one of our products at &lt;a href=" www.sparkfun.com"&gt;SparkFun.com&lt;/a&gt;.
&lt;br&gt;
&lt;br&gt;
&lt;b&gt;Licensing:&lt;/b&gt; Creative Commons ShareAlike 4.0 International - https://creativecommons.org/licenses/by-sa/4.0/ 
&lt;br&gt;
&lt;br&gt;
You are welcome to use this library for commercial purposes. For attribution, we ask that when you begin to sell your device using our footprint, you email us with a link to the product being sold. We want bragging rights that we helped (in a very small part) to create your 8th world wonder. We would like the opportunity to feature your device on our homepage.</description>
<packages>
<package name="ESP32_THING" urn="urn:adsk.eagle:footprint:37309/1" library_version="1">
<description>&lt;h3&gt;SparkFun ESP32 Thing Board&lt;/h3&gt;
&lt;p&gt;Pins, silkscreen, and logos matching the layout of the &lt;a href=https://www.sparkfun.com/products/13907&gt;SparkFun ESP32 Thing&lt;/a&gt;&lt;/p&gt;

&lt;p&gt;Specifications:
&lt;ul&gt;&lt;li&gt;Pin count: 40&lt;/li&gt;
&lt;li&gt;Area: 2.32" x 1.00"&lt;/li&gt;
&lt;/p&gt;</description>
<pad name="1" x="1.27" y="1.27" drill="1.016" diameter="1.8796" shape="square" rot="R180"/>
<pad name="2" x="1.27" y="3.81" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="3" x="1.27" y="6.35" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="4" x="1.27" y="8.89" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="5" x="1.27" y="11.43" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="6" x="1.27" y="13.97" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="7" x="1.27" y="16.51" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="8" x="1.27" y="19.05" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="9" x="1.27" y="21.59" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="10" x="1.27" y="24.13" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="11" x="1.27" y="26.67" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="12" x="1.27" y="29.21" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="13" x="1.27" y="31.75" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="14" x="1.27" y="34.29" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="15" x="1.27" y="36.83" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="16" x="1.27" y="39.37" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="17" x="1.27" y="41.91" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="18" x="1.27" y="44.45" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="19" x="1.27" y="46.99" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="20" x="1.27" y="49.53" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="40" x="24.13" y="1.27" drill="1.016" diameter="1.8796" shape="square" rot="R180"/>
<pad name="39" x="24.13" y="3.81" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="38" x="24.13" y="6.35" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="37" x="24.13" y="8.89" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="36" x="24.13" y="11.43" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="35" x="24.13" y="13.97" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="34" x="24.13" y="16.51" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="33" x="24.13" y="19.05" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="32" x="24.13" y="21.59" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="31" x="24.13" y="24.13" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="30" x="24.13" y="26.67" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="29" x="24.13" y="29.21" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="28" x="24.13" y="31.75" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="27" x="24.13" y="34.29" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="26" x="24.13" y="36.83" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="25" x="24.13" y="39.37" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="24" x="24.13" y="41.91" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="23" x="24.13" y="44.45" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="22" x="24.13" y="46.99" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="21" x="24.13" y="49.53" drill="1.016" diameter="1.8796" rot="R180"/>
<wire x1="0" y1="0" x2="0" y2="51.435" width="0.2032" layer="51"/>
<wire x1="0" y1="51.435" x2="0" y2="58.293" width="0.2032" layer="51"/>
<wire x1="0" y1="58.293" x2="0.635" y2="58.928" width="0.2032" layer="51" curve="-90"/>
<wire x1="0.635" y1="58.928" x2="24.765" y2="58.928" width="0.2032" layer="51"/>
<wire x1="24.765" y1="58.928" x2="25.4" y2="58.293" width="0.2032" layer="51" curve="-90"/>
<wire x1="25.4" y1="58.293" x2="25.4" y2="51.435" width="0.2032" layer="51"/>
<wire x1="25.4" y1="51.435" x2="25.4" y2="0" width="0.2032" layer="51"/>
<wire x1="25.4" y1="0" x2="0" y2="0" width="0.2032" layer="51"/>
<wire x1="0" y1="51.435" x2="25.4" y2="51.435" width="0.127" layer="51"/>
<text x="2.413" y="34.29" size="0.889" layer="51" font="vector" ratio="15" align="center-left">34</text>
<text x="2.413" y="31.75" size="0.889" layer="51" font="vector" ratio="15" align="center-left">35</text>
<text x="2.413" y="39.37" size="0.889" layer="51" font="vector" ratio="15" align="center-left">32</text>
<text x="2.413" y="36.83" size="0.889" layer="51" font="vector" ratio="15" align="center-left">33</text>
<text x="2.413" y="29.21" size="0.889" layer="51" font="vector" ratio="15" align="center-left">25</text>
<text x="2.413" y="26.67" size="0.889" layer="51" font="vector" ratio="15" align="center-left">26</text>
<text x="2.3876" y="24.13" size="0.889" layer="51" font="vector" ratio="15" align="center-left">27</text>
<text x="2.3876" y="21.59" size="0.889" layer="51" font="vector" ratio="15" align="center-left">14</text>
<text x="2.3876" y="19.05" size="0.889" layer="51" font="vector" ratio="15" align="center-left">12</text>
<text x="2.413" y="16.637" size="0.889" layer="51" font="vector" ratio="15" align="center-left">13</text>
<text x="2.413" y="6.477" size="0.889" layer="51" font="vector" ratio="15" align="center-left">VBAT</text>
<text x="23.2029" y="36.83" size="0.889" layer="51" font="vector" ratio="15" align="center-right">19</text>
<text x="23.2029" y="46.99" size="0.889" layer="51" font="vector" ratio="15" align="center-right">21</text>
<text x="23.2283" y="39.37" size="0.889" layer="51" font="vector" ratio="15" align="center-right">22</text>
<text x="23.2537" y="41.91" size="0.889" layer="51" font="vector" ratio="15" align="center-right">RX</text>
<text x="23.2029" y="44.45" size="0.889" layer="51" font="vector" ratio="15" align="center-right">TX</text>
<text x="23.2029" y="34.29" size="0.889" layer="51" font="vector" ratio="15" align="center-right">23</text>
<text x="23.2029" y="31.75" size="0.889" layer="51" font="vector" ratio="15" align="center-right">18</text>
<text x="23.2029" y="29.21" size="0.889" layer="51" font="vector" ratio="15" align="center-right">5</text>
<text x="23.2029" y="26.67" size="0.889" layer="51" font="vector" ratio="15" align="center-right">15</text>
<text x="23.2029" y="24.13" size="0.889" layer="51" font="vector" ratio="15" align="center-right">2</text>
<text x="23.2029" y="21.59" size="0.889" layer="51" font="vector" ratio="15" align="center-right">0</text>
<text x="23.2029" y="19.05" size="0.889" layer="51" font="vector" ratio="15" align="center-right">4</text>
<text x="23.2029" y="16.51" size="0.889" layer="51" font="vector" ratio="15" align="center-right">17</text>
<text x="23.2029" y="13.97" size="0.889" layer="51" font="vector" ratio="15" align="center-right">16</text>
<text x="2.413" y="41.91" size="0.889" layer="51" font="vector" ratio="15" align="center-left">39</text>
<text x="2.413" y="44.45" size="0.889" layer="51" font="vector" ratio="15" align="center-left">38</text>
<text x="2.413" y="46.99" size="0.889" layer="51" font="vector" ratio="15" align="center-left">37</text>
<text x="2.413" y="49.53" size="0.889" layer="51" font="vector" ratio="15" align="center-left">36</text>
<text x="23.1267" y="49.53" size="0.889" layer="51" font="vector" ratio="15" align="center-right">GND</text>
<text x="2.413" y="14.097" size="0.889" layer="51" font="vector" ratio="15" align="center-left">!RST!</text>
<text x="2.413" y="11.557" size="0.889" layer="51" font="vector" ratio="15" align="center-left">3V3</text>
<text x="2.413" y="9.017" size="0.889" layer="51" font="vector" ratio="15" align="center-left">GND</text>
<text x="2.413" y="3.937" size="0.889" layer="51" font="vector" ratio="15" align="center-left">VUSB</text>
<text x="2.413" y="1.397" size="0.889" layer="51" font="vector" ratio="15" align="center-left">GND</text>
<text x="23.2029" y="11.43" size="0.889" layer="51" font="vector" ratio="15" align="center-right">3V3</text>
<text x="23.2029" y="8.89" size="0.889" layer="51" font="vector" ratio="15" align="center-right">GND</text>
<text x="23.2029" y="6.35" size="0.889" layer="51" font="vector" ratio="15" align="center-right">VBAT</text>
<text x="23.2029" y="3.81" size="0.889" layer="51" font="vector" ratio="15" align="center-right">VUSB</text>
<text x="23.2029" y="1.27" size="0.889" layer="51" font="vector" ratio="15" align="center-right">GND</text>
<text x="12.7" y="2.54" size="0.6096" layer="25" font="vector" ratio="20" align="top-center">&gt;Name</text>
<text x="12.7" y="1.27" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;Value</text>
<text x="12.7" y="52.324" size="0.889" layer="51" font="vector" align="center">Antenna Keepout</text>
<polygon width="0.127" layer="41">
<vertex x="0" y="51.435"/>
<vertex x="25.4" y="51.435"/>
<vertex x="25.4" y="58.928"/>
<vertex x="21.463" y="58.928"/>
<vertex x="0" y="58.928"/>
</polygon>
<polygon width="0.127" layer="42">
<vertex x="0" y="51.435"/>
<vertex x="25.4" y="51.435"/>
<vertex x="25.4" y="58.928"/>
<vertex x="21.463" y="58.928"/>
<vertex x="0" y="58.928"/>
</polygon>
</package>
<package name="ESP32_THING_NO_ANT" urn="urn:adsk.eagle:footprint:37310/1" library_version="1">
<pad name="1" x="1.27" y="1.27" drill="1.016" diameter="1.8796" shape="square" rot="R180"/>
<pad name="2" x="1.27" y="3.81" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="3" x="1.27" y="6.35" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="4" x="1.27" y="8.89" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="5" x="1.27" y="11.43" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="6" x="1.27" y="13.97" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="7" x="1.27" y="16.51" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="8" x="1.27" y="19.05" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="9" x="1.27" y="21.59" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="10" x="1.27" y="24.13" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="11" x="1.27" y="26.67" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="12" x="1.27" y="29.21" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="13" x="1.27" y="31.75" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="14" x="1.27" y="34.29" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="15" x="1.27" y="36.83" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="16" x="1.27" y="39.37" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="17" x="1.27" y="41.91" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="18" x="1.27" y="44.45" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="19" x="1.27" y="46.99" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="20" x="1.27" y="49.53" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="40" x="24.13" y="1.27" drill="1.016" diameter="1.8796" shape="square" rot="R180"/>
<pad name="39" x="24.13" y="3.81" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="38" x="24.13" y="6.35" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="37" x="24.13" y="8.89" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="36" x="24.13" y="11.43" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="35" x="24.13" y="13.97" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="34" x="24.13" y="16.51" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="33" x="24.13" y="19.05" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="32" x="24.13" y="21.59" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="31" x="24.13" y="24.13" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="30" x="24.13" y="26.67" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="29" x="24.13" y="29.21" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="28" x="24.13" y="31.75" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="27" x="24.13" y="34.29" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="26" x="24.13" y="36.83" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="25" x="24.13" y="39.37" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="24" x="24.13" y="41.91" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="23" x="24.13" y="44.45" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="22" x="24.13" y="46.99" drill="1.016" diameter="1.8796" rot="R180"/>
<pad name="21" x="24.13" y="49.53" drill="1.016" diameter="1.8796" rot="R180"/>
<wire x1="0" y1="0" x2="0" y2="51.435" width="0.2032" layer="51"/>
<wire x1="0" y1="51.435" x2="0" y2="58.293" width="0.2032" layer="51"/>
<wire x1="0" y1="58.293" x2="0.635" y2="58.928" width="0.2032" layer="51" curve="-90"/>
<wire x1="0.635" y1="58.928" x2="24.765" y2="58.928" width="0.2032" layer="51"/>
<wire x1="24.765" y1="58.928" x2="25.4" y2="58.293" width="0.2032" layer="51" curve="-90"/>
<wire x1="25.4" y1="58.293" x2="25.4" y2="51.435" width="0.2032" layer="51"/>
<wire x1="25.4" y1="51.435" x2="25.4" y2="0" width="0.2032" layer="51"/>
<wire x1="25.4" y1="0" x2="0" y2="0" width="0.2032" layer="51"/>
<text x="2.413" y="34.29" size="0.889" layer="51" font="vector" ratio="15" align="center-left">34</text>
<text x="2.413" y="31.75" size="0.889" layer="51" font="vector" ratio="15" align="center-left">35</text>
<text x="2.413" y="39.37" size="0.889" layer="51" font="vector" ratio="15" align="center-left">32</text>
<text x="2.413" y="36.83" size="0.889" layer="51" font="vector" ratio="15" align="center-left">33</text>
<text x="2.413" y="29.21" size="0.889" layer="51" font="vector" ratio="15" align="center-left">25</text>
<text x="2.413" y="26.67" size="0.889" layer="51" font="vector" ratio="15" align="center-left">26</text>
<text x="2.3876" y="24.13" size="0.889" layer="51" font="vector" ratio="15" align="center-left">27</text>
<text x="2.3876" y="21.59" size="0.889" layer="51" font="vector" ratio="15" align="center-left">14</text>
<text x="2.3876" y="19.05" size="0.889" layer="51" font="vector" ratio="15" align="center-left">12</text>
<text x="2.413" y="16.637" size="0.889" layer="51" font="vector" ratio="15" align="center-left">13</text>
<text x="2.413" y="6.477" size="0.889" layer="51" font="vector" ratio="15" align="center-left">VBAT</text>
<text x="23.2029" y="36.83" size="0.889" layer="51" font="vector" ratio="15" align="center-right">19</text>
<text x="23.2029" y="46.99" size="0.889" layer="51" font="vector" ratio="15" align="center-right">21</text>
<text x="23.2283" y="39.37" size="0.889" layer="51" font="vector" ratio="15" align="center-right">22</text>
<text x="23.2537" y="41.91" size="0.889" layer="51" font="vector" ratio="15" align="center-right">RX</text>
<text x="23.2029" y="44.45" size="0.889" layer="51" font="vector" ratio="15" align="center-right">TX</text>
<text x="23.2029" y="34.29" size="0.889" layer="51" font="vector" ratio="15" align="center-right">23</text>
<text x="23.2029" y="31.75" size="0.889" layer="51" font="vector" ratio="15" align="center-right">18</text>
<text x="23.2029" y="29.21" size="0.889" layer="51" font="vector" ratio="15" align="center-right">5</text>
<text x="23.2029" y="26.67" size="0.889" layer="51" font="vector" ratio="15" align="center-right">15</text>
<text x="23.2029" y="24.13" size="0.889" layer="51" font="vector" ratio="15" align="center-right">2</text>
<text x="23.2029" y="21.59" size="0.889" layer="51" font="vector" ratio="15" align="center-right">0</text>
<text x="23.2029" y="19.05" size="0.889" layer="51" font="vector" ratio="15" align="center-right">4</text>
<text x="23.2029" y="16.51" size="0.889" layer="51" font="vector" ratio="15" align="center-right">17</text>
<text x="23.2029" y="13.97" size="0.889" layer="51" font="vector" ratio="15" align="center-right">16</text>
<text x="2.413" y="41.91" size="0.889" layer="51" font="vector" ratio="15" align="center-left">39</text>
<text x="2.413" y="44.45" size="0.889" layer="51" font="vector" ratio="15" align="center-left">38</text>
<text x="2.413" y="46.99" size="0.889" layer="51" font="vector" ratio="15" align="center-left">37</text>
<text x="2.413" y="49.53" size="0.889" layer="51" font="vector" ratio="15" align="center-left">36</text>
<text x="23.1267" y="49.53" size="0.889" layer="51" font="vector" ratio="15" align="center-right">GND</text>
<text x="2.413" y="14.097" size="0.889" layer="51" font="vector" ratio="15" align="center-left">!RST!</text>
<text x="2.413" y="11.557" size="0.889" layer="51" font="vector" ratio="15" align="center-left">3V3</text>
<text x="2.413" y="9.017" size="0.889" layer="51" font="vector" ratio="15" align="center-left">GND</text>
<text x="2.413" y="3.937" size="0.889" layer="51" font="vector" ratio="15" align="center-left">VUSB</text>
<text x="2.413" y="1.397" size="0.889" layer="51" font="vector" ratio="15" align="center-left">GND</text>
<text x="23.2029" y="11.43" size="0.889" layer="51" font="vector" ratio="15" align="center-right">3V3</text>
<text x="23.2029" y="8.89" size="0.889" layer="51" font="vector" ratio="15" align="center-right">GND</text>
<text x="23.2029" y="6.35" size="0.889" layer="51" font="vector" ratio="15" align="center-right">VBAT</text>
<text x="23.2029" y="3.81" size="0.889" layer="51" font="vector" ratio="15" align="center-right">VUSB</text>
<text x="23.2029" y="1.27" size="0.889" layer="51" font="vector" ratio="15" align="center-right">GND</text>
<text x="12.7" y="2.54" size="0.6096" layer="25" font="vector" ratio="20" align="top-center">&gt;Name</text>
<text x="12.7" y="1.27" size="0.6096" layer="27" font="vector" ratio="20" align="top-center">&gt;Value</text>
</package>
</packages>
<packages3d>
<package3d name="ESP32_THING" urn="urn:adsk.eagle:package:37350/1" type="box" library_version="1">
<description>SparkFun ESP32 Thing Board
Pins, silkscreen, and logos matching the layout of the SparkFun ESP32 Thing

Specifications:
Pin count: 40
Area: 2.32" x 1.00"
</description>
<packageinstances>
<packageinstance name="ESP32_THING"/>
</packageinstances>
</package3d>
<package3d name="ESP32_THING_NO_ANT" urn="urn:adsk.eagle:package:37351/1" type="box" library_version="1">
<packageinstances>
<packageinstance name="ESP32_THING_NO_ANT"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="ESP32_THING" urn="urn:adsk.eagle:symbol:37308/1" library_version="1">
<description>&lt;h3&gt;SparkFun ESP32 Thing Board&lt;/h3&gt;
&lt;p&gt;Pins, silkscreen, and logos matching the layout of the &lt;a href=https://www.sparkfun.com/products/13907&gt;SparkFun ESP32 Thing&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;Dimensions: 2.32" x 1.00"&lt;/p&gt;</description>
<pin name="GND@1" x="-12.7" y="-22.86" length="short"/>
<pin name="VUSB@2" x="-12.7" y="-20.32" length="short"/>
<pin name="VBAT@3" x="-12.7" y="-17.78" length="short"/>
<pin name="GND@4" x="-12.7" y="-15.24" length="short"/>
<pin name="3V3@5" x="-12.7" y="-12.7" length="short"/>
<pin name="!RST!" x="-12.7" y="-10.16" length="short"/>
<pin name="13" x="-12.7" y="-7.62" length="short"/>
<pin name="12" x="-12.7" y="-5.08" length="short"/>
<pin name="14" x="-12.7" y="-2.54" length="short"/>
<pin name="27" x="-12.7" y="0" length="short"/>
<pin name="26" x="-12.7" y="2.54" length="short"/>
<pin name="25" x="-12.7" y="5.08" length="short"/>
<pin name="35" x="-12.7" y="7.62" length="short"/>
<pin name="34" x="-12.7" y="10.16" length="short"/>
<pin name="33" x="-12.7" y="12.7" length="short"/>
<pin name="32" x="-12.7" y="15.24" length="short"/>
<pin name="39" x="-12.7" y="17.78" length="short"/>
<pin name="38" x="-12.7" y="20.32" length="short"/>
<pin name="37" x="-12.7" y="22.86" length="short"/>
<pin name="36" x="-12.7" y="25.4" length="short"/>
<pin name="GND@40" x="12.7" y="-22.86" length="short" rot="R180"/>
<pin name="VUSB@39" x="12.7" y="-20.32" length="short" rot="R180"/>
<pin name="VBAT@38" x="12.7" y="-17.78" length="short" rot="R180"/>
<pin name="GND@37" x="12.7" y="-15.24" length="short" rot="R180"/>
<pin name="3V3@36" x="12.7" y="-12.7" length="short" rot="R180"/>
<pin name="16" x="12.7" y="-10.16" length="short" rot="R180"/>
<pin name="17" x="12.7" y="-7.62" length="short" rot="R180"/>
<pin name="4" x="12.7" y="-5.08" length="short" rot="R180"/>
<pin name="0" x="12.7" y="-2.54" length="short" rot="R180"/>
<pin name="2" x="12.7" y="0" length="short" rot="R180"/>
<pin name="15" x="12.7" y="2.54" length="short" rot="R180"/>
<pin name="5" x="12.7" y="5.08" length="short" rot="R180"/>
<pin name="18" x="12.7" y="7.62" length="short" rot="R180"/>
<pin name="23" x="12.7" y="10.16" length="short" rot="R180"/>
<pin name="19" x="12.7" y="12.7" length="short" rot="R180"/>
<pin name="22" x="12.7" y="15.24" length="short" rot="R180"/>
<pin name="RX" x="12.7" y="17.78" length="short" rot="R180"/>
<pin name="TX" x="12.7" y="20.32" length="short" rot="R180"/>
<pin name="21" x="12.7" y="22.86" length="short" rot="R180"/>
<pin name="GND@21" x="12.7" y="25.4" length="short" rot="R180"/>
<wire x1="-10.16" y1="-25.4" x2="-10.16" y2="27.94" width="0.254" layer="94"/>
<wire x1="-10.16" y1="27.94" x2="10.16" y2="27.94" width="0.254" layer="94"/>
<wire x1="10.16" y1="27.94" x2="10.16" y2="-25.4" width="0.254" layer="94"/>
<wire x1="10.16" y1="-25.4" x2="-10.16" y2="-25.4" width="0.254" layer="94"/>
<text x="-10.16" y="28.194" size="1.778" layer="95" font="vector">&gt;Name</text>
<text x="-10.16" y="-25.654" size="1.778" layer="96" font="vector" align="top-left">&gt;Value</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="ESP32_THING" urn="urn:adsk.eagle:component:37380/1" prefix="B" library_version="1">
<description>&lt;h3&gt;SparkFun ESP32 Thing Board&lt;/h3&gt;
&lt;p&gt;Pins, silkscreen, and logos matching the layout of the &lt;a href=https://www.sparkfun.com/products/13907&gt;SparkFun ESP32 Thing&lt;/a&gt;&lt;/p&gt;
&lt;p&gt;Dimensions: 2.32" x 1.00"&lt;/p&gt;</description>
<gates>
<gate name="G$1" symbol="ESP32_THING" x="0" y="0"/>
</gates>
<devices>
<device name="_W_ANT_KEEPOUT" package="ESP32_THING">
<connects>
<connect gate="G$1" pin="!RST!" pad="6"/>
<connect gate="G$1" pin="0" pad="32"/>
<connect gate="G$1" pin="12" pad="8"/>
<connect gate="G$1" pin="13" pad="7"/>
<connect gate="G$1" pin="14" pad="9"/>
<connect gate="G$1" pin="15" pad="30"/>
<connect gate="G$1" pin="16" pad="35"/>
<connect gate="G$1" pin="17" pad="34"/>
<connect gate="G$1" pin="18" pad="28"/>
<connect gate="G$1" pin="19" pad="26"/>
<connect gate="G$1" pin="2" pad="31"/>
<connect gate="G$1" pin="21" pad="22"/>
<connect gate="G$1" pin="22" pad="25"/>
<connect gate="G$1" pin="23" pad="27"/>
<connect gate="G$1" pin="25" pad="12"/>
<connect gate="G$1" pin="26" pad="11"/>
<connect gate="G$1" pin="27" pad="10"/>
<connect gate="G$1" pin="32" pad="16"/>
<connect gate="G$1" pin="33" pad="15"/>
<connect gate="G$1" pin="34" pad="14"/>
<connect gate="G$1" pin="35" pad="13"/>
<connect gate="G$1" pin="36" pad="20"/>
<connect gate="G$1" pin="37" pad="19"/>
<connect gate="G$1" pin="38" pad="18"/>
<connect gate="G$1" pin="39" pad="17"/>
<connect gate="G$1" pin="3V3@36" pad="36"/>
<connect gate="G$1" pin="3V3@5" pad="5"/>
<connect gate="G$1" pin="4" pad="33"/>
<connect gate="G$1" pin="5" pad="29"/>
<connect gate="G$1" pin="GND@1" pad="1"/>
<connect gate="G$1" pin="GND@21" pad="21"/>
<connect gate="G$1" pin="GND@37" pad="37"/>
<connect gate="G$1" pin="GND@4" pad="4"/>
<connect gate="G$1" pin="GND@40" pad="40"/>
<connect gate="G$1" pin="RX" pad="24"/>
<connect gate="G$1" pin="TX" pad="23"/>
<connect gate="G$1" pin="VBAT@3" pad="3"/>
<connect gate="G$1" pin="VBAT@38" pad="38"/>
<connect gate="G$1" pin="VUSB@2" pad="2"/>
<connect gate="G$1" pin="VUSB@39" pad="39"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37350/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="SF_ID" value="DEV-13907"/>
</technology>
</technologies>
</device>
<device name="_WO_ANT_KEEPOUT" package="ESP32_THING_NO_ANT">
<connects>
<connect gate="G$1" pin="!RST!" pad="6"/>
<connect gate="G$1" pin="0" pad="32"/>
<connect gate="G$1" pin="12" pad="8"/>
<connect gate="G$1" pin="13" pad="7"/>
<connect gate="G$1" pin="14" pad="9"/>
<connect gate="G$1" pin="15" pad="30"/>
<connect gate="G$1" pin="16" pad="35"/>
<connect gate="G$1" pin="17" pad="34"/>
<connect gate="G$1" pin="18" pad="28"/>
<connect gate="G$1" pin="19" pad="26"/>
<connect gate="G$1" pin="2" pad="31"/>
<connect gate="G$1" pin="21" pad="22"/>
<connect gate="G$1" pin="22" pad="25"/>
<connect gate="G$1" pin="23" pad="27"/>
<connect gate="G$1" pin="25" pad="12"/>
<connect gate="G$1" pin="26" pad="11"/>
<connect gate="G$1" pin="27" pad="10"/>
<connect gate="G$1" pin="32" pad="16"/>
<connect gate="G$1" pin="33" pad="15"/>
<connect gate="G$1" pin="34" pad="14"/>
<connect gate="G$1" pin="35" pad="13"/>
<connect gate="G$1" pin="36" pad="20"/>
<connect gate="G$1" pin="37" pad="19"/>
<connect gate="G$1" pin="38" pad="18"/>
<connect gate="G$1" pin="39" pad="17"/>
<connect gate="G$1" pin="3V3@36" pad="36"/>
<connect gate="G$1" pin="3V3@5" pad="5"/>
<connect gate="G$1" pin="4" pad="33"/>
<connect gate="G$1" pin="5" pad="29"/>
<connect gate="G$1" pin="GND@1" pad="1"/>
<connect gate="G$1" pin="GND@21" pad="21"/>
<connect gate="G$1" pin="GND@37" pad="37"/>
<connect gate="G$1" pin="GND@4" pad="4"/>
<connect gate="G$1" pin="GND@40" pad="40"/>
<connect gate="G$1" pin="RX" pad="24"/>
<connect gate="G$1" pin="TX" pad="23"/>
<connect gate="G$1" pin="VBAT@3" pad="3"/>
<connect gate="G$1" pin="VBAT@38" pad="38"/>
<connect gate="G$1" pin="VUSB@2" pad="2"/>
<connect gate="G$1" pin="VUSB@39" pad="39"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:37351/1"/>
</package3dinstances>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U1" library="si4735" deviceset="SI4735-D60-GU" device=""/>
<part name="GND1" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="R4" library="SparkFun-Resistors" library_urn="urn:adsk.eagle:library:532" deviceset="4.7KOHM" device="-0603-1/10W-1%" package3d_urn="urn:adsk.eagle:package:39650/1" value="4.7k"/>
<part name="R5" library="SparkFun-Resistors" library_urn="urn:adsk.eagle:library:532" deviceset="4.7KOHM" device="-0603-1/10W-1%" package3d_urn="urn:adsk.eagle:package:39650/1" value="4.7k"/>
<part name="SUPPLY1" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="VCC" device=""/>
<part name="C1" library="SparkFun-Capacitors" library_urn="urn:adsk.eagle:library:510" deviceset="22NF/22,000PF" device="-0603-50V-10%" package3d_urn="urn:adsk.eagle:package:37414/1" value="22nF"/>
<part name="Y1" library="SparkFun-Clocks" library_urn="urn:adsk.eagle:library:511" deviceset="CRYSTAL" device="PTH-2X6" package3d_urn="urn:adsk.eagle:package:37514/1"/>
<part name="GND3" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="C6" library="SparkFun-Capacitors" library_urn="urn:adsk.eagle:library:510" deviceset="22PF" device="-0603-50V-5%" package3d_urn="urn:adsk.eagle:package:37414/1" value="22pF"/>
<part name="C5" library="SparkFun-Capacitors" library_urn="urn:adsk.eagle:library:510" deviceset="22PF" device="-0603-50V-5%" package3d_urn="urn:adsk.eagle:package:37414/1" value="22pF"/>
<part name="C8" library="SparkFun-Capacitors" library_urn="urn:adsk.eagle:library:510" deviceset="4.7UF" device="-1206-16V-(+80/-20%)" package3d_urn="urn:adsk.eagle:package:37426/1" value="4.7uF"/>
<part name="C7" library="SparkFun-Capacitors" library_urn="urn:adsk.eagle:library:510" deviceset="4.7UF" device="-1206-16V-(+80/-20%)" package3d_urn="urn:adsk.eagle:package:37426/1" value="4.7uF"/>
<part name="J1" library="SparkFun-Connectors" library_urn="urn:adsk.eagle:library:513" deviceset="AUDIO_JACK_2.5MM" device="SPECIAL_POGO_PINS" package3d_urn="urn:adsk.eagle:package:38005/1"/>
<part name="GND4" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="C4" library="rcl" library_urn="urn:adsk.eagle:library:334" deviceset="CX" device="XC10B6" package3d_urn="urn:adsk.eagle:package:23700/1" value="100nF"/>
<part name="C3" library="rcl" library_urn="urn:adsk.eagle:library:334" deviceset="CX" device="XC10B6" package3d_urn="urn:adsk.eagle:package:23700/1"/>
<part name="L1" library="ngspice-simulation" library_urn="urn:adsk.eagle:library:527439" deviceset="L" device=""/>
<part name="C2" library="rcl" library_urn="urn:adsk.eagle:library:334" deviceset="CX" device="XC10B6" package3d_urn="urn:adsk.eagle:package:23700/1"/>
<part name="FM" library="discrete" library_urn="urn:adsk.eagle:library:211" deviceset="ANTENNA" device="" package3d_urn="urn:adsk.eagle:package:12928/1"/>
<part name="AM" library="discrete" library_urn="urn:adsk.eagle:library:211" deviceset="ANTENNA" device="" package3d_urn="urn:adsk.eagle:package:12928/1"/>
<part name="GND5" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="R3" library="SparkFun-Resistors" library_urn="urn:adsk.eagle:library:532" deviceset="2.2KOHM" device="-0603-1/10W-1%" package3d_urn="urn:adsk.eagle:package:39650/1" value="2.2k"/>
<part name="GND7" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="ESP32" library="SparkFun-Boards" library_urn="urn:adsk.eagle:library:509" deviceset="ESP32_THING" device="_WO_ANT_KEEPOUT" package3d_urn="urn:adsk.eagle:package:37351/1" value="See your ESP32 pinout"/>
<part name="SUPPLY2" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="VCC" device=""/>
<part name="GND2" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
<part name="SUPPLY3" library="supply2" library_urn="urn:adsk.eagle:library:372" deviceset="VCC" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U1" gate="A" x="-129.54" y="162.56" smashed="yes">
<attribute name="NAME" x="-98.7044" y="171.6786" size="2.0828" layer="95" ratio="6" rot="SR0"/>
<attribute name="VALUE" x="-99.3394" y="169.1386" size="2.0828" layer="96" ratio="6" rot="SR0"/>
</instance>
<instance part="GND1" gate="1" x="-50.8" y="132.08" smashed="yes">
<attribute name="VALUE" x="-53.34" y="129.54" size="1.778" layer="96"/>
</instance>
<instance part="R4" gate="G$1" x="-38.1" y="114.3" smashed="yes" rot="R90">
<attribute name="NAME" x="-39.624" y="114.3" size="1.778" layer="95" font="vector" rot="R90" align="bottom-center"/>
<attribute name="VALUE" x="-36.576" y="114.3" size="1.778" layer="96" font="vector" rot="R90" align="top-center"/>
</instance>
<instance part="R5" gate="G$1" x="-27.94" y="114.3" smashed="yes" rot="R90">
<attribute name="NAME" x="-32.004" y="114.3" size="1.778" layer="95" font="vector" rot="R270" align="bottom-center"/>
<attribute name="VALUE" x="-23.876" y="114.3" size="1.778" layer="96" font="vector" rot="R270" align="top-center"/>
</instance>
<instance part="SUPPLY1" gate="G$1" x="-43.18" y="154.94" smashed="yes" rot="R270">
<attribute name="VALUE" x="-40.005" y="154.305" size="1.778" layer="96"/>
</instance>
<instance part="C1" gate="G$1" x="-50.8" y="160.02" smashed="yes" rot="R90">
<attribute name="NAME" x="-53.721" y="161.544" size="1.778" layer="95" font="vector" rot="R90"/>
<attribute name="VALUE" x="-48.641" y="161.544" size="1.778" layer="96" font="vector" rot="R90"/>
</instance>
<instance part="Y1" gate="G$1" x="-96.52" y="187.96" smashed="yes">
<attribute name="NAME" x="-96.52" y="189.992" size="1.778" layer="95" font="vector" align="bottom-center"/>
<attribute name="VALUE" x="-96.52" y="185.928" size="1.778" layer="96" font="vector" align="top-center"/>
</instance>
<instance part="GND3" gate="1" x="-96.52" y="177.8" smashed="yes">
<attribute name="VALUE" x="-99.06" y="175.26" size="1.778" layer="96"/>
</instance>
<instance part="C6" gate="G$1" x="-86.36" y="182.88" smashed="yes">
<attribute name="NAME" x="-84.836" y="185.801" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="-84.836" y="180.721" size="1.778" layer="96" font="vector"/>
</instance>
<instance part="C5" gate="G$1" x="-109.22" y="182.88" smashed="yes">
<attribute name="NAME" x="-107.696" y="185.801" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="-107.696" y="180.721" size="1.778" layer="96" font="vector"/>
</instance>
<instance part="C8" gate="G$1" x="-50.8" y="172.72" smashed="yes">
<attribute name="NAME" x="-49.276" y="175.641" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="-49.276" y="170.561" size="1.778" layer="96" font="vector"/>
</instance>
<instance part="C7" gate="G$1" x="-60.96" y="172.72" smashed="yes">
<attribute name="NAME" x="-59.436" y="175.641" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="-59.436" y="170.561" size="1.778" layer="96" font="vector"/>
</instance>
<instance part="J1" gate="G$1" x="-68.58" y="182.88" smashed="yes">
<attribute name="NAME" x="-73.66" y="185.928" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="-73.66" y="177.8" size="1.778" layer="96" font="vector"/>
</instance>
<instance part="GND4" gate="1" x="-40.64" y="180.34" smashed="yes">
<attribute name="VALUE" x="-43.18" y="177.8" size="1.778" layer="96"/>
</instance>
<instance part="C4" gate="G$1" x="-38.1" y="172.72" smashed="yes">
<attribute name="NAME" x="-36.576" y="173.101" size="1.778" layer="95"/>
<attribute name="VALUE" x="-44.196" y="168.021" size="1.778" layer="96"/>
</instance>
<instance part="C3" gate="G$1" x="-124.46" y="119.38" smashed="yes">
<attribute name="NAME" x="-122.936" y="119.761" size="1.778" layer="95"/>
<attribute name="VALUE" x="-122.936" y="114.681" size="1.778" layer="96"/>
</instance>
<instance part="L1" gate="G$1" x="-124.46" y="106.68" smashed="yes" rot="R90">
<attribute name="NAME" x="-127" y="104.14" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="-121.92" y="104.14" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="C2" gate="G$1" x="-149.86" y="154.94" smashed="yes">
<attribute name="NAME" x="-148.336" y="155.321" size="1.778" layer="95"/>
<attribute name="VALUE" x="-148.336" y="150.241" size="1.778" layer="96"/>
</instance>
<instance part="FM" gate="A" x="-167.64" y="165.1" smashed="yes">
<attribute name="NAME" x="-170.18" y="166.37" size="1.778" layer="95"/>
<attribute name="VALUE" x="-165.1" y="160.02" size="1.778" layer="96"/>
</instance>
<instance part="AM" gate="A" x="-106.68" y="121.92" smashed="yes">
<attribute name="NAME" x="-109.22" y="123.19" size="1.778" layer="95"/>
<attribute name="VALUE" x="-104.14" y="116.84" size="1.778" layer="96"/>
</instance>
<instance part="GND5" gate="1" x="-152.4" y="121.92" smashed="yes">
<attribute name="VALUE" x="-154.94" y="119.38" size="1.778" layer="96"/>
</instance>
<instance part="R3" gate="G$1" x="-121.92" y="187.96" smashed="yes">
<attribute name="NAME" x="-121.92" y="189.484" size="1.778" layer="95" font="vector" align="bottom-center"/>
<attribute name="VALUE" x="-121.92" y="186.436" size="1.778" layer="96" font="vector" align="top-center"/>
</instance>
<instance part="GND7" gate="1" x="-152.4" y="132.08" smashed="yes">
<attribute name="VALUE" x="-154.94" y="129.54" size="1.778" layer="96"/>
</instance>
<instance part="ESP32" gate="G$1" x="-2.54" y="165.1" smashed="yes">
<attribute name="NAME" x="-12.7" y="193.294" size="1.778" layer="95" font="vector"/>
<attribute name="VALUE" x="-15.24" y="136.906" size="1.778" layer="96" font="vector" align="top-left"/>
</instance>
<instance part="SUPPLY2" gate="G$1" x="-33.02" y="101.6" smashed="yes" rot="R180">
<attribute name="VALUE" x="-31.115" y="98.425" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="GND2" gate="1" x="12.7" y="203.2" smashed="yes" rot="R180">
<attribute name="VALUE" x="15.24" y="205.74" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="SUPPLY3" gate="G$1" x="-20.32" y="154.94" smashed="yes">
<attribute name="VALUE" x="-22.225" y="158.115" size="1.778" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="U1" gate="A" pin="!SEN"/>
<wire x1="-60.96" y1="142.24" x2="-50.8" y2="142.24" width="0.1524" layer="91"/>
<pinref part="GND1" gate="1" pin="GND"/>
<wire x1="-50.8" y1="142.24" x2="-50.8" y2="134.62" width="0.1524" layer="91"/>
<pinref part="U1" gate="A" pin="GND"/>
<pinref part="U1" gate="A" pin="GND_2"/>
<wire x1="-60.96" y1="137.16" x2="-60.96" y2="134.62" width="0.1524" layer="91"/>
<wire x1="-50.8" y1="134.62" x2="-60.96" y2="134.62" width="0.1524" layer="91"/>
<junction x="-50.8" y="134.62"/>
<junction x="-60.96" y="134.62"/>
<junction x="-60.96" y="137.16"/>
<junction x="-60.96" y="142.24"/>
</segment>
<segment>
<pinref part="C5" gate="G$1" pin="2"/>
<pinref part="GND3" gate="1" pin="GND"/>
<wire x1="-109.22" y1="180.34" x2="-96.52" y2="180.34" width="0.1524" layer="91"/>
<pinref part="C6" gate="G$1" pin="2"/>
<wire x1="-86.36" y1="180.34" x2="-96.52" y2="180.34" width="0.1524" layer="91"/>
<junction x="-96.52" y="180.34"/>
</segment>
<segment>
<pinref part="J1" gate="G$1" pin="SLEEVE"/>
<wire x1="-63.5" y1="185.42" x2="-40.64" y2="185.42" width="0.1524" layer="91"/>
<pinref part="GND4" gate="1" pin="GND"/>
<wire x1="-40.64" y1="185.42" x2="-40.64" y2="182.88" width="0.1524" layer="91"/>
<junction x="-40.64" y="182.88"/>
<wire x1="-40.64" y1="182.88" x2="-35.56" y2="182.88" width="0.1524" layer="91"/>
<wire x1="-35.56" y1="182.88" x2="-35.56" y2="175.26" width="0.1524" layer="91"/>
<pinref part="C4" gate="G$1" pin="1"/>
<wire x1="-35.56" y1="175.26" x2="-38.1" y2="175.26" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U1" gate="A" pin="RFGND"/>
<wire x1="-127" y1="142.24" x2="-132.08" y2="142.24" width="0.1524" layer="91"/>
<wire x1="-132.08" y1="142.24" x2="-132.08" y2="124.46" width="0.1524" layer="91"/>
<pinref part="L1" gate="G$1" pin="1"/>
<wire x1="-132.08" y1="124.46" x2="-132.08" y2="101.6" width="0.1524" layer="91"/>
<wire x1="-132.08" y1="101.6" x2="-124.46" y2="101.6" width="0.1524" layer="91"/>
<pinref part="GND5" gate="1" pin="GND"/>
<wire x1="-152.4" y1="124.46" x2="-132.08" y2="124.46" width="0.1524" layer="91"/>
<junction x="-132.08" y="124.46"/>
</segment>
<segment>
<pinref part="U1" gate="A" pin="NC_4"/>
<pinref part="U1" gate="A" pin="NC"/>
<wire x1="-127" y1="139.7" x2="-127" y2="137.16" width="0.1524" layer="91"/>
<wire x1="-127" y1="137.16" x2="-152.4" y2="137.16" width="0.1524" layer="91"/>
<junction x="-127" y="137.16"/>
<pinref part="GND7" gate="1" pin="GND"/>
<wire x1="-152.4" y1="137.16" x2="-152.4" y2="134.62" width="0.1524" layer="91"/>
<junction x="-127" y="139.7"/>
</segment>
<segment>
<pinref part="ESP32" gate="G$1" pin="GND@21"/>
<wire x1="10.16" y1="190.5" x2="12.7" y2="190.5" width="0.1524" layer="91"/>
<pinref part="GND2" gate="1" pin="GND"/>
<wire x1="12.7" y1="190.5" x2="12.7" y2="200.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="SDA" class="0">
<segment>
<pinref part="R5" gate="G$1" pin="2"/>
<wire x1="-27.94" y1="129.54" x2="-27.94" y2="127" width="0.1524" layer="91"/>
<pinref part="U1" gate="A" pin="SDIO"/>
<wire x1="-27.94" y1="127" x2="-27.94" y2="119.38" width="0.1524" layer="91"/>
<wire x1="-60.96" y1="147.32" x2="-33.02" y2="147.32" width="0.1524" layer="91"/>
<wire x1="-33.02" y1="147.32" x2="-33.02" y2="129.54" width="0.1524" layer="91"/>
<wire x1="-33.02" y1="129.54" x2="-27.94" y2="129.54" width="0.1524" layer="91"/>
<pinref part="ESP32" gate="G$1" pin="22"/>
<wire x1="10.16" y1="180.34" x2="10.16" y2="177.8" width="0.1524" layer="91"/>
<wire x1="10.16" y1="177.8" x2="27.94" y2="177.8" width="0.1524" layer="91"/>
<wire x1="27.94" y1="177.8" x2="27.94" y2="127" width="0.1524" layer="91"/>
<wire x1="27.94" y1="127" x2="-27.94" y2="127" width="0.1524" layer="91"/>
<junction x="-27.94" y="127"/>
<pinref part="ESP32" gate="G$1" pin="19"/>
<junction x="10.16" y="177.8"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="R4" gate="G$1" pin="2"/>
<pinref part="U1" gate="A" pin="SCLK"/>
<wire x1="-60.96" y1="144.78" x2="-38.1" y2="144.78" width="0.1524" layer="91"/>
<wire x1="-38.1" y1="144.78" x2="-38.1" y2="121.92" width="0.1524" layer="91"/>
<pinref part="ESP32" gate="G$1" pin="21"/>
<wire x1="-38.1" y1="121.92" x2="-38.1" y2="119.38" width="0.1524" layer="91"/>
<wire x1="10.16" y1="187.96" x2="30.48" y2="187.96" width="0.1524" layer="91"/>
<wire x1="30.48" y1="187.96" x2="30.48" y2="121.92" width="0.1524" layer="91"/>
<wire x1="30.48" y1="121.92" x2="-38.1" y2="121.92" width="0.1524" layer="91"/>
<junction x="-38.1" y="121.92"/>
</segment>
</net>
<net name="VCC" class="0">
<segment>
<pinref part="U1" gate="A" pin="VA"/>
<pinref part="U1" gate="A" pin="VD"/>
<wire x1="-60.96" y1="154.94" x2="-60.96" y2="152.4" width="0.1524" layer="91"/>
<pinref part="SUPPLY1" gate="G$1" pin="VCC"/>
<wire x1="-60.96" y1="154.94" x2="-45.72" y2="154.94" width="0.1524" layer="91"/>
<junction x="-60.96" y="154.94"/>
<pinref part="C1" gate="G$1" pin="2"/>
<wire x1="-48.26" y1="160.02" x2="-45.72" y2="160.02" width="0.1524" layer="91"/>
<wire x1="-45.72" y1="160.02" x2="-45.72" y2="154.94" width="0.1524" layer="91"/>
<junction x="-45.72" y="154.94"/>
<pinref part="C4" gate="G$1" pin="2"/>
<wire x1="-38.1" y1="167.64" x2="-38.1" y2="160.02" width="0.1524" layer="91"/>
<wire x1="-38.1" y1="160.02" x2="-45.72" y2="160.02" width="0.1524" layer="91"/>
<junction x="-45.72" y="160.02"/>
</segment>
<segment>
<pinref part="R4" gate="G$1" pin="1"/>
<wire x1="-38.1" y1="109.22" x2="-33.02" y2="109.22" width="0.1524" layer="91"/>
<wire x1="-33.02" y1="109.22" x2="-33.02" y2="104.14" width="0.1524" layer="91"/>
<pinref part="R5" gate="G$1" pin="1"/>
<wire x1="-27.94" y1="109.22" x2="-33.02" y2="109.22" width="0.1524" layer="91"/>
<junction x="-33.02" y="109.22"/>
<pinref part="SUPPLY2" gate="G$1" pin="VCC"/>
<junction x="-33.02" y="104.14"/>
</segment>
<segment>
<pinref part="ESP32" gate="G$1" pin="3V3@5"/>
<pinref part="SUPPLY3" gate="G$1" pin="VCC"/>
<wire x1="-15.24" y1="152.4" x2="-20.32" y2="152.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="C1" gate="G$1" pin="1"/>
<wire x1="-55.88" y1="160.02" x2="-55.88" y2="157.48" width="0.1524" layer="91"/>
<pinref part="U1" gate="A" pin="DBYP"/>
<wire x1="-55.88" y1="157.48" x2="-60.96" y2="157.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="Y1" gate="G$1" pin="2"/>
<pinref part="C6" gate="G$1" pin="1"/>
<wire x1="-93.98" y1="187.96" x2="-86.36" y2="187.96" width="0.1524" layer="91"/>
<pinref part="U1" gate="A" pin="RCLK"/>
<wire x1="-60.96" y1="149.86" x2="-33.02" y2="149.86" width="0.1524" layer="91"/>
<wire x1="-33.02" y1="149.86" x2="-33.02" y2="187.96" width="0.1524" layer="91"/>
<wire x1="-33.02" y1="187.96" x2="-86.36" y2="187.96" width="0.1524" layer="91"/>
<junction x="-86.36" y="187.96"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="U1" gate="A" pin="GPO3/[DCLK]"/>
<wire x1="-127" y1="157.48" x2="-132.08" y2="157.48" width="0.1524" layer="91"/>
<wire x1="-132.08" y1="157.48" x2="-132.08" y2="187.96" width="0.1524" layer="91"/>
<wire x1="-132.08" y1="187.96" x2="-127" y2="187.96" width="0.1524" layer="91"/>
<pinref part="R3" gate="G$1" pin="1"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="U1" gate="A" pin="LOUT/[DFS]"/>
<pinref part="C7" gate="G$1" pin="2"/>
<wire x1="-60.96" y1="162.56" x2="-60.96" y2="170.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="U1" gate="A" pin="ROUT/[DOUT]"/>
<wire x1="-60.96" y1="160.02" x2="-58.42" y2="160.02" width="0.1524" layer="91"/>
<wire x1="-58.42" y1="160.02" x2="-58.42" y2="167.64" width="0.1524" layer="91"/>
<wire x1="-58.42" y1="167.64" x2="-50.8" y2="167.64" width="0.1524" layer="91"/>
<pinref part="C8" gate="G$1" pin="2"/>
<wire x1="-50.8" y1="167.64" x2="-50.8" y2="170.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="J1" gate="G$1" pin="LEFT"/>
<pinref part="C7" gate="G$1" pin="1"/>
<wire x1="-63.5" y1="180.34" x2="-60.96" y2="180.34" width="0.1524" layer="91"/>
<wire x1="-60.96" y1="180.34" x2="-60.96" y2="177.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="J1" gate="G$1" pin="RIGHT"/>
<pinref part="C8" gate="G$1" pin="1"/>
<wire x1="-63.5" y1="182.88" x2="-50.8" y2="182.88" width="0.1524" layer="91"/>
<wire x1="-50.8" y1="182.88" x2="-50.8" y2="177.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="L1" gate="G$1" pin="2"/>
<pinref part="C3" gate="G$1" pin="2"/>
<junction x="-124.46" y="114.3"/>
<pinref part="AM" gate="A" pin="1"/>
<wire x1="-106.68" y1="116.84" x2="-106.68" y2="114.3" width="0.1524" layer="91"/>
<wire x1="-106.68" y1="114.3" x2="-124.46" y2="114.3" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="U1" gate="A" pin="AMI"/>
<wire x1="-127" y1="134.62" x2="-127" y2="121.92" width="0.1524" layer="91"/>
<pinref part="C3" gate="G$1" pin="1"/>
<wire x1="-127" y1="121.92" x2="-124.46" y2="121.92" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="U1" gate="A" pin="FMI"/>
<wire x1="-127" y1="144.78" x2="-149.86" y2="144.78" width="0.1524" layer="91"/>
<pinref part="C2" gate="G$1" pin="2"/>
<wire x1="-149.86" y1="144.78" x2="-149.86" y2="149.86" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="C2" gate="G$1" pin="1"/>
<wire x1="-149.86" y1="157.48" x2="-167.64" y2="157.48" width="0.1524" layer="91"/>
<pinref part="FM" gate="A" pin="1"/>
<wire x1="-167.64" y1="157.48" x2="-167.64" y2="160.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="C5" gate="G$1" pin="1"/>
<pinref part="Y1" gate="G$1" pin="1"/>
<wire x1="-99.06" y1="187.96" x2="-109.22" y2="187.96" width="0.1524" layer="91"/>
<junction x="-109.22" y="187.96"/>
<wire x1="-116.84" y1="187.96" x2="-109.22" y2="187.96" width="0.1524" layer="91"/>
<pinref part="R3" gate="G$1" pin="2"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="U1" gate="A" pin="!RST"/>
<wire x1="-60.96" y1="139.7" x2="-22.86" y2="139.7" width="0.1524" layer="91"/>
<wire x1="-22.86" y1="139.7" x2="-22.86" y2="170.18" width="0.1524" layer="91"/>
<pinref part="ESP32" gate="G$1" pin="25"/>
<wire x1="-22.86" y1="170.18" x2="-15.24" y2="170.18" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
<note version="8.4" severity="warning">
Since Version 8.4, EAGLE supports properties for SPICE simulation. 
Probes in schematics and SPICE mapping objects found in parts and library devices
will not be understood with this version. Update EAGLE to the latest version
for full support of SPICE simulation. 
</note>
</compatibility>
</eagle>

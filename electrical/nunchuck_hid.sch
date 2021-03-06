EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 5D5CE799
P 5950 4100
F 0 "A1" H 5950 3011 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 5950 2920 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 6100 3150 50  0001 L CNN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 5950 3100 50  0001 C CNN
	1    5950 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5D5D84E5
P 3300 4250
F 0 "#PWR0101" H 3300 4000 50  0001 C CNN
F 1 "GND" H 3305 4077 50  0000 C CNN
F 2 "" H 3300 4250 50  0001 C CNN
F 3 "" H 3300 4250 50  0001 C CNN
	1    3300 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 4100 3300 4250
$Comp
L Connector:USB_B_Mini J1
U 1 1 5D5D1431
P 3300 3700
F 0 "J1" H 3357 4167 50  0000 C CNN
F 1 "USB_B_Mini" H 3357 4076 50  0000 C CNN
F 2 "" H 3450 3650 50  0001 C CNN
F 3 "~" H 3450 3650 50  0001 C CNN
	1    3300 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0102
U 1 1 5D5DCA1E
P 3750 3500
F 0 "#PWR0102" H 3750 3350 50  0001 C CNN
F 1 "+5V" H 3765 3673 50  0000 C CNN
F 2 "" H 3750 3500 50  0001 C CNN
F 3 "" H 3750 3500 50  0001 C CNN
	1    3750 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 3500 3750 3500
$Comp
L Device:R_US R1
U 1 1 5D5E16EB
P 4000 3700
F 0 "R1" V 3795 3700 50  0000 C CNN
F 1 "68R" V 3886 3700 50  0000 C CNN
F 2 "" V 4040 3690 50  0001 C CNN
F 3 "~" H 4000 3700 50  0001 C CNN
	1    4000 3700
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R2
U 1 1 5D5E1EF6
P 4300 3800
F 0 "R2" V 4095 3800 50  0000 C CNN
F 1 "68R" V 4186 3800 50  0000 C CNN
F 2 "" V 4340 3790 50  0001 C CNN
F 3 "~" H 4300 3800 50  0001 C CNN
	1    4300 3800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R3
U 1 1 5D5E22BB
P 4600 3950
F 0 "R3" H 4532 3904 50  0000 R CNN
F 1 "1K5" H 4532 3995 50  0000 R CNN
F 2 "" V 4640 3940 50  0001 C CNN
F 3 "~" H 4600 3950 50  0001 C CNN
	1    4600 3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	3600 3700 3850 3700
Wire Wire Line
	3600 3800 4150 3800
$Comp
L power:+5V #PWR0103
U 1 1 5D5F2E4C
P 4600 4250
F 0 "#PWR0103" H 4600 4100 50  0001 C CNN
F 1 "+5V" H 4615 4423 50  0000 C CNN
F 2 "" H 4600 4250 50  0001 C CNN
F 3 "" H 4600 4250 50  0001 C CNN
	1    4600 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	4600 4100 4600 4250
Text Label 5300 4000 2    50   ~ 0
SCL0
Text Label 5300 4100 2    50   ~ 0
SDA0
Wire Wire Line
	5300 4000 5450 4000
Wire Wire Line
	5300 4100 5450 4100
Wire Wire Line
	5300 4200 5450 4200
Wire Wire Line
	5300 4300 5450 4300
Wire Wire Line
	8100 3850 8250 3850
Wire Wire Line
	8100 4700 8250 4700
Wire Wire Line
	7050 4550 7200 4550
Wire Wire Line
	7050 3700 7200 3700
Text Label 8250 4700 0    50   ~ 0
SDA1
Text Label 8250 3850 0    50   ~ 0
SDA0
Text Label 7050 4550 2    50   ~ 0
SCL1
Text Label 7050 3700 2    50   ~ 0
SCL0
Text Label 5300 4300 2    50   ~ 0
SDA1
Text Label 5300 4200 2    50   ~ 0
SCL1
$Comp
L nintendo:wii_nunchuck U2
U 1 1 5D5D0A33
P 7650 4650
F 0 "U2" H 7100 5000 50  0000 C CNN
F 1 "right_chuck" H 7300 4900 50  0000 C CNN
F 2 "" H 7400 5100 50  0001 C CNN
F 3 "" H 7400 5100 50  0001 C CNN
	1    7650 4650
	1    0    0    -1  
$EndComp
$Comp
L nintendo:wii_nunchuck U1
U 1 1 5D5CFE02
P 7650 3800
F 0 "U1" H 7100 4150 50  0000 C CNN
F 1 "left_chuck" H 7300 4050 50  0000 C CNN
F 2 "" H 7400 4250 50  0001 C CNN
F 3 "" H 7400 4250 50  0001 C CNN
	1    7650 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 3700 5450 3700
Wire Wire Line
	4450 3800 4600 3800
Wire Wire Line
	4600 3800 5450 3800
Connection ~ 4600 3800
Wire Wire Line
	5950 5100 6050 5100
$Comp
L power:GND #PWR0104
U 1 1 5D606353
P 6400 5100
F 0 "#PWR0104" H 6400 4850 50  0001 C CNN
F 1 "GND" H 6405 4927 50  0000 C CNN
F 2 "" H 6400 5100 50  0001 C CNN
F 3 "" H 6400 5100 50  0001 C CNN
	1    6400 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 5100 6400 5100
Connection ~ 6050 5100
$Comp
L power:+5V #PWR0105
U 1 1 5D607222
P 6150 2950
F 0 "#PWR0105" H 6150 2800 50  0001 C CNN
F 1 "+5V" H 6165 3123 50  0000 C CNN
F 2 "" H 6150 2950 50  0001 C CNN
F 3 "" H 6150 2950 50  0001 C CNN
	1    6150 2950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0106
U 1 1 5D6075B1
P 5950 2950
F 0 "#PWR0106" H 5950 2800 50  0001 C CNN
F 1 "+3V3" H 5965 3123 50  0000 C CNN
F 2 "" H 5950 2950 50  0001 C CNN
F 3 "" H 5950 2950 50  0001 C CNN
	1    5950 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 2950 6150 3100
Wire Wire Line
	5950 2950 6050 2950
Wire Wire Line
	6050 2950 6050 3100
$Comp
L power:+3V3 #PWR0107
U 1 1 5D608DCF
P 7050 3850
F 0 "#PWR0107" H 7050 3700 50  0001 C CNN
F 1 "+3V3" V 7065 3978 50  0000 L CNN
F 2 "" H 7050 3850 50  0001 C CNN
F 3 "" H 7050 3850 50  0001 C CNN
	1    7050 3850
	0    -1   -1   0   
$EndComp
$Comp
L power:+3V3 #PWR0108
U 1 1 5D609F07
P 7050 4700
F 0 "#PWR0108" H 7050 4550 50  0001 C CNN
F 1 "+3V3" V 7065 4828 50  0000 L CNN
F 2 "" H 7050 4700 50  0001 C CNN
F 3 "" H 7050 4700 50  0001 C CNN
	1    7050 4700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 5D60A3CE
P 8250 3700
F 0 "#PWR0109" H 8250 3450 50  0001 C CNN
F 1 "GND" V 8255 3572 50  0000 R CNN
F 2 "" H 8250 3700 50  0001 C CNN
F 3 "" H 8250 3700 50  0001 C CNN
	1    8250 3700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5D60A9D1
P 8250 4550
F 0 "#PWR0110" H 8250 4300 50  0001 C CNN
F 1 "GND" V 8255 4422 50  0000 R CNN
F 2 "" H 8250 4550 50  0001 C CNN
F 3 "" H 8250 4550 50  0001 C CNN
	1    8250 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7050 3850 7200 3850
Wire Wire Line
	7050 4700 7200 4700
$Comp
L Device:D_Zener D1
U 1 1 5D5DD93A
P 3850 4100
F 0 "D1" V 3804 4179 50  0000 L CNN
F 1 "3V6" V 3895 4179 50  0000 L CNN
F 2 "" H 3850 4100 50  0001 C CNN
F 3 "~" H 3850 4100 50  0001 C CNN
	1    3850 4100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 5D5E2E9A
P 3850 4400
F 0 "#PWR0111" H 3850 4150 50  0001 C CNN
F 1 "GND" H 3855 4227 50  0000 C CNN
F 2 "" H 3850 4400 50  0001 C CNN
F 3 "" H 3850 4400 50  0001 C CNN
	1    3850 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 4400 3850 4250
Connection ~ 3850 4250
Wire Wire Line
	3850 4250 4150 4250
$Comp
L Device:D_Zener D2
U 1 1 5D5E0D08
P 4150 4100
F 0 "D2" V 4104 4179 50  0000 L CNN
F 1 "3V6" V 4195 4179 50  0000 L CNN
F 2 "" H 4150 4100 50  0001 C CNN
F 3 "~" H 4150 4100 50  0001 C CNN
	1    4150 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	3850 3950 3850 3700
Connection ~ 3850 3700
Wire Wire Line
	4150 3950 4150 3800
Connection ~ 4150 3800
Wire Wire Line
	8100 3700 8250 3700
Wire Wire Line
	8100 4550 8250 4550
$EndSCHEMATC

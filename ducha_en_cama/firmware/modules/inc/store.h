/*
 * Store.h
 *
 *  Created on: 03/06/2024
 *      Author: Prototipado
 */
#ifndef STORE_H
#define STORE_H

/*==================[inclusions]=============================================*/
#include <string.h>
#include <stdio.h>
#include "conditions.h"
#include "ds3231.h"
#include "definitions.h"

/*==================[macros]=================================================*/
#define DIR_0 0x0000
#define DIR_1 0x0001
#define DIR_2 0x0002
#define DIR_3 0x0003
#define DIR_4 0x0004
#define DIR_5 0x0005
#define DIR_6 0x0006
#define DIR_7 0x0007
#define DIR_8 0x0008
#define DIR_9 0x0009
#define DIR_10 0x000A
#define DIR_11 0x000B
#define DIR_12 0x000C
#define DIR_13 0x000D
#define DIR_14 0x000E
#define DIR_15 0x000F
#define DIR_16 0x0010
#define DIR_17 0x0011
#define DIR_18 0x0012
#define DIR_19 0x0013
#define DIR_20 0x0014
#define DIR_21 0x0015
#define DIR_22 0x0016
#define DIR_23 0x0017
#define DIR_24 0x0018
#define DIR_25 0x0019
#define DIR_26 0x001A
#define DIR_27 0x001B
#define DIR_28 0x001C
#define DIR_29 0x001D
#define DIR_30 0x001E
#define DIR_31 0x001F
#define DIR_32 0x0020
#define DIR_33 0x0021
#define DIR_34 0x0022
#define DIR_35 0x0023
#define DIR_36 0x0024
#define DIR_37 0x0025
#define DIR_38 0x0026
#define DIR_39 0x0027
#define DIR_40 0x0028
#define DIR_41 0x0029
#define DIR_42 0x002A
#define DIR_43 0x002B
#define DIR_44 0x002C
#define DIR_45 0x002D
#define DIR_46 0x002E
#define DIR_47 0x002F
#define DIR_48 0x0030
#define DIR_49 0x0031
#define DIR_50 0x0032
#define DIR_51 0x0033
#define DIR_52 0x0034
#define DIR_53 0x0035
#define DIR_54 0x0036
#define DIR_55 0x0037
#define DIR_56 0x0038
#define DIR_57 0x0039
#define DIR_58 0x003A
#define DIR_59 0x003B
#define DIR_60 0x003C
#define DIR_61 0x003D
#define DIR_62 0x003E
#define DIR_63 0x003F
#define DIR_64 0x0040
#define DIR_65 0x0041
#define DIR_66 0x0042
#define DIR_67 0x0043
#define DIR_68 0x0044
#define DIR_69 0x0045
#define DIR_70 0x0046
#define DIR_71 0x0047
#define DIR_72 0x0048
#define DIR_73 0x0049
#define DIR_74 0x004A
#define DIR_75 0x004B
#define DIR_76 0x004C
#define DIR_77 0x004D
#define DIR_78 0x004E
#define DIR_79 0x004F
#define DIR_80 0x0050
#define DIR_81 0x0051
#define DIR_82 0x0052
#define DIR_83 0x0053
#define DIR_84 0x0054
#define DIR_85 0x0055
#define DIR_86 0x0056
#define DIR_87 0x0057
#define DIR_88 0x0058
#define DIR_89 0x0059
#define DIR_90 0x005A
#define DIR_91 0x005B
#define DIR_92 0x005C
#define DIR_93 0x005D
#define DIR_94 0x005E
#define DIR_95 0x005F
#define DIR_96 0x0060
#define DIR_97 0x0061
#define DIR_98 0x0062
#define DIR_99 0x0063
#define DIR_100 0x0064
/*==================[typedef]================================================*/

/*==================[internal data declaration]==============================*/
#define OFFSET 255
#define MAX_ADRESS 4095
/*==================[internal functions declaration]=========================*/
void Read_Data(void);
void Write_Data(void);
/*==================[external functions declaration]=========================*/


#endif #ifndef STORE_H
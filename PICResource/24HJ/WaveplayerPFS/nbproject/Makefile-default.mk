#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1387409687/uint16toa.o ${OBJECTDIR}/_ext/1387409687/uint8toa.o ${OBJECTDIR}/_ext/1539374888/adc.o ${OBJECTDIR}/_ext/1539409824/dmadef.o ${OBJECTDIR}/_ext/1102637588/dmaSPI.o ${OBJECTDIR}/_ext/178018670/dmaUart.o ${OBJECTDIR}/_ext/481915157/hardUart.o ${OBJECTDIR}/_ext/1539392564/spi.o ${OBJECTDIR}/_ext/1237177208/pff.o ${OBJECTDIR}/_ext/1777254300/sd.o ${OBJECTDIR}/_ext/1777254300/picsd.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1532717492/ringbuffer.o ${OBJECTDIR}/_ext/43898991/delay.o ${OBJECTDIR}/_ext/2105551405/linkedlist.o ${OBJECTDIR}/_ext/1218362732/stack.o ${OBJECTDIR}/_ext/1472/hardwareSpecific.o ${OBJECTDIR}/_ext/1045608385/waveplayer.o ${OBJECTDIR}/_ext/1045608385/picdriverwav.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1387409687/uint16toa.o.d ${OBJECTDIR}/_ext/1387409687/uint8toa.o.d ${OBJECTDIR}/_ext/1539374888/adc.o.d ${OBJECTDIR}/_ext/1539409824/dmadef.o.d ${OBJECTDIR}/_ext/1102637588/dmaSPI.o.d ${OBJECTDIR}/_ext/178018670/dmaUart.o.d ${OBJECTDIR}/_ext/481915157/hardUart.o.d ${OBJECTDIR}/_ext/1539392564/spi.o.d ${OBJECTDIR}/_ext/1237177208/pff.o.d ${OBJECTDIR}/_ext/1777254300/sd.o.d ${OBJECTDIR}/_ext/1777254300/picsd.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1532717492/ringbuffer.o.d ${OBJECTDIR}/_ext/43898991/delay.o.d ${OBJECTDIR}/_ext/2105551405/linkedlist.o.d ${OBJECTDIR}/_ext/1218362732/stack.o.d ${OBJECTDIR}/_ext/1472/hardwareSpecific.o.d ${OBJECTDIR}/_ext/1045608385/waveplayer.o.d ${OBJECTDIR}/_ext/1045608385/picdriverwav.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1387409687/uint16toa.o ${OBJECTDIR}/_ext/1387409687/uint8toa.o ${OBJECTDIR}/_ext/1539374888/adc.o ${OBJECTDIR}/_ext/1539409824/dmadef.o ${OBJECTDIR}/_ext/1102637588/dmaSPI.o ${OBJECTDIR}/_ext/178018670/dmaUart.o ${OBJECTDIR}/_ext/481915157/hardUart.o ${OBJECTDIR}/_ext/1539392564/spi.o ${OBJECTDIR}/_ext/1237177208/pff.o ${OBJECTDIR}/_ext/1777254300/sd.o ${OBJECTDIR}/_ext/1777254300/picsd.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1532717492/ringbuffer.o ${OBJECTDIR}/_ext/43898991/delay.o ${OBJECTDIR}/_ext/2105551405/linkedlist.o ${OBJECTDIR}/_ext/1218362732/stack.o ${OBJECTDIR}/_ext/1472/hardwareSpecific.o ${OBJECTDIR}/_ext/1045608385/waveplayer.o ${OBJECTDIR}/_ext/1045608385/picdriverwav.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

# The following macros may be used in the pre and post step lines
Device=PIC24HJ64GP202
ProjectDir="M:\Robotics\Source\PICResource\24HJ\WaveplayerPFS"
ConfName=default
ImagePath="dist\default\${IMAGE_TYPE}\WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="dist\default\${IMAGE_TYPE}"
ImageName="WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"

.build-conf:  .pre ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: []"
	@
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=24HJ64GP202
MP_LINKER_FILE_OPTION=,-Tp24HJ64GP202.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1387409687/uint16toa.o: ../../../FuncLib/Other/mmculib/uint16toa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1387409687 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint16toa.o.d 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint16toa.o.ok ${OBJECTDIR}/_ext/1387409687/uint16toa.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1387409687/uint16toa.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1387409687/uint16toa.o.d" -o ${OBJECTDIR}/_ext/1387409687/uint16toa.o ../../../FuncLib/Other/mmculib/uint16toa.c  
	
${OBJECTDIR}/_ext/1387409687/uint8toa.o: ../../../FuncLib/Other/mmculib/uint8toa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1387409687 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint8toa.o.d 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint8toa.o.ok ${OBJECTDIR}/_ext/1387409687/uint8toa.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1387409687/uint8toa.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1387409687/uint8toa.o.d" -o ${OBJECTDIR}/_ext/1387409687/uint8toa.o ../../../FuncLib/Other/mmculib/uint8toa.c  
	
${OBJECTDIR}/_ext/1539374888/adc.o: ../../Peripherals/ADC/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1539374888 
	@${RM} ${OBJECTDIR}/_ext/1539374888/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1539374888/adc.o.ok ${OBJECTDIR}/_ext/1539374888/adc.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1539374888/adc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1539374888/adc.o.d" -o ${OBJECTDIR}/_ext/1539374888/adc.o ../../Peripherals/ADC/adc.c  
	
${OBJECTDIR}/_ext/1539409824/dmadef.o: ../../Peripherals/dma/dmadef.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1539409824 
	@${RM} ${OBJECTDIR}/_ext/1539409824/dmadef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1539409824/dmadef.o.ok ${OBJECTDIR}/_ext/1539409824/dmadef.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1539409824/dmadef.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1539409824/dmadef.o.d" -o ${OBJECTDIR}/_ext/1539409824/dmadef.o ../../Peripherals/dma/dmadef.c  
	
${OBJECTDIR}/_ext/1102637588/dmaSPI.o: ../../Peripherals/dmaSPI/dmaSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1102637588 
	@${RM} ${OBJECTDIR}/_ext/1102637588/dmaSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1102637588/dmaSPI.o.ok ${OBJECTDIR}/_ext/1102637588/dmaSPI.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1102637588/dmaSPI.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1102637588/dmaSPI.o.d" -o ${OBJECTDIR}/_ext/1102637588/dmaSPI.o ../../Peripherals/dmaSPI/dmaSPI.c  
	
${OBJECTDIR}/_ext/178018670/dmaUart.o: ../../Peripherals/dmaUART/dmaUart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/178018670 
	@${RM} ${OBJECTDIR}/_ext/178018670/dmaUart.o.d 
	@${RM} ${OBJECTDIR}/_ext/178018670/dmaUart.o.ok ${OBJECTDIR}/_ext/178018670/dmaUart.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/178018670/dmaUart.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/178018670/dmaUart.o.d" -o ${OBJECTDIR}/_ext/178018670/dmaUart.o ../../Peripherals/dmaUART/dmaUart.c  
	
${OBJECTDIR}/_ext/481915157/hardUart.o: ../../Peripherals/hardUART24H/hardUart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/481915157 
	@${RM} ${OBJECTDIR}/_ext/481915157/hardUart.o.d 
	@${RM} ${OBJECTDIR}/_ext/481915157/hardUart.o.ok ${OBJECTDIR}/_ext/481915157/hardUart.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/481915157/hardUart.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/481915157/hardUart.o.d" -o ${OBJECTDIR}/_ext/481915157/hardUart.o ../../Peripherals/hardUART24H/hardUart.c  
	
${OBJECTDIR}/_ext/1539392564/spi.o: ../../Peripherals/SPI/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1539392564 
	@${RM} ${OBJECTDIR}/_ext/1539392564/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1539392564/spi.o.ok ${OBJECTDIR}/_ext/1539392564/spi.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1539392564/spi.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1539392564/spi.o.d" -o ${OBJECTDIR}/_ext/1539392564/spi.o ../../Peripherals/SPI/spi.c  
	
${OBJECTDIR}/_ext/1237177208/pff.o: ../../../FuncLib/Applications/PetitFS/pff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1237177208 
	@${RM} ${OBJECTDIR}/_ext/1237177208/pff.o.d 
	@${RM} ${OBJECTDIR}/_ext/1237177208/pff.o.ok ${OBJECTDIR}/_ext/1237177208/pff.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1237177208/pff.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1237177208/pff.o.d" -o ${OBJECTDIR}/_ext/1237177208/pff.o ../../../FuncLib/Applications/PetitFS/pff.c  
	
${OBJECTDIR}/_ext/1777254300/sd.o: ../../../FuncLib/External/SD_MMC/sd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1777254300 
	@${RM} ${OBJECTDIR}/_ext/1777254300/sd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1777254300/sd.o.ok ${OBJECTDIR}/_ext/1777254300/sd.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1777254300/sd.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1777254300/sd.o.d" -o ${OBJECTDIR}/_ext/1777254300/sd.o ../../../FuncLib/External/SD_MMC/sd.c  
	
${OBJECTDIR}/_ext/1777254300/picsd.o: ../../../FuncLib/External/SD_MMC/picsd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1777254300 
	@${RM} ${OBJECTDIR}/_ext/1777254300/picsd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1777254300/picsd.o.ok ${OBJECTDIR}/_ext/1777254300/picsd.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1777254300/picsd.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1777254300/picsd.o.d" -o ${OBJECTDIR}/_ext/1777254300/picsd.o ../../../FuncLib/External/SD_MMC/picsd.c  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.ok ${OBJECTDIR}/_ext/1472/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  
	
${OBJECTDIR}/_ext/1532717492/ringbuffer.o: ../../../FuncLib/Other/RingBuffer/ringbuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1532717492 
	@${RM} ${OBJECTDIR}/_ext/1532717492/ringbuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1532717492/ringbuffer.o.ok ${OBJECTDIR}/_ext/1532717492/ringbuffer.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1532717492/ringbuffer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1532717492/ringbuffer.o.d" -o ${OBJECTDIR}/_ext/1532717492/ringbuffer.o ../../../FuncLib/Other/RingBuffer/ringbuffer.c  
	
${OBJECTDIR}/_ext/43898991/delay.o: ../../delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/43898991 
	@${RM} ${OBJECTDIR}/_ext/43898991/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/delay.o.ok ${OBJECTDIR}/_ext/43898991/delay.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/delay.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/43898991/delay.o.d" -o ${OBJECTDIR}/_ext/43898991/delay.o ../../delay.c  
	
${OBJECTDIR}/_ext/2105551405/linkedlist.o: ../../../FuncLib/Other/linkedlist/linkedlist.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2105551405 
	@${RM} ${OBJECTDIR}/_ext/2105551405/linkedlist.o.d 
	@${RM} ${OBJECTDIR}/_ext/2105551405/linkedlist.o.ok ${OBJECTDIR}/_ext/2105551405/linkedlist.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2105551405/linkedlist.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/2105551405/linkedlist.o.d" -o ${OBJECTDIR}/_ext/2105551405/linkedlist.o ../../../FuncLib/Other/linkedlist/linkedlist.c  
	
${OBJECTDIR}/_ext/1218362732/stack.o: ../../../FuncLib/Other/stack/stack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1218362732 
	@${RM} ${OBJECTDIR}/_ext/1218362732/stack.o.d 
	@${RM} ${OBJECTDIR}/_ext/1218362732/stack.o.ok ${OBJECTDIR}/_ext/1218362732/stack.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1218362732/stack.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1218362732/stack.o.d" -o ${OBJECTDIR}/_ext/1218362732/stack.o ../../../FuncLib/Other/stack/stack.c  
	
${OBJECTDIR}/_ext/1472/hardwareSpecific.o: ../hardwareSpecific.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/hardwareSpecific.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hardwareSpecific.o.ok ${OBJECTDIR}/_ext/1472/hardwareSpecific.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hardwareSpecific.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1472/hardwareSpecific.o.d" -o ${OBJECTDIR}/_ext/1472/hardwareSpecific.o ../hardwareSpecific.c  
	
${OBJECTDIR}/_ext/1045608385/waveplayer.o: ../../../FuncLib/External/WavePlayer/waveplayer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1045608385 
	@${RM} ${OBJECTDIR}/_ext/1045608385/waveplayer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1045608385/waveplayer.o.ok ${OBJECTDIR}/_ext/1045608385/waveplayer.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1045608385/waveplayer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1045608385/waveplayer.o.d" -o ${OBJECTDIR}/_ext/1045608385/waveplayer.o ../../../FuncLib/External/WavePlayer/waveplayer.c  
	
${OBJECTDIR}/_ext/1045608385/picdriverwav.o: ../../../FuncLib/External/WavePlayer/picdriverwav.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1045608385 
	@${RM} ${OBJECTDIR}/_ext/1045608385/picdriverwav.o.d 
	@${RM} ${OBJECTDIR}/_ext/1045608385/picdriverwav.o.ok ${OBJECTDIR}/_ext/1045608385/picdriverwav.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1045608385/picdriverwav.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1045608385/picdriverwav.o.d" -o ${OBJECTDIR}/_ext/1045608385/picdriverwav.o ../../../FuncLib/External/WavePlayer/picdriverwav.c  
	
else
${OBJECTDIR}/_ext/1387409687/uint16toa.o: ../../../FuncLib/Other/mmculib/uint16toa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1387409687 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint16toa.o.d 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint16toa.o.ok ${OBJECTDIR}/_ext/1387409687/uint16toa.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1387409687/uint16toa.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1387409687/uint16toa.o.d" -o ${OBJECTDIR}/_ext/1387409687/uint16toa.o ../../../FuncLib/Other/mmculib/uint16toa.c  
	
${OBJECTDIR}/_ext/1387409687/uint8toa.o: ../../../FuncLib/Other/mmculib/uint8toa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1387409687 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint8toa.o.d 
	@${RM} ${OBJECTDIR}/_ext/1387409687/uint8toa.o.ok ${OBJECTDIR}/_ext/1387409687/uint8toa.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1387409687/uint8toa.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1387409687/uint8toa.o.d" -o ${OBJECTDIR}/_ext/1387409687/uint8toa.o ../../../FuncLib/Other/mmculib/uint8toa.c  
	
${OBJECTDIR}/_ext/1539374888/adc.o: ../../Peripherals/ADC/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1539374888 
	@${RM} ${OBJECTDIR}/_ext/1539374888/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1539374888/adc.o.ok ${OBJECTDIR}/_ext/1539374888/adc.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1539374888/adc.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1539374888/adc.o.d" -o ${OBJECTDIR}/_ext/1539374888/adc.o ../../Peripherals/ADC/adc.c  
	
${OBJECTDIR}/_ext/1539409824/dmadef.o: ../../Peripherals/dma/dmadef.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1539409824 
	@${RM} ${OBJECTDIR}/_ext/1539409824/dmadef.o.d 
	@${RM} ${OBJECTDIR}/_ext/1539409824/dmadef.o.ok ${OBJECTDIR}/_ext/1539409824/dmadef.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1539409824/dmadef.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1539409824/dmadef.o.d" -o ${OBJECTDIR}/_ext/1539409824/dmadef.o ../../Peripherals/dma/dmadef.c  
	
${OBJECTDIR}/_ext/1102637588/dmaSPI.o: ../../Peripherals/dmaSPI/dmaSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1102637588 
	@${RM} ${OBJECTDIR}/_ext/1102637588/dmaSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1102637588/dmaSPI.o.ok ${OBJECTDIR}/_ext/1102637588/dmaSPI.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1102637588/dmaSPI.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1102637588/dmaSPI.o.d" -o ${OBJECTDIR}/_ext/1102637588/dmaSPI.o ../../Peripherals/dmaSPI/dmaSPI.c  
	
${OBJECTDIR}/_ext/178018670/dmaUart.o: ../../Peripherals/dmaUART/dmaUart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/178018670 
	@${RM} ${OBJECTDIR}/_ext/178018670/dmaUart.o.d 
	@${RM} ${OBJECTDIR}/_ext/178018670/dmaUart.o.ok ${OBJECTDIR}/_ext/178018670/dmaUart.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/178018670/dmaUart.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/178018670/dmaUart.o.d" -o ${OBJECTDIR}/_ext/178018670/dmaUart.o ../../Peripherals/dmaUART/dmaUart.c  
	
${OBJECTDIR}/_ext/481915157/hardUart.o: ../../Peripherals/hardUART24H/hardUart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/481915157 
	@${RM} ${OBJECTDIR}/_ext/481915157/hardUart.o.d 
	@${RM} ${OBJECTDIR}/_ext/481915157/hardUart.o.ok ${OBJECTDIR}/_ext/481915157/hardUart.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/481915157/hardUart.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/481915157/hardUart.o.d" -o ${OBJECTDIR}/_ext/481915157/hardUart.o ../../Peripherals/hardUART24H/hardUart.c  
	
${OBJECTDIR}/_ext/1539392564/spi.o: ../../Peripherals/SPI/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1539392564 
	@${RM} ${OBJECTDIR}/_ext/1539392564/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1539392564/spi.o.ok ${OBJECTDIR}/_ext/1539392564/spi.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1539392564/spi.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1539392564/spi.o.d" -o ${OBJECTDIR}/_ext/1539392564/spi.o ../../Peripherals/SPI/spi.c  
	
${OBJECTDIR}/_ext/1237177208/pff.o: ../../../FuncLib/Applications/PetitFS/pff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1237177208 
	@${RM} ${OBJECTDIR}/_ext/1237177208/pff.o.d 
	@${RM} ${OBJECTDIR}/_ext/1237177208/pff.o.ok ${OBJECTDIR}/_ext/1237177208/pff.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1237177208/pff.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1237177208/pff.o.d" -o ${OBJECTDIR}/_ext/1237177208/pff.o ../../../FuncLib/Applications/PetitFS/pff.c  
	
${OBJECTDIR}/_ext/1777254300/sd.o: ../../../FuncLib/External/SD_MMC/sd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1777254300 
	@${RM} ${OBJECTDIR}/_ext/1777254300/sd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1777254300/sd.o.ok ${OBJECTDIR}/_ext/1777254300/sd.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1777254300/sd.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1777254300/sd.o.d" -o ${OBJECTDIR}/_ext/1777254300/sd.o ../../../FuncLib/External/SD_MMC/sd.c  
	
${OBJECTDIR}/_ext/1777254300/picsd.o: ../../../FuncLib/External/SD_MMC/picsd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1777254300 
	@${RM} ${OBJECTDIR}/_ext/1777254300/picsd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1777254300/picsd.o.ok ${OBJECTDIR}/_ext/1777254300/picsd.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1777254300/picsd.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1777254300/picsd.o.d" -o ${OBJECTDIR}/_ext/1777254300/picsd.o ../../../FuncLib/External/SD_MMC/picsd.c  
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.ok ${OBJECTDIR}/_ext/1472/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  
	
${OBJECTDIR}/_ext/1532717492/ringbuffer.o: ../../../FuncLib/Other/RingBuffer/ringbuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1532717492 
	@${RM} ${OBJECTDIR}/_ext/1532717492/ringbuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1532717492/ringbuffer.o.ok ${OBJECTDIR}/_ext/1532717492/ringbuffer.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1532717492/ringbuffer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1532717492/ringbuffer.o.d" -o ${OBJECTDIR}/_ext/1532717492/ringbuffer.o ../../../FuncLib/Other/RingBuffer/ringbuffer.c  
	
${OBJECTDIR}/_ext/43898991/delay.o: ../../delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/43898991 
	@${RM} ${OBJECTDIR}/_ext/43898991/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/delay.o.ok ${OBJECTDIR}/_ext/43898991/delay.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/delay.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/43898991/delay.o.d" -o ${OBJECTDIR}/_ext/43898991/delay.o ../../delay.c  
	
${OBJECTDIR}/_ext/2105551405/linkedlist.o: ../../../FuncLib/Other/linkedlist/linkedlist.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2105551405 
	@${RM} ${OBJECTDIR}/_ext/2105551405/linkedlist.o.d 
	@${RM} ${OBJECTDIR}/_ext/2105551405/linkedlist.o.ok ${OBJECTDIR}/_ext/2105551405/linkedlist.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2105551405/linkedlist.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/2105551405/linkedlist.o.d" -o ${OBJECTDIR}/_ext/2105551405/linkedlist.o ../../../FuncLib/Other/linkedlist/linkedlist.c  
	
${OBJECTDIR}/_ext/1218362732/stack.o: ../../../FuncLib/Other/stack/stack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1218362732 
	@${RM} ${OBJECTDIR}/_ext/1218362732/stack.o.d 
	@${RM} ${OBJECTDIR}/_ext/1218362732/stack.o.ok ${OBJECTDIR}/_ext/1218362732/stack.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1218362732/stack.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1218362732/stack.o.d" -o ${OBJECTDIR}/_ext/1218362732/stack.o ../../../FuncLib/Other/stack/stack.c  
	
${OBJECTDIR}/_ext/1472/hardwareSpecific.o: ../hardwareSpecific.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/hardwareSpecific.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hardwareSpecific.o.ok ${OBJECTDIR}/_ext/1472/hardwareSpecific.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hardwareSpecific.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1472/hardwareSpecific.o.d" -o ${OBJECTDIR}/_ext/1472/hardwareSpecific.o ../hardwareSpecific.c  
	
${OBJECTDIR}/_ext/1045608385/waveplayer.o: ../../../FuncLib/External/WavePlayer/waveplayer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1045608385 
	@${RM} ${OBJECTDIR}/_ext/1045608385/waveplayer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1045608385/waveplayer.o.ok ${OBJECTDIR}/_ext/1045608385/waveplayer.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1045608385/waveplayer.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1045608385/waveplayer.o.d" -o ${OBJECTDIR}/_ext/1045608385/waveplayer.o ../../../FuncLib/External/WavePlayer/waveplayer.c  
	
${OBJECTDIR}/_ext/1045608385/picdriverwav.o: ../../../FuncLib/External/WavePlayer/picdriverwav.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1045608385 
	@${RM} ${OBJECTDIR}/_ext/1045608385/picdriverwav.o.d 
	@${RM} ${OBJECTDIR}/_ext/1045608385/picdriverwav.o.ok ${OBJECTDIR}/_ext/1045608385/picdriverwav.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1045608385/picdriverwav.o.d" $(SILENT) -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -D__PIC24H__ -DFcy=40000000 -I"../../../FuncLib/Other" -I"/D:/Microchip/MPLAB C30/include" -I"../../Peripherals" -I"../.." -I"../../Peripherals/hardUART24H" -I"/D:/Microchip/MPLAB C30/support/PIC24H/h" -I"../../../FuncLib/Other/RingBuffer" -I"/D:/Microchip/MPLAB C30/support/peripheral_30F_24H_33F" -I"../../../FuncLib/Other/linkedlist" -I"M:/Robotics/Source/PICResource/24HJ" -I"../../../FuncLib/External" -I"../../../FuncLib/Applications" -Os -fomit-frame-pointer -MMD -MF "${OBJECTDIR}/_ext/1045608385/picdriverwav.o.d" -o ${OBJECTDIR}/_ext/1045608385/picdriverwav.o ../../../FuncLib/External/WavePlayer/picdriverwav.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "D:\Microchip\MPLAB C30\lib\PIC24H\libp24HJ64GP202-coff.a" "D:\Microchip\MPLAB C30\lib\PIC24H\libp24HJ64GP202-elf.a"     -Wl,--defsym=__MPLAB_BUILD=1,--heap=0,-L"..",-Map="${DISTDIR}/First24HJ.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "D:\Microchip\MPLAB C30\lib\PIC24H\libp24HJ64GP202-coff.a" "D:\Microchip\MPLAB C30\lib\PIC24H\libp24HJ64GP202-elf.a"     -Wl,--defsym=__MPLAB_BUILD=1,--heap=0,-L"..",-Map="${DISTDIR}/First24HJ.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/WaveplayerPFS.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif

.pre:
	@echo "--------------------------------------"
	@echo "User defined pre-build step: []"
	@
	@echo "--------------------------------------"

# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif

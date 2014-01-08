#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a pre- and a post- target defined where you can add customization code.
#
# This makefile implements macros and targets common to all configurations.
#
# NOCDDL


# Building and Cleaning subprojects are done by default, but can be controlled with the SUB
# macro. If SUB=no, subprojects will not be built or cleaned. The following macro
# statements set BUILD_SUB-CONF and CLEAN_SUB-CONF to .build-reqprojects-conf
# and .clean-reqprojects-conf unless SUB has the value 'no'
SUB_no=NO
SUBPROJECTS=${SUB_${SUB}}
BUILD_SUBPROJECTS_=.build-subprojects
BUILD_SUBPROJECTS_NO=
BUILD_SUBPROJECTS=${BUILD_SUBPROJECTS_${SUBPROJECTS}}
CLEAN_SUBPROJECTS_=.clean-subprojects
CLEAN_SUBPROJECTS_NO=
CLEAN_SUBPROJECTS=${CLEAN_SUBPROJECTS_${SUBPROJECTS}}


# Project Name
PROJECTNAME=MPLAB.X

# Active Configuration
DEFAULTCONF=LPC_USB_Development_Kit_PIC16F1459
CONF=${DEFAULTCONF}

# All Configurations
ALLCONFS=LPC_USB_Development_Kit_PIC16F1459 LPC_USB_Development_Kit_PIC18F14K50 PIC18F46J50_PIM PIC18F47J53_PIM PIC18F87J50_PIM PIC18F87J94_PIM PIC18F97J94_PIM PICDEM_FSUSB PICDEM_FSUSB_K50 PIC18F_Starter_Kit_1 PIC24F_Starter_Kit_1 PIC24FJ64GB004_PIM PIC24FJ256GB110_PIM PIC24FJ256GB210_PIM PIC24FJ256DA210_PIM PIC24FJ64GB502_Microstick PIC32MX460F512L_PIM PIC32MX795F512L_PIM PIC32_USB_Starter_Board PIC32_USB_Starter_Kit_II 


# build
.build-impl: .build-pre
	${MAKE} -f nbproject/Makefile-${CONF}.mk SUBPROJECTS=${SUBPROJECTS} .build-conf


# clean
.clean-impl: .clean-pre
	${MAKE} -f nbproject/Makefile-${CONF}.mk SUBPROJECTS=${SUBPROJECTS} .clean-conf

# clobber
.clobber-impl: .clobber-pre .depcheck-impl
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=LPC_USB_Development_Kit_PIC16F1459 clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=LPC_USB_Development_Kit_PIC18F14K50 clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F46J50_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F47J53_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F87J50_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F87J94_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F97J94_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PICDEM_FSUSB clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PICDEM_FSUSB_K50 clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F_Starter_Kit_1 clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24F_Starter_Kit_1 clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ64GB004_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ256GB110_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ256GB210_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ256DA210_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ64GB502_Microstick clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32MX460F512L_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32MX795F512L_PIM clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32_USB_Starter_Board clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32_USB_Starter_Kit_II clean



# all
.all-impl: .all-pre .depcheck-impl
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=LPC_USB_Development_Kit_PIC16F1459 build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=LPC_USB_Development_Kit_PIC18F14K50 build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F46J50_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F47J53_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F87J50_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F87J94_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F97J94_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PICDEM_FSUSB build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PICDEM_FSUSB_K50 build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC18F_Starter_Kit_1 build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24F_Starter_Kit_1 build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ64GB004_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ256GB110_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ256GB210_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ256DA210_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC24FJ64GB502_Microstick build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32MX460F512L_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32MX795F512L_PIM build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32_USB_Starter_Board build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=PIC32_USB_Starter_Kit_II build



# dependency checking support
.depcheck-impl:
#	@echo "# This code depends on make tool being used" >.dep.inc
#	@if [ -n "${MAKE_VERSION}" ]; then \
#	    echo "DEPFILES=\$$(wildcard \$$(addsuffix .d, \$${OBJECTFILES}))" >>.dep.inc; \
#	    echo "ifneq (\$${DEPFILES},)" >>.dep.inc; \
#	    echo "include \$${DEPFILES}" >>.dep.inc; \
#	    echo "endif" >>.dep.inc; \
#	else \
#	    echo ".KEEP_STATE:" >>.dep.inc; \
#	    echo ".KEEP_STATE_FILE:.make.state.\$${CONF}" >>.dep.inc; \
#	fi

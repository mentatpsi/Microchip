/*********************************************************************
 *
 *  Google Static Map v2 API client
 *  Module for Microchip TCP/IP Stack
 *   -Downloads compressed map images from the http://maps.google.com site
 *	 -Reference: Google Static Map v2 API 
 *	    http://code.google.com/apis/maps/documentation/staticmaps/index.html
 *
 *********************************************************************
 * FileName:        GoogleStaticMapClient.h
 * Dependencies:    None
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F, PIC32
 * Compiler:        Microchip C32 v1.05 or higher
 *					Microchip C30 v3.12 or higher
 *					Microchip C18 v3.30 or higher
 *					HI-TECH PICC-18 PRO 9.63PL2 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2010 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date    	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder     03/10/2010	Original (started from GenericTCPClient.c)
 ********************************************************************/
#ifndef GOOGLESTATICMAPCLIENT_H
#define GOOGLESTATICMAPCLIENT_H

#include "GoogleMapMenu.h"

// State structure defining a Google Static Maps download request
typedef struct
{
	unsigned char *szCenter;	// Pointer to a statically allocated string indicating the map center position.  Ex: "2355 W. Chandler Blvd., Chandler, AZ" or "33.304524,-111.882024" where 33.304524 and -111.882024 are longitude and latitude coordinates.
	unsigned char vZoom;		// Zoom level from 0 (whole world in a single map) to 21+ (see if someone is bald or not)
	unsigned short vSizeX;		// Horizontal X resolution of the return photo in pixels.  Google supports up to 640 horizontal pixels.
	unsigned short vSizeY;		// Vertical Y resolution of the return photo in pixels.  Google supports up to 640 vertical pixels.
	unsigned char vFormat;		// Image format returned.  Can be either GOOGLE_STATIC_MAP_PNG, GOOGLE_STATIC_MAP_PNG32, GOOGLE_STATIC_MAP_GIF, GOOGLE_STATIC_MAP_JPG, or GOOGLE_STATIC_MAP_JPG_BASELINE.
	unsigned char vMapType;		// Map type requested.  Can be either GOOGLE_STATIC_MAP_ROADMAP, GOOGLE_STATIC_MAP_SATELLITE, GOOGLE_STATIC_MAP_HYBRID, or GOOGLE_STATIC_MAP_TERRAIN.
	unsigned char vMobile;		// Flag inticating if mobile optimizations should be turned on or not.  0 indicates that the imagery should be the same as PC would show, non-zero indicates that mobile optimized fonts should be used to enhance readability on tiny screens.  Note that if you attempt to use mobile and hybrid modes, you may get a server error instead of a returned map.
	unsigned char *szLanguage;	// Optional pointer to a statically allocated string indicating the language to print street names and labels in.  Set this parameter to NULL if you want to use the server default language (normally varies with the location you are viewing).
	unsigned char vSensor;		// Flag indicating if the szCenter parameter was generated from a sensor, such as a GPS device.  This is a required parameter for Google's use.
	enum
	{
		GOOGLE_STATIC_MAP_START = 0,
		GOOGLE_STATIC_MAP_SOCKET_OBTAINED,
		GOOGLE_STATIC_MAP_PROCESS_RESPONSE_HTTP_CODE,
		GOOGLE_STATIC_MAP_PROCESS_RESPONSE_HTTP_HEADERS,
		GOOGLE_STATIC_MAP_PROCESS_RESPONSE_BODY,
		GOOGLE_STATIC_MAP_DISCONNECT,
		GOOGLE_STATIC_MAP_DONE
	} smClient;					// For initial calls, set this parameter to GOOGLE_STATIC_MAP_START.  For subsequent calls, this is an internal parameter that must be preserved unmodified.
	unsigned long dwEDSWritePtr;// Internal parameter.  This parameter does not need initializing.  Pointer to memory were the image will be downloaded to.
	unsigned long dwImageLength;// Internal parameter.  This parameter does not need initializing.  Byte count of the downloaded image.
	unsigned long dwTimer;		// Internal parameter.  This parameter does not need initializing.  Timer for handling timeouts and state changes.
	TCP_SOCKET hSocket;			// Internal parameter.  This parameter does not need initializing.  Handle to TCP socket to use.
	signed char vReturnCode;	// Internal parameter.  This parameter does not need initializing.
} GOOGLE_STATIC_MAP_STATE;


// Google Static Map vFormat parameters
#define GOOGLE_STATIC_MAP_PNG			0x00u	// 8-bit PNG (Portable Network Graphics) lossless format
#define GOOGLE_STATIC_MAP_PNG32			0x01u	// 32-bit PNG (Portable Network Graphics) lossless format
#define GOOGLE_STATIC_MAP_GIF			0x02u	// GIF (Graphics Interchange Format) lossless format
#define GOOGLE_STATIC_MAP_JPG			0x03u	// JPEG (Joint Photographic Experts Group) lossy format
#define GOOGLE_STATIC_MAP_JPG_BASELINE	0x04u	// Non-progressive JPEG (Joint Photographic Experts Group) lossy format

// Google Static Map vMapType parameters
#define GOOGLE_STATIC_MAP_ROADMAP	0x00u	// Roadmap
#define GOOGLE_STATIC_MAP_SATELLITE	0x01u	// Satellite photo
#define GOOGLE_STATIC_MAP_HYBRID	0x02u	// Satellite photo with Roadmap info overlayed ontop
#define GOOGLE_STATIC_MAP_TERRAIN	0x03u	// Terrain

#ifndef __PIC32MX__
#define DOWNLOAD_IMAGE_BUFFER_ADDRESS	(32768ul)
#else
extern unsigned char vDownloadedImage[];
#define DOWNLOAD_IMAGE_BUFFER_ADDRESS  ((DWORD)vDownloadedImage)
#endif 
extern GOOGLE_STATIC_MAP_STATE MapDownloaderState;

// Public function prototypes
void GoogleStaticMapReset(GOOGLE_STATIC_MAP_STATE *state);
signed char GoogleStaticMapClient(GOOGLE_STATIC_MAP_STATE *state);

// Public callback function prototypes
void GoogleStaticMapUserAction(USER_INPUT_REQUEST *UserState);

#endif

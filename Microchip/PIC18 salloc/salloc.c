/*****************************************************************************
 *
 *              Simple SRAM Dynamic Memory Allocation 
 *
 *****************************************************************************
 * FileName:        sralloc.c
 * Dependencies:    
 * Processor:       PIC18F with CAN
 * Compiler:       	C18 02.20.00 or higher
 * Linker:          MPLINK 03.40.00 or higher
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the "Company") is intended and supplied to you, the Company's
 * customer, for use solely and exclusively with products manufactured
 * by the Company. 
 *
 * The software is owned by the Company and/or its supplier, and is 
 * protected under applicable copyright laws. All rights are reserved. 
 * Any use in violation of the foregoing restrictions may subject the 
 * user to criminal sanctions under applicable laws, as well as to 
 * civil liability for the breach of the terms and conditions of this 
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, 
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * This is a simple dynamic memory allocation module. The following are the 
 * supported services:
 *
 * unsigned char * NEAR SRAMalloc(NEAR unsigned char nBytes) 
 * void SRAMfree(unsigned char * NEAR pSRAM)
 * void SRAMInitHeap(void)
 *
 * This version of the dynamic memory allocation limits the segment size
 * to 126 bytes. This is specifically designed such to enable better
 * performance by limiting pointer manipulation.
 *
 *
 * How it works:
 * The model is based on a simple form of a linked list. A block of memory
 * refered to as the dynamic heap is split into segments. Each segment 
 * has a single byte header that references the next segment in the list
 * as well as indicating whether the segment is allocated. Consiquently 
 * the reference implicitly identifies the length of the segment. 
 *
 * This method also enables the possibility of allowing a large number
 * of memory allocations. The maximum is limited by the defined heap size.
 *
 * SRAMalloc() is used to split or merge segments to be allocated.
 * SRAMfree() is used to release segments.
 * 
 * Example:
 *	----------
 *	|  0x7F  |	0x200   Header Seg1
 *	|        |
 *	|        |
 *	|        |
 *	|        | 
 *	|        |
 *	|        |
 *	|  0x89  |  0x27F	Header Seg2 (allocated)
 *	|        |
 *	|        |
 *	|  0x77  |  0x288	Header Seg3
 *	|        |
 *	|        |
 *	|        |
 *	|        |
 *	|        |
 *	|        |
 *	|        |
 *	|  0x00  |	0x2FF 	Tail
 *	----------
 *
 *	
 *	Bit 7	Bit 6 	Bit 5	Bit 4	Bit 3	Bit 2	Bit 1	Bit 0
 *	
 *	Alloc	------------- reference to next Header --------------
 *
 *	
 * Recomendations:
 * Although this model will allow dynamic allocation down to a single byte,
 * doing so sacrifices performance. With more segments within the heap, more
 * time is required to attempt to allocate memory. Plus every segment requires 
 * a header byte; therefore, smaller segments require more memory. There is 
 * also the possibility of fragmentation, which could ultimately doom an 
 * application by reducing the largest allocatable block of memory. Thus the 
 * recomendation is to allocate at least 8 bytes of memory.  
 *
 *	
 *
 * Author               Date        Version 	Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Ross Fosler			05/25/03	v1.03		...	First release
 * 
 *****************************************************************************/

// Summary: A macro used to enable nead-model RAM addressing
// Description: By uncommenting the NEAR_MODEL macro, the user can enable near-model RAM addressing when using dynamic FSFILE object
//              allocation with PIC18
#define NEAR_MODEL

// Summary: A macro used to define the heap size for PIC18
// Description: When using dynamic FSFILE object allocation with PIC18, the MAX_HEAP_SIZE will allow the user to specify the size
//              of the dynamic heap to use
#define	MAX_HEAP_SIZE		0x100



#if defined(NEAR_MODEL)
// Summary: A macro used to specify the near-model action
// Description: Functions can be declared using the NEAR macro.  If the NEAR_MODEL macro is uncommented, the NEAR macro will be ignored.
#define	NEAR	near
#else
#define NEAR
#endif

// Description: A macro used to determine the maximum size of a dynamic memory segment.
#define	_MAX_SEGMENT_SIZE	0x7F
// Description: A macro used to determine the heap initialization size.
#define _MAX_HEAP_SIZE 	MAX_HEAP_SIZE-1



// Summary: The segment header data type
// Description: The SALLOC union allows the PIC18 dynamic memory allocation algorithm to perform bitwise accesses on segment headers.
typedef union _SALLOC
{
	unsigned char byte;
	struct _BITS
	{
		unsigned count:7;
		unsigned alloc:1;	
	}bits;
}SALLOC;





/*********************************************************************
 * Reserve the memory heap
 ********************************************************************/
#pragma 	udata	_SRAM_ALLOC_HEAP
// Summary: The PIC18 dynamic memory heap
// Description: The _uDynamicHeap array is used as a heap for PIC18 dynamic memory allocation.
unsigned char _uDynamicHeap[MAX_HEAP_SIZE];


/*********************************************************************
 * Set the memory type
 ********************************************************************/
#if defined(NEAR_MODEL)
#pragma		udata access	_SRAM_ALLOC
#else
#pragma		udata _SRAM_ALLOC
#endif



/*********************************************************************
 * Private function declarations
 ********************************************************************/
NEAR unsigned char _SRAMmerge(SALLOC * NEAR pSegA);




/*********************************************************************
 * Function:        unsigned char * SRAMalloc(unsigned char length)
 *
 * PreCondition:    A memory block must be allocated in the linker, 
 *					and	the memory headers and tail must already be 
 *					set via the	function SRAMInitHeap().
 *
 * Input:        	unsigned char nBytes - Number of bytes to allocate.	
 *                  
 * Output:         	unsigned char * - A pointer to the requested block
 *					of memory.
 *
 * Side Effects:    
 *
 * Overview:        This functions allocates a chunk of memory from
 *					the heap. The maximum segment size for this 
 *					version is 126 bytes. If the heap does not have
 *					an available segment of sufficient size it will
 *					attempt to create a segment; otherwise a NULL 
 *					pointer is returned. If allocation is succeessful
 *					then a pointer to the requested block is returned.
 *
 * Note:            The calling function must maintain the pointer
 *					to correctly free memory at runtime.
 ********************************************************************/
unsigned char * NEAR SRAMalloc(NEAR unsigned char nBytes)
{
	SALLOC * NEAR pHeap;
	SALLOC * NEAR temp;
	NEAR SALLOC segHeader;
	NEAR unsigned char segLen;
	
	// Do not allow allocation above the max minus one bytes
	if (nBytes > (_MAX_SEGMENT_SIZE - 1)) return (0);
	
	// Init the pointer to the heap
	pHeap = (SALLOC *)_uDynamicHeap;
	
	while (1)
	{
		// Get the header of the segment
		segHeader = *pHeap;
		
		// Extract the segment length from the segment
		segLen = segHeader.bits.count - 1;

		// A null segment indicates the end of the table
		if (segHeader.byte == 0) return (0);
				
		// If this segment is not allocated then attempt to allocate it
		if (!(segHeader.bits.alloc))
		{
			// If the free segment is too small then attempt to merge
			if (nBytes > segLen)
			{
				// If the merge fails them move on to the next segment
				if (!(_SRAMmerge(pHeap))) pHeap += segHeader.bits.count;	
			}
			else
		
			// If the segment length matches the request then allocate the
			// header and return the pointer
			if (nBytes == segLen)
			{
				// Allocate the segment
				(*pHeap).bits.alloc = 1;  
					
				// Return the pointer to the caller
				return ((unsigned char *)(pHeap + 1));
			}
	
			// Else create a new segment
			else 
			{
				// Reset the header to point to a new segment
				(*pHeap).byte = nBytes + 0x81;
	
				// Remember the pointer to the first segment
				temp = pHeap + 1;
	
				// Point to the new segment
				pHeap += (nBytes + 1);
	
				// Insert the header for the new segment
				(*pHeap).byte = segLen - nBytes;  
					
				// Return the pointer to the user
				return ((unsigned char *) temp);
			}			
		}

		// else set the pointer to the next segment header in the heap
		else 
		{
			pHeap += segHeader.bits.count;
		}
	}
}



/*********************************************************************
 * Function:        void SRAMfree(unsigned char * pSRAM)
 *
 * PreCondition:  	The pointer must have been returned from a 
 *					previously allocation via SRAMalloc().  
 *
 * Input:        	unsigned char * pSRAM - pointer to the allocated 	
 *                  
 * Output:         	void
 *
 * Side Effects:    
 *
 * Overview:       	This function de-allocates a previously allocated
 *					segment of memory. 
 *
 * Note:            The pointer must be a valid pointer returned from
 *					SRAMalloc(); otherwise, the segment may not be 
 *					successfully de-allocated, and the heap may be 
 *					corrupted.
 ********************************************************************/
void SRAMfree(unsigned char * NEAR pSRAM)
{	
	// Release the segment
	(*(SALLOC *)(pSRAM - 1)).bits.alloc = 0;
}



/*********************************************************************
 * Function:        void SRAMInitHeap(void)
 *
 * PreCondition:    
 *
 * Input:  			void      	
 *                  
 * Output:         	void
 *
 * Side Effects:    
 *
 * Overview:      	This function initializes the dynamic heap. It 
 *					inserts segment headers to maximize segment space.
 *
 * Note:            This function must be called at least one time. 
 *					And it could be called more times to reset the 
 *					heap.
 ********************************************************************/
void SRAMInitHeap(void)
{
	unsigned char * NEAR pHeap;
	NEAR unsigned int count;
	
	pHeap = _uDynamicHeap;
	count = _MAX_HEAP_SIZE;

	while (1)
	{
		if (count > _MAX_SEGMENT_SIZE)
		{
			*pHeap = _MAX_SEGMENT_SIZE;
			pHeap += _MAX_SEGMENT_SIZE;
			count = count - _MAX_SEGMENT_SIZE;
		}
		else
		{
			*pHeap = count;
			*(pHeap + count) = 0;
			return;
		}
	}
}




/*********************************************************************
 * Function:        unsigned char _SRAMmerge(SALLOC * NEAR pSegA)
 *
 * PreCondition:    
 *
 * Input:      		SALLOC * NEAR pSegA - pointer to the first segment.  	
 *                  
 * Output:         	usnigned char - returns the length of the 
 *					merged segment or zero if failed to merge.
 *
 * Side Effects:    
 *
 * Overview:        This function tries to merge adjacent segments
 *					that have not been allocated. The largest possible
 *					segment is merged if possible.
 *
 * Note:            
 ********************************************************************/
NEAR unsigned char _SRAMmerge(SALLOC * NEAR pSegA)
{
	SALLOC * NEAR pSegB;
	NEAR SALLOC uSegA, uSegB, uSum;
	

	// Init the pointer to the heap
	pSegB = pSegA + (*pSegA).byte;
		
	// Extract the headers for faster processing
	uSegA = *pSegA;
	uSegB = *pSegB;
	
	// Quit if the tail has been found
	if (uSegB.byte == 0) return (0);
	
	// If either segment is allocated then do not merge
	if (uSegA.bits.alloc || uSegB.bits.alloc) return (0);
		
	// If the first segment is max then nothing to merge
	if (uSegA.bits.count == _MAX_SEGMENT_SIZE) return (0);
		
	// Get the sum of the two segments		
	uSum.byte = uSegA.byte + uSegB.byte;	
		
		
	// If the sum of the two segments are > than the largest segment
	// then create a new segment equal to the max segment size and
	// point to the next segments			
	if ((uSum.byte) > _MAX_SEGMENT_SIZE)
	{
		(*pSegA).byte = _MAX_SEGMENT_SIZE;
		pSegA += _MAX_SEGMENT_SIZE; //(*pSeg1).byte;  
		pSegB += uSegB.byte; //(*pSeg2).byte ;   
		(*pSegA).byte = pSegB - pSegA;
		
		return (_MAX_SEGMENT_SIZE);
	}
	// Else combine the two segments into one segment and
	// do not adjust the pointers to the next segment
	else
	{
		return ((*pSegA).byte = uSum.byte);
	}
}




/*
    Copyright 2005-2012 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>

int getCPUCount()
{
	int processorCount = 0;
	// Get the mask of available processors for this process.
	DWORD_PTR ProcessAffinityMask;
	DWORD_PTR SystemAffinityMask;
	DWORD_PTR ProcessorBit;

	if (GetProcessAffinityMask(GetCurrentProcess(), &ProcessAffinityMask, &SystemAffinityMask)) {
		// Check each bit in the mask for an available processor.
		for (ProcessorBit = 1; ProcessorBit > 0; ProcessorBit <<= 1) {
			// Increase the processor count
			if (ProcessAffinityMask & ProcessorBit)
				processorCount++;
		}
	}
	return processorCount;
}
#else
#include <unistd.h>
/*-------------------------------------------------
 * gets CPU freqeuency in Hz (Linux only)
 * from /proc/cpuinfo
 *------------------------------------------------*/

double getCPUFreq() {
   #define BUFLEN 110

   FILE* sysinfo;
   char* ptr;
   char buf[BUFLEN];
   char key[] = "cpu MHz";
   int keylen = sizeof( key ) - 1;
   double freq = -1;

   sysinfo = fopen( "/proc/cpuinfo", "r" );
   if( sysinfo != NULL ) {
      while( fgets( buf, BUFLEN, sysinfo ) != NULL ) {
         if( !strncmp( buf, key, keylen ) ) {
            ptr = strstr( buf, ":" );
            freq = atof( ptr+1 ) * 1000000;
            break;
         }
      }
      fclose( sysinfo );
   }
   fprintf(stderr, "Freq = %f GHz\n", freq / 1000000000);
   return freq;
}

int getCPUCount() {
	return sysconf(_SC_NPROCESSORS_CONF);
}

#endif

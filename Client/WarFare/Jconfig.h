﻿////////////////////////////////////////////////////////////////////////
//
//	Note : this file is included as part of the Smaller Animals Software
//	JpegFile package. Though this file has not been modified from it's 
//	original IJG 6a form, it is not the responsibility on the Independent
//	JPEG Group to answer questions regarding this code.
//	
//	Any questions you have about this code should be addressed to :
//
//	CHRISDL@PAGESZ.NET	- the distributor of this package.
//
//	Remember, by including this code in the JpegFile package, Smaller 
//	Animals Software assumes all responsibilities for answering questions
//	about it. If we (SA Software) can't answer your questions ourselves, we 
//	will direct you to people who can.
//
//	Thanks, CDL.
//
////////////////////////////////////////////////////////////////////////

/* jconfig.mc6 --- jconfig.h for Microsoft C on MS-DOS, version 6.00A & up. */
/* see jconfig.doc for explanations */

#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
/* #define void char */
/* #define const */
#undef CHAR_IS_UNSIGNED
#ifndef HAVE_STDDEF_H
#define HAVE_STDDEF_H
#endif
#ifndef HAVE_STDLIB_H
#define HAVE_STDLIB_H
#endif
#undef NEED_BSD_STRINGS
#undef NEED_SYS_TYPES_H
//#define NEED_FAR_POINTERS	/* for small or medium memory model */
#undef NEED_FAR_POINTERS	/* for small or medium memory model */
#undef NEED_SHORT_EXTERNAL_NAMES
#undef INCOMPLETE_TYPES_BROKEN

#ifdef JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED

#define USE_MSDOS_MEMMGR	/* Define this if you use jmemdos.c */

#define MAX_ALLOC_CHUNK 65520L	/* Maximum request to malloc() */

#define USE_FMEM		/* Microsoft has _fmemcpy() and _fmemset() */

#define NEED_FHEAPMIN		/* far heap management routines are broken */

#define SHORTxLCONST_32		/* enable compiler-specific DCT optimization */
/* Note: the above define is known to improve the code with Microsoft C 6.00A.
 * I do not know whether it is good for later compiler versions.
 * Please report any info on this point to jpeg-info@uunet.uu.net.
 */

#endif /* JPEG_INTERNALS */

#ifdef JPEG_CJPEG_DJPEG

#define BMP_SUPPORTED		/* BMP image file format */
#define GIF_SUPPORTED		/* GIF image file format */
#define PPM_SUPPORTED		/* PBMPLUS PPM/PGM image file format */
#undef RLE_SUPPORTED		/* Utah RLE image file format */
#define TARGA_SUPPORTED		/* Targa image file format */

#define TWO_FILE_COMMANDLINE
#define USE_SETMODE		/* Microsoft has setmode() */
#define NEED_SIGNAL_CATCHER	/* Define this if you use jmemdos.c */
#undef DONT_USE_B_MODE
#undef PROGRESS_REPORT		/* optional */

#endif /* JPEG_CJPEG_DJPEG */

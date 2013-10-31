#ifndef WAVE_H
#define WAVE_H

/**
 * cutted from formats.h file of aplay ALSA utility
 */

#include <endian.h>
#include <byteswap.h>


/* Definitions for Microsoft WAVE format */

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define LE_SHORT(v)		(v)
#define LE_INT(v)		(v)
#define BE_SHORT(v)		bswap_16(v)
#define BE_INT(v)		bswap_32(v)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define COMPOSE_ID(a,b,c,d)	((d) | ((c)<<8) | ((b)<<16) | ((a)<<24))
#define LE_SHORT(v)		bswap_16(v)
#define LE_INT(v)		bswap_32(v)
#define BE_SHORT(v)		(v)
#define BE_INT(v)		(v)
#else
#error "Wrong endian"
#endif

#define WAV_RIFF		COMPOSE_ID('R','I','F','F')
#define WAV_WAVE		COMPOSE_ID('W','A','V','E')
#define WAV_FMT			COMPOSE_ID('f','m','t',' ')
#define WAV_DATA		COMPOSE_ID('d','a','t','a')
#define WAV_PCM_CODE		1

/* it's in chunks like .voc and AMIGA iff, but my source say there
   are in only in this combination, so I combined them in one header;
   it works on all WAVE-file I have
 */

typedef struct
              {
              char ChunkID[4];		/* 'RIFF' */
              u_int ChunkSize;		/* filelen started from next field */
              char Format[4];		/* 'WAVE' */
              char SubChunk1ID[4];	/* 'fmt ' */
              u_int SubChunk1Size;	/* filelen */
              u_short AudioFormat;	/* should be 1 for PCM-code */
              u_short NumChannels;	/* 1 Mono, 2 Stereo */
              u_int SampleRate;		/* frequence of sample */
              u_int ByteRate;		/* bytes per second */
              u_short BlockAlign;	/* samplesize; 1 or 2 bytes */
              u_short BitsPerSample;	/* 8, 12 or 16 bit */
              char SubChunk2ID[4];	/* 'data' */
              u_int SubChunk2Size;	/* samplecount */
              char Data[1];
              } Wav;

#endif

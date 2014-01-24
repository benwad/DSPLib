#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>

#include "ops.h"

void print_format(int format)
{
	int type = format & SF_FORMAT_TYPEMASK;
	int subtype = format & SF_FORMAT_SUBMASK;
	printf("Format type: 0x%x, Subtype: 0x%x\n", type, subtype);
}

void print_file_info(SF_INFO* sf_info)
{
	printf("%lli frames\n", sf_info->frames);
	printf("Sample rate: %i\n", sf_info->samplerate);
	printf("%i channels\n", sf_info->channels);
	print_format(sf_info->format);
	printf("%i sections\n", sf_info->sections);
	printf("Seekable: %i\n", sf_info->seekable);
}

void print_buffer(float* buffer, unsigned int numFrames)
{
	for (int i=0; i < numFrames; i++) {
		if (buffer[i] > 0.0) {
			printf("%f\n", buffer[i]);
		}
	}

}

float* get_buffer(SNDFILE* objSndFile, SF_INFO* sf_info)
{
	float* buf = malloc(sizeof(float) * sf_info->frames);
	float* bufPtr = buf;

	for (int i=0; i < sf_info->frames; i++) {
		sf_read_float(objSndFile, bufPtr, 1);
		bufPtr++;
	}

	return buf;
}

int buffers_are_equal(float* buffer1, float* buffer2, int numFrames)
{
	int ret = 1;

	for (int i=0; i < numFrames; i++) {
		printf("%f\t->\t%f\n", buffer1[i], buffer2[i]);
		if (buffer1[i] != buffer2[i]) {
			ret = 0;
		}
	}

	return ret;
}

int main( int argc, char* argv[] )
{
	const char* sFilename = "/Users/benwadsworth/Documents/sunvox/instruments/drums/crash.wav";
	
	SF_INFO sf_info;
	SNDFILE* objSndFile = sf_open(sFilename, SFM_READ, &sf_info);
	
	print_file_info(&sf_info);

	float* sampleBuffer = get_buffer(objSndFile, &sf_info);

	sf_close(objSndFile);
	
//	float* newBuffer = malloc(sizeof(float) * sf_info.frames);
//	memcpy(newBuffer, sampleBuffer, sf_info.frames * sizeof(float));

//	flip_buffer(newBuffer, sf_info.frames);

	float* newBuffer = flex_buffer(sampleBuffer, sf_info.frames, sf_info.frames*2, INTERPOLATION_LINEAR);
	reverse_buffer(newBuffer, sf_info.frames*2);
	
	SF_INFO out_info = sf_info;
	
	SNDFILE* objOutFile = sf_open("crash_reversed.wav", SFM_WRITE, &out_info);
	
	sf_write_float(objOutFile, newBuffer, sf_info.frames*2);

//	printf("Buffers are%s equal\n", (buffers_are_equal(sampleBuffer, newBuffer, sf_info.frames)) ? "" : " not");
	
	sf_close(objOutFile);
}

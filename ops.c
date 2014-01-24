#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include <fftw3.h>

#include "ops.h"

/*
 * reverse_buffer: reverse the frames of a buffer in-place
 */
void reverse_buffer(float* buffer, unsigned int numFrames)
{
	for (int i=0; i < (numFrames / 2.0); i++)
	{
		float tmp = buffer[i];
		buffer[i] = buffer[numFrames-i-1];
		buffer[numFrames-i-1] = tmp;
	}
}

/*
 * chunk_reverse: an experiment, doesn't actually work
 */
void chunk_reverse(float* buffer, unsigned int numFrames)
{
	int chunk_size = 2000;
	int numChunks = numFrames / chunk_size + 1;
	printf("Num chunks: %i, (total frames %i of %i)\n", numChunks, numChunks * chunk_size, numFrames);
	
	float* chunks[numChunks];
	int currIndex = 0;
	
	for (int i=0; i < numChunks; i++)
	{
		int fixed_chunk_size = (chunk_size < numFrames - currIndex) ? chunk_size : numFrames - currIndex;
		
		chunks[i] = malloc(sizeof(float) * fixed_chunk_size);
		for (int j=0; j < fixed_chunk_size; j++)
		{
			chunks[i][j] = buffer[currIndex];
			currIndex++;
		}
		
		reverse_buffer(chunks[i], fixed_chunk_size);
	}
	
	printf("Finished first loop\n");
	
	currIndex = 0;
	
	for (int i=0; i < numChunks; i++)
	{
		int fixed_chunk_size = (chunk_size < numFrames - currIndex) ? chunk_size : numFrames - currIndex;

		for (int j=0; j < fixed_chunk_size; j++)
		{
			buffer[currIndex] = chunks[i][j];
			currIndex++;
		}
		free(chunks[i]);
	}
}

/*
 * flip_buffer: flip the waveform of a buffer in-place
 */
void flip_buffer(float* buffer, unsigned int numFrames)
{
	for (int i=0; i < numFrames; i++)
	{
		buffer[i] *= -1.0f;
	}
}

/*
 * get_linear_interpolation: get an interpolated value between two values
 * at the given interval
 */
float get_linear_interpolation(float firstValue, float secondValue, float fInterval)
{
	float fLower = (firstValue < secondValue) ? firstValue : secondValue;
	float fHigher = (firstValue > secondValue) ? firstValue : secondValue;

	if (fLower == fHigher) {
		return fLower;
	}

	float fDiff = fHigher - fLower;
	return fLower + (fDiff * fInterval);
}

/*
 * flex_buffer: stretches/compresses buffer to new size
 *
 * Doesn't do anything to the buffer passed in, returns a pointer
 * to the new, stretched buffer
 *
 * */
float* flex_buffer(float* buffer, unsigned int numFrames, unsigned int newSize, Interpolation interp_type)
{
	float* outBuffer = malloc(sizeof(float) * newSize);
	
	// Iterate through the new buffer, filling in the interpolated value from the old buffer
	for (int i=0; i < newSize; i++) {
		float fIndex = ((float)numFrames / (float)newSize) * i;
		int iFloor = floorf(fIndex);
		int iCeil = ceilf(fIndex);
		float fInterp = fIndex - iFloor;
		float lowValue = buffer[iFloor];
		float highValue = buffer[iCeil];

		switch (interp_type) {
			case INTERPOLATION_LINEAR:
				outBuffer[i] = get_linear_interpolation(lowValue, highValue, fInterp);
				break;
			case INTERPOLATION_CUBIC:
				printf("Cubic interpolation not yet complete\n");
				break;
			case INTERPOLATION_BICUBIC:
				printf("Bicubic interpolation not yet complete\n");
				break;
			default:
				printf("Invalid interpolation setting\n");
		}
	}
	
	return outBuffer;
}

void lowpass(float* buffer, float cutoff, unsigned int numFrames)
{

}

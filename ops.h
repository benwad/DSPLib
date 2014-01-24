typedef enum {
	INTERPOLATION_LINEAR,
	INTERPOLATION_CUBIC,
	INTERPOLATION_BICUBIC,
} Interpolation;

void reverse_buffer(float* buffer, unsigned int numFrames);
void chunk_reverse(float* buffer, unsigned int numFrames);
void flip_buffer(float* buffer, unsigned int numFrames);
float* flex_buffer(float* buffer, unsigned int numFrames, unsigned int newSize, Interpolation interp_type);
void lowpass(float* buffer, float cutoff, unsigned int numFrames);


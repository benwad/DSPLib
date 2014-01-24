
all:
	clang ops.c soundfile.c -lsndfile -lfftw3 -lm -o soundfile;

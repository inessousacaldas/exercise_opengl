#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

// std includes
#include <memory>

struct AudioFile
{
	int channels;
	int sampleRate;
	int samples;
	std::unique_ptr<short> data;

	int getSampleCount() const {
		return channels * samples;
	}
};

#endif
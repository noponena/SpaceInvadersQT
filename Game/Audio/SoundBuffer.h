#pragma once
#include <AL/al.h>
#include <vector>
#include <QString>


namespace Game {
namespace Audio {

class SoundBuffer
{
public:
	static SoundBuffer* get();

    ALuint addSoundEffectFromResource(const QString &resourcePath);
	ALuint addSoundEffect(const char* filename);
	bool removeSoundEffect(const ALuint& buffer);

private:
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> p_SoundEffectBuffers;
};

}
}

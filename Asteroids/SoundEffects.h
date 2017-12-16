#pragma once
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>

class SoundEffects
{
public:
	const enum soundType { BEAT, THRUST, LASER, EXPLOSION, SAUCER, NEWLIFE };

	SoundEffects();
	void Play(soundType st);
	void Stop(SoundEffects::soundType st);
	~SoundEffects();

private:
	bool beat1;
	sf::Sound sineSound;
	sf::Sound laserSound;
	sf::Sound beat1Sound;
	sf::Sound beat2Sound;
	sf::Sound thrustSound;
	sf::Sound explosionSound;
	sf::Sound saucerSound;
	sf::Sound newlifeSound;
	sf::SoundBuffer sineBuffer;
	sf::SoundBuffer laserBuffer;
	sf::SoundBuffer beat1Buffer;
	sf::SoundBuffer beat2Buffer;
	sf::SoundBuffer thrustBuffer;
	sf::SoundBuffer explosionBuffer;
	sf::SoundBuffer saucerBuffer1;
	sf::SoundBuffer saucerBuffer2;
	sf::SoundBuffer saucerBuffer;
	sf::SoundBuffer newlifeBuffer;
};


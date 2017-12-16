#include "stdafx.h"
#include "SoundEffects.h"
#include "Synth.hpp"

const unsigned SAMPLES = 44100;
const unsigned SAMPLE_RATE = 44100;
const unsigned AMPLITUDE = 30000;
const double TWO_PI = 6.28318;

SoundEffects::SoundEffects()
{
	// LASER
	Synth::sEnvelope env = { 0.0, 0.0, 0.0, 0.3, 1.0, 1.0 };
	Synth::sTone baseTone = { Synth::OSC_SAW_DIG, 800.0, 400.0, 0.8 };
	Synth::generate(&laserBuffer, env, baseTone, 20000, 22050);
	laserSound.setBuffer(laserBuffer);

	// BEAT1
	env = { 0.03, 0.0, 0.05, 0.01, 1.0, 1.0 };
	baseTone = { Synth::OSC_SQUARE, 60.0, 60.0, 0.20 };
	Synth::sTone sineTone = { Synth::OSC_SINE, 60.0, 60.0, 0.70 };

	std::vector <Synth::sTone> toneVect;
	toneVect.push_back(baseTone);
	toneVect.push_back(sineTone);

	Synth::generate(&beat1Buffer, env, toneVect, 10000, 22050);
	beat1Sound.setBuffer(beat1Buffer);

	// BEAT2
	env = { 0.03, 0.0, 0.05, 0.01, 1.0, 1.0 };
	baseTone = { Synth::OSC_SQUARE, 50.0, 50.0, 0.20 };
	sineTone = { Synth::OSC_SINE, 50.0, 50.0, 0.70 };

	std::vector <Synth::sTone> toneVect2;
	toneVect2.push_back(baseTone);
	toneVect2.push_back(sineTone);

	Synth::generate(&beat2Buffer, env, toneVect2, 10000, 22050);
	beat2Sound.setBuffer(beat2Buffer);


	// THRUST
	env = { 0.00, 0.0, 0.01, 0.0, 1.0, 1.0 };
	Synth::sTone noise = { Synth::OSC_NOISE, 880.0, 880.0, 0.2 };

	Synth::generate(&thrustBuffer, env, noise, 30000, 22050);
	thrustSound.setPitch(0.03f);
	thrustSound.setBuffer(thrustBuffer);

	// EXPLOSION
	env = { 0.00, 0.0, 0.0, 0.1, 1.0, 1.0 };
	baseTone = { Synth::OSC_SAW_DIG, 70.0, 70.0, 0.1 };
	noise = { Synth::OSC_NOISE, 880.0, 880.0, 0.8 };

	std::vector <Synth::sTone> tone;
	tone.push_back(baseTone);
	tone.push_back(noise);

	Synth::generate(&explosionBuffer, env, tone, 30000, 22050);
	explosionSound.setPitch(0.08f);
	explosionSound.setBuffer(explosionBuffer);

	// SAUCER - SIREN
	env = { 0.0, 0.0, 0.0, 0.15, 1.0, 1.0 };
	baseTone = { Synth::OSC_SQUARE, 1500.0, 1800.0, 0.5 };
	Synth::generate(&saucerBuffer, env, baseTone, 2000, 22050);
	saucerSound.setBuffer(saucerBuffer);

	//// SAUCER - SIREN
	//env = { 0.0, 0.0, 0.0, 0.1, 1.0, 1.0 };
	//baseTone = { Synth::OSC_TRIANGLE, 1100.0, 1000.0, 0.8 };
	//Synth::generate(&saucerBuffer1, env, baseTone, 20000, 22050);
	//baseTone = { Synth::OSC_TRIANGLE, 1000.0, 1100.0, 0.8 };
	//Synth::generate(&saucerBuffer2, env, baseTone, 20000, 22050);

	//// Combine two SoundBuffers to one
	//// calculate and allocate size
	//int firstBufferSize = saucerBuffer1.getSampleCount();
	//int secondBufferSize = saucerBuffer2.getSampleCount();
	//int iBufferSize = firstBufferSize + secondBufferSize;

	//sf::Int16 * iRawBuffer;
	//iRawBuffer = new sf::Int16[iBufferSize];

	//const sf::Int16 * iFirstBuffer;
	////iFirstBuffer = new sf::Int16[firstBufferSize];
	//iFirstBuffer = saucerBuffer1.getSamples();

	//for (int n = 0; n < firstBufferSize; n++)
	//{
	//	iRawBuffer[n] = iFirstBuffer[n];
	//}

	////delete iFirstBuffer;

	//const sf::Int16 * iSecondBuffer;
	////iSecondBuffer = new sf::Int16[secondBufferSize];
	//iSecondBuffer = saucerBuffer2.getSamples();

	//for (int n = 0; n < secondBufferSize; n++)
	//{
	//	iRawBuffer[firstBufferSize+n] = iSecondBuffer[n];
	//}

	////delete iSecondBuffer;

	//saucerBuffer.loadFromSamples(iRawBuffer, iBufferSize, 1, 22050);

	//saucerSound.setBuffer(saucerBuffer);


	// NEW LIFE - LONG PING
	env = { 0.01, 0.1, 1.0, 0.1, 2.0, 0.5 };
	baseTone = { Synth::OSC_SINE, 1500.0, 1500.0, 0.8 };
	Synth::generate(&newlifeBuffer, env, baseTone, 15000, 44100);
	newlifeSound.setBuffer(newlifeBuffer);

}

void SoundEffects::Play(SoundEffects::soundType st)
{
	switch (st)
	{
	case THRUST:
		if (thrustSound.getStatus() != sf::Sound::Playing)
		{
			thrustSound.setLoop(true);
			thrustSound.play();
		}
		break;
	case LASER:
		laserSound.play();
		break;
	case EXPLOSION:
		explosionSound.play();
		break;
	case BEAT:
		if (beat1 == true)
		{
			beat1Sound.play();
			beat1 = false;
		}
		else
		{
			beat2Sound.play();
			beat1 = true;
		}
		break;
	case SAUCER:
		if (saucerSound.getStatus() != sf::Sound::Playing)
		{
			saucerSound.setLoop(true);
			saucerSound.play();
		}
		break;
	case NEWLIFE:
		newlifeSound.play();
		break;
	}
}

void SoundEffects::Stop(SoundEffects::soundType st)
{
	switch (st)
	{
	case THRUST:
		if (thrustSound.getStatus() == sf::Sound::Playing)
		{
			thrustSound.stop();
		}
		break;
	case LASER:
	case EXPLOSION:
	case BEAT:
		break;
	case SAUCER:
		if (saucerSound.getStatus() == sf::Sound::Playing)
		{
			saucerSound.stop();
		}
		break;
	}

}

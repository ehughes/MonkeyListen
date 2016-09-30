/*
 * MonkeyListenConfig.h
 *
 *  Created on: Sep 29, 2016
 *      Author: emh203
 */

#ifndef SOURCE_MONKEYLISTENCONFIG_H_
#define SOURCE_MONKEYLISTENCONFIG_H_




/***
 *       _____             __ _
 *      / ____|           / _(_)
 *     | |     ___  _ __ | |_ _  __ _
 *     | |    / _ \| '_ \|  _| |/ _` |
 *     | |___| (_) | | | | | | | (_| |
 *      \_____\___/|_| |_|_| |_|\__, |
 *                               __/ |
 *                              |___/
 */

#define  MODE_SPECTROGRAM				0
#define  MODE_TIME_DOMAIN_PLUS_FFT		1

//Select the MonkeyListen Mode here!

#define MONKEY_LISTEN_MODE	MODE_TIME_DOMAIN_PLUS_FFT
//#define MONKEY_LISTEN_MODE	MODE_SPECTROGRAM

//Pick A Sample Rate.   16KHz works well for general viewing as most "voice" artifacts are quite low. (our FFT is 128 Points)
#define AUDIO_SAMPLE_RATE		16000

#define AUDIO_CAPTURE_BUFFER_SIZE	128

#endif /* SOURCE_MONKEYLISTENCONFIG_H_ */

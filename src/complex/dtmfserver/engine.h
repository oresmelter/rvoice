#ifndef ENGINE_H
#define ENGINE_H

/**
 *
 * open device for play
 *
 */
int open_device();

/**
 *
 * set up device parameters
 *
 */
int setup_device();

/**
 *
 * playing dtmf tone
 *
 */
int play_dtmf();

/**
 *
 * playing voice files
 *
 */
int play_voice();

/**
 *
 * playing sound from files
 *
 */
int play_sound();

/**
 *
 * making a pause
 *
 */
int play_pause();


#endif

#ifndef __HAVE_SHARED_DEFS_H
#define __HAVE_SHARED_DEFS_H

// NOTE: If you change the order of these,
// make sure you re-upload to slaves
enum packet_op {
	OP_LIGHTS_OFF = 0,
	OP_DISCOTIME,
	OP_PLAY_MUSIC,
	OP_STOP_DISCOTIME,
	OP_PAUSE_MUSIC,
	OP_LIGHTS_ON,
	// like OP_PLAY_MUSIC, but also delegate the discotime to be triggered
	// by the speaker system at the moment playback actually starts.
	OP_PLAY_MUSIC_WITH_DISCOTIME,
};

#endif

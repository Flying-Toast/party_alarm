#ifndef __HAVE_SHARED_DEFS_H
#define __HAVE_SHARED_DEFS_H

// NOTE: If you change the order of these,
// make sure you re-upload to slaves
enum packet_op {
	OP_LIGHTS_OFF = 0,
	OP_DISCOTIME,
	OP_PLAYPAUSE_MUSIC,
	OP_STOP_DISCOTIME,
};

#endif

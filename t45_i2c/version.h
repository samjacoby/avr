#ifndef __version_h_
#define __version_h_

#define FW_SIGNATURE 'A'

/**
 * Firmware version number, so that software can check the firmware version and know
 * what features might be available/whether an upgrade might be necessary
 */
#define VERSION_L 0x01
#define VERSION_H 0x00

/**
 * Initials of the maintainer of this branch of the firmware, so that each maintainer
 * can use his/her own branch version numbers without worrying about collisions
 */
#define INITIALS1 'S'
#define INITIALS2 'D'
#define INITIALS3 'J'

#endif // __version_h_


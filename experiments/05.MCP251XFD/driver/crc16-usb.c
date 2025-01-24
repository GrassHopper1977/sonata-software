#include "crc16-usb.h"

uint16_t crc16_usb(const uint8_t* data, size_t bytes) {
    const uint16_t poly16 = 0xA001;
    uint16_t crc16 = 0xffff;
    uint16_t i;

    for (i = 0; i < bytes; i++) {
        uint16_t udata = data[i];
        int bit = 8;

        while (bit--) {
            if ((udata ^ crc16) & 0x01) {
                crc16 >>= 1;
                crc16 ^= poly16;
            }
            else {
                crc16 >>= 1;
            }

            udata >>= 1;
        }
    }
    // Invert contents to generate crc field
    crc16 ^= 0xffff;

    return crc16;
}
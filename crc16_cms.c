#include "crc16_cms.h"


// crc hesaplama
uint16_t crc16_cms_calc(const uint8_t* data, size_t len) {
    uint16_t crc = init;

    for (size_t i = 0; i < len; i++) {
        uint8_t index = (crc >> 8) ^ data[i];
        crc = (crc << 8) ^ crc16_cms_table[index];
    }
    return crc ^ xorOut;;
}


#include "UUID.h"

#include <string>
#include <random>

UUID createUUID() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 255);

    UUID res = 0;

    for (int i = 0; i < 16; i++) {
        res |= ((UUID)dist(rng)) << (i * 8);
    }

    return res;
}
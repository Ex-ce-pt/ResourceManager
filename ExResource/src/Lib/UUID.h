#pragma once

#include <stdint.h>

typedef uint64_t UUID;

/*
A small UUID creation function.
Not ideal (to say the least).
*/
UUID createUUID();
#pragma once

#include <string>
#include <random>

typedef uint64_t UUID;

/*
A small UUID creation function.
Not ideal (to say the least).
*/
UUID createUUID();
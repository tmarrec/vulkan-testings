#pragma once

#include <bitset>

// Entity alias and maximum
using Entity = uint64_t;
const Entity MAX_ENTITIES = 2048;

// Components alias and maximum
using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 16;

// Signature alias
using Signature = std::bitset<MAX_COMPONENTS>;


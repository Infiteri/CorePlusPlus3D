#include "UUID.h"

#include <random>
#include <unordered_set>

namespace Core
{
    static std::random_device randomDevice;
    static std::mt19937_64 engine(randomDevice());
    static std::uniform_int_distribution<uint64_t> uniformDistribution; // Use uniform_int_distribution

    UUID::UUID()
    {
        id = uniformDistribution(engine);
    }

    UUID::UUID(uint64_t id) // Correct the type in the constructor parameter
    {
        this->id = id;
    }

    UUID::~UUID()
    {
    }
}

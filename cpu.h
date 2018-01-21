#ifndef CPU
#define CPU

#include <cstdint>

class Cpu{

private:
    uint16_t register_[8];

public:
    Cpu();
    ~Cpu();
    void increaseRegister(int number, uint16_t value);
    void decreaseRegister(int number, uint16_t value);
    void setRegister(int number, uint16_t value);
    void fromRegisterToMemory(int number, uint16_t * mem_offset);
};

#endif

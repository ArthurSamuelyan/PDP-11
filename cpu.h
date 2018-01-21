#ifndef CPU
#define CPU

#include <cstdint>

class Cpu{

private:


public:
    uint16_t register_[8];
    bool N, Z, V, C;            //condition codes
    Cpu();
    ~Cpu();
    void increaseRegister(int number, uint16_t value);
    void decreaseRegister(int number, uint16_t value);
    void setRegister(int number, uint16_t value);
    void fromRegisterToMemory(int number, uint16_t * mem_offset);
    uint16_t getRegister(int number);
};

#endif

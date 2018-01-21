#include <cpu.h>

Cpu::Cpu(){

}

Cpu::~Cpu(){

}

void Cpu::increaseRegister(int number, uint16_t value){
    register_[number] += value;
}

void Cpu::decreaseRegister(int number, uint16_t value){
    register_[number] -= value;
}

void Cpu::setRegister(int number, uint16_t value){
    register_[number] = value;
}

void Cpu::fromRegisterToMemory(int number, uint16_t * mem_offset){
    (*mem_offset) = register_[number];
}

uint16_t Cpu::getRegister(int number){
    return register_[number];
}

#include <cstdint>
#include <cpu.h>

enum Instructions{
    CLR,
    INC,
    DEC,
    MOV,
    CMP,
    ADD,
    SUB

};

struct instructionInfo{
    Instructions instruction;
    uint16_t opcode;
    uint16_t dest;
    uint16_t dest_mode;
    uint16_t source;
    uint16_t source_mode;
    bool byte;
};

class InstructionHandler{

private:


public:
    Cpu * cpu;                              //Pointer to CPU object
    uint16_t instr;                         //Word that stores current instruction
    uint16_t * memory;                      //Pointer to first word in emulator memory
    uint16_t pc_increase;                   //Program Counter increase after performing instruction. (R7 += pc_increase*2)
    InstructionHandler();
    ~InstructionHandler();
    uint16_t getInstruction(uint16_t * mem_offset);
    instructionInfo getInfo(uint16_t instr);
    void performInstruction(instructionInfo info);
};


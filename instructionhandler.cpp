#include <instructionhandler.h>
#include <stdio.h>

uint16_t * getContents(Cpu * cpu, uint16_t mode, uint16_t dest, uint16_t * memory, uint16_t * instr_ptr, uint16_t * pc_increase){
    uint16_t * ptr;
    switch(mode)
    {
        case(0):                        //register mode
        {
            ptr = &(cpu->register_[dest]);
        };
        case(2):
        {
            ptr = (memory + (cpu->getRegister(dest) / 2));
            cpu->increaseRegister(dest, 2);
        };
        case(4):
        {
            cpu->decreaseRegister(dest, 2);
            ptr = (memory + (cpu->getRegister(dest) / 2));
        };
        case(6):
        {
            ptr = (memory + (cpu->getRegister(dest) / 2) + (*(instr_ptr+(*pc_increase))));      //*pc_increase not always equals 1
            (*pc_increase) += 1;                            //1 more words have been red. Thus program counter should be increased.
        }
        case(1):
        {
            ptr = (memory + (cpu->getRegister(dest) / 2));
        };
        case(3):
        {
            ptr = (memory + (cpu->getRegister(dest) / 2));
            ptr = (memory + (*ptr));
            cpu->increaseRegister(dest, 2);
        };
        case(5):
        {
            cpu->decreaseRegister(dest, 2);
            ptr = (memory + (cpu->getRegister(dest) / 2));
            ptr = (memory + (*ptr));
        };
        case(7):
        {
            ptr = (memory + (cpu->getRegister(dest) / 2) + (*(instr_ptr+(*pc_increase))));
            ptr = (memory + (*ptr));
            (*pc_increase) += 1;
        };
        default:
        {
            printf("Error: wrong mode");
        }
    }
    return ptr;
}


InstructionHandler::InstructionHandler(){

}

InstructionHandler::~InstructionHandler(){

}

uint16_t InstructionHandler::getInstruction(uint16_t *mem_offset){
    uint16_t instruction = *mem_offset;
    return instruction;
}

instructionInfo InstructionHandler::getInfo(uint16_t instr){
    instructionInfo info;
    if ((instr & 0b0111111111000000) == 0b0000101000000000){
        info.instruction = CLR;
        info.opcode = instr >> 6;
        info.dest = instr & 0b0000000000000111;
        info.dest_mode = (instr >> 3) & 0b0000000000000111;
        (instr >> 15) ? info.byte = true : info.byte = false;
    }
    else if ((instr & 0b0111111111000000) == 0b0000101010000000){
        info.instruction = INC;
        info.opcode = instr >> 6;
        info.dest = instr & 0b0000000000000111;
        info.dest_mode = (instr >> 3) & 0b0000000000000111;
        (instr >> 15) ? info.byte = true : info.byte = false;
    }
    else if ((instr & 0b0111111111000000) == 0b0000101011000000){
        info.instruction = DEC;
        info.opcode = instr >> 6;
        info.dest = instr & 0b0000000000000111;
        info.dest_mode = (instr >> 3) & 0b0000000000000111;
        (instr >> 15) ? info.byte = true : info.byte = false;
    }
    else if ((instr & 0b0111000000000000) == 0b0001000000000000){
        info.instruction = MOV;
        info.opcode = instr >> 12;
        info.dest = instr & 0b0000000000000111;
        info.dest_mode = (instr >> 3) & 0b0000000000000111;
        info.source = (instr >> 6) & 0b0000000000000111;
        info.source_mode = (instr >> 9) & 0b0000000000000111;
        (instr >> 15) ? info.byte = true : info.byte = false;
    }
    else if ((instr & 0b0111000000000000) == 0b0010000000000000){
        info.instruction = CMP;
        info.opcode = instr >> 12;
        info.dest = instr & 0b0000000000000111;
        info.dest_mode = (instr >> 3) & 0b0000000000000111;
        info.source = (instr >> 6) & 0b0000000000000111;
        info.source_mode = (instr >> 9) & 0b0000000000000111;
        (instr >> 15) ? info.byte = true : info.byte = false;
    }
    else if ((instr & 0b1111000000000000) == 0b0110000000000000){
        info.instruction = ADD;
        info.opcode = instr >> 12;
        info.dest = instr & 0b0000000000000111;
        info.dest_mode = (instr >> 3) & 0b0000000000000111;
        info.source = (instr >> 6) & 0b0000000000000111;
        info.source_mode = (instr >> 9) & 0b0000000000000111;
        info.byte = false;
    }
    else if ((instr & 0b1111000000000000) == 0b1110000000000000){
        info.instruction = SUB;
        info.opcode = instr >> 12;
        info.dest = instr & 0b0000000000000111;
        info.dest_mode = (instr >> 3) & 0b0000000000000111;
        info.source = (instr >> 6) & 0b0000000000000111;
        info.source_mode = (instr >> 9) & 0b0000000000000111;
        info.byte = false;
    }
    return info;
}

void InstructionHandler::performInstruction(instructionInfo info){
    pc_increase = 1;
    switch (info.instruction){
        case(CLR):
        {
            uint16_t * dest_ptr = getContents(cpu, info.dest_mode, info.dest, memory, (uint16_t*)(cpu->getRegister(7) / 2), &pc_increase);
            *dest_ptr = 0;
            cpu->increaseRegister(7, pc_increase * 2);
        };
        case(MOV):
        {
            uint16_t * source_ptr = getContents(cpu, info.source_mode, info.source, memory, (uint16_t*)(cpu->getRegister(7) / 2), &pc_increase);
            uint16_t * dest_ptr = getContents(cpu, info.dest_mode, info.dest, memory, (uint16_t*)(cpu->getRegister(7) / 2), &pc_increase);
            *dest_ptr = *source_ptr;
            cpu->increaseRegister(7, pc_increase * 2);
        }
    }
}

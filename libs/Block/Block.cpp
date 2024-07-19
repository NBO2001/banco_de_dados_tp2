#include "./Block.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include "stdlib.h"


Block::Block(){
    n_regs = 0;
    size = 0;
    block = (unsigned char*) malloc(BLOCK_SIZE);
}

Block::~Block() {
    destroy();
}


int Block::write(void* fp){

    FILE* fpp = (FILE*) fp;

    fwrite(&n_regs, sizeof(unsigned short), 1, fpp);
    fwrite(&size, sizeof(unsigned short), 1, fpp);
    fwrite(block, BLOCK_SIZE-4, 1, fpp);

    return 1;

}

void Block::read_from_file(void* fp){

    FILE* fpp = (FILE*) fp;

    fread(&n_regs, sizeof(unsigned short), 1, fpp);
    fread(&size, sizeof(unsigned short), 1, fpp);
    fread(block, BLOCK_SIZE-4, 1, fpp);

}

int Block::insert(char* reg, unsigned short size_reg){

    unsigned short space_free = size_free();

    if(size_reg > space_free) return BLOCK_WITHOUT_SPACE; 

    memcpy(block+size, reg, size_reg);

    // printf("Size reg insert: %d\n", size_reg);

    size += size_reg;

    n_regs++;

    return 1;

}

unsigned short Block::size_free(){

    unsigned short size_here = BLOCK_SIZE-size-4;

    if(size_here < 0){
        fprintf(stderr,"ERROR: TAMANHO (%d) DO BLOCK INVALIDO\n", size_here);
        exit(0);
    }

    return size_here;

}

void Block::destroy() {
    if (block != nullptr) {
        free(block);
        block = nullptr;
    }
}

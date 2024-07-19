#include <cstring>
#include <cstdio>
#include <iostream>
#include "stdlib.h"
#include "./Database.h"

Database::Database(char* base_filename) {

    fp = fopen(base_filename, "rb+");
    pathFile = base_filename;

    if (fp == NULL) {
        
        fp = fopen(base_filename, "wb+");
        if (fp == NULL) {
            fprintf(stderr,"Unable to create file!\n");
            exit(1);
        }

        n_blocks = 0;
        fwrite(&n_blocks, sizeof(n_blocks), 1, fp);
        rewind(fp);
    } else {
        fread(&n_blocks, sizeof(n_blocks), 1, fp);
    }
}

/* Era uma função para recarregar o arquivo, mas é inutil*/
void Database::reboot(){

    fp = fopen(pathFile, "wb");
    if (fp == NULL) {
        fprintf(stderr,"Unable to create file!\n");
        exit(1);
    }
    n_blocks = 0;
    fwrite(&n_blocks, sizeof(n_blocks), 1, fp);
    rewind(fp);

}


Database::~Database(){
    if(fp) fclose(fp);
}

void Database::add_block(int qnt_blocks){

    if (!fp) {
        printf("Database file is not open!\n");
        return;
    }

    unsigned int postion = (n_blocks*BLOCK_SIZE) + 4;

    n_blocks += qnt_blocks;

    fseek(fp, 0, SEEK_SET);
    fwrite(&n_blocks, sizeof(n_blocks), 1, fp);

    fseek(fp, postion, SEEK_SET);
    
    // Create and write each block
    Block bk;
    for (int i = 0; i < qnt_blocks; i++) {
        bk.write(fp);
    }
}


Block* Database::read(int id_block){

    if(id_block > n_blocks) return nullptr;

    unsigned int position = (id_block*BLOCK_SIZE) + 4;

    fseek(fp, position, SEEK_SET);

    Block* bk = new Block();
    bk->read_from_file(fp);

    return bk;
}

int Database::write(Block* bk, int id_block){

    if(id_block > n_blocks) return 0;

    unsigned int postion = (id_block*BLOCK_SIZE) + 4;

    fseek(fp, postion, SEEK_SET);

    bk->write(fp);

    return 1;
}
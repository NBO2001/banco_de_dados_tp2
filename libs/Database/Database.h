#ifndef DATABASE_H
#define DATABASE_H

#include <cstdio> // For FILE
#include "../Block/Block.h"

class Database{

    public:
        unsigned int n_blocks;
        FILE* fp;

        Database(char* base_filename);
        ~Database();
        void add_block(int qnt_blocks);

        Block* read(int id_block);
        int write(Block* bk, int id_block);
        void reboot();

    private:
        char* pathFile;
};

#endif
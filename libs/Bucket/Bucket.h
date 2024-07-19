#ifndef BUCKET_H
#define BUCKET_H

#include "../Database/Database.h"
#include "../Register/Register.h"

#define BLOCKS_PER_BUCKET 70

class Bucket{

    public:
        Database* db;
        Bucket(Database* database);
        int insertInBucket(char* reg, unsigned short size_reg, int bucketIndex);
        int insertInBucket(Register* reg, int bucketIndex);
        int* get_blocks(int bucketIndex); // Return the index of blocks

        int insertInBlock(char* reg, unsigned short size_reg, int blockIndex);
        int insertInBlock(Register* reg, int blockIndex);

};

#endif
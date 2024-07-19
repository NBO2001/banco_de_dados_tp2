#include <cstring>
#include <cstdio>
#include <iostream>
#include "stdlib.h"
#include "./Hashing.h"
#include "../Bucket/Bucket.h"

#define SEED    0x392
#define OVERFLOW_BUCKETS 100


Hashing::Hashing(Database* bd){
    if(bd == nullptr){
        fprintf(stderr, "Error in file open\n");
        exit(1);
    }

    fileBase = bd;
	blocksFull = 0;
}


int Hashing::insert(unsigned int key, Register* reg){

    Bucket bucket = Bucket(fileBase);

	unsigned int buckets = fileBase->n_blocks/BLOCKS_PER_BUCKET;
	unsigned int bucktes_uteis = (buckets * 0.90);
	unsigned int blocks_uteis = (bucktes_uteis*BLOCKS_PER_BUCKET);

	unsigned int hash1 = key % blocks_uteis; 
	unsigned int bucketIndex = hash1 / BLOCKS_PER_BUCKET; 

    int status = bucket.insertInBucket(reg,bucketIndex);

    if(status==0){
		
        for(int bkIndex=(blocks_uteis+blocksFull); bkIndex < fileBase->n_blocks; bkIndex++){

            status = bucket.insertInBlock(reg,bkIndex);

			// if(status==0 && (blocksFull)%10000 == 1){printf("\t Overflow(%d)\n", blocksFull+1);}

			if(status==0){blocksFull++;}

            if(status==1) return status;

        }

    }

    return status;


}

Register* block_contains(Block* bk, int id){
    
    unsigned int offset = 0;

    if(bk->n_regs == 0) return nullptr;

    Register* reg = new Register();

    for(int i=0; i < bk->n_regs; i++){

        unsigned short size_reg = 0;
        memcpy(&size_reg, bk->block+offset, 2);
        offset += 2;

        reg->from_bytes((char*) bk->block+offset);
        offset += size_reg;

        if(reg->id == id){
            return reg;
        }
        
    }

    delete reg;

    return NULL;

}

Register* Hashing::search(unsigned int key){

	Register* reg = nullptr;
	Block* bk;

	Bucket bucket = Bucket(fileBase);

	unsigned int buckets = fileBase->n_blocks/BLOCKS_PER_BUCKET;
	unsigned int bucktes_uteis = (buckets * 0.90);
	unsigned int blocks_uteis = (bucktes_uteis*BLOCKS_PER_BUCKET);

	unsigned int hash1 = key % blocks_uteis; 
	unsigned int bucketIndex = hash1 / BLOCKS_PER_BUCKET; 

	printf("====Bucket(%d)====\n", bucketIndex);

    int* blocks_vec = bucket.get_blocks(bucketIndex);

	for(int i=0; i < BLOCKS_PER_BUCKET; i++){

		int blockIndex = blocks_vec[i];
		printf("\t===Block(%d)==\n", blocks_vec[i]);
		bk = fileBase->read(blockIndex);
		reg = block_contains(bk, key);

		if(reg){
			free(blocks_vec);
			return reg;
		}

		delete bk;

	}

	for(int bkIndex=(blocks_uteis+blocksFull); bkIndex < fileBase->n_blocks; bkIndex++){

		bk = fileBase->read(bkIndex);
		reg = block_contains(bk, key);

		if(reg){
			delete bk;
			return reg;
		}

		delete bk;

	}

    return reg;
}
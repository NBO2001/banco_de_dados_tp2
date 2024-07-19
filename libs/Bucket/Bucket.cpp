#include "./Bucket.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include "stdlib.h"
#include "../Block/Block.h"

Bucket::Bucket(Database* database){
    db = database;   
}

int Bucket::insertInBlock(char* reg, unsigned short size_reg, int blockIndex){

    Block* block = db->read(blockIndex);

    if(!block) return 0;

    if(block->insert(reg, size_reg)){
        db->write(block, blockIndex);
        delete block;
        return 1;
    }

    delete block;

    return 0;

}

int Bucket::insertInBlock(Register* reg, int blockIndex){
                
    char* my_vec = reg->to_bytes(); // Crio o vetor de bytes
    unsigned short size_reg = 0; 

    memcpy(&size_reg, my_vec, 2);
    size_reg += 2;

    int status = insertInBlock(my_vec, size_reg, blockIndex);

    free(my_vec);

    return status;
}

/**
 * Função que insere no bucket a partir de um vetor de bytes e um index do bucket
*/
int Bucket::insertInBucket(char* reg, unsigned short size_reg, int bucketIndex){

    int blockIndexStart = bucketIndex * BLOCKS_PER_BUCKET;
    int blockIndexEnd = blockIndexStart + BLOCKS_PER_BUCKET;

    // Insere no bloco do bucket que estive espaço
    for(int blockIndex=blockIndexStart; blockIndex < blockIndexEnd; blockIndex++){

        Block* block = db->read(blockIndex);

        if(block->insert(reg, size_reg)){
            db->write(block, blockIndex);
            delete block;
            return 1;
        }

        delete block;

    }

    return 0;

}

/**
 * Função que insere no bucket a partir de um objeto da classe Registro e um index do bucket
*/

int Bucket::insertInBucket(Register* reg, int bucketIndex){
                
    char* my_vec = reg->to_bytes(); // Crio o vetor de bytes
    unsigned short size_reg = 0; 

    memcpy(&size_reg, my_vec, 2);
    size_reg += 2;

    int status = insertInBucket(my_vec, size_reg, bucketIndex);

    free(my_vec);

    return status;
}

/**
 * Coloca o index dos blocos pertencentes a um bucket em um array
 * Facilita para não ter que carregar todos os blocos do bucket
 *  para a memória principal.
*/

int* Bucket::get_blocks(int bucketIndex){

    int* indexBuckts = (int*) malloc(sizeof(int)*BLOCKS_PER_BUCKET);

    int blockIndexStart = bucketIndex * BLOCKS_PER_BUCKET;
    int blockIndexEnd = blockIndexStart + BLOCKS_PER_BUCKET;

    for(int blockIndex=blockIndexStart; blockIndex < blockIndexEnd; blockIndex++){
        indexBuckts[blockIndex-blockIndexStart] = blockIndex;
    }

    return indexBuckts;

}

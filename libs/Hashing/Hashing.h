#ifndef HASHING_H
#define HASHING_H

#include "../Database/Database.h"
#include "../Register/Register.h"

class Hashing{

    public:

        //Constructor
        Hashing(Database* bd);
        int insert(unsigned int key, Register* reg);
        Register* search(unsigned int key);
        Register* remove(void * key, unsigned int sizeKey); 

    private:
        Database* fileBase;
        unsigned int blocksFull;

        unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed );

};

#endif
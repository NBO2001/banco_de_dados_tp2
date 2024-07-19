#ifndef BLOCK_H
#define BLOCK_H

#define BLOCK_SIZE 4096
#define BLOCK_WITHOUT_SPACE 0

class Block{

    public:
        unsigned short n_regs;
        unsigned short size; // Occupation of block
        unsigned char* block;

        // Constructor and Destructor
        Block();
        ~Block();

         // Public member functions
        unsigned short size_free();
        int insert(char* reg, unsigned short size_reg);
        int write(void* fp); // write in file
        void read_from_file(void* fp);
        void destroy();
};

  

#endif
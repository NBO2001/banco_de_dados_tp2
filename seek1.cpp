#include "./libs/BPtree/BPTree.hpp"
#include "./libs/Database/Database.h"
#include "./libs/Block/Block.h"
#include "./libs/Register/Register.h"

#define ORDER 340

using namespace std;

void block_print(Block *bk, int id)
{
    unsigned int offset = 0;

    Register *reg = new Register();

    for(int i = 0; i < bk->n_regs; i++)
    {
        uint16_t size_reg = 0;
        memcpy(&size_reg, bk->block + offset, sizeof(size_reg));
        offset += 2;

        reg->from_bytes((char *) bk->block+offset);
        offset += size_reg;

        if(reg->id == id)
        {
            reg->print();
            delete reg;
            return;
        }
    }
    delete reg;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "USAGE: seek1 <ID>\n");
        return 1;
    }

    uint32_t ID = atoi(argv[1]);
    Database dbFile = Database((char*) "./downloads/db.bin");
    Block *blk;

    BPTree<uint32_t, ORDER> bptree;
    bptree.deserialize("./downloads/primary_index.bin");
    uint64_t offset = bptree.search(ID);

    printf("Bloco(%d)\n",(int) offset);
    blk = dbFile.read(offset);

    block_print(blk, ID);

    delete blk;

    return 0;
}
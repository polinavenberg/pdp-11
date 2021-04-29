#define pc reg[7]
#define MEMSIZE (64*1024)
typedef unsigned char byte; //8 bit
typedef unsigned short int word; //16 bit
typedef word adr; //16 bit

#define BYTE_SIZE 8
#define REG_SIZE 8

byte mem[MEMSIZE];
word reg[8];

typedef struct{
    word val;
    word adr;
} Args;

Args ss, dd, b_flag, r, nn, xx;

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*func)(void);
} Command;


void b_write(adr a, byte b);

byte b_read(adr a);

void w_write(adr a, word w);

word w_read(adr a);

void load_file();

void mem_dump(adr start, word n);

void trace(const char * fmt, ...);


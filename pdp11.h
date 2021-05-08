#define pc reg[7]
#define sp reg[6]
#define MEMSIZE (64*1024)
typedef unsigned char byte; //8 bit
typedef unsigned short int word; //16 bit
typedef word adr; //16 bit

#define BYTE_SIZE 8
#define REG_SIZE 8
#define ostat 0177564
#define odata 0177566
#define HAS_DD 1
#define HAS_SS 2
#define HAS_SS_DD 3
#define HAS_NOTHING 0


typedef struct{
    word val;
    word adr;
} Args;


typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*func)(void);
    word params;
} Command;


void b_write(adr a, byte b);

byte b_read(adr a);

void w_write(adr a, word w);

word w_read(adr a);

void load_file();

void mem_dump(adr start, word n);

void trace(const char * fmt, ...);

void print_reg();

void print_mem(word start_adr, word end_adr);

void run();


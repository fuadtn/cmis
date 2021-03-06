#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

using namespace std;

const int MAX_LEN = 1048576; // 1Мб

static FILE *i_file;
static FILE *k_file;
static FILE *o_file;
static FILE *p_file;

static int i_length;
static int k_length;
static int o_length;
static int p_length;

static char *i_message;
static char *k_message;
static char *o_message;
static char *p_message;

// vigeneer.cpp
int vigeneer(int argc, char argv[][128]);
int GetIndex(char x);
void CopyString(char s_key[], int original_length);
void v_encryption(char i_message[], char k_message[]);
void v_decryption(char i_message[], char k_message[]);

// aes.cpp
int aes(int argc, char argv[][128]);
int mix_columns(int inverse);
int sub_bytes(int inverse);
int sub_word(int index);
int shift_rows(int inverse);
int shift_word(int index);
int add_rkey(int inverse, int number);
int key_expansion();
int encryption();
int decryption();

// stg.cpp
int stg(int argc, char argv[][128]);

struct BITMAPFILEHEADER
{
	unsigned int    Type;
	unsigned long   Size;
	unsigned int    Reserved1;
	unsigned int    Reserved2;
	unsigned long   OffBits;
};

struct BITMAPINFOHEADER
{
	unsigned int    Size;
	int             Width;
	int             Height;
	unsigned short  Planes;
	unsigned short  BitCount;
	unsigned int    Compression;
	unsigned int    SizeImage;
	int             XPelsPerMeter;
	int             YPelsPerMeter;
	unsigned int    ClrUsed;
	unsigned int    ClrImportant;
};

struct RGBQUAD
{
	int   blue;
	int   green;
	int   red;
	int   reserved;
};

unsigned short read_u16(FILE *fp);
unsigned int read_u32(FILE *fp);
int read_s32(FILE *fp);

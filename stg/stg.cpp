#include <string.h>
#include <stdio.h>
#include <iostream>

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

using namespace std;

unsigned short read_u16(FILE *fp);
unsigned int read_u32(FILE *fp);
int read_s32(FILE *fp);

int main(int argc, char *argv[])
{
	FILE *p_file;
	FILE *i_file;	
	
	char *i_message;
	
	unsigned short usi_length;
	int z_count = -1;
	char m_symbol;
	char h_buffer[1078];

	if (strcmp(argv[1], "--help") == 0)
	{
		cout << "./stg [encryption key] [picture file] [input file]" << endl << endl;
		cout << "steganography.bmp (32 bits/pixel) | encryption key:" << endl;
		cout << "-e - encryption;" << endl;
		cout << "-d - decryption;" << endl;
		return 0;
	}
	if (argc < 4)
	{
		cout << "error: too few arguments" << endl;
		return -1;
	}

	p_file = fopen(argv[2], "rb");
	i_file = fopen(argv[3], "rb");
	if (p_file == NULL || (i_file == NULL && strcmp(argv[1], "-e") == 0))
	{
		cout << "error: can't open file" << endl;
		return -1;
	}
	
	BITMAPFILEHEADER bf_header;

	bf_header.Type = read_u16(p_file);
	bf_header.Size = read_u32(p_file);
	bf_header.Reserved1 = read_u16(p_file);
	bf_header.Reserved2 = read_u16(p_file);
	bf_header.OffBits = read_u32(p_file);

	BITMAPINFOHEADER bi_header;

	bi_header.Size = read_u32(p_file);
	bi_header.Width = read_s32(p_file);
	bi_header.Height = read_s32(p_file);
	bi_header.Planes = read_u16(p_file);
	bi_header.BitCount = read_u16(p_file);
	bi_header.Compression = read_u32(p_file);
	bi_header.SizeImage = read_u32(p_file);
	bi_header.XPelsPerMeter = read_s32(p_file);
	bi_header.YPelsPerMeter = read_s32(p_file);
	bi_header.ClrUsed = read_u32(p_file);
	bi_header.ClrImportant = read_u32(p_file);

	if (bi_header.BitCount != 32)
	{
		cout << "error: incorrect bitcount" << endl;
		return 0;
	}

	if (strcmp(argv[1], "-e") == 0)
	{
		fseek(p_file, 0, SEEK_SET);		
		for (int i = 0; i < bf_header.OffBits; i++)
		fread(&h_buffer[i], sizeof(char), 1, p_file);

		RGBQUAD *rgb = new RGBQUAD[bi_header.Width * bi_header.Height];

		fseek(p_file, bf_header.OffBits, SEEK_SET);

		for (int i = 0; i < bi_header.Width * bi_header.Height; i++)
		{
			rgb[i].blue = getc(p_file);
			rgb[i].green = getc(p_file);
			rgb[i].red = getc(p_file);
			rgb[i].reserved = getc(p_file);
		}

		while (!feof(p_file))
		{
			getc(p_file);
			z_count++;
		}

		p_file = fopen(argv[2], "wb");

		usi_length = 3 * bi_header.Width * bi_header.Height;
		i_message = new char[usi_length];

		fgets(i_message, usi_length - 2, i_file);
		usi_length = strlen(i_message);

		if (usi_length + 2 > bi_header.Width * bi_header.Height)
		{
			cout << "error: too long i_message" << endl;
			return 0;
		}

		for (int i = 0; i < bf_header.OffBits; i++)
		putc(h_buffer[i], p_file);

		for (int i = 0; i < bi_header.Width * bi_header.Height; i++)
		{
			putc(rgb[i].blue, p_file);
			putc(rgb[i].green, p_file);
			putc(rgb[i].red, p_file);

			if (i == 0)
				putc(usi_length, p_file);
			else if (i == 1)
				putc(usi_length >> 8, p_file);
			else if (i < usi_length + 2)
				putc(i_message[i - 2], p_file);
			else
				putc(0, p_file);
		}

		for (int i = 0; i < z_count; i++)
		putc(0, p_file);
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
		i_file = fopen(argv[3], "wb");
		fseek(p_file, bf_header.OffBits + 3, SEEK_SET);

		unsigned char b0 = getc(p_file);
		getc(p_file);
		getc(p_file);
		getc(p_file);
		unsigned char b1 = getc(p_file);

		usi_length = ((b1 << 8) | b0);

		for (int i = 0; i < usi_length; i++)
		{
			getc(p_file);
			getc(p_file);
			getc(p_file);
			m_symbol = getc(p_file);
			putc(m_symbol, i_file);
		}
	}

	fclose(p_file);
	fclose(i_file);
	return 0;
}

unsigned short read_u16(FILE *fp)
{
	unsigned char b0 = getc(fp);
	unsigned char b1 = getc(fp);

	return ((b1 << 8) | b0);
}

unsigned int read_u32(FILE *fp)
{
	unsigned char b0 = getc(fp);
	unsigned char b1 = getc(fp);
	unsigned char b2 = getc(fp);
	unsigned char b3 = getc(fp);

	return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

int read_s32(FILE *fp)
{
	unsigned char b0 = getc(fp);
	unsigned char b1 = getc(fp);
	unsigned char b2 = getc(fp);
	unsigned char b3 = getc(fp);

	return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

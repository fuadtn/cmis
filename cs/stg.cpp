#include "cs.h"

// [-e] [изображение] [оригинальный текст]
// [-d] [изображение] [зашифрованный текст]

int stg(int argc, char argv[][128])
{
	if (strcmp(argv[1], "--help") == 0)
	{
		cout << endl << "steganography.bmp (32 bits/pixel)" << endl;
		cout << "-e - encryption;" << endl;
		cout << "-d - decryption;" << endl;
		cout << "example: ./stg -e in.bmp message.txt" << endl << endl;
		return 0;
	}
	if (argc < 4)
	{
		cout << "error in stg: too few arguments" << endl;
		return -1;
	}

	int z_count = -1;
	unsigned short m_length;
	char m_symbol;
	char *message;
	char h_buffer[1078];

	FILE *pFile = fopen(argv[2], "rb");
	FILE *mFile = fopen(argv[3], "rb");
	if (pFile == NULL || (mFile == NULL && strcmp(argv[1], "-e") == 0))
	{
		cout << "error in stg: can't open file" << endl;
		return -1;
	}
	
	BITMAPFILEHEADER bf_header;

	bf_header.Type = read_u16(pFile);
	bf_header.Size = read_u32(pFile);
	bf_header.Reserved1 = read_u16(pFile);
	bf_header.Reserved2 = read_u16(pFile);
	bf_header.OffBits = read_u32(pFile);

	BITMAPINFOHEADER bi_header;

	bi_header.Size = read_u32(pFile);
	bi_header.Width = read_s32(pFile);
	bi_header.Height = read_s32(pFile);
	bi_header.Planes = read_u16(pFile);
	bi_header.BitCount = read_u16(pFile);
	bi_header.Compression = read_u32(pFile);
	bi_header.SizeImage = read_u32(pFile);
	bi_header.XPelsPerMeter = read_s32(pFile);
	bi_header.YPelsPerMeter = read_s32(pFile);
	bi_header.ClrUsed = read_u32(pFile);
	bi_header.ClrImportant = read_u32(pFile);

	if (bi_header.BitCount != 32)
	{
		cout << "error in stg: incorrect bitcount" << endl;
		return -1;
	}

	if (strcmp(argv[1], "-e") == 0)
	{
		fseek(pFile, 0, SEEK_SET);
		fgets(h_buffer, bf_header.OffBits, pFile);

		RGBQUAD *rgb = new RGBQUAD[bi_header.Width * bi_header.Height];

		fseek(pFile, bf_header.OffBits, SEEK_SET);

		for (int i = 0; i < bi_header.Width * bi_header.Height; i++)
		{
			rgb[i].blue = getc(pFile);
			rgb[i].green = getc(pFile);
			rgb[i].red = getc(pFile);
			rgb[i].reserved = getc(pFile);
		}

		while (!feof(pFile))
		{
			getc(pFile);
			z_count++;
		}

		pFile = fopen(argv[2], "wb");

		m_length = 3 * bi_header.Width * bi_header.Height;
		message = new char[m_length];

		fgets(message, m_length - 2, mFile);
		m_length = strlen(message);

		if (m_length + 2 > bi_header.Width * bi_header.Height)
		{
			cout << "error in stg: too long message" << endl;
			return -1;
		}

		for (int i = 0; i < bf_header.OffBits; i++)
		putc(h_buffer[i], pFile);

		for (int i = 0; i < bi_header.Width * bi_header.Height; i++)
		{
			putc(rgb[i].blue, pFile);
			putc(rgb[i].green, pFile);
			putc(rgb[i].red, pFile);

			if (i == 0)
				putc(m_length, pFile);
			else if (i == 1)
				putc(m_length >> 8, pFile);
			else if (i < m_length + 2)
				putc(message[i - 2], pFile);
			else
				putc(0, pFile);
		}

		for (int i = 0; i < z_count; i++)
		putc(0, pFile);
	}


	else if (strcmp(argv[1], "-d") == 0)
	{
		mFile = fopen(argv[3], "wb");
		fseek(pFile, bf_header.OffBits + 3, SEEK_SET);

		unsigned char b0 = getc(pFile);
		getc(pFile);
		getc(pFile);
		getc(pFile);
		unsigned char b1 = getc(pFile);

		m_length = ((b1 << 8) | b0);

		for (int i = 0; i < m_length; i++)
		{
			getc(pFile);
			getc(pFile);
			getc(pFile);
			m_symbol = getc(pFile);
			putc(m_symbol, mFile);
		}
	}

	fclose(pFile);
	fclose(mFile);
	return bf_header.Size;
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

#include "cs.h"

// [-e] [оригинальный текст] [ключ]
// [-d] [зашифрованный текст] [ключ]

int vigeneer(int argc, char argv[][128])
{
	ifstream f_read;
	ofstream f_write;
	
    char s_original[1024];
    char s_key[1024];
    char s_cipher[1024];

    for(int i = 0; i < 1024; i++)
	{
        s_original[i] = '\0';
        s_key[i] = '\0';
        s_cipher[i] = '\0';
	}
	
	if(argc < 4)
	{
		cout << "error: too few arguments" << endl;
		return -1;
	}
	

	if (argc > 1 && strcmp(argv[1], "-d") == 0)
	{
		f_read.open(argv[2], ios_base::binary);
		f_read.getline(s_cipher, 1024);
		f_read.close();
	
		f_read.open(argv[3], ios_base::binary);
		f_read.getline(s_key, 1024);
		f_read.close();
	
		f_Decryption(s_original, s_key, s_cipher);
		
		f_write.open(argv[2], ios_base::binary);
		for (int i = 0; i < strlen(s_cipher); i++)
		f_write << s_original[i];
		f_write.close();
	}
	else if (argc > 1 && strcmp(argv[1], "-e") == 0)
	{
		f_read.open(argv[2], ios_base::binary);
		f_read.getline(s_original, 1024);
		f_read.close();
	
		f_read.open(argv[3], ios_base::binary);
		f_read.getline(s_key, 1024);
		f_read.close();
	
		f_Encryption(s_original, s_key, s_cipher);
		
		f_write.open(argv[2], ios_base::binary);
		for (int i = 0; i < strlen(s_original); i++)
		f_write << s_cipher[i];
		f_write.close();
	}
	else
	{
		cout << "error: unknown argument" << endl;
		return -1;
	}
	
    return 0;
}

int GetIndex(char x)
{
    char s_alfabet[26];
    for(int i = 0; i < 26; i++)
    s_alfabet[i] = 97 + i;

    for(int i = 0; i < 26; i++)
    if(x == s_alfabet[i])
    return i;
    return -1;
}

void CopyString(char s_key[], int original_length)
{
    int i = 0;
    int j = strlen(s_key);

    while( j < original_length)
    {
        s_key[j] = s_key[i];
        i++;
        j++;
    }
}

void f_Encryption(char s_original[], char s_key[], char s_cipher[])
{
    CopyString(s_key, strlen(s_original));

    for(int i = 0; i < strlen(s_original); i++)
    s_cipher[i] = (GetIndex(s_original[i]) + GetIndex(s_key[i])) % 26 + 97;
}

void f_Decryption(char s_original[], char s_key[], char s_cipher[])
{
    CopyString(s_key, strlen(s_cipher));

    for(int i = 0; i < strlen(s_cipher); i++)
    s_original[i] = (GetIndex(s_cipher[i]) - GetIndex(s_key[i]) + 26) % 26 + 97;
}

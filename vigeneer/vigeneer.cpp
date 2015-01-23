#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

FILE *i_file;
FILE *k_file;
FILE *o_file;

int i_length;
int k_length;

char *i_message;
char *k_message;
    
int GetIndex(char x);
void CopyString(char k_message[], int original_length);

void v_encryption(char i_message[], char k_message[]);
void v_decryption(char i_message[], char k_message[]);

int main(int argc, char *argv[])
{
	if (argc == 2)
	if (strcmp(argv[1], "--help") == 0)
	{
		cout << endl;
		cout << "./vigeneer [encryption key] [input file] [key file] [out file]" << endl;
		cout << endl;		
		cout << " encryption key:" << endl;
		cout << "-e\t\tencryption;" << endl;
		cout << "-d\t\tdecryption;" << endl;
		cout << endl;
		return 0;
	}

	if(argc < 5)
	{
		cout << "error: too few arguments" << endl;
		return -1;
	}
	
	i_file = fopen(argv[2], "rb");
	if (i_file == NULL)
	{
		cout << "error: can't open input file" << endl;
		return 0;
	}
	k_file = fopen(argv[3], "rb");
	if (k_file == NULL)
	{
		cout << "error: can't open key file" << endl;
		return 0;
	}
	
	fseek(i_file, 0, SEEK_END);
	i_length = ftell(i_file);
	fseek(i_file, 0, SEEK_SET);
	
	fseek(k_file, 0, SEEK_END);
	k_length = ftell(k_file);
	fseek(k_file, 0, SEEK_SET);
	
	i_message = new char [i_length];
	k_message = new char [k_length];
	
	for (int i = 0; i < i_length; i++)
	fread(&i_message[i], sizeof(char), 1, i_file);
			
	for (int i = 0; i < k_length; i++)
	fread(&k_message[i], sizeof(char), 1, k_file);
	
	fclose(i_file);
	fclose(k_file);
	
	if (strcmp(argv[1], "-d") == 0)
	{
		v_decryption(i_message, k_message);
	}
	else if (strcmp(argv[1], "-e") == 0)
	{	
		v_encryption(i_message, k_message);
	}
	else
	{
		cout << "error: unknown argument" << endl;
		return -1;
	}
	
	o_file = fopen(argv[4], "wb");
	if (o_file == NULL)
	{
		cout << "error: can't write in / create file" << endl;
		return 0;
	}

	for (int i = 0; i < i_length; i++)
	fwrite(&i_message[i], sizeof(char), 1, i_file);
	fclose(i_file);
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

void CopyString(char k_message[], int original_length)
{
    int i = 0;
    int j = strlen(k_message);

    while (j < original_length)
    {
        k_message[j] = k_message[i];
        i++;
        j++;
    }
}

void v_encryption(char i_message[], char k_message[])
{
    CopyString(k_message, i_length);

    for(int i = 0; i < i_length; i++)
    i_message[i] = (GetIndex(i_message[i]) + GetIndex(k_message[i])) % 26 + 97;
}

void v_decryption(char i_message[], char k_message[])
{
    CopyString(k_message, i_length);

    for(int i = 0; i < i_length; i++)
    i_message[i] = (GetIndex(i_message[i]) - GetIndex(k_message[i]) + 26) % 26 + 97;
}

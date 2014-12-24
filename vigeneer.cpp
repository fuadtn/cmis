#include <iostream>
#include <string.h>

using namespace std;

int GetIndex(char x);
void CopyString(char s_key[], int original_length);
void f_Encryption(char s_original[], char s_key[], char s_cipher[]);
void f_Decryption(char s_original[], char s_key[], char s_cipher[]);

int main(int argc, char *argv[])
{
    char s_original[1024];
    char s_key[1024];
    char s_cipher[1024];

    for(int i = 0; i < 1024; i++)
	{
        s_original[i] = '\0';
        s_key[i] = '\0';
        s_cipher[i] = '\0';
	}

  if (argc > 1 && strcmp(argv[1], "-d") == 0)
    f_Decryption(s_original, s_key, s_cipher);
  else if (argc > 1 && strcmp(argv[1], "-e") == 0)
    f_Encryption(s_original, s_key, s_cipher);
  else
  {
    cout << "error: unknown argument" << endl;
    return 0;
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
    cout << "Enter the original text: ";
    cin  >> s_original;
    cout << "Enter the key: ";
    cin  >> s_key;

    CopyString(s_key, strlen(s_original));

    for(int i = 0; i < strlen(s_original); i++)
    s_cipher[i] = (GetIndex(s_original[i]) + GetIndex(s_key[i])) % 26 + 97;
    cout << endl << "Result: " << s_cipher << endl;
}

void f_Decryption(char s_original[], char s_key[], char s_cipher[])
{
    cout << "Enter the cipher: ";
    cin  >> s_cipher;
    cout << "Enter the key: ";
    cin  >> s_key;

    CopyString(s_key, strlen(s_cipher));

    for(int i = 0; i < strlen(s_cipher); i++)
    s_original[i] = (GetIndex(s_cipher[i]) - GetIndex(s_key[i]) + 26) % 26 + 97;
    cout << endl << "Result: " << s_original << endl;
}

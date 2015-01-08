#include "cs.h"

// -e: [оригинальный текст] [ключ] [изображение]

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addres;
	FILE *bFile = fopen(argv[3], "rb");
	char argn[5][128];
	char buffer[MAX_LEN];
	int s_handle;
	int report;
	int bmp_size;
	
	// соединение_________________________________
	
	s_addres.sin_family = AF_INET;
	s_addres.sin_port = htons(7500);
	s_addres.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	s_handle = socket(AF_INET, SOCK_STREAM, 0);
	if (s_handle < 0)
	{
		perror("socket error: ");
		return 0;
	}

	report = connect(s_handle, (struct sockaddr*) &s_addres, sizeof(s_addres));
	if (report < 0)
	{
		perror("connect error: ");
		return 0;
	}
	
	// ___________________________________________
	
	if (argc < 4)
	{
		cout << "error: too few arguments" << endl;
		return -1;
	}
	
	strcpy(argn[1], "-e");
	strcpy(argn[2], argv[1]);
	strcpy(argn[3], argv[2]);
	report = vigeneer(argc, argn);	
	if (report == -1)
	{
		cout << "error: vigeneer" << endl;
		return 0;
	}
	report = aes(argc, argn);
	if (report == -1)
	{
		cout << "error: aes" << endl;
		return 0;
	}
	
	strcpy(argn[2], argv[3]);
	strcpy(argn[3], argv[1]);
	bmp_size = stg(argc, argn);
	if (bmp_size == -1)
	{
		cout << "error: steganography" << endl;
		return 0;
	}
	

	for(int i = 0; i < bmp_size; i++)
	fread(&buffer[i], sizeof(char), 1, bFile);
	fclose(bFile);
	
	report = send(s_handle, buffer, bmp_size, 0);
	if (report != bmp_size)
	{
		perror("send error: ");
		cout << endl << report << endl;
		return 0;
	}
	
	shutdown(s_handle, 2);	
	return 0;
}


#include "cs.h"

// -e: [оригинальный текст] [ключ] [изображение]

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addres;	
	char argn[5][128];
	int s_handle;
	int report;

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
	
	p_length = stg(argc, argn);
	if (p_length == -1)
	{
		cout << "error: steganography" << endl;
		return 0;
	}
	
	p_file = fopen(argv[3], "rb");
	p_message = new char[p_length];

	for(int i = 0; i < p_length; i++)
	fread(&p_message[i], sizeof(char), 1, p_file);
	fclose(p_file);
	
	report = send(s_handle, p_message, p_length, 0);
	if (report != p_length)
	{
		perror("send error: ");
		cout << endl << report << endl;
		return 0;
	}
	
	shutdown(s_handle, 2);	
	return 0;
}

#include "cs.h"

// -d: [ключ] [выходной файл]

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addres;
	char argn[5][128];
	int s_handle;
	int n_handle;
	int report;
	
	// соединение_______________________________
	
	s_addres.sin_family = AF_INET;
	s_addres.sin_port = htons(7500);
	s_addres.sin_addr.s_addr = htonl(INADDR_ANY);
	
	s_handle = socket(AF_INET, SOCK_STREAM, 0);
	if (s_handle < 0)
	{
		perror("socket error: ");
		return 0;
	}
	
	report = bind(s_handle, (struct sockaddr *) &s_addres, sizeof(s_addres));
	if (report < 0)
	{
		perror("bind error: ");
		return 0;
	}

	report = listen(s_handle, 1);
	if (report < 0)
	{
		perror("listen error: ");
		return 0;
	}
	
	n_handle = accept(s_handle, NULL, NULL);
	if (n_handle < 0)
	{
		perror("accept error: ");
		return 0;
	}

	p_message = new char[MAX_LEN];
	p_length = recv(n_handle, p_message, MAX_LEN, 0);
	if (p_length < 0)
	{
		perror("recv error: ");
		return 0;
	}
	
	// ___________________________________________
	
	if (argc < 3)
	{
		cout << "error: too few arguments" << endl;
		return -1;
	}

	p_file = fopen("temp.bmp", "wb");
	
	for (int i = 0; i < p_length; i++)
	fwrite(&p_message[i], sizeof(char), 1, p_file);
	fclose(p_file);

	strcpy(argn[1], "-d");	
	strcpy(argn[2], "temp.bmp");
	strcpy(argn[3], argv[2]);
	report = stg(argc + 1, argn);
	if (report == -1)
	{
		cout << "error: steganography" << endl;
		return 0;
	}

	strcpy(argn[2], argv[2]);
	strcpy(argn[3], argv[1]);
	report = aes(argc + 1, argn);
	if (report == -1)
	{
		cout << "error: vigeneer" << endl;
		return 0;
	}
	report = vigeneer(argc + 1, argn);
	if (report == -1)
	{
		cout << "error: aes" << endl;
		return 0;
	}
	remove("temp.bmp");
	return 0;
}

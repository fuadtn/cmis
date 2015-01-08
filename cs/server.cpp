#include "cs.h"

// -d: [ключ] [выходной файл]

int main(int argc, char *argv[])
{
	struct sockaddr_in s_addres;
	FILE *bFile = fopen("temp.bmp", "wb");
	char argn[5][128];
	char buffer[MAX_LEN];
	int s_handle;
	int n_handle;
	int report;
	int bmp_size;
	
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

	bmp_size = recv(n_handle, buffer, MAX_LEN, 0);
	if (bmp_size < 0)
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
	
	for (int i = 0; i < bmp_size; i++)
	fwrite(&buffer[i], sizeof(char), 1, bFile);
	fclose(bFile);

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
	return 0;
}


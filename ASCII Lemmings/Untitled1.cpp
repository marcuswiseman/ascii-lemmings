/* MSNMessenger DPAPI
*
* tombkeeper[0x40]nsfocus[0x2e]com
* tombkeeper[0x40]xfocus[0x2e]net
* 2004.08.11
*/

#include <Windows.h>


#pragma comment(lib, "Advapi32.lib")

#define FCHK(a) if (!(a)) {printf(#a " failed\n"); return 0;}

typedef struct _CRYPTOAPI_BLOB {
DWORD cbData;
BYTE* pbData;
} DATA_BLOB;

typedef struct _CRYPTPROTECT_PROMPTSTRUCT {
DWORD cbSize;
DWORD dwPromptFlags;
HWND hwndApp;
LPCWSTR szPrompt;
} CRYPTPROTECT_PROMPTSTRUCT, *PCRYPTPROTECT_PROMPTSTRUCT;

typedef BOOL (WINAPI *PCryptUnprotectData)(
DATA_BLOB* pDataIn,
LPWSTR* ppszDataDescr,
DATA_BLOB* pOptionalEntropy,
PVOID pvReserved,
CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct,
DWORD dwFlags,
DATA_BLOB* pDataOut
);

PCryptUnprotectData CryptUnprotectData = NULL;


int main(void)
{
int ret;
HMODULE hNtdll;

HKEY hKey;
DWORD dwType;
char Data[0x100] = {0};
DWORD dwSize;

DATA_BLOB DataIn;
DATA_BLOB DataOut;

ret = RegOpenKeyEx
(
HKEY_CURRENT_USER,
"Software\\Microsoft\\MSNMessenger",
0,
KEY_READ,
&hKey
);
if( ret != ERROR_SUCCESS ) return 1;

ret = RegQueryValueEx
(
hKey,
"Password.NET Messenger Service",
NULL,
&dwType,
Data,
&dwSize
);
if( ret != ERROR_SUCCESS ) return 1;

FCHK ((hNtdll = LoadLibrary ("Crypt32.dll")) != NULL);
FCHK ((CryptUnprotectData = (PCryptUnprotectData)
GetProcAddress (hNtdll, "CryptUnprotectData")) != NULL);

DataIn.pbData = Data + 2; //
DataIn.cbData = dwSize-2;

CryptUnprotectData
(
&DataIn,
NULL,
NULL,
NULL,
NULL,
1,
&DataOut
);

base64_decode (DataOut.pbData, Data, strlen(DataOut.pbData));
printf ( "MSN Password: %s\n", Data);
return 0;
}

//copied from GNU libc - libc/resolv/base64.c
int base64_decode (char const *src, char *target, size_t targsize)
{
static const char Base64[] =

"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char Pad64 = '=';

int tarindex, state, ch;
char *pos;

state = 0;
tarindex = 0;

while ((ch = *src++) != '\0')
{
if (isspace (ch)) /* Skip whitespace anywhere. */
continue;

if (ch == Pad64)
break;

pos = strchr (Base64, ch);
if (pos == 0) /* A non-base64 character. */
return (-1);

switch (state)
{
case 0:
if (target)
{
if ((size_t) tarindex >= targsize)
return (-1);
target[tarindex] = (pos - Base64) << 2;
}
state = 1;
break;
case 1:
if (target)
{
if ((size_t) tarindex + 1 >= targsize)
return (-1);
target[tarindex] |= (pos - Base64) >> 4;
target[tarindex + 1] = ((pos - Base64) & 0x0f) << 4;
}
tarindex++;
state = 2;
break;
case 2:
if (target)
{
if ((size_t) tarindex + 1 >= targsize)
return (-1);
target[tarindex] |= (pos - Base64) >> 2;
target[tarindex + 1] = ((pos - Base64) & 0x03) << 6;
}
tarindex++;
state = 3;
break;
case 3:
if (target)
{
if ((size_t) tarindex >= targsize)
return (-1);
target[tarindex] |= (pos - Base64);
}
tarindex++;
state = 0;
break;
default:
abort ();
}
}

/*
* We are done decoding Base-64 chars. Let's see if we ended
* on a byte boundary, and/or with erroneous trailing characters.
*/

if (ch == Pad64)
{ /* We got a pad char. */
ch = *src++; /* Skip it, get next. */
switch (state)
{
case 0: /* Invalid = in first position */
case 1: /* Invalid = in second position */
return (-1);

case 2: /* Valid, means one byte of info */
/* Skip any number of spaces. */
for ((void) NULL; ch != '\0'; ch = *src++)
if (!isspace (ch))
break;
/* Make sure there is another trailing = sign. */
if (ch != Pad64)
return (-1);
ch = *src++; /* Skip the = */
/* Fall through to "single trailing =" case. */
/* FALLTHROUGH */

case 3: /* Valid, means two bytes of info */
/*
* We know this char is an =. Is there anything but
* whitespace after it?
*/
for ((void) NULL; ch != '\0'; ch = *src++)
if (!isspace (ch))
return (-1);

/*
* Now make sure for cases 2 and 3 that the "extra"
* bits that slopped past the last full byte were
* zeros. If we don't check them, they become a
* subliminal channel.
*/
if (target && target[tarindex] != 0)
return (-1);
}
}
else
{
/*
* We ended by seeing the end of the string. Make sure we
* have no partial bytes lying around.
*/
if (state != 0)
return (-1);
}

return (tarindex);
}

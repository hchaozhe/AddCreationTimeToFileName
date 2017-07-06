#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")

void DisplayErrorBox(LPTSTR lpszFunction);



int _tmain(int argc, TCHAR *argv[])
{
   WIN32_FIND_DATA ffd;
   LARGE_INTEGER filesize;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;
   FILETIME ftCreate, ftAccess, ftWrite;
   SYSTEMTIME stUTC, stLocal;
   // int  namechangeresult;
   TCHAR oldname[MAX_PATH] ;
   TCHAR newname[MAX_PATH] ;
   TCHAR fulloldname[MAX_PATH] ;
   TCHAR fullnewname[MAX_PATH] ;
   TCHAR newname_ext[MAX_PATH] ;
   int rename_result;
   int len_name;
   int len_ext;
   const char ch = '.';
   char *ext;
   // If the directory is not specified as a command-line argument,
   // print usage.

   if(argc != 2)
   {
      _tprintf(TEXT("\nUsage: %s <directory name>\n"), argv[0]);
      return (-1);
   }

   // Check that the input path plus 3 is not longer than MAX_PATH.
   // Three characters are for the "\*" plus NULL appended below.

   StringCchLength(argv[1], MAX_PATH, &length_of_arg);

   if (length_of_arg > (MAX_PATH - 3))
   {
      _tprintf(TEXT("\nDirectory path is too long.\n"));
      return (-1);
   }

   _tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

   // Prepare string for use with FindFile functions.  First, copy the
   // string to a buffer, then append '\*' to the directory name.

   StringCchCopy(szDir, MAX_PATH, argv[1]);
   StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

   // Find the first file in the directory.

   hFind = FindFirstFile(szDir, &ffd);

   if (INVALID_HANDLE_VALUE == hFind) 
   {
      DisplayErrorBox(TEXT("FindFirstFile"));
      return dwError;
   } 
   
   // List all the files in the directory with some info about them.

   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
         _tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
      }
      else
      {

    // Convert the last-write time to local time.
    ftCreate=ffd.ftCreationTime;
	ftWrite=ffd.ftLastWriteTime;
	ftAccess=ffd.ftLastAccessTime;
    // FileTimeToSystemTime(&ftCreate, &stUTC);
	FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.

         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
         _tprintf(TEXT(" %s Create Time %02d/%02d/%d  %02d:%02d:%02d \n"), 
	    ffd.cFileName,stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
      strcpy(oldname, ffd.cFileName);
	  ext = strchr(oldname,ch);
	  len_ext = strlen(ext);
	  len_name = strlen(oldname)-len_ext;
	  sprintf(newname_ext,"_%02d_%02d_%02d",stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

	  //printf(" %s %s %d %d \n",oldname,ext,len_name,len_ext);
	  memset(newname, '\0', sizeof(newname));
	  strncpy(newname,oldname,len_name);
	  strcat(newname,newname_ext);
	  strcat(newname,ext);
	  // need full path to rename files
	  strcpy(fulloldname,argv[1]);
	  strcat(fulloldname,"\\");
	  strcat(fulloldname,oldname);
	  strcpy(fullnewname,argv[1]);
	  strcat(fullnewname,"\\");
	  strcat(fullnewname,newname);
	  rename_result= rename( fulloldname , fullnewname );
	  // printf("%s %s",fullnewname,fulloldname);
	  printf("%s %d Rename %d \n", newname,strlen(newname),rename_result);
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);
 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES) 
   {
      DisplayErrorBox(TEXT("FindFirstFile"));
   }

   FindClose(hFind);
   return dwError;
}


void DisplayErrorBox(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message and clean up

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}


#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <stdio.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */



    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_INFORMATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_INFORMATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           WS_EX_ACCEPTFILES,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Enable MIDI Drums",       /* Title Text */
           WS_DLGFRAME | WS_SYSMENU, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           400,                 /* The programs width */
           200,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);
    
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


void reverseOrder(char* array, size_t len)
{
    size_t i;
    char tempByte;
    for (i = 0; i<len/2; i++)
    {
        tempByte = array[i];
        array[i] = array[len-i-1];
        array[len-i-1] = tempByte;
    }
}

void* findSubString(char* data, char* string, size_t numBytes)
{
    size_t i;
    for (i = 0; i<numBytes; i++)
    {
        char* ptr = 0;
        if (data[i]==string[0])
        {
            ptr = strstr(&data[i], string);
            if (ptr)
            {
                return ptr;
            }
        }
    }
    return 0;
}

#define BUFFERSIZE (256)
#define MIDI_TRACK_HEADER "MTrk"
#define MIDI_TRACK_HEADER_SIZE (8)

enum
{
  NO_FILE = 0,
  NOT_MIDI_FILE,
  EDIT_SUCCESS,   
};

const char sysExString[] = {0x00, 0xF0, 0x05, 0x7E, 0x7F, 0x09, 0x01, 0xF7};

int AddMIDIGMResetCode(char* filename)
{
   FILE* pFile;
   char* buffer;
   char* buffer2;
   char* ptr;
   long lSize;
   long* MIDITrackSize;
   char cMIDITrackSize[4];
   
   int ret = NO_FILE;
   
   pFile = fopen(filename, "r+b");
   if( pFile != NULL )
   {
       fseek (pFile , 0 , SEEK_END);
       lSize = ftell (pFile);
       rewind(pFile);
       
       buffer = (char*) malloc (sizeof(char)*(lSize+20));
       fread (buffer,1,lSize,pFile);
       
       ptr = (char*)findSubString(buffer, MIDI_TRACK_HEADER, lSize);
       //Copy the last part and move it over 8 bytes
       if( ptr )
       {
           rewind(pFile);
           memmove(ptr+8+8, ptr+8, lSize - (ptr-buffer) );
           memcpy(ptr+8, sysExString, sizeof(sysExString));
           
           memcpy(&cMIDITrackSize[0], &ptr[4], 4);
           reverseOrder(cMIDITrackSize, 4);
           MIDITrackSize = (long*)&cMIDITrackSize[0];
           *MIDITrackSize = *MIDITrackSize + 8;
           reverseOrder(cMIDITrackSize, 4);
           memcpy(&ptr[4], &cMIDITrackSize[0], 4);
           
           
           fwrite( buffer, lSize+8, 1, pFile );
           
           ret = EDIT_SUCCESS;
       }
       else
       {
           ret = NOT_MIDI_FILE;
       }    
       fclose(pFile);
       
   }
   return ret;
   
}


/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR mainMessage[] = _T("Drop MIDI files here");
    int ret;
    
    switch (message)                  /* handle the messages */
    {
           
        case WM_DROPFILES:
             {
             HDROP hDropInfo = (HDROP)wParam;
             char sItem[MAX_PATH];
             for(int i = 0; DragQueryFileA(hDropInfo, i, (LPSTR)sItem, sizeof(sItem)); i++)
             {                  
                 char outputString[MAX_PATH+30];                 
                 ret = AddMIDIGMResetCode(sItem);
                 
                 if( ret == EDIT_SUCCESS)
                 {
                     
                     sprintf(outputString, "Successfully converted:\n %s", sItem);
                     MessageBox(hwnd, outputString, "Success!", MB_OK | MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
                 }
                 
                 if( ret != EDIT_SUCCESS)
                 {
                     sprintf(outputString, "Error: File not found or a valid MIDI file\n(%s)", sItem);
                     MessageBox(hwnd, sItem, "Error!", MB_OK | MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND);
                 }
                 
             }
             }
             EndPaint(hwnd, &ps);
             break;
                
           
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);
            // Here your application is laid out.
            // For this introduction, we just print out "Hello, World!"
            // in the top left corner.
            TextOut(hdc, 5, 5, mainMessage, _tcslen(mainMessage));
            // End application-specific layout section.
            EndPaint(hwnd, &ps);
            break;           
           

            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

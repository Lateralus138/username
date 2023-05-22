//////////////////////////////////////////////////
// 'User Name' - Get the current user's name.   //
// � 2023 Ian Pride - New Pride Software /      //
// Services                                     //
//////////////////////////////////////////////////
#include <Windows.h>
#include <iostream>
#include "Globals.h"
#include <Lmcons.h>
#include "resource.h"
#include "UserNameRegex.h"
#include "WStringCase.h"
#include <chrono>
#include <thread>
int main(int argc, const char* argv[])
{
  const HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  SetConsoleMode(hInput, ENABLE_VIRTUAL_TERMINAL_INPUT);
  const HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleMode(hOutput, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleTitleW(L"User Name");
  std::this_thread::sleep_for(std::chrono::milliseconds(40));
  const HINSTANCE hInstance = GetModuleHandleW(NULL);
  const HICON hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_ICON2));
  if (hIcon)
  {
    const HWND HWND_ = GetConsoleWindow();
    if (HWND_ != NULL)
    {
      SendMessageW(HWND_, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
  }
  wchar_t userNameBuffer[UNLEN] = { 0 };
  DWORD userNameBufferSize = UNLEN;
  unsigned int caseMode = 0;
  std::wstring username;
  if (argc > 1)
  {
    const std::vector<std::string> args(argv + 1, argv + argc);
    for (const std::string arg : args)
    {
      if (std::regex_match(arg, UserNameRegex::HELP))
      {
        const std::wstring HELPMESSAGE =
          L"\n"
          "User Name - Get the current user's user name with\n"
          "            case options.\n"
          "\n"
          "username  [/? | /h | /help] | [/l | /lower] | [/u | /upper]\n"
          "          [/t | /title]\n"
          "\n"
          "  /?, /h, /help   This help screen.\n"
          "  /l, /lower      Output is in lower case.\n"
          "  /u, /upper      Output is in upper case.\n"
          "  /t, /title      Output is in title case.\n"
          "\n";
        std::wcout << HELPMESSAGE;
        WaitCheck();
        return EXIT_SUCCESS;
      }
      if (std::regex_match(arg, UserNameRegex::LOWER))
      {
        caseMode = 1;
        continue;
      }
      if (std::regex_match(arg, UserNameRegex::UPPER))
      {
        caseMode = 2;
        continue;
      }
      if (std::regex_match(arg, UserNameRegex::TITLE))
      {
        caseMode = 3;
        continue;
      }
    }
  }
  const BOOL USERNAME_OBTAINED =
    GetUserNameW(userNameBuffer, &userNameBufferSize);
  if (!USERNAME_OBTAINED)
  {
    std::wcerr << "The user name could not be retrieved.\n";
    WaitCheck();
    return EXIT_FAILURE;
  }
  WStringCase wstringCase(userNameBuffer);
  switch (caseMode)
  {
  case 1:
    username = wstringCase.lower();
    break;
  case 2:
    username = wstringCase.upper();
    break;
  case 3:
    username = wstringCase.title();
    break;
  default:
    username = wstringCase.getString();
  }
  std::wcout << username << '\n';
  WaitCheck();
  return EXIT_SUCCESS;
}
#include <windows.h>

char *wc2c(const wchar_t *src)
{
  int length = WideCharToMultiByte(CP_UTF8, 0, src, -1, 0, 0, NULL, NULL);
  char *out = (char*)malloc((length+1) * sizeof(char));
  WideCharToMultiByte(CP_UTF8, 0, src, -1, out, length, NULL, NULL);
  return out;
}

wchar_t *c2wc(const char *src)
{
  int length = MultiByteToWideChar(CP_UTF8, 0, src, -1, 0, 0);
  wchar_t *out = (wchar_t*)malloc((length+1) * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, src, -1, out, length);
  return out;
}

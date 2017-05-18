#if !defined(_MY_MACRO_H_INCLUDED_)
#define _MY_MACRO_H_INCLUDED_

#define _IsKeyDown(exp) (GetAsyncKeyState(exp) & 0xff00)
#define _IsKeyDowned(exp) (GetAsyncKeyState(exp) & 0x00ff)

#endif // !defined(_MY_MACRO_H_INCLUDED_)

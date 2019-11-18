#pragma once
#ifdef _DEBUG

#include <Windows.h>
#include <memory>

#define AST(){\
	CHAR ast_mes[256];\
	wsprintf(ast_mes,"%s %dçsñ⁄\0",__FILE__,__LINE__);\
	MessageBox(0,ast_mes,"±ª∞ƒï\é¶", MB_OK);\
	}

#define TRACE(fmt, ...) printf(fmt, __VA_ARGS__);

class _DebugConOut
{
public:
	static _DebugConOut& GetInstance(void)
	{
		return *sInstance;
	}

private:
	struct _DebugDeleter
	{
		void operator ()(_DebugConOut* _debug)const
		{
			delete _debug;
		}
	};

	_DebugConOut();
	~_DebugConOut();

	static std::unique_ptr<_DebugConOut,_DebugDeleter> sInstance;

	FILE* _debugFp = nullptr;
};

#else
#define AST()
#define TRACE(fmt, ...)
#endif // _DEBUG
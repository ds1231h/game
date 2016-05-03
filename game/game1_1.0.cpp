/*******************************************************************
程序：Type Game
文件：game.cpp
功能：Type Game
********************************************************************/
#include <windows.h>
#include <windowsx.h>
#include <time.h>

#define strNum 16
TCHAR getChar[strNum];
int score = 0;
BOOL gbInGame = FALSE;

// 函数声明
BOOL InitWindow (HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void create_word();// 使用随机数生成字符
void goal(TCHAR* tword, TCHAR inputWord);// 输入生成字符与键盘输入字符，判断是否相等

/*******************************************************************
函数：WinMain ()
功能：Win32 应用程序入口函数。创建主窗口，处理消息循环
********************************************************************/
int WINAPI WinMain (HINSTANCE hInstance,         //当前实例句柄
	HINSTANCE hPrevInstance,     //前一个实例句柄
	PSTR szCmdLine,              //命令行字符
	int iCmdShow)                //窗口显示方式
{
	MSG msg;

	// 创建主窗口
	if (!InitWindow (hInstance, iCmdShow))
		return FALSE;

	// 进入消息循环：从该应用程序的消息队列中检取消息，
	// 送到消息处理过程，当检取到WM_QUIT消息时，退出消息循环。
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return msg.wParam;
}

/******************************************************************
函数：InitWindow ()
功能：创建窗口。
*******************************************************************/
BOOL InitWindow (HINSTANCE hInstance, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("EasyWin");  //应用程序名称
	HWND hwnd;                                    //窗口句柄
	WNDCLASS wcMainWnd;                           //窗口类结构
	ATOM a;

	// 填充窗口类结构
	wcMainWnd.style = CS_VREDRAW | CS_HREDRAW;
	wcMainWnd.lpfnWndProc = WinProc;
	wcMainWnd.cbClsExtra = 0;
	wcMainWnd.cbWndExtra = 0;
	wcMainWnd.hInstance = hInstance;
	wcMainWnd.hIcon = LoadIcon (hInstance, IDI_APPLICATION);
	wcMainWnd.hCursor = LoadCursor (NULL, IDC_ARROW);
	wcMainWnd.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
	wcMainWnd.lpszMenuName = NULL;
	wcMainWnd.lpszClassName = szAppName;

	// 注册窗口类
	a = RegisterClass (&wcMainWnd);

	if (!a)
	{
		MessageBox (NULL, TEXT ("注册窗口类失败！"), szAppName,
			MB_ICONERROR);
		return 0;
	}

	// 创建主窗口
	hwnd = CreateWindow (szAppName, 						//窗口类名称
		TEXT ("Windows 应用程序基本框架"),  //窗口标题
		WS_OVERLAPPEDWINDOW,				//窗口风格
		100,								//窗口位置的x坐标
		100,								//窗口位置的y坐标
		400,								//窗口的宽度
		300,								//窗口的高度
		NULL,								//父窗口句柄
		NULL,								//菜单句柄
		hInstance,							//应用程序实例句柄
		NULL);								//窗口创建数据指针

	if( !hwnd ) return FALSE;

	// 显示并更新窗口
	ShowWindow( hwnd, iCmdShow );
	UpdateWindow( hwnd );
	return TRUE;
}

/******************************************************************
create_word  TCHAR getChar[strNum]为全局变量
*******************************************************************/
void create_word()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < strNum-1; i++)
	{
		int j = rand()%3;
		if (j)
		{
			getChar[i] = 'A' + rand()%('Z' - 'A' + 1) + (j - 1)*32;
		} 
		else
		{
			getChar[i] = '0' + rand()%('9'-'0'+1);
		}
		
	}
}


/******************************************************************
判断是否加分（正确才加分）  int score为全局变量
*******************************************************************/
void goal(TCHAR* tword, TCHAR inputWord)
{
	if (tword[0] == inputWord)
	{
		score++;
	}
	// Moves one buffer to another.
	// 这里的sizeof(TCHAR)*(strNum - 1)是：
	// TCHAR的大小×15
	memmove(tword, tword + 1, sizeof(TCHAR)*(strNum - 1));
}

/******************************************************************
函数：WinProc ()
功能：处理主窗口消息
*******************************************************************/
LRESULT CALLBACK WinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	TCHAR		aChar ,scoreString[20];

	switch (message)
	{
	case WM_CREATE:         // 创建消息
		if (gbInGame == TRUE)
		{
			create_word();
		}
		return 0;
	case WM_KEYDOWN:         // 击键消息
		switch (wParam)
		{
			if (gbInGame == FALSE)
			{
				case VK_RETURN:
					gbInGame = TRUE;
					break;
			}
		case VK_ESCAPE:
			MessageBox (hWnd, TEXT ("退出游戏!"), TEXT ("ESC"), MB_OK);
			PostQuitMessage(0);
			break;
		}
		return 0;

	case WM_CHAR:
		if (gbInGame == TRUE)
		{
			aChar = wParam;
			if (getChar[0] == 0)
			{
				if (LOWORD(wParam) == VK_RETURN)// 每一轮字符串打完后要按回车才继续进行游戏
				{
					create_word();
				}
			}
			goal(getChar, aChar);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;

		case WM_PAINT:              // 客户区重绘消息
		// 取得设备环境句柄
		hdc = BeginPaint (hWnd, &ps); 
		// 取得窗口客户区矩形
		GetClientRect (hWnd, &rect);
		if (gbInGame == FALSE)
		{
			// 输出文字
			DrawText (hdc, TEXT("按 ENTER（回车） 键开始游戏"),
				-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			rect.top = rect.bottom - 150;
		} 
		else
		{
			// 输出文字
			DrawText (hdc, getChar,
				-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			rect.top = rect.bottom - 150;
			// The wsprintf function formats and stores a series of characters 
			// and values in a buffer. 
			wsprintf(scoreString, TEXT("得分：%d"), score);
			DrawText (hdc, scoreString,
				-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			if (getChar[0] == 0){
				rect.top = rect.bottom - 200;
				DrawText(hdc, TEXT("按 ENTER（回车） 键继续"),
					-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			// 释放资源
			EndPaint (hWnd, &ps);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	// 调用缺省消息处理过程
	return DefWindowProc (hWnd, message, wParam, lParam);

} // 函数 WinProc 结束


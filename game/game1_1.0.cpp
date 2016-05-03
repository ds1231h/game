/*******************************************************************
����Type Game
�ļ���game.cpp
���ܣ�Type Game
********************************************************************/
#include <windows.h>
#include <windowsx.h>
#include <time.h>

#define strNum 16
TCHAR getChar[strNum];
int score = 0;
BOOL gbInGame = FALSE;

// ��������
BOOL InitWindow (HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void create_word();// ʹ������������ַ�
void goal(TCHAR* tword, TCHAR inputWord);// ���������ַ�����������ַ����ж��Ƿ����

/*******************************************************************
������WinMain ()
���ܣ�Win32 Ӧ�ó�����ں��������������ڣ�������Ϣѭ��
********************************************************************/
int WINAPI WinMain (HINSTANCE hInstance,         //��ǰʵ�����
	HINSTANCE hPrevInstance,     //ǰһ��ʵ�����
	PSTR szCmdLine,              //�������ַ�
	int iCmdShow)                //������ʾ��ʽ
{
	MSG msg;

	// ����������
	if (!InitWindow (hInstance, iCmdShow))
		return FALSE;

	// ������Ϣѭ�����Ӹ�Ӧ�ó������Ϣ�����м�ȡ��Ϣ��
	// �͵���Ϣ������̣�����ȡ��WM_QUIT��Ϣʱ���˳���Ϣѭ����
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return msg.wParam;
}

/******************************************************************
������InitWindow ()
���ܣ��������ڡ�
*******************************************************************/
BOOL InitWindow (HINSTANCE hInstance, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("EasyWin");  //Ӧ�ó�������
	HWND hwnd;                                    //���ھ��
	WNDCLASS wcMainWnd;                           //������ṹ
	ATOM a;

	// ��䴰����ṹ
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

	// ע�ᴰ����
	a = RegisterClass (&wcMainWnd);

	if (!a)
	{
		MessageBox (NULL, TEXT ("ע�ᴰ����ʧ�ܣ�"), szAppName,
			MB_ICONERROR);
		return 0;
	}

	// ����������
	hwnd = CreateWindow (szAppName, 						//����������
		TEXT ("Windows Ӧ�ó���������"),  //���ڱ���
		WS_OVERLAPPEDWINDOW,				//���ڷ��
		100,								//����λ�õ�x����
		100,								//����λ�õ�y����
		400,								//���ڵĿ��
		300,								//���ڵĸ߶�
		NULL,								//�����ھ��
		NULL,								//�˵����
		hInstance,							//Ӧ�ó���ʵ�����
		NULL);								//���ڴ�������ָ��

	if( !hwnd ) return FALSE;

	// ��ʾ�����´���
	ShowWindow( hwnd, iCmdShow );
	UpdateWindow( hwnd );
	return TRUE;
}

/******************************************************************
create_word  TCHAR getChar[strNum]Ϊȫ�ֱ���
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
�ж��Ƿ�ӷ֣���ȷ�żӷ֣�  int scoreΪȫ�ֱ���
*******************************************************************/
void goal(TCHAR* tword, TCHAR inputWord)
{
	if (tword[0] == inputWord)
	{
		score++;
	}
	// Moves one buffer to another.
	// �����sizeof(TCHAR)*(strNum - 1)�ǣ�
	// TCHAR�Ĵ�С��15
	memmove(tword, tword + 1, sizeof(TCHAR)*(strNum - 1));
}

/******************************************************************
������WinProc ()
���ܣ�������������Ϣ
*******************************************************************/
LRESULT CALLBACK WinProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	TCHAR		aChar ,scoreString[20];

	switch (message)
	{
	case WM_CREATE:         // ������Ϣ
		if (gbInGame == TRUE)
		{
			create_word();
		}
		return 0;
	case WM_KEYDOWN:         // ������Ϣ
		switch (wParam)
		{
			if (gbInGame == FALSE)
			{
				case VK_RETURN:
					gbInGame = TRUE;
					break;
			}
		case VK_ESCAPE:
			MessageBox (hWnd, TEXT ("�˳���Ϸ!"), TEXT ("ESC"), MB_OK);
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
				if (LOWORD(wParam) == VK_RETURN)// ÿһ���ַ��������Ҫ���س��ż���������Ϸ
				{
					create_word();
				}
			}
			goal(getChar, aChar);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;

		case WM_PAINT:              // �ͻ����ػ���Ϣ
		// ȡ���豸�������
		hdc = BeginPaint (hWnd, &ps); 
		// ȡ�ô��ڿͻ�������
		GetClientRect (hWnd, &rect);
		if (gbInGame == FALSE)
		{
			// �������
			DrawText (hdc, TEXT("�� ENTER���س��� ����ʼ��Ϸ"),
				-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			rect.top = rect.bottom - 150;
		} 
		else
		{
			// �������
			DrawText (hdc, getChar,
				-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			rect.top = rect.bottom - 150;
			// The wsprintf function formats and stores a series of characters 
			// and values in a buffer. 
			wsprintf(scoreString, TEXT("�÷֣�%d"), score);
			DrawText (hdc, scoreString,
				-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			if (getChar[0] == 0){
				rect.top = rect.bottom - 200;
				DrawText(hdc, TEXT("�� ENTER���س��� ������"),
					-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			// �ͷ���Դ
			EndPaint (hWnd, &ps);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	// ����ȱʡ��Ϣ�������
	return DefWindowProc (hWnd, message, wParam, lParam);

} // ���� WinProc ����


// dashmp4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "dashmp4.h"
#include "dashmp4Dlg.h"

#include "h264frame.h"

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
SOCKET g_socket = INVALID_SOCKET;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDashmp4Dlg dialog

CDashmp4Dlg::CDashmp4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDashmp4Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDashmp4Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDashmp4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDashmp4Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDashmp4Dlg, CDialog)
	//{{AFX_MSG_MAP(CDashmp4Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDOK3, OnOk3)
	ON_BN_CLICKED(IDOK4, OnOk4)
	ON_BN_CLICKED(IDOK5, OnOk5)
	ON_BN_CLICKED(IDOK6, OnOk6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDashmp4Dlg message handlers

BOOL CDashmp4Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	{
		
		WORD wVersionRequested;  
		WSADATA wsaData;  
		int ret;  		
		BOOL fSuccess = TRUE;  
		
		//WinSock初始化  
		wVersionRequested = MAKEWORD(2, 2); //希望使用的WinSock DLL的版本  
		ret = WSAStartup(wVersionRequested, &wsaData);  //加载套接字库  
		if(ret!=0)  
		{  
			AfxMessageBox("WSAStartup() failed!\n");  
			//return 0;  
		}  
		//确认WinSock DLL支持版本2.2  
		if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)  
		{  
			WSACleanup();   //释放为该程序分配的资源，终止对winsock动态库的使用  
			AfxMessageBox("Invalid WinSock version!\n");  
			//return 0;  
		}  
		
		
		
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDashmp4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDashmp4Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDashmp4Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}





void CDashmp4Dlg::OnOK() 
{		
	SOCKET sClient; //连接套接字  
	struct sockaddr_in saServer; //服务器地址信息  		
	//创建Socket,使用TCP协议  
	g_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if (g_socket == INVALID_SOCKET)  
	{  
		WSACleanup();  
		AfxMessageBox("socket() failed!\n");  
		//return 0;  
	}  
	
	//构建服务器地址信息  
	saServer.sin_family = AF_INET; //地址家族  
	saServer.sin_port = htons(1984); //注意转化为网络节序  
	saServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  
	//saServer.sin_addr.S_un.S_addr = inet_addr("114.215.175.1");
	//连接服务器  
	int ret = connect(g_socket, (struct sockaddr *)&saServer, sizeof(saServer));  
	if (ret == SOCKET_ERROR)  
	{  
		AfxMessageBox("connect() failed!\n");  
		closesocket(g_socket); //关闭套接字  
		WSACleanup();  
		//return 0;  
	}  
	
	/*FILE* pFiletest = fopen("d:/dest.flv", "rb");
	if (!pFiletest)
	{
		AfxMessageBox("read file error");
		return;
	}
	
	int dwFlvLen = 30*1024;
	char* pBuffer = new char[dwFlvLen];
	while (1)
	{		
		while (!feof(pFiletest)) 
		{		
			int nRet = fread(pBuffer, 1, dwFlvLen, pFiletest);
			if (nRet != dwFlvLen)
			{
				break;
			}			
			::send(g_socket, (const char*)(&nRet), 4, 0);
			::send(g_socket, (const char*)pBuffer, nRet, 0);	
			Sleep(30);
		}
		fseek(pFiletest, 0, SEEK_SET);
		continue;
	}
	fclose(pFiletest);
	return;*/
	if (g_socket != INVALID_SOCKET)
	{
		CString strdeviceid = "123abcdef32153421";
		int nLen = strdeviceid.GetLength();

		::send(g_socket, (const char*)&nLen, 4, 0);
		::send(g_socket, (LPCTSTR)strdeviceid, nLen, 0);	
	}


	//FILE* pFile = fopen("D:\\dashmp4-master\\dashmp4demo\\Debug\\test.h264", "rb");
	FILE* pFile = fopen("c:\\dest.h264", "rb");
	if (!pFile)
	{
		AfxMessageBox("read file error");
		return;
	}

	

	
	u8* buffer = new u8[1*1024*1024];
	u8* dstflvbuffer = new u8[1*1024*1024];
	u8* flvheader = new u8[1*1024*1024];
	u32 dwTime = 0;
	u32 dwLen = 0;	
	
	CFlv flv;

	while (1)
	{
		
		while (1)
		{
			//Sleep(30);
			int nRet = fread(&dwLen, 1, 4, pFile);
			if (4 != nRet) 
			{
				break;
			}
			
			nRet = fread(buffer, 1, dwLen, pFile);
			if (nRet != dwLen)
			{
				break;	
			}
			Sleep(30);
			
			Buffer bufH264;
			bufH264.pBuffer = buffer;
			bufH264.dwBufLen = dwLen;
			Buffer bufFlvheader;
			bufFlvheader.pBuffer = flvheader;
			bufFlvheader.dwBufLen = 1*1024*1024;
			
			Buffer bufFlvData;
			bufFlvData.pBuffer = dstflvbuffer;
			bufFlvData.dwBufLen = 1*1024*1024;
			u32 dwRet = flv.ConvertH264ToFlv(&bufH264, 0, &bufFlvData, &bufFlvheader);
			if (dwRet | HAS_FLVHEADER)
			{
				::send(g_socket, (const char*)(&bufFlvheader.dwBufLen), 4, 0);
				::send(g_socket, (const char*)bufFlvheader.pBuffer, bufFlvheader.dwBufLen, 0);	
			}
			if (dwRet | HAS_FLVFRAMEDATA)
			{
				::send(g_socket, (const char*)(&bufFlvData.dwBufLen), 4, 0);
				::send(g_socket, (const char*)bufFlvData.pBuffer, bufFlvData.dwBufLen, 0);	
			}			
				
		}
		fseek(pFile, 0, SEEK_SET);
	}
	
	fclose(pFile);		
}

void CDashmp4Dlg::OnOk2() 
{
	u8* buffer = new u8[12*1024*1024];
	memset(buffer, 0, 12*1024*1024);
	FILE* pFile = fopen("c:\\1920_1080_h264.dat", "rb");
	int nRet = fread(buffer, 1, 12*1024*1024, pFile);
	fclose(pFile);

	FILE* pDestFile = fopen("c:\\dest.h264", "wb");
	NalUnitBuf nal;
	H264Frame frame(buffer, nRet, 0);
	int lastindex = 0;
	for(int inx = 0; inx < frame.vecNals.size(); inx++)
	{				
		if ((frame.vecNals[inx].pBufStart[0] & 0x1f) == 6) 
		{
			if (lastindex < inx)
			{
				u32 dwLen = (inx - lastindex)*4;
				for(int nalindex = lastindex; nalindex < inx; nalindex++)
				{
					dwLen += frame.vecNals[nalindex].dwNalLen;
				}
				fwrite(&dwLen, 1, 4, pDestFile);
				for(int nalbufindex = lastindex; nalbufindex < inx; nalbufindex++)
				{
					byte nalheader[4] = {0, 0, 0, 1};
					fwrite(nalheader, 1, 4, pDestFile);
					fwrite(frame.vecNals[nalbufindex].pBufStart, 1, frame.vecNals[nalbufindex].dwNalLen, pDestFile);
				}
				lastindex = inx+1;
			}
		}
	}
	fclose(pDestFile);
	

}



#include <time.h>

void getlocaltime(char*  localtm)
{
				time_t now; //实例化time_t结构
				struct tm *timenow; //实例化tm结构指针
				time(&now);     //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
				
				timenow = gmtime(&now);				
				
				char* daytime;
				daytime = asctime(timenow);
				
				printf("%s", daytime);
				char *pch, *week, *mon, *day, *time, *year;
				pch = strtok(daytime, " ");
				int i = 0;
				while(pch != NULL)
				{
					printf("%s", pch);
						switch(i)
					{
				case 0: week = pch; break;
				case 1: mon = pch; break;
				case 2: day = pch; break;
				case 3: time = pch; break;
				case 4: year = pch; break;
				default: break;
					}
					pch = strtok(NULL, " ");
					i++;    
				}
				
				char realyear[5]={0};
				strncpy(realyear, year, 4);     				
				sprintf(localtm, "Date: %s, %s %s %s %s GMT", week, day, mon, realyear, time);
					//return localtime;
}
void CDashmp4Dlg::OnOk3() 
{
	SOCKET sServer; //连接套接字  
	struct sockaddr_in saServer; //服务器地址信息  		
	//创建Socket,使用TCP协议  
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if (sServer== INVALID_SOCKET)  
	{  
		WSACleanup();  
		AfxMessageBox("socket() failed!\n");  
		//return 0;  
	}  
	
	//构建服务器地址信息  
	saServer.sin_family = AF_INET; //地址家族  
	saServer.sin_port = htons(1984); //注意转化为网络节序  
	saServer.sin_addr.s_addr = inet_addr("0.0.0.0");  	
	int nret = ::bind(sServer, (sockaddr*)&saServer, sizeof(saServer));
	nret = ::listen(sServer, 5);
	while (1)
	{
		SOCKET sClient = ::accept(sServer, NULL, NULL);		
		char bufrequest[20480] = {0};
		
		/*char szlocaltime[100];
		getlocaltime(szlocaltime);*/
		
		bool bfind = false;
		int nRead = 0;
		while (1) 
		{
			int nRet = recv(sClient, &bufrequest[nRead], 20480-nRead, 0);
			if (nRet <= 0)
			{
				break;
			}
			nRead += nRet;
			if (strstr(bufrequest, "\r\n\r\n"))
			{
				bfind = true;
				break;
			}
		}
		if (false == bfind)
		{
			AfxMessageBox("no header");
			continue;
		}
		
		
		int x = sClient;
		CString strheader = 		
			"HTTP/1.1 200 OK\r\n"
			"Connection: close\r\n"
			"Content-Type: video/x-flv\r\n"
			"Transfer-Encoding: chunked\r\n"
			"Access-Control-Allow-Origin: *\r\n";
		//strheader += szlocaltime;
		//strheader += "\r\n\r\n";
		strheader += "\r\n";
		
		
		
		int nRet = send(sClient, (LPCSTR)strheader, strheader.GetLength(), 0);
		
		if (0)
		{
			FILE* pFile = fopen("D:\\nginx-1.10.3\\html\\flv\\video3.flv", "rb");
			char buffer[50*1024];
			while (!feof(pFile)) 
			{
				int nRead = fread(buffer, 1, 10*1024, pFile);
				if (nRead > 0) 
				{
					CString strLen;
					strLen.Format("%x\r\n", nRead);
					int nerr = send(sClient, (LPCSTR)strLen, strLen.GetLength(), 0);
					
					nerr = send(sClient, buffer, nRead, 0);
					if (nerr == SOCKET_ERROR)
					{
						CString strInfo;
						strInfo.Format("socket,error %d", ::WSAGetLastError());
						//AfxMessageBox(strInfo);
						break;
					}
					nerr = send(sClient, "\r\n", 2, 0);
				}
				else
				{
					break;
				}
				Sleep(20);
			}
			fclose(pFile);
		}
		else
		{
			
			
			FILE* pFile = fopen("c:\\test.h264", "rb");
			//FILE* pFile = fopen("c:\\seeyouagain.h264", "rb");
			FILE* pFiledest = fopen("c:\\dest.flv", "wb");
			if (!pFile)
			{
				AfxMessageBox("read file error");
				return;
			}						
			
			
			u8* buffer = new u8[1*1024*1024];
			u8* dstflvbuffer = new u8[1*1024*1024];
			u8* flvheader = new u8[1*1024*1024];
			u32 dwTime = 0;
			u32 dwLen = 0;	
			
			CFlv flv;
			
			
			while (!feof(pFile))
			{
				//Sleep(30);
				int nRet = fread(&dwLen, 1, 4, pFile);
				if (4 != nRet)
				{
					break;
				}
				
				nRet = fread(buffer, 1, dwLen, pFile);
				if (nRet != dwLen)
				{
					break;	
				}
				Sleep(30);
				
				Buffer bufH264;
				bufH264.pBuffer = buffer;
				bufH264.dwBufLen = dwLen;
				Buffer bufFlvheader;
				bufFlvheader.pBuffer = flvheader;
				bufFlvheader.dwBufLen = 1*1024*1024;
				
				Buffer bufFlvData;
				bufFlvData.pBuffer = dstflvbuffer;
				bufFlvData.dwBufLen = 1*1024*1024;
				u32 dwRet = flv.ConvertH264ToFlv(&bufH264, dwTime, &bufFlvData, &bufFlvheader, 1);
				if ((dwRet & HAS_FLVHEADER) == HAS_FLVHEADER)
				{
					fwrite(bufFlvheader.pBuffer, 1, bufFlvheader.dwBufLen, pFiledest);
					
					CString strLen;
					strLen.Format("%x\r\n", bufFlvheader.dwBufLen);
					int nerr = send(sClient, (LPCSTR)strLen, strLen.GetLength(), 0);
					
					nerr = ::send(sClient, (const char*)bufFlvheader.pBuffer, bufFlvheader.dwBufLen, 0);	
					nerr = send(sClient, "\r\n", 2, 0);
					if (nerr == INVALID_SOCKET)
					{
						break;
					}
					
				}
				if ((dwRet & HAS_FLVFRAMEDATA)==HAS_FLVFRAMEDATA)
				{
					fwrite(bufFlvData.pBuffer, 1, bufFlvData.dwBufLen, pFiledest);
					
					CString strLen;
					strLen.Format("%x\r\n", bufFlvData.dwBufLen);
					int nerr = send(sClient, (LPCSTR)strLen, strLen.GetLength(), 0);
					
					nerr = ::send(sClient, (const char*)bufFlvData.pBuffer, bufFlvData.dwBufLen, 0);
					if (nerr != bufFlvData.dwBufLen)
					{
						AfxMessageBox("what?!");
					}
					nerr = send(sClient, "\r\n", 2, 0);
					
					if (nerr == INVALID_SOCKET)
					{
						break;
					}
					
					dwTime += 40;
				}
				
			}
			send(sClient, "0\r\n\r\n", 5, 0);

			fclose(pFile);
			fclose(pFiledest);
			closesocket(sClient);
		}
	}
	closesocket(sServer);
}




void CDashmp4Dlg::OnOk4() 
{
	FILE* pAudioFile = fopen("c:\\audio.flv", "wb");

	int index = 0;
	FILE* pFile = fopen("D:\\nginx-1.10.3\\html\\flv\\video3.flv", "rb");	
	index += 13;
	char* pbufferdata = new char[1024*1024];
	while (!feof(pFile)) 
	{
		fseek(pFile, index, SEEK_SET);
		unsigned char buf[4] = {0};
		fread(buf, 1, 4, pFile);
		DWORD nTagLen = (buf[1]<<16) | (buf[2]<<8) | buf[3];
		if (buf[0] == 8)
		{
			int nLen = nTagLen + 7+4;
			fwrite(buf, 1, 4, pAudioFile);
			fread(pbufferdata, 1, nLen, pFile);
			fwrite(pbufferdata, 1, nLen, pAudioFile);
		}		


		index += 11;
		index += nTagLen;		
		index += 4;
	}
	fclose(pFile);
	fclose(pAudioFile);
	delete [] pbufferdata;
}

void CDashmp4Dlg::OnOk5() 
{	
	FILE* pDest = fopen("c:\\combile.flv", "wb");
	

	CFlvReader flvVideo;
	CFlvReader flvAudio;
	flvVideo.Open("c:\\dest.flv");
	flvAudio.Open("c:\\audio.flv");
	char* szbuffer = new char[1024*1024];
	unsigned int dwVideoTime, dwAudioTime;
	int dwVideoLen, dwAudioLen;
	while (flvVideo.GetNextFrameTimeandLen(9, dwVideoTime, dwVideoLen) &&
		   flvAudio.GetNextFrameTimeandLen(8, dwAudioTime, dwAudioLen)) 
	{
		if (dwVideoTime > dwAudioTime)
		{
			flvAudio.GetNextFrameBuffer(szbuffer, dwAudioLen);
			fwrite(szbuffer, 1, dwAudioLen, pDest);
			
		}
		else
		{
			flvVideo.GetNextFrameBuffer(szbuffer, dwVideoLen);
			fwrite(szbuffer, 1, dwVideoLen, pDest);
		}
	}

	delete [] szbuffer;
	fclose(pDest);
}

void CDashmp4Dlg::OnOk6() 
{	
	FILE* pDest = fopen("c:\\seeyouagain.h264", "wb");	
	
	CFlvReader flvVideo;	
	flvVideo.Open("D:\\nginx-1.10.3\\html\\flv\\video3.flv");	
	unsigned char* szbuffer = new unsigned char[1024*1024];
	unsigned int dwVideoTime;
	int dwVideoLen;	
	

	char sznaluint[4] = {0, 0, 0, 1};
	while (flvVideo.GetNextFrameTimeandLen(9, dwVideoTime, dwVideoLen)) 
	{
		
		flvVideo.GetNextFrameRawBuffer((char*)szbuffer, dwVideoLen);
		if (szbuffer[1] == 1)
		{
			u32 totallen = dwVideoLen -5 ;
			fwrite(&totallen, 1, 4, pDest);
			int nIndex = 5;
			while (nIndex < totallen)
			{
				u32 nnallen = (szbuffer[nIndex] << 24) |
					(szbuffer[nIndex+1] << 16) |
					(szbuffer[nIndex+2] << 8) |
					(szbuffer[nIndex+3]);
				nIndex += 4;
				fwrite(sznaluint, 1, 4, pDest);
				fwrite(&szbuffer[nIndex], 1, nnallen, pDest);
				nIndex += nnallen;
			}
		}
		else if (szbuffer[1] == 0)
		{
			char spsbuffer[2048];
			char ppsbuffer[1024];
			// sps pps
			int spslen = 0;
			int ppslen = 0;
			int nIndex  = 11;
			spslen = (szbuffer[nIndex] << 8) | szbuffer[nIndex+1];
			nIndex += 2;			
			memcpy(spsbuffer, &szbuffer[nIndex], spslen);			
			nIndex += spslen;
			if (szbuffer[nIndex] != 1)
			{
				AfxMessageBox("pps is not 1 sth is wrong");
			}
			nIndex += 1;
			ppslen = (szbuffer[nIndex] << 8) | szbuffer[nIndex+1];
			nIndex+=2;
			memcpy(ppsbuffer, &szbuffer[nIndex], ppslen);
			
			int totallen =  4 + 4 + spslen + ppslen;
			fwrite(&totallen, 1, 4, pDest);
			fwrite(sznaluint, 1, 4, pDest);
			fwrite(spsbuffer, 1, spslen, pDest);
			fwrite(sznaluint, 1, 4, pDest);
			fwrite(ppsbuffer, 1, ppslen, pDest);
		}
		
		//fwrite(szbuffer, 1, dwVideoLen, pDest);		
	}
	
	delete [] szbuffer;
	fclose(pDest);
}

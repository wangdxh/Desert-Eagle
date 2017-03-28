#ifndef _WXH_H264_FRAME_H_
#define _WXH_H264_FRAME_H_


typedef unsigned char	u8;
typedef unsigned short  u16;
typedef short           s16;
typedef char            s8;
typedef int		s32,BOOL32;
typedef unsigned long    u32;

#ifdef WIN32
typedef unsigned __int64 u64;
#else 
typedef unsigned long long u64;
#endif


#include <vector>
//using namespace std;

struct Buffer 
{
	Buffer()
	{
		pBuffer = NULL;
		dwBufLen = 0;
	}
	u8* pBuffer;
	u32 dwBufLen;
};

struct NalUnitBuf
{	
	u8* pBufStart;
	u32 dwNalLen; // no 0 0 0 1 or 0 0 1
				  /*u8 nalUnitType;
				  u32 pts;
	u32 dts;*/
};
class H264Frame
{
public:
	H264Frame(u8* pBuffer, u32 dwLen, u32 dwTime)
	{
		bIsKeyFrame = -1;
		this->dwTime = dwTime;
		this->tBuf.pBuffer = pBuffer;
		this->tBuf.dwBufLen = dwLen;
		
		this->GetNalunit();
	}
	BOOL32 IsKeyFrame()
	{
		if (-1 != bIsKeyFrame) 
		{
			return bIsKeyFrame;
		}
		bIsKeyFrame = FALSE;
		for(int inx = 0; inx < vecNals.size(); inx++)
		{
			NalUnitBuf nal = vecNals[inx];
			if ((nal.pBufStart[0] & 0x1f) == 5) 
			{
				bIsKeyFrame = TRUE;
			}
		}
		return bIsKeyFrame;
	}
	BOOL32 IsVideo()
	{
		return TRUE;
	}
	// 7 sps  8 pps
	BOOL32 GetSpsorPpsInfo(u32 dwtype, NalUnitBuf* retnal)
	{
		BOOL32 bFind = FALSE;
		for(int inx = 0; inx < vecNals.size(); inx++)
		{
			NalUnitBuf nal = vecNals[inx];
			if ((nal.pBufStart[0] & 0x1f) == dwtype) 
			{
				if (retnal)
				{
					*retnal = nal;
				}				
				bFind = TRUE;				
			}
		}		
		return bFind;
	}
	
	BOOL32 HaveSpsAndPps()
	{
		BOOL32 bFindSps = FALSE;
		BOOL32 bFindpps = FALSE;
		for(int inx = 0; inx < vecNals.size(); inx++)
		{
			NalUnitBuf nal = vecNals[inx];
			if ((nal.pBufStart[0] & 0x1f) == 7) 
			{
				bFindSps = TRUE;				
			}
			if ((nal.pBufStart[0] & 0x1f) == 8) 
			{
				bFindpps = TRUE;				
			}
		}		
		return bFindSps && bFindpps;
	}
	
	u32 GetTotalFrameSize()
	{
		u32 dwTotalSize = 4 * vecNals.size();
		for(int inx = 0; inx < vecNals.size(); inx++)
		{
			dwTotalSize += vecNals[inx].dwNalLen;			
		}
		return dwTotalSize;
	}
	void WriteFrameToBuffer(u8* pBuffer)
	{
		u32 offset = 0;
		
		for(int inx = 0; inx < vecNals.size(); inx++)
		{
			u32 dwNalSize = vecNals[inx].dwNalLen;
			
			pBuffer[offset++] = (dwNalSize >> 24 & 0xff);
			pBuffer[offset++] = (dwNalSize >> 16 & 0xff);
			pBuffer[offset++] = (dwNalSize >> 8 & 0xff);
			pBuffer[offset++] = (dwNalSize & 0xff);
			
			memcpy(pBuffer+offset, vecNals[inx].pBufStart, dwNalSize);
			offset += dwNalSize;
			
		}		
	}
	void GetNalunit()
	{		
		vecNals.clear();
		
		u8* buffer = tBuf.pBuffer;
		u32 dwLen = tBuf.dwBufLen;				
		
		int i;
		int syncPoint = 0;
		for (; syncPoint < dwLen - 3; syncPoint++) 
		{
			if (buffer[syncPoint + 2] == 1) 
			{
				// the sync point is properly aligned
				i = syncPoint + 5;
				break;
			}
		}
		
		while (i < dwLen) 
		{
			// look at the current byte to determine if we've hit the end of
			// a NAL unit boundary
			switch (buffer[i]) 
			{
			case 0:
				// skip past non-sync sequences
				if (buffer[i - 1] != 0) {
					i += 2;
					break;
				} else if (buffer[i - 2] != 0) {
					i++;
					break;
				}
				
				// deliver the NAL unit if it isn't empty
				if (syncPoint + 3 != i - 2) 
				{
					//this.trigger('data', buffer.subarray(syncPoint + 3, i - 2));
					// one nalu
					NalUnitBuf tNal;
					tNal.pBufStart = &buffer[syncPoint+3];
					tNal.dwNalLen = i-2 - (syncPoint + 3);
					vecNals.push_back(tNal);
				}
				
				// drop trailing zeroes
				do 
				{
					i++;
				} while (buffer[i] != 1 && i < dwLen);
				syncPoint = i - 2;
				i += 3;
				break;
			case 1:
				// skip past non-sync sequences
				if (buffer[i - 1] != 0 ||
					buffer[i - 2] != 0) 
				{
					i += 3;
					break;
				}
				
				// deliver the NAL unit
				//this.trigger('data', buffer.subarray(syncPoint + 3, i - 2));
				
				NalUnitBuf tNal;
				tNal.pBufStart = &buffer[syncPoint+3];
				tNal.dwNalLen = i-2 - (syncPoint + 3);
				vecNals.push_back(tNal);
				
				syncPoint = i - 2;
				i += 3;
				break;
			default:
				// the current byte isn't a one or zero, so it cannot be part
				// of a sync sequence
				i += 3;
				break;
			}
		}
		// filter out the NAL units that were delivered
		//buffer = buffer.subarray(syncPoint);
		if (syncPoint < dwLen)
		{			
			NalUnitBuf tNal;
			tNal.pBufStart = &buffer[syncPoint+3];
			tNal.dwNalLen = dwLen - syncPoint - 3;
			vecNals.push_back(tNal);			
		}
		i -= syncPoint;
		syncPoint = 0;
	}
	
public:
	u32 dwTime;
	u32 duration;
	Buffer tBuf; // one full frame
	std::vector <NalUnitBuf> vecNals;
private:
	
	
	BOOL32 bIsKeyFrame;	
}; 


//////////////////////////////////////////////////////////////////////////
// h264 has a extra head
// flv and header buffer should is bigger suggests is 1*1024*1024
#define FLV_ERROR 0x0
#define HAS_FLVHEADER    0x1
#define HAS_FLVFRAMEDATA 0x2
//dwtimestamp 40ms or 33ms depends on framerate

class CFlv
{
public:
	CFlv()
	{
		firstkeyframe = 0;
		flvheader = 0;
	}
	
	u32 ConvertH264ToFlv(Buffer* pBufH264, u32 dwTimeStamp, Buffer* pBufFlvFrame, Buffer* pbuFlvHeader, BOOL bTotalFlv = 0)
	{
		u32 dwRet = FLV_ERROR;
		H264Frame tFrame(pBufH264->pBuffer, pBufH264->dwBufLen, dwTimeStamp);
		tFrame.dwTime = dwTimeStamp;
		
		if (0 == flvheader)
		{
			if (NULL == spsbuf.pBuffer) 
			{
				NalUnitBuf spsnal;
				if (tFrame.GetSpsorPpsInfo(7, &spsnal))
				{
					spsbuf.dwBufLen = spsnal.dwNalLen;
					spsbuf.pBuffer = new u8[spsbuf.dwBufLen];
					memcpy(spsbuf.pBuffer, spsnal.pBufStart, spsbuf.dwBufLen);
				}
			}
			if (NULL == ppsbuf.pBuffer)
			{
				NalUnitBuf ppsnal;
				if (tFrame.GetSpsorPpsInfo(8, &ppsnal))
				{
					ppsbuf.dwBufLen = ppsnal.dwNalLen;
					ppsbuf.pBuffer = new u8[ppsbuf.dwBufLen];
					memcpy(ppsbuf.pBuffer, ppsnal.pBufStart, ppsbuf.dwBufLen);
				}
			}
			
			if (spsbuf.pBuffer && ppsbuf.pBuffer)
			{
				flvheader = 1;
				u32 taglen = 1 + 1 + 3 + (1 + 3 + 2 + 2 + spsbuf.dwBufLen + 1 + 2 + ppsbuf.dwBufLen);
				
				u32 dwflvheader = 13 + 11 + taglen + 4;
				u8* ptflvheader = new u8[dwflvheader];					
				memset(ptflvheader, 0, dwflvheader);
				
				
				int nIndex = 0;
				u8 header[13] = {0x46, 0x4c, 0x56, 0x01, 0x01, 
					0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00};
				memcpy(&ptflvheader[nIndex], header, 13);
				nIndex += 13;
				
				u8 tagheader[11] = {0};
				tagheader[0] = 9; //video
				
				
				tagheader[1] = (taglen >> 16) & 0xff;
				tagheader[2] = (taglen >> 8) & 0xff;
				tagheader[3] = taglen & 0xff;
				//timestamp
				memcpy(&ptflvheader[nIndex], tagheader, 11);
				nIndex += 11;
				
				
				u8* tagdata = new u8[taglen];
				memset(tagdata, 0, taglen);
				tagdata[0] = 0x17;	//1byte keyframe 0x17 or 0x27
				//1byte 0 spspps  1 nalu
				//3bytes 0x0, 0, 0 // cts
				
				tagdata[5] = 0x01; //version
				tagdata[6] = spsbuf.pBuffer[1];
				tagdata[7] = spsbuf.pBuffer[2];
				tagdata[8] = spsbuf.pBuffer[3];
				tagdata[9] = 0xff;
				tagdata[10] = 0xe1; // follow one sps
				u16 spslen = spsbuf.dwBufLen; //2bytes spelen
				tagdata[11] = (spslen >> 8) & 0xff;
				tagdata[12] = spslen& 0xff;
				memcpy(&tagdata[13], spsbuf.pBuffer, spsbuf.dwBufLen); // sps
				int ntagindex = 13 + spsbuf.dwBufLen;
				tagdata[ntagindex] = 0x01;// 1 pps
				u16 ppslen = ppsbuf.dwBufLen; //2bytes ppslen
				tagdata[ntagindex+1] = (ppslen >> 8) & 0xff;
				tagdata[ntagindex+2] = ppslen& 0xff;
				memcpy(&tagdata[ntagindex+3], ppsbuf.pBuffer, ppsbuf.dwBufLen); // pps
				
				memcpy(&ptflvheader[nIndex], tagdata, taglen);
				nIndex += taglen;
				delete [] tagdata;
				
				u32 lasttaglen = taglen + 11;
				ptflvheader[nIndex++] = (lasttaglen >> 24) & 0xff;
				ptflvheader[nIndex++] = (lasttaglen >> 16) & 0xff;
				ptflvheader[nIndex++] = (lasttaglen >> 8) & 0xff;
				ptflvheader[nIndex++] = lasttaglen & 0xff;
				
				
				memcpy(pbuFlvHeader->pBuffer, ptflvheader, dwflvheader);
				pbuFlvHeader->dwBufLen = dwflvheader;					
				
				delete[] ptflvheader;
				dwRet |= HAS_FLVHEADER;
			}
			else
			{
				return dwRet;
			}
			
		}
		
		if (flvheader && 0 == firstkeyframe)
		{
			if (tFrame.IsVideo() && tFrame.IsKeyFrame())
			{
				firstkeyframe = 1;
			}
			else
			{
				return dwRet;
			}				
		}
		if (flvheader && firstkeyframe) 
		{
			if (bTotalFlv)
			{
				u8* dstflvbuffer = pBufFlvFrame->pBuffer;
				u32 taglen = 5 + tFrame.GetTotalFrameSize();
				u32 dwflvtaglen = 11 + taglen + 4;
				
				u8 tagheader[16] = {0};
				tagheader[0] = 9; //video								
				tagheader[1] = (taglen >> 16) & 0xff;
				tagheader[2] = (taglen >> 8) & 0xff;
				tagheader[3] = taglen & 0xff;
				
				// nb timestamp
				tagheader[4] = (tFrame.dwTime>> 16) & 0xff;
				tagheader[5] = (tFrame.dwTime >> 8) & 0xff;
				tagheader[6] = tFrame.dwTime & 0xff;
				tagheader[7] = (tFrame.dwTime>> 24) & 0xff;
				
				tagheader[11] = tFrame.IsKeyFrame() ? 0x17 : 0x27;
				tagheader[12] = 1;
				
				int nindex = 0;
				memcpy(dstflvbuffer, tagheader, 16);
				nindex += 16;
				tFrame.WriteFrameToBuffer(&dstflvbuffer[nindex]);
				nindex += tFrame.GetTotalFrameSize();
				
				u32 nlasttaglen = taglen + 11;
				
				dstflvbuffer[nindex++] = (nlasttaglen >> 24) & 0xff;
				dstflvbuffer[nindex++] = (nlasttaglen >> 16) & 0xff;
				dstflvbuffer[nindex++] = (nlasttaglen >> 8) & 0xff;
				dstflvbuffer[nindex++] = nlasttaglen  & 0xff;
				pBufFlvFrame->dwBufLen = dwflvtaglen;
				
			}						
			else
			{				
				u32 taglen = 5 + tFrame.GetTotalFrameSize();
				u32 dwflvtaglen = taglen+4;
				
				u8* dstflvbuffer = pBufFlvFrame->pBuffer;
				
				dstflvbuffer[0] = tFrame.IsKeyFrame() ? 0x17 : 0x27;
				dstflvbuffer[1] = 1;
				dstflvbuffer[2] = 0;
				dstflvbuffer[3] = 0;
				dstflvbuffer[4] = 0;

				int nindex = 5;
				tFrame.WriteFrameToBuffer(&dstflvbuffer[nindex]);
				nindex += tFrame.GetTotalFrameSize();
				
				u32 nlasttaglen = taglen + 11;
				dstflvbuffer[nindex++] = (nlasttaglen >> 24) & 0xff;
				dstflvbuffer[nindex++] = (nlasttaglen >> 16) & 0xff;
				dstflvbuffer[nindex++] = (nlasttaglen >> 8) & 0xff;
				dstflvbuffer[nindex++] = nlasttaglen  & 0xff;
				
				pBufFlvFrame->dwBufLen = dwflvtaglen;
			}
			dwRet |= HAS_FLVFRAMEDATA;
		}
		return dwRet;	
	}
	
private:
	Buffer spsbuf;
	Buffer ppsbuf;
	u32 firstkeyframe;
	u32 flvheader;
};



class CFlvReader
{
public:
	CFlvReader()
	{
		m_index = 13;
		m_pFile = NULL;
	}
	~CFlvReader()
	{
		if (m_pFile)
		{
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}
	BOOL Open(char* strFile)
	{
		m_pFile = fopen(strFile, "rb");
		return TRUE;
	}
	BOOL GetNextFrameTimeandLen(int type, unsigned int & time, int& len)
	{
		while (!feof(m_pFile))
		{
			fseek(m_pFile, m_index, SEEK_SET);		
			unsigned char buf[11] = {0};
			fread(buf, 1, 11, m_pFile);
			DWORD nTagLen = (buf[1]<<16) | (buf[2]<<8) | buf[3];			
			if (buf[0] == type)
			{
				time = (buf[4]<<16) | (buf[5]<<8) | buf[6];
				len = 11 + nTagLen + 4;
				return TRUE;				
			}
			m_index += 11;
			m_index += nTagLen;		
			m_index += 4;
		}
		return FALSE;
	}
	BOOL GetNextFrameBuffer(char* szBuffer, int& nBuferLen)
	{
		fseek(m_pFile, m_index, SEEK_SET);				
		fread(szBuffer, 1, nBuferLen, m_pFile);
		m_index += nBuferLen;		
		return TRUE;
	}
	BOOL GetNextFrameRawBuffer(char* szBuffer, int& nBuferLen)
	{
		fseek(m_pFile, m_index+11, SEEK_SET);				
		fread(szBuffer, 1, nBuferLen-15, m_pFile);
		m_index += nBuferLen;
		nBuferLen -= 15;
		return TRUE;
	}
private:
	int m_index;
	FILE* m_pFile;
};



#endif
#pragma once
#include <iostream>
#include <codecvt>
#include <locale>
#include <string>
using namespace std;

#define FLOOR(x) (FLOAT)((INT)x)

inline BOOL UTF16toUTF8(_In_ WCHAR* src, _Out_ CHAR* dest, _In_ SIZE_T destLen) {

	if (src == nullptr || dest == nullptr)
		return FALSE;

	wstring_convert<codecvt_utf8_utf16<WCHAR>> wchar_to_utf8;
	wstring wstr(src);
	string utf8str = wchar_to_utf8.to_bytes(wstr);

	if (destLen < utf8str.size())
		return FALSE;

	strcpy_s(dest, destLen, utf8str.c_str());

	return TRUE;
}
inline BOOL ANSItoUTF8(_In_ CHAR* src, _Out_ CHAR* dest, _In_ SIZE_T destLen) {

	if (src == nullptr || dest == nullptr)
		return FALSE;

	BSTR bstr;
	int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, NULL);
	bstr = SysAllocStringLen(NULL, len);

	MultiByteToWideChar(CP_UTF8, 0, src, -1, bstr, len);

	len = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);

	if (len < 1)
		return FALSE;

	if (len < destLen) {
		WideCharToMultiByte(CP_ACP, 0, bstr, -1, dest, (int)destLen, NULL, NULL);
	}
	else {
		SysFreeString(bstr);
		return FALSE;
	}

	return TRUE;
}
inline BOOL TextCommma(_In_ CHAR* src, _Out_ CHAR* dest) {

	if (src == nullptr || dest == nullptr) {
		return FALSE;
	}

	SIZE_T len = strlen(src);

	while (*src) {
		*dest++ = *src++;

		if (--len && (len % 3) == 0)
			*dest++ = ',';
	}
	*dest++ = 0;
	return TRUE;
}

#define MAX_BUFFER_LENGTH 1024

#pragma pack(push, 1)

typedef struct _ETHERNETHEADER {
	UCHAR DesMac[6];
	UCHAR SrcMac[6];
	USHORT Type;
}ETHERNETHEADER;

typedef struct _IPHEADER {
	//little endian begin
	USHORT len : 4;					//����
	USHORT version : 4;				//��� ����
	// little endian end
	USHORT tos : 8;					// ���� ����
	USHORT length;					// ��ü ����
	USHORT id;						// 16 ��Ʈ ���̵�
	//little endian begin
	USHORT fragment_offset1 : 5;	// ����ȭ �ɼ�
	USHORT flags : 3;				// flag
	USHORT fragment_offset2 : 8;	// ����ȭ �ɼ�
	//little endian end
	USHORT ttl : 8;					// ttl
	USHORT protocol : 8;			// protocol tcp==06
	USHORT checksum;				// ��� ý��
	ULONG src_ip;					// ����� IP
	ULONG dest_ip;					// ������ IP
}IPHEADER;

typedef struct _TCPHEADER {
	USHORT src_port;				// ����� port
	USHORT dest_port;				// ������ port
	ULONG sqc_number;				// ������ �ѹ�
	ULONG ack_number;				// ack �ѹ�
	//little endian begin
	USHORT reserved1 : 4;			// reserved
	USHORT length : 4;				// ��� ����
	USHORT fin : 1;					// FIN
	USHORT syn : 1;					// SYN
	USHORT rst : 1;					// RST
	USHORT psh : 1;					// PSH
	USHORT ack : 1;					// ACK
	USHORT urg : 1;					// URG
	USHORT reserved2 : 2;			// reserved
	//little endian end
	USHORT window_size;				// ������ ũ��
	USHORT tcp_checksum;			// TCP checksum
	USHORT urg_point;				//��� ������
}TCPHEADER;

typedef struct _IPV4PACKET {
	ETHERNETHEADER* _ethernetHeader;
	IPHEADER* _ipHeader;
	TCPHEADER* _tcpHeader;
	const UCHAR* _data;
	USHORT _datalength;
}IPv4Packet;

#pragma pack(pop)
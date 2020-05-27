#pragma once

#include <iostream>
#include "winsock2.h"
#include <vector>
#include <mutex>
#include <bitset>
#pragma comment (lib,"ws2_32.lib")
class  PlayerMsg
{
	std::string m_fromip;
	std::string m_toip;
	char m_msgs[256];
public:
	
	void setFromIP(std::string _ip)
	{
		m_fromip = _ip;

	}
	void setToIP(std::string _ip)
	{
		m_toip = _ip;
	}
	void setMsg(const char* _msg)
	{
		strcpy(m_msgs, _msg);

	}
	std::string getFromIP(std::string _ip)
	{
		return m_fromip;

	}
	std::string getToIP()
	{
		return m_toip;
	}
	void getMsg(char* _msg)
	{
		strcpy(_msg, m_msgs);

	}




};
class  ClientData
{
	std::string m_clientip;
	SOCKET* m_clientsocket;
	std::vector<PlayerMsg> m_recvmsg;

public:
	void setClientIP(std::string _ip)
	{
		m_clientip = _ip;
	}
	void setClientSocket(SOCKET* _socket)
	{
		m_clientsocket = _socket;
	}
	std::string getClientIP()
	{
		return m_clientip;
	}
	SOCKET* getClientSocket()
	{
		return m_clientsocket;
	}
	std::vector<PlayerMsg>& getToClientMsgs()
	{
		return m_recvmsg;
	}



};




class WebSocketData {
public:
	std::bitset<1> fin;                                               //帧标记
	std::bitset<4> opcode;					//操作码
	std::bitset<1> masked;					        //掩码
	std::bitset<7>payload_size;				//数据大小
	std::bitset<8>* masking_key;//yanma
	std::bitset<8>* data;//数据
	int mask_offset;					//掩码偏移

	unsigned char res[3];
};

class  STCPServer
{
	STCPServer();
public:
	~STCPServer();
	bool wsEncodeFrame(std::string inMessage, std::string& outFrame, std::bitset<4> _FrameType);
	SOCKET* Start(std::string _ip, int _port);
	static void ResponseToClient(const char* _responsedata, SOCKET* _clientsocket);
	void RegeditMsg(const char* _responsedata, std::string  _fromip, SOCKET* _toclientsocket);
	void RegeditBroadcard(const char* _responsedata);
	void SetClientHandleFunction(int (*_func)(SOCKET*, int));
	static STCPServer* m_stcp;
	static STCPServer* GetInstance();
	bool WebConType;
	bool wsReadHeader(const  char* _data, int _size, WebSocketData* _wsdata, char* _buff);
	void setFin(std::bitset<8>* _pbitset, bool state);
	void setEcode(std::bitset<8>* _pbitset, int state);
	void setMasked(std::bitset<8>* _pbitset, bool state);
	void setDataLength(std::bitset<8>* _pbitset, int size);
	void setData(std::bitset<8>* _bit8, std::string  inMessage);
	char* ParseBitset8ToString(std::bitset<8>* _bit8, int _size);
	//bool TransFormWebHead(char* _data, int _size, unsigned char& _type);
	static std::vector<PlayerMsg> m_playermsgs;
	static std::vector<ClientData> m_clientdatas;
	void (*m_handlerResponseFunction)(const char* _str, int _size, const char* _fromip, SOCKET* _toclientsocket);
	void SetHandlerResponseFunction(void (*_handlerResponseFunction)(const char*, int, const char*, SOCKET*));
	static std::string _findClientIP(SOCKET* _socket);
	static std::vector<PlayerMsg>* _findMsg(SOCKET* _socket);
	static char* HasMsg(SOCKET*);
	void(*m_handleWebSocketConnect)(char*, char*);
	char *(*m_handleWebSocketData)(char*);
	static std::mutex m_mut;
	void SetType(int _type);
	int GetType();
	void SetHandleWebSocketConnectFunction(void(*_handleWebSocketConnect)(char*, char*));
	void SetHandleWebSocketDataFunction(char *(*_handleWebSocketData)(char*));
private:
	std::string m_onlyIP;
	SOCKET* m_onlyLinkSocket;
	SOCKET* m_curConnectSocket;
	int (*m_handleClientCon)(SOCKET*, int);
	int m_type;
	//创建链接申请套接字
	SOCKET* _createLinkApplicationSocket();
	int _bindIPPort(std::string _ip, int _port);
	void _listen(int _num);
	void _accept();
};
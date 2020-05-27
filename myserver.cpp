// myserver.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <sstream>
#include "winsock2.h"
#include <thread>
#include <condition_variable>
#include <mutex>
#include "STCPServer.h"
#include <regex>
#include <bitset>
#include "SHA2.h"
#include "base64.h"

#pragma comment (lib,"ws2_32.lib")
#pragma comment (lib,"STCPServer.lib")

using namespace std;
STCPServer* tcpserver = STCPServer::GetInstance();
void wsconnectback(  char * _data, char * request)
{
    std::string wsid;
    std::string mail = _data;
    std::regex reg("\r\n");
    std::sregex_token_iterator pos(mail.begin(), mail.end(), reg, -1);
    decltype(pos) end;
    for (; pos != end; ++pos)
    {
        const std::string item = pos->str();
        std::cmatch m2;
        auto ret = std::regex_match(item.c_str(), m2, std::regex("(.*): (.*)"));

        for (auto i = 0; i < (int)m2.size(); ++i)
        {
            if (m2[i].str() == "Sec-WebSocket-Key")
                wsid = m2[i + 1].str();
        }
    }

    std::string root = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    strcat(request, "HTTP/1.1 101 Switching Protocols\r\n");
    strcat(request, "Connection: upgrade\r\n");
    strcat(request, "Sec-WebSocket-Accept: ");
    std::string server_key = wsid;
    server_key += root;

    SHA2 sha;
    int ret;
    uint8_t Message_Digest[20];
    ;
    ret = sha.TransformToHASH(server_key.c_str(), Message_Digest, strlen(server_key.c_str()));
    if (ret == 0) {
        
        for (int i = 0; i < 20; ++i)
        {
            printf("%02x", Message_Digest[i]);
        }
    }
    else {
        printf("error\n");
    }

   
    printf("\n");
    base64 base;
    server_key = base.base64_encode(reinterpret_cast<const unsigned char*>(Message_Digest), 20);
    server_key += "\r\n";
    strcat(request, server_key.c_str());
    strcat(request, "Upgrade: websocket\r\n\r\n");
    


}
void wsconnectfun(  char * _data,  char* _newdata)
{
    //获取标识符
    wsconnectback(_newdata, _data);
   

}

char * handlerWebData(char* _str)
{
    strcpy(_str, "iknow");
    char* newstr = new char[6];
    memset(newstr, 6, 0);
    strcpy(newstr, "iknow");
    return newstr;
    //tcpserver->RegeditBroadcard("系统广播:有玩家进行了一对一对话");
    //char a[256];
    //sprintf(a, "%s:%s", _fromip, _str);
    //tcpserver->RegeditMsg(_str,_fromip, _toclientsocket);

}
int main()
{

    WSAData wsa = { 0 };
    WSAStartup(MAKEWORD(2,2),&wsa);
    tcpserver->SetType(1);
   tcpserver->SetHandleWebSocketDataFunction(handlerWebData);
    tcpserver->SetHandleWebSocketConnectFunction(wsconnectback);
    //tcpserver->Start("192.168.0.196",5678);
    tcpserver->Start("127.0.0.1", 5678);


    WSACleanup();

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

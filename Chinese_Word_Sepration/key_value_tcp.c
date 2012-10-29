#include "key_value_tcp.h"

void _tcp_int_and_listen(int port,void callback(char*)){
	WORD wVersionRequested;
    WSADATA wsaData;
    int ret, nLeft, length;
    SOCKET sListen, sServer; //�����׽��֣������׽���
    struct sockaddr_in saServer, saClient; //��ַ��Ϣ   
    char *ptr;//���ڱ�����Ϣ��ָ��   
	 
	char receiveMessage[STR_SIZE];
	ZeroMemory(receiveMessage,STR_SIZE);

    //WinSock��ʼ��
    wVersionRequested=MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL �İ汾
    ret=WSAStartup(wVersionRequested, &wsaData);
    if(ret!=0)
    {
        printf("WSAStartup() failed!\n");
        return ;
    }
    //����Socket,ʹ��TCPЭ��
    sListen=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sListen == INVALID_SOCKET)
    {
        WSACleanup();
        printf("socket() faild!\n");
        return;
    }
    //�������ص�ַ��Ϣ
    saServer.sin_family = AF_INET; //��ַ����
    saServer.sin_port = htons(port); //ע��ת��Ϊ�����ֽ���
    saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //ʹ��INADDR_ANY ָʾ�����ַ
  
    //��
    ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
    if (ret == SOCKET_ERROR)
    {
        printf("bind() faild! code:%d\n", WSAGetLastError());
        closesocket(sListen); //�ر��׽���
        WSACleanup();
        return;
    }
  
    //������������
    ret = listen(sListen, 5);
    if (ret == SOCKET_ERROR)
    {
        printf("listen() faild! code:%d\n", WSAGetLastError());
        closesocket(sListen); //�ر��׽���
        return;
    }
  
    printf("Waiting for client connecting!\n");
    printf("Tips: Ctrl+c to quit!\n");
    //�����ȴ����ܿͻ�������
	 while(1)//ѭ�������ͻ��ˣ���Զ��ֹͣ�����ԣ��ڱ���Ŀ�У�����û����������
	 {
	  length = sizeof(saClient);
	  sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
	  if (sServer == INVALID_SOCKET)
	  {
	   printf("accept() faild! code:%d\n", WSAGetLastError());
	   closesocket(sListen); //�ر��׽���
	   WSACleanup();
	   return;
	  }       


	recv(sServer,receiveMessage,STR_SIZE,0);
	callback(receiveMessage);
 
 } 
return;
}

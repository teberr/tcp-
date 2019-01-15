// hello_server_win.c : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
void ErrorHandling(char* message);


int main(int argc,char * argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//소켓 라이브러리를 초기화 하고 있다.
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //소켓에 ip주소와 포트번호를 할당함
	servAddr.sin_port = htons(atoi(argv[1]));


	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)//listen 함수호출을 통해서 생성한 소켓을 서버 소켓으로 완성
		ErrorHandling("listen Error");

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);//클라이언트의 연결 요청을 수락하기 위해서 accept 함수를 호출
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	send(hClntSock, message , sizeof(message), 0);// 연결된 클라이언트에 메시지를 보냄
	closesocket(hClntSock);//소켓 라이브러리 해제
	closesocket(hServSock);
	WSACleanup();

    return 0;
}

void ErrorHandling(char* message)
{

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
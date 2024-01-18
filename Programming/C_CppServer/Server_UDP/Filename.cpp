#include "CHelper.h"


int main()
{
	WSADATA wsa; // winsoket 초기화
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup 에러" << endl;
		return -1;
	}

	/////
	SOCKET mySocket = socket(AF_INET, SOCK_DGRAM, 0);  //소켓으로 사용하는 함수
	if (mySocket == SOCKET_ERROR/*-1*/)
	{
		cout << "socket 생성 에러" << endl;

		WSACleanup();
		return -1;
	}
	/////


	///////////////////////////////////////////////////////////
	//받기 // 바인딩 전에 sendto를 사용하면 안된다. 
	sockaddr_in recvAddr = {}; 
	///// 주소를 적어도 되나. 이곳으로 들어오는 모든 것을 받게 할 수도 있다. 
	recvAddr.sin_family = AF_INET; 
	recvAddr.sin_port = htons(12345);
	recvAddr.sin_addr.s_addr = INADDR_ANY;   // 수신할 IP 컴퓨터에 붙여진 모든 IP 


	result = bind(mySocket, (sockaddr*)&recvAddr, sizeof(recvAddr)); //바인드 처리

	if (result != 0)
	{ /*0이여야 바인딩 되는 것*/
		cout << "바인딩 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	//어디로 어떤 내용을 보낼 것인지 
	//주소설정 필요
	sockaddr_in destAddr = {}; ///*구조체 데이터 함수없는 클래스, 비어두면 0으로 초기화됨*/
	destAddr.sin_family = AF_INET; //인터넷으로
	destAddr.sin_port = htons(12345); //문자열 숫자 등이 아닌 만들어둔 방식으로 바꿔서 넣어준다. 

	//#include <WS2tcpip.h>
	inet_pton(AF_INET, "127.0.0.1", &destAddr.sin_addr); // ip주소 넣어주기 보낼곳 (에코서버 내것으로 보내기)
	
	while (1)
	{
		/*입력한 것을 보낸다*/
		char inputStr[512] = "";
		cout << "송신할 내용 : ";
		fgets(inputStr, 100, stdin);
	
		sendto(mySocket/*넣어줄 소켓*/,
			inputStr /*어떤메세지를 넣읗지*/,
			100 /*문자열길이*/,
			0/*속성값 없음 플래그*/,
			(sockaddr*)&destAddr/*포인터 문법*/,
			sizeof(destAddr)/*destAddr의 바이트 크기*/);
		////////////////////////////////////////////////////////////////

		char buffer[1024] = "";
		sockaddr_in senderAddr;
		socklen_t   senderAddrLen = sizeof(senderAddr);

		//몇바이트가 들어오는지 
		int receivedBytes = recvfrom(mySocket, 
							buffer, 
							sizeof(buffer)/*수신된 내용 저장*/,
							0, 
							(sockaddr*)&senderAddr, 
							&senderAddrLen);
		//수신된 내용 출력
		cout << "수신된 내용 : " << buffer << endl;

		if (strcmp(buffer, "quit") == 0)
		{
			break;
		}
	}


	///// 이곳 까지 오면 socket 생성 됨
	closesocket(mySocket); // 후처리 잘 해줘야 한다. 
	WSACleanup(); //실행시 이상없으면 지워버리기

	return 0;
}
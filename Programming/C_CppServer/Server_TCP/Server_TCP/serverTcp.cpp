//#include "CHelper.h"
#include "UTF8toANSI.h"


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

	/////socket
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);  //소켓으로 사용하는 함수
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

	//listen : 문지기 소켓 
	if (listen(mySocket, SOMAXCONN/*최대허용인원*/) == SOCKET_ERROR)
	{
		cout << "Listen 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	// accept : (소켓들 오는거 기다리다가 알려주세요가 accept) accept안되면 안됨
	cout << "연결 대기 시작" << endl;

	auto clientSocket = accept(mySocket, nullptr, nullptr); //새롭게 연결된 소켓을 뱉어준다?

	if (clientSocket == INVALID_SOCKET)
	{
		cout << "Accept 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	cout << "클라 연결 성공" << endl;
	
	
	// recv send
	while (true)
	{
		char buffer[512] = "";
		int recvByte = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0)
		{
			cout << "클라 연결 문제" << endl;
			break;
		}

		cout << "서버에서 수신한 메시지" << buffer << endl;
		send(clientSocket, buffer, sizeof(buffer), 0);

	}

	closesocket(clientSocket);
	closesocket(mySocket);
	WSACleanup(); //실행시 이상없으면 지워버리기

	return 1;
}

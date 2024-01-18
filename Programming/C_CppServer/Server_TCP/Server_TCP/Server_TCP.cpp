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

	//socket
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM/*tcp연결*/, 0);


	//소켓 생성
	if (mySocket == INVALID_SOCKET)
	{
		cerr << "socket 생성 에러" << endl;
		WSACleanup();

		return -1;
	}

	// 수신자의 주소 설정
	sockaddr_in receiverAddr = {};
	receiverAddr.sin_family = AF_INET;    //수신 프로토콜
	receiverAddr.sin_port = htons(12345); // 수신자의 포트 번호
	receiverAddr.sin_addr.s_addr = INADDR_ANY;   // 수신할 IP 컴퓨터에 붙여진 모든 IP 


	//bind
	//소켓 바인딩
	result = bind(mySocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	if (result != 0)
	{
		cerr << "binding 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	//TCP : listen
	if (listen(mySocket, SOMAXCONN/*최대 허용 인원*/) == SOCKET_ERROR)
	{
		cerr << "listen 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	//accept
	cout << "연결 대기 시작" << endl;
	
	auto clientSocket = accept(mySocket/*새롭게 연결된 소켓을 뱉어준다. */, nullptr, nullptr);
	
	// 혹여나 소켓에 문제가 생겼을 경우
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Accept 에러" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	// 성공했다면 연결 
	cout << "클라 연결 성공" << endl;

	//recv send
	//보내는것 
	while (true)
	{
		char buffer[512] = "";
		int recvBytes = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0) // 0이면 무엇이낙 문제가 있는 것
		{
			cout << "클라 연결 문제" << endl;
			break;
		}

		cout << "서버에서 수신한 메시지 : " << buffer << endl;	
		send(clientSocket, buffer, strlen(buffer), 0);
	}
	

	closesocket(clientSocket);
	closesocket(mySocket);
	 
	WSACleanup(); //실행시 이상없으면 지워버리기
	return 1;
}

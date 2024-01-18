#include "../Server_TCP/CHelper.h"


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
	inet_pton(AF_INET, "127.0.0.1", &receiverAddr.sin_addr);  //클라이언트 개인 에코주소


	// 연결 시도. 
	result = connect(mySocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	// 연결 문제가 있다면
	if (result == SOCKET_ERROR)
	{
		cerr << "연결 에러" << endl;
		closesocket(mySocket);
		WSACleanup();

		return -1;
	}

	//문제가 없다면
	cout << "서버에 연결되었습니다." << endl;

	while (true)
	{
		string msg;

		cout << "입력 : ";
		getline(cin, msg); //띄어쓰기도 판단.

		if (msg == "exit")
		{
			break;
		}

		//문자열 보내기
		send(mySocket, msg.c_str()/*문자열 처음위치*/, msg.length(), 0);

		char buffer[512] = "";
		int recvBytes = recv(mySocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0) // 0이나오면 끊김
		{
			cout << "연결 문제" << endl;
			break;
		}
		cout << "수신 메시지 : " << buffer << endl;
	}

	system("pause"); // 대기



	closesocket(mySocket);
	WSACleanup(); //실행시 이상없으면 지워버리기
	return 1;
}

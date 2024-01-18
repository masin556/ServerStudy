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

	cout << "서버시작" << endl;

	//어떤 사람인지 구분 
	map<string, sockaddr_in> userList;


	while (1)
	{
		char buffer[1024] = "";
		sockaddr_in senderAddr;
		socklen_t   senderAddrLen = sizeof(senderAddr);

		//몇바이트가 들어오는지 
		int receivedBytes = recvfrom(
			mySocket,
			buffer,
			sizeof(buffer)/*수신된 내용 저장*/,
			0,
			(sockaddr*)&senderAddr, //보낸사람
			&senderAddrLen
		);


			/*
		IP:Port 로 문자열 만들고
		map에 있는 것인지 확인한 후 없으면 추가
		*/

		char senderIP[256] = "";
		inet_ntop(AF_INET, &senderAddr.sin_addr, senderIP, sizeof(senderIP));

		//새로운 유저 정보 저장 
		char userInfo[256]; // "127.0.0.1:12345  
		sprintf_s(userInfo, "%s:%d", senderIP, senderAddr.sin_port); //


		// 에러가 났다. 
		if (receivedBytes < 0)
		{
			int errCode = WSAGetLastError(); // 에러 검출

			//강제 종료 된 사람이 발생
			if (errCode == WSAECONNRESET)
			{
				userList.erase(userInfo); // 해당 키값을 지워준다. 에러가 날시
				cout << userInfo << " : 연결 해제" << endl;
				continue;
			}
			//접속이 끊긴 것
		}
		else //에러가 안났다면
		{
			userList[userInfo] = senderAddr;
		}



		//map에 있는 모든 사람에게 전송
		auto finder = userList.find(userInfo);
		//자료구조 안에 해당 유저 정보는 없다 map
		if (finder == userList.end())
		{
			userList.insert(make_pair(userInfo, senderAddr));
		}

		/*
		userList[userInfo] = senderAdr;
		*/

		for (auto i = userList.begin(); i != userList.end(); i++) /*begin 시작 부터 end()끝이 아니면*/
		{
			//송신된 내용을 클라이언트에 보내기 
			sendto(
				mySocket/*넣어줄 소켓*/,
				buffer /*어떤메세지를 넣읗지*/,
				strlen(buffer),
				0/*속성값 없음 플래그*/,
				(sockaddr*)&i->second/*받을사람*/,
				sizeof(i->second)/*destAddr의 바이트 크기*/
			);
		}


		//UTF8 -> ANSI변환 (한글 되도록) UTF8toANSI.h
		UTF8to_ANSI(buffer, sizeof(buffer));

		//수신된 내용 출력
		//cout << "수신된 내용 : " << buffer << endl;
		

		//quit 입력시
		//strcmp 말고, strstr(앞 뒤 안에 그것이 있는지)
		if (strstr(buffer, "quit") != nullptr)
		{
			break;
		}
	}


	///// 이곳 까지 오면 socket 생성 됨
	closesocket(mySocket); // 후처리 잘 해줘야 한다. 
	WSACleanup(); //실행시 이상없으면 지워버리기

	return 0;
}

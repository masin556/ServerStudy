//#include "CHelper.h"
#include "UTF8toANSI.h"


int main()
{
	WSADATA wsa; // winsoket �ʱ�ȭ
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		cout << "WSAStartup ����" << endl;
		return -1;
	}

	/////
	SOCKET mySocket = socket(AF_INET, SOCK_DGRAM, 0);  //�������� ����ϴ� �Լ�
	if (mySocket == SOCKET_ERROR/*-1*/)
	{
		cout << "socket ���� ����" << endl;

		WSACleanup();
		return -1;
	}
	/////


	///////////////////////////////////////////////////////////
	//�ޱ� // ���ε� ���� sendto�� ����ϸ� �ȵȴ�. 
	sockaddr_in recvAddr = {};
	///// �ּҸ� ��� �ǳ�. �̰����� ������ ��� ���� �ް� �� ���� �ִ�. 
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(12345);
	recvAddr.sin_addr.s_addr = INADDR_ANY;   // ������ IP ��ǻ�Ϳ� �ٿ��� ��� IP 


	result = bind(mySocket, (sockaddr*)&recvAddr, sizeof(recvAddr)); //���ε� ó��

	if (result != 0)
	{ /*0�̿��� ���ε� �Ǵ� ��*/
		cout << "���ε� ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	//���� � ������ ���� ������ 
	//�ּҼ��� �ʿ�
	sockaddr_in destAddr = {}; ///*����ü ������ �Լ����� Ŭ����, ���θ� 0���� �ʱ�ȭ��*/
	destAddr.sin_family = AF_INET; //���ͳ�����
	destAddr.sin_port = htons(12345); //���ڿ� ���� ���� �ƴ� ������ ������� �ٲ㼭 �־��ش�. 


	//#include <WS2tcpip.h>
	inet_pton(AF_INET, "127.0.0.1", &destAddr.sin_addr); // ip�ּ� �־��ֱ� ������ (���ڼ��� �������� ������)

	cout << "��������" << endl;

	//� ������� ���� 
	map<string, sockaddr_in> userList;


	while (1)
	{
		char buffer[1024] = "";
		sockaddr_in senderAddr;
		socklen_t   senderAddrLen = sizeof(senderAddr);

		//�����Ʈ�� �������� 
		int receivedBytes = recvfrom(
			mySocket,
			buffer,
			sizeof(buffer)/*���ŵ� ���� ����*/,
			0,
			(sockaddr*)&senderAddr, //�������
			&senderAddrLen
		);


			/*
		IP:Port �� ���ڿ� �����
		map�� �ִ� ������ Ȯ���� �� ������ �߰�
		*/

		char senderIP[256] = "";
		inet_ntop(AF_INET, &senderAddr.sin_addr, senderIP, sizeof(senderIP));

		//���ο� ���� ���� ���� 
		char userInfo[256]; // "127.0.0.1:12345  
		sprintf_s(userInfo, "%s:%d", senderIP, senderAddr.sin_port); //


		// ������ ����. 
		if (receivedBytes < 0)
		{
			int errCode = WSAGetLastError(); // ���� ����

			//���� ���� �� ����� �߻�
			if (errCode == WSAECONNRESET)
			{
				userList.erase(userInfo); // �ش� Ű���� �����ش�. ������ ����
				cout << userInfo << " : ���� ����" << endl;
				continue;
			}
			//������ ���� ��
		}
		else //������ �ȳ��ٸ�
		{
			userList[userInfo] = senderAddr;
		}



		//map�� �ִ� ��� ������� ����
		auto finder = userList.find(userInfo);
		//�ڷᱸ�� �ȿ� �ش� ���� ������ ���� map
		if (finder == userList.end())
		{
			userList.insert(make_pair(userInfo, senderAddr));
		}

		/*
		userList[userInfo] = senderAdr;
		*/

		for (auto i = userList.begin(); i != userList.end(); i++) /*begin ���� ���� end()���� �ƴϸ�*/
		{
			//�۽ŵ� ������ Ŭ���̾�Ʈ�� ������ 
			sendto(
				mySocket/*�־��� ����*/,
				buffer /*��޼����� ������*/,
				strlen(buffer),
				0/*�Ӽ��� ���� �÷���*/,
				(sockaddr*)&i->second/*�������*/,
				sizeof(i->second)/*destAddr�� ����Ʈ ũ��*/
			);
		}


		//UTF8 -> ANSI��ȯ (�ѱ� �ǵ���) UTF8toANSI.h
		UTF8to_ANSI(buffer, sizeof(buffer));

		//���ŵ� ���� ���
		//cout << "���ŵ� ���� : " << buffer << endl;
		

		//quit �Է½�
		//strcmp ����, strstr(�� �� �ȿ� �װ��� �ִ���)
		if (strstr(buffer, "quit") != nullptr)
		{
			break;
		}
	}


	///// �̰� ���� ���� socket ���� ��
	closesocket(mySocket); // ��ó�� �� ����� �Ѵ�. 
	WSACleanup(); //����� �̻������ ����������

	return 0;
}

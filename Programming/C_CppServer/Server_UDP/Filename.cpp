#include "CHelper.h"


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
	
	while (1)
	{
		/*�Է��� ���� ������*/
		char inputStr[512] = "";
		cout << "�۽��� ���� : ";
		fgets(inputStr, 100, stdin);
	
		sendto(mySocket/*�־��� ����*/,
			inputStr /*��޼����� ������*/,
			100 /*���ڿ�����*/,
			0/*�Ӽ��� ���� �÷���*/,
			(sockaddr*)&destAddr/*������ ����*/,
			sizeof(destAddr)/*destAddr�� ����Ʈ ũ��*/);
		////////////////////////////////////////////////////////////////

		char buffer[1024] = "";
		sockaddr_in senderAddr;
		socklen_t   senderAddrLen = sizeof(senderAddr);

		//�����Ʈ�� �������� 
		int receivedBytes = recvfrom(mySocket, 
							buffer, 
							sizeof(buffer)/*���ŵ� ���� ����*/,
							0, 
							(sockaddr*)&senderAddr, 
							&senderAddrLen);
		//���ŵ� ���� ���
		cout << "���ŵ� ���� : " << buffer << endl;

		if (strcmp(buffer, "quit") == 0)
		{
			break;
		}
	}


	///// �̰� ���� ���� socket ���� ��
	closesocket(mySocket); // ��ó�� �� ����� �Ѵ�. 
	WSACleanup(); //����� �̻������ ����������

	return 0;
}
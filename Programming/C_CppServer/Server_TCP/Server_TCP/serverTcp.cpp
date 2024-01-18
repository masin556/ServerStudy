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

	/////socket
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM, 0);  //�������� ����ϴ� �Լ�
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

	//listen : ������ ���� 
	if (listen(mySocket, SOMAXCONN/*�ִ�����ο�*/) == SOCKET_ERROR)
	{
		cout << "Listen ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	// accept : (���ϵ� ���°� ��ٸ��ٰ� �˷��ּ��䰡 accept) accept�ȵǸ� �ȵ�
	cout << "���� ��� ����" << endl;

	auto clientSocket = accept(mySocket, nullptr, nullptr); //���Ӱ� ����� ������ ����ش�?

	if (clientSocket == INVALID_SOCKET)
	{
		cout << "Accept ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	cout << "Ŭ�� ���� ����" << endl;
	
	
	// recv send
	while (true)
	{
		char buffer[512] = "";
		int recvByte = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0)
		{
			cout << "Ŭ�� ���� ����" << endl;
			break;
		}

		cout << "�������� ������ �޽���" << buffer << endl;
		send(clientSocket, buffer, sizeof(buffer), 0);

	}

	closesocket(clientSocket);
	closesocket(mySocket);
	WSACleanup(); //����� �̻������ ����������

	return 1;
}

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

	//socket
	SOCKET mySocket = socket(AF_INET, SOCK_STREAM/*tcp����*/, 0);


	//���� ����
	if (mySocket == INVALID_SOCKET)
	{
		cerr << "socket ���� ����" << endl;
		WSACleanup();

		return -1;
	}

	// �������� �ּ� ����
	sockaddr_in receiverAddr = {};
	receiverAddr.sin_family = AF_INET;    //���� ��������
	receiverAddr.sin_port = htons(12345); // �������� ��Ʈ ��ȣ
	receiverAddr.sin_addr.s_addr = INADDR_ANY;   // ������ IP ��ǻ�Ϳ� �ٿ��� ��� IP 


	//bind
	//���� ���ε�
	result = bind(mySocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	if (result != 0)
	{
		cerr << "binding ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	//TCP : listen
	if (listen(mySocket, SOMAXCONN/*�ִ� ��� �ο�*/) == SOCKET_ERROR)
	{
		cerr << "listen ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}


	//accept
	cout << "���� ��� ����" << endl;
	
	auto clientSocket = accept(mySocket/*���Ӱ� ����� ������ ����ش�. */, nullptr, nullptr);
	
	// Ȥ���� ���Ͽ� ������ ������ ���
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Accept ����" << endl;
		closesocket(mySocket);
		WSACleanup();
		return -1;
	}

	// �����ߴٸ� ���� 
	cout << "Ŭ�� ���� ����" << endl;

	//recv send
	//�����°� 
	while (true)
	{
		char buffer[512] = "";
		int recvBytes = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0) // 0�̸� �����̳� ������ �ִ� ��
		{
			cout << "Ŭ�� ���� ����" << endl;
			break;
		}

		cout << "�������� ������ �޽��� : " << buffer << endl;	
		send(clientSocket, buffer, strlen(buffer), 0);
	}
	

	closesocket(clientSocket);
	closesocket(mySocket);
	 
	WSACleanup(); //����� �̻������ ����������
	return 1;
}

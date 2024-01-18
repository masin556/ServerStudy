#include "../Server_TCP/CHelper.h"


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
	inet_pton(AF_INET, "127.0.0.1", &receiverAddr.sin_addr);  //Ŭ���̾�Ʈ ���� �����ּ�


	// ���� �õ�. 
	result = connect(mySocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	// ���� ������ �ִٸ�
	if (result == SOCKET_ERROR)
	{
		cerr << "���� ����" << endl;
		closesocket(mySocket);
		WSACleanup();

		return -1;
	}

	//������ ���ٸ�
	cout << "������ ����Ǿ����ϴ�." << endl;

	while (true)
	{
		string msg;

		cout << "�Է� : ";
		getline(cin, msg); //���⵵ �Ǵ�.

		if (msg == "exit")
		{
			break;
		}

		//���ڿ� ������
		send(mySocket, msg.c_str()/*���ڿ� ó����ġ*/, msg.length(), 0);

		char buffer[512] = "";
		int recvBytes = recv(mySocket, buffer, sizeof(buffer), 0);

		if (recvBytes <= 0) // 0�̳����� ����
		{
			cout << "���� ����" << endl;
			break;
		}
		cout << "���� �޽��� : " << buffer << endl;
	}

	system("pause"); // ���



	closesocket(mySocket);
	WSACleanup(); //����� �̻������ ����������
	return 1;
}

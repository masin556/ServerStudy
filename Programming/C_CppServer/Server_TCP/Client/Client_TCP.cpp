#include "../Server_TCP/CHelper.h"

int main()
{

	cout << "������ �����" << endl;

	while (true)
	{
		string msg;

		cout << "�Է� :";
		getline(cin, msg);

		if (msg == "exit")
		{
			break;
		}

		send(mySocket, msg.c_str(), msg.length(), 0);

		//�޴� ��
		char buffer[512] = "";
		int recvBytes = recv(mySocket, buffer, sizeof(buffer), 0); //wait������ �۽�

		if (recvBytes <= 0)
		{
			break;
		}

		cout << "���� �޽��� : " << buffer << endl;
	}

	system("pause");

}

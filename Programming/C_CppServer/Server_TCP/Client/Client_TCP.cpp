#include "../Server_TCP/CHelper.h"

int main()
{

	cout << "서버에 연결된" << endl;

	while (true)
	{
		string msg;

		cout << "입력 :";
		getline(cin, msg);

		if (msg == "exit")
		{
			break;
		}

		send(mySocket, msg.c_str(), msg.length(), 0);

		//받는 것
		char buffer[512] = "";
		int recvBytes = recv(mySocket, buffer, sizeof(buffer), 0); //wait잡히고 송신

		if (recvBytes <= 0)
		{
			break;
		}

		cout << "수신 메시지 : " << buffer << endl;
	}

	system("pause");

}

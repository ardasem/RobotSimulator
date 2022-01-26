
#include<stdio.h>
#include<iostream>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512	//Max length of buffer
#define PORT 6000	//The port on which to listen for incoming data
#define HOSTNAME 127.0.0.1 //IP Address


void Send(SOCKET s,char *SendBuffer, int recv_len,sockaddr_in &si_other,int slen){
if (sendto(s, SendBuffer, recv_len, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
{
	printf("sendto() failed with error code : %d", WSAGetLastError());
	exit(EXIT_FAILURE);
}
}
int main()
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	char SendBuffer[BUFLEN];
	WSADATA wsa;
	int RequestIdAns, RequestIdReq, CommandNumberReq, ServiceReqLoad,BlockNumberReq;

	slen = sizeof(si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");

	//keep listening for data
	while (1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, ' ', BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		

		//Robot Control/Status Reading
		for (int i = 0; i < 40; ++i) {
			std::cout << std::hex << (int)buf[i];
		}

		RequestIdReq = buf[11];
		CommandNumberReq = buf[24];
		BlockNumberReq = buf[16];
		ServiceReqLoad = buf[29];

		
		if (CommandNumberReq == 0x72) {
		int RequestIdAns = RequestIdReq;
			
			//YERC
			SendBuffer[0] = 0x59;       
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Reserve2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;

			//Service
			SendBuffer[24] = 0x81;

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//DataDivision		
			SendBuffer[32] = 0xFF;
			SendBuffer[33] = 0x00;
			SendBuffer[34] = 0x00;
			SendBuffer[35] = 0x00;
			SendBuffer[36] = 0xFF;
			SendBuffer[37] = 0x00;
			SendBuffer[38] = 0x00;
			SendBuffer[39] = 0x00;

			Send(s, SendBuffer, recv_len, si_other, slen);
		}

		

		//Robot Control/Data Writing to Register
		if (CommandNumberReq == 0x79) {
			RequestIdAns = RequestIdReq;
			//YERC
			SendBuffer[0] = 0x59;    
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Res2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;
			//Service
			SendBuffer[24] = 0x82;

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//DataDivision
			SendBuffer[32] = 0xFF;
			SendBuffer[33] = 0x00;
			SendBuffer[34] = 0x00;
			SendBuffer[35] = 0x00;
			SendBuffer[36] = 0xFF;
			SendBuffer[37] = 0x00;
			SendBuffer[38] = 0x00;
			SendBuffer[39] = 0x00;

			Send(s, SendBuffer, recv_len, si_other, slen);
		}


	//Read Position Data		
		if (CommandNumberReq == 0x75) {
			RequestIdAns = RequestIdReq;
			//YERC
			SendBuffer[0] = 0x59;
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Res2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;

			//Service
			SendBuffer[24] = 0x0E; //????

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//DataDivision
			SendBuffer[32] = 0x00; //Data Type
			SendBuffer[33] = 0x00; //Form
			SendBuffer[34] = 0x00; //Tool Number
			SendBuffer[35] = 0x00; //User Coordinate
			SendBuffer[36] = 0x00; //Extended form
			SendBuffer[37] = 0x00; //First Axis
			SendBuffer[38] = 0x00; //Second Axis
			SendBuffer[39] = 0x00; //Third Axis
			SendBuffer[40] = 0x00; //Fourth Axis
			SendBuffer[41] = 0x00; //Fifth Axis
			SendBuffer[42] = 0x00; //Sixth Axis
			SendBuffer[43] = 0x00; //Seventh Axis
			SendBuffer[44] = 0x00; //Eigth' Axis
			

			Send(s, SendBuffer, recv_len, si_other, slen);

			//It works!!
		}

		//Start Job Command
		if (CommandNumberReq == 0x86) {
			RequestIdAns = RequestIdReq;
			//YERC
			SendBuffer[0] = 0x59;
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Res2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;

			//Service
			SendBuffer[24] = 0x00; //????

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//No DataDivision
			SendBuffer[32] = 0xFF;
			SendBuffer[33] = 0x00;
			SendBuffer[34] = 0x00;
			SendBuffer[35] = 0x00;
			SendBuffer[36] = 0xFF;
			SendBuffer[37] = 0x00;
			SendBuffer[38] = 0x00;
			SendBuffer[39] = 0x00;
	


			Send(s, SendBuffer, recv_len, si_other, slen);

			//It is not working check for other stuff!!
		}

		//Select Job Command
		if (CommandNumberReq == 0x87) {
			RequestIdAns = RequestIdReq;
			//YERC
			SendBuffer[0] = 0x59;
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Res2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;

			//Service
			SendBuffer[24] = 0x0E; //????

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//No DataDivision



			Send(s, SendBuffer, recv_len, si_other, slen);

		}

		//ServoHold
		if (CommandNumberReq == 0x83) {
			RequestIdAns = RequestIdReq;
			//YERC
			SendBuffer[0] = 0x59;
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Res2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;

			//Service
			SendBuffer[24] = 0x0E; //????

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//No DataDivision



			Send(s, SendBuffer, recv_len, si_other, slen);

		}

		//Move Pulse
		if (CommandNumberReq == 0x8B) {
			RequestIdAns = RequestIdReq;
			//YERC
			SendBuffer[0] = 0x59;
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Res2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;

			//Service
			SendBuffer[24] = 0x0E; //????

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//No DataDivision



			Send(s, SendBuffer, recv_len, si_other, slen);

		}
		//Move Cartesian
		if (CommandNumberReq == 0x8C) {
			RequestIdAns = RequestIdReq;
			//YERC
			SendBuffer[0] = 0x59;
			SendBuffer[1] = 0x45;
			SendBuffer[2] = 0x52;
			SendBuffer[3] = 0x43;

			//HeaderSize
			SendBuffer[4] = 0x00;
			SendBuffer[5] = 0x20;

			//DataSize
			SendBuffer[6] = 0x00;
			SendBuffer[7] = 0x00;

			//Reserve1
			SendBuffer[8] = 3;

			//ProcDiv
			SendBuffer[9] = 1;

			//Ack
			SendBuffer[10] = 0x01;

			//RequestId
			SendBuffer[11] = RequestIdAns;

			//AllocateBlock
			SendBuffer[12] = 0x00;
			SendBuffer[13] = 0x00;
			SendBuffer[14] = 0x00;
			SendBuffer[15] = 0x80;


			//Res2
			SendBuffer[16] = 0x39;
			SendBuffer[17] = 0x39;
			SendBuffer[18] = 0x39;
			SendBuffer[19] = 0x39;
			SendBuffer[20] = 0x39;
			SendBuffer[21] = 0x39;
			SendBuffer[22] = 0x39;
			SendBuffer[23] = 0x39;

			//Service
			SendBuffer[24] = 0x0E; //????

			//Status
			SendBuffer[25] = 0x00;


			//AddedStatusSize1
			SendBuffer[26] = 0x00;

			//Padding1
			SendBuffer[27] = 0x00;

			//AddedStatusSize2
			SendBuffer[28] = 0x00;

			//Padding2
			SendBuffer[29] = 0x00;
			SendBuffer[30] = 0x00;
			SendBuffer[31] = 0x00;

			//No DataDivision



			Send(s, SendBuffer, recv_len, si_other, slen);

		}



		//LoadFile

		if (ServiceReqLoad=0x15) {

			if (sizeof(buf) != NULL) {


				if (BlockNumberReq == 0x80) {
					//ACK Request
					RequestIdAns = RequestIdReq;
					//YERC
					SendBuffer[0] = 0x59;
					SendBuffer[1] = 0x45;
					SendBuffer[2] = 0x52;
					SendBuffer[3] = 0x43;

					//HeaderSize
					SendBuffer[4] = 0x00;
					SendBuffer[5] = 0x20;

					//DataSize
					SendBuffer[6] = 0x00;
					SendBuffer[7] = 0x00;

					//Reserve1
					SendBuffer[8] = 3;

					//ProcDiv
					SendBuffer[9] = 2;

					//Ack
					SendBuffer[10] = 0x01;

					//RequestId
					SendBuffer[11] = RequestIdAns;

					//AllocateBlock
					SendBuffer[12] = 0x00;
					SendBuffer[13] = 0x00;
					SendBuffer[14] = 0x00;
					SendBuffer[15] = BlockNumberReq; //Big endian mı small endian mı emin değiliz, bir aksilik çıkarsa kodun burasına bakılabilir. gelen paketin bu kısmını ters çevirmeden kullanmak gerekebilir***

					//Res2
					SendBuffer[16] = 0x39;
					SendBuffer[17] = 0x39;
					SendBuffer[18] = 0x39;
					SendBuffer[19] = 0x39;
					SendBuffer[20] = 0x39;
					SendBuffer[21] = 0x39;
					SendBuffer[22] = 0x39;
					SendBuffer[23] = 0x39;

					//Service
					SendBuffer[24] = 0x95; //????

					//Status
					SendBuffer[25] = 0x00;


					//AddedStatusSize1
					SendBuffer[26] = 0x00;

					//Padding1
					SendBuffer[27] = 0x00;

					//AddedStatusSize2
					SendBuffer[28] = 0x00;

					//Padding2
					SendBuffer[29] = 0x00;
					SendBuffer[30] = 0x00;
					SendBuffer[31] = 0x00;

					//No DataDivision

					Send(s, SendBuffer, recv_len, si_other, slen);
					break;
				}

				//ACK Request
				RequestIdAns = RequestIdReq;
				//YERC
				SendBuffer[0] = 0x59;
				SendBuffer[1] = 0x45;
				SendBuffer[2] = 0x52;
				SendBuffer[3] = 0x43;

				//HeaderSize
				SendBuffer[4] = 0x00;
				SendBuffer[5] = 0x20;

				//DataSize
				SendBuffer[6] = 0x00;
				SendBuffer[7] = 0x00;

				//Reserve1
				SendBuffer[8] = 3;

				//ProcDiv
				SendBuffer[9] = 2;

				//Ack
				SendBuffer[10] = 0x01;

				//RequestId
				SendBuffer[11] = RequestIdAns;

				//AllocateBlock
				SendBuffer[12] = 0x00;
				SendBuffer[13] = 0x00;
				SendBuffer[14] = 0x00;
				SendBuffer[15] = BlockNumberReq;

				//Res2
				SendBuffer[16] = 0x39;
				SendBuffer[17] = 0x39;
				SendBuffer[18] = 0x39;
				SendBuffer[19] = 0x39;
				SendBuffer[20] = 0x39;
				SendBuffer[21] = 0x39;
				SendBuffer[22] = 0x39;
				SendBuffer[23] = 0x39;

				//Service
				SendBuffer[24] = 0x95; //????

				//Status
				SendBuffer[25] = 0x00;


				//AddedStatusSize1
				SendBuffer[26] = 0x00;

				//Padding1
				SendBuffer[27] = 0x00;

				//AddedStatusSize2
				SendBuffer[28] = 0x00;

				//Padding2
				SendBuffer[29] = 0x00;
				SendBuffer[30] = 0x00;
				SendBuffer[31] = 0x00;

				//No DataDivision

				Send(s, SendBuffer, recv_len, si_other, slen);

				

			}

		}

		
	}

	closesocket(s);
	WSACleanup();

	return 0;
}
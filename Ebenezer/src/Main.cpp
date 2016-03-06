/*
*/

#include "stdafx.h"
#include "Define.h"//#include "DefineTest.h"

#include "EbenezerDlg.h"
#include <time.h>

#include "User.h" //#include "UserTest.h"

//-----------------------------------------------------------------------------
int running = 1;
int next_ind = 0;
TCPsocket server_socket;
CUser* clients[MAX_USER]; //CUserTest* clients[MAX_USER];
SDLNet_SocketSet socket_set;

CEbenezerDlg server;

//-----------------------------------------------------------------------------
void SendData(int index, uint8_t* data, uint16_t length, uint16_t flag);

//-----------------------------------------------------------------------------
void CloseSocket(int index) {
	if(clients[index]->m_tcpSocket == NULL) {
		fprintf(stderr, "ER: Attempted to delete a NULL socket.\n");
		return;
	}

	if(SDLNet_TCP_DelSocket(socket_set, clients[index]->m_tcpSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	SDLNet_TCP_Close(clients[index]->m_tcpSocket);
	clients[index]->m_tcpSocket = NULL;

	delete clients[index];
	clients[index] = NULL;

	/*
	uint16_t send_offset = 0;
	uint8_t send_data[MAX_PACKET_SIZE];

	memcpy(send_data+send_offset, &index, sizeof(uint32_t));
	send_offset += sizeof(uint32_t);

	int ind2;
	for(ind2=0; ind2<MAX_USER; ++ind2) {
		if(ind2 == index) continue;
		if(!clients[ind2].in_use) continue;
		SendData(ind2, send_data, send_offset, FLAG_PLAYER_DISCONNECT);
	}
	*/
}

//-----------------------------------------------------------------------------
int AcceptSocket(int index) {
	if(clients[index]) {
		fprintf(stderr, "ER: Overriding socket at index %d.\n", index);
		CloseSocket(index);
	}

	clients[index] = new CUser(&server);//CUserTest(&server);

	clients[index]->m_tcpSocket = SDLNet_TCP_Accept(server_socket);
	if(clients[index]->m_tcpSocket == NULL) return 0;

	if(SDLNet_TCP_AddSocket(socket_set, clients[index]->m_tcpSocket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	return 1;
}

//-----------------------------------------------------------------------------
void SendData(int index, uint8_t* data, uint16_t length, uint16_t flag) {
	if(clients[index]->m_tcpSocket == NULL) return;

	int offset = 0;
	uint8_t temp_data[MAX_PACKET_SIZE];

	memcpy(temp_data+offset, &flag, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	memcpy(temp_data+offset, data, length);
	offset += length;

	int num_sent = SDLNet_TCP_Send(clients[index]->m_tcpSocket, temp_data, offset);
	if(num_sent < offset) {
		fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		CloseSocket(index);
	}
}

//-----------------------------------------------------------------------------
uint8_t* RecvData(int index, uint16_t* length) {
	if(clients[index]->m_tcpSocket == NULL) return NULL;

	uint8_t temp_data[MAX_PACKET_SIZE];
	int num_recv = SDLNet_TCP_Recv(clients[index]->m_tcpSocket, temp_data, MAX_PACKET_SIZE);

	if(num_recv <= 0) {
		CloseSocket(index);

		const char* err = SDLNet_GetError();
		if(strlen(err) == 0) {
			/*
			uint16_t send_offset = 0;
			uint8_t send_data[MAX_PACKET];

			memcpy(send_data+send_offset, &index, sizeof(uint32_t));
			send_offset += sizeof(uint32_t);

			int ind2;
			for(ind2=0; ind2<MAX_USER; ++ind2) {
				if(ind2 == index) continue;
				if(!clients[ind2].in_use) continue;
				SendData(ind2, send_data, send_offset, FLAG_PLAYER_DISCONNECT);
			}
			*/

			printf("DB: client disconnected\n");
		} else {
			fprintf(stderr, "ER: SDLNet_TCP_Recv: %s\n", err);
		}

		return NULL;
	} else {
		*length = num_recv;

		uint8_t* data = (uint8_t*) malloc(num_recv*sizeof(uint8_t));
		memcpy(data, temp_data, num_recv);

		return data;
	}
}

//-----------------------------------------------------------------------------
void ProcessData(int index, uint8_t* data, uint16_t length, uint16_t* offset) {
	if(data == NULL) return;
	if(clients[index]->m_tcpSocket == NULL) return;

	*offset += clients[index]->ReceivedData(data, length);
}

//-----------------------------------------------------------------------------
int SDL_main(int argc, char** argv) {
	// TODO: the multi-threaded stuff need a lot of work...
	/*
	CEbenezerDlg server;

	if(!server.OnInitDialog()) {
		system("pause");
		return -1;
	}

	system("pause"); // NOTE: keeps thread running
	*/

	srand((unsigned int) time(NULL));

	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0) {
		fprintf(stderr, "ER: SDL_Init: %s\n", SDL_GetError());
		exit(-1);
	}

	if(SDLNet_Init() == -1) {
		fprintf(stderr, "ER: SDLNet_Init: %s\n", SDLNet_GetError());
		exit(-1);
	}

	IPaddress ip;
	if(SDLNet_ResolveHost(&ip, NULL, _LISTEN_PORT) == -1) {
		fprintf(stderr, "ER: SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(-1);
	}

	server_socket = SDLNet_TCP_Open(&ip);
	if(server_socket == NULL) {
		fprintf(stderr, "ER: SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(-1);
	}

	socket_set = SDLNet_AllocSocketSet(MAX_USER+1);
	if(socket_set == NULL) {
		fprintf(stderr, "ER: SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
		exit(-1);
	}

	if(SDLNet_TCP_AddSocket(socket_set, server_socket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	// TEMP: doing any server intialization here
	server.GetTimeFromIni();

	if (!server.MapFileLoad()) {
		printf("ER: unable to load map files\n");
		return -1;
	}
	//

	while(running) {
		int num_rdy = SDLNet_CheckSockets(socket_set, 100);

		if(num_rdy <= 0) {
			// NOTE: none of the sockets are ready
			int ind;
			for(ind=0; ind<MAX_USER; ++ind) {
				if(clients[ind]==NULL || clients[ind]->m_tcpSocket==NULL) continue;

				// TODO: maybe do some stuff on each client
			}
		} else {
			if(SDLNet_SocketReady(server_socket)) {
				int got_socket = AcceptSocket(next_ind);
				if(!got_socket) {
					num_rdy--;
					continue;
				}

				// NOTE: get a new index
				int chk_count;
				for(chk_count=0; chk_count<MAX_USER; ++chk_count) {
					if(clients[(next_ind+chk_count)%MAX_USER] == NULL) break;
				}

				next_ind = (next_ind+chk_count)%MAX_USER;
				printf("DB: new connection (next_ind = %d)\n", next_ind);

				num_rdy--;
			}

			int ind;
			for(ind=0; (ind<MAX_USER) && num_rdy; ++ind) {
				if(clients[ind]==NULL || clients[ind]->m_tcpSocket==NULL) continue;
				if(!SDLNet_SocketReady(clients[ind]->m_tcpSocket)) continue;

				uint8_t* data;
				uint16_t length;
				
				data = RecvData(ind, &length);
				if(data == NULL) {
					num_rdy--;
					continue;
				}

				uint16_t offset = 0;
				while(offset < length) {
					uint16_t old_offset = offset;
					ProcessData(ind, data, length, &offset);

					for(int m=old_offset; m<offset; ++m)
						printf("0x%02X ", data[m]);
					printf("\n\n");
				}

				free(data);
				num_rdy--;
			}
		}
	}

	if(SDLNet_TCP_DelSocket(socket_set, server_socket) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_DelSocket: %s\n", SDLNet_GetError());
		exit(-1);
	} SDLNet_TCP_Close(server_socket);

	int i;
	for(i=0; i<MAX_USER; ++i) {
		if(clients[i] == NULL) continue;
		CloseSocket(i);
	}

	SDLNet_FreeSocketSet(socket_set);
	SDLNet_Quit();
	SDL_Quit();

	return 0;
}

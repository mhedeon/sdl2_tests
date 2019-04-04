#include "test.h"

void CloseSocket(int index)
{
	if(sockets[index] == NULL)
		error_log("ER: Attempted to delete a NULL socket.");

	if(SDLNet_TCP_DelSocket(socket_set, sockets[index]) == -1)
		error_log("ER: SDLNet_TCP_DelSocket:");

	memset(&clients[index], 0x00, sizeof(Client));
	SDLNet_TCP_Close(sockets[index]);
	sockets[index] = NULL;
}

int AcceptSocket(int index)
{
	if(sockets[index]) {
		fprintf(stderr, "ER: Overriding socket at index %d.\n", index);
		CloseSocket(index);
	}

	sockets[index] = SDLNet_TCP_Accept(server_socket);
	if(sockets[index] == NULL) return 0;

	clients[index].in_use = 1;
	if(SDLNet_TCP_AddSocket(socket_set, sockets[index]) == -1) {
		fprintf(stderr, "ER: SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
		exit(-1);
	}

	return 1;
}

uint8_t* RecvData(int index, uint16_t* length, uint16_t* flag)
{
	uint8_t temp_data[MAX_PACKET];
	int num_recv = SDLNet_TCP_Recv(sockets[index], temp_data, MAX_PACKET);

	if(num_recv <= 0) {
		CloseSocket(index);
		const char* err = SDLNet_GetError();
		if(strlen(err) == 0) {
			printf("DB: client disconnected\n");
		} else {
			fprintf(stderr, "ER: SDLNet_TCP_Recv: %s\n", err);
		}

		return NULL;
	} else {
		int offset = 0;
		*flag = *(uint16_t*) &temp_data[offset];
		offset += sizeof(uint16_t);

		*length = (num_recv-offset);

		uint8_t* data = (uint8_t*) malloc((num_recv-offset)*sizeof(uint8_t));
		memcpy(data, &temp_data[offset], (num_recv-offset));

		return data;
	}
}

void SendData(int index, uint8_t* data, uint16_t length, uint16_t flag)
{
	uint8_t temp_data[MAX_PACKET];

	int offset = 0;
	memcpy(temp_data+offset, &flag, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	memcpy(temp_data+offset, data, length);
	offset += length;

	int num_sent = SDLNet_TCP_Send(sockets[index], temp_data, offset);
	if(num_sent < offset) {
		fprintf(stderr, "ER: SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		CloseSocket(index);
	}
}

int main()
{
	t_window *win = create_win("test TCP", 500, 500);

	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0)
		return (error_log("ER: SDL_Init:"));
	if(SDLNet_Init() == -1)
		return (error_log("ER: SDLNet_Init:"));

	IPaddress ip;
	if(SDLNet_ResolveHost(&ip, NULL, 8069) == -1)
		return (error_log("ER: SDLNet_ResolveHost:"));

	server_socket = SDLNet_TCP_Open(&ip);
	if(server_socket == NULL)
		return (error_log("ER: SDLNet_TCP_Open:"));

	socket_set = SDLNet_AllocSocketSet(MAX_SOCKETS + 1);
	if (socket_set == NULL)
		return (error_log ("ER: SDLNet_AllocSocketSet:"));

	if(SDLNet_TCP_AddSocket(socket_set, server_socket) == -1)
		return (error_log ("ER: SDLNet_TCP_AddSocket:"));

	int running = 1;
	while(running)
	{
		int num_rdy = SDLNet_CheckSockets(socket_set, 1000);

		if(num_rdy <= 0)
		{
			int ind;
			for(ind=0; ind<MAX_SOCKETS; ++ind)
			{
				if (!clients[ind].in_use) continue ;
				clients[ind].amt_wood += 4;
			}
		}
		else
		{
			if(SDLNet_SocketReady(server_socket))
			{
				int got_socket = AcceptSocket(next_ind);
				if(!got_socket) {
					num_rdy--;
					continue;
				}

				// NOTE: get a new index
				int chk_count;
				for(chk_count=0; chk_count<MAX_SOCKETS; ++chk_count) {
					if(sockets[(next_ind+chk_count)%MAX_SOCKETS] == NULL) break;
				}

				next_ind = (next_ind+chk_count)%MAX_SOCKETS;
				printf("DB: new connection (next_ind = %d)\n", next_ind);

				num_rdy--;
			}
		}

		int ind;
		for(ind=0; (ind<MAX_SOCKETS) && num_rdy; ++ind)
		{
			if(sockets[ind] == NULL) continue;
			if(!SDLNet_SocketReady(sockets[ind])) continue;

			uint8_t* data;
			uint16_t flag;
			uint16_t length;
						
			data = RecvData(ind, &length, &flag);
			if(data == NULL)
			{
				num_rdy--;
				continue;
			}

			switch(flag)
			{
				case FLAG_WOOD_UPDATE:
				{
					uint16_t offset = 0;
					uint8_t send_data[MAX_PACKET];

					memcpy(send_data+offset, &clients[ind].amt_wood, sizeof(uint8_t));
					offset += sizeof(uint8_t);
					SendData(ind, send_data, offset, FLAG_WOOD_UPDATE);
					break;
				}
				case FLAG_QUIT:
				{
					running = 0;
					printf("DB: shutdown by client id: %d\n", ind);
					break;
				}
			}
			free(data);
			num_rdy--;
		}
		SDL_Event e;
		while (SDL_PollEvent(&e))
			if ((KEY == SDLK_ESCAPE) || e.type == SDL_QUIT)
				running = 0;

	}

	if(SDLNet_TCP_DelSocket(socket_set, server_socket) == -1)
		return (error_log ("ER: SDLNet_TCP_DelSocket:"));
	SDLNet_TCP_Close(server_socket);

	int i;
	for(i=0; i<MAX_SOCKETS; ++i)
	{
		if(sockets[i] == NULL) continue;
		CloseSocket(i);
	}

	SDLNet_FreeSocketSet(socket_set);
	SDLNet_Quit();
	SDL_Quit();
	// system("leaks test");
	return (0);
}
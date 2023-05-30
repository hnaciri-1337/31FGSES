#include "teamInfo.hpp"

const int BUF_SIZE = 42 * 4096;
int max = 0;
fd_set fds, wfds, rfds;
char bufRead[BUF_SIZE];

void fatal_error()
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

int  create_socket(const char* host, const char *port)
{
	struct addrinfo	hints;
	int				socket_listen;
	int				opt = 1;
	struct addrinfo	*bind_address; 

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	getaddrinfo(host, port, &hints, &bind_address);
	socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol); 

	if (socket_listen == -1)
			fatal_error ();
	if (setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
			fatal_error ();
		if(setsockopt(socket_listen, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt))<0)
			fatal_error ();
	if (bind(socket_listen,bind_address->ai_addr, bind_address->ai_addrlen)) 
			fatal_error ();
	freeaddrinfo(bind_address);
	if (listen(socket_listen, 300) < 0)
			fatal_error ();

	return (socket_listen);
}

int main(int ac, char **av)
{
	std::ofstream outputFile("teamsData.txt", std::ios::app);
	if (!outputFile)
		outputFile.open("teamsData.txt");
	int sockfd = create_socket ("172.20.186.106", "4000");
	if (sockfd < 0)
		fatal_error();
	FD_ZERO(&fds);
	max = sockfd;
	FD_SET(sockfd, &fds);
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	addr.sin_family = AF_INET;
	std::cout << "Server running on 172.20.186.106:4000" << std::endl;
	while (1)
	{
		wfds = rfds = fds;
		if (select(max + 1, &wfds, &rfds, NULL, NULL) < 0)
			continue ;
		for (int s = 0; s <= max; s++)
		{
			if (FD_ISSET(s, &wfds) && s == sockfd)
			{
				int clientSock = accept(sockfd, (struct sockaddr *)&addr, &addr_len);
				if (clientSock < 0)
					continue ;
				max = (clientSock > max) ? clientSock : max;
				FD_SET(clientSock, &fds);
				break ;
			}
			if (FD_ISSET(s, &wfds) && s != sockfd)
			{
				int res = recv(s, bufRead, 42 * 4096, 0);
				bufRead[res] = 0;
				if (!strncmp (bufRead, "stop", 4))
					return 0;
				std::cout << "|" << bufRead << "|";
				outputFile << bufRead;
				outputFile.flush();
				FD_CLR(s, &fds);
				close(s);
			}
		}
	}
	return 0;
}

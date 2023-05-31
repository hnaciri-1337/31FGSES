#include "teamInfo.hpp"

const int BUF_SIZE = 42 * 4096;

std::map <int, teamInfo>	clients;
int max = 0;
fd_set fds, wfds, rfds;
char bufRead[BUF_SIZE], bufWrite[BUF_SIZE];

#define CLEAR_TERMINAL "\033[2J\033[1;1H"

void fatal_error()
{
	write(2, "Fatal error\n", 12);
	exit(1);
}

void send_client(int s)
{
	send(s, bufWrite, strlen(bufWrite), 0);
}

int  create_socket(const char* host, const char *port)
{
	struct addrinfo	hints;
	int				socket_listen;
	int				opt = 1;
	struct addrinfo	*bind_address; 

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // ipv4
	hints.ai_socktype = SOCK_STREAM; // Use tcp
	hints.ai_flags = AI_PASSIVE; // for getaddrinfo() to generates an address for bind
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

void	getClients ()
{
	std::ifstream inputFile("teamsData.txt");
	int i = 0;
	while (!inputFile.eof ())
	{
		std::string	m;
		std::getline (inputFile, m);
		if (m.empty () || m[0] == ' ' || m[0] == '\n')
			break ;
		int	membersNumber = stoi (m);
		std::string					teamName;
		std::string					teamId;
		std::vector <std::string>	names (membersNumber);
		std::getline (inputFile, teamName);
		for (int i = 0; i < membersNumber; i++)
			std::getline (inputFile, names[i]);
		std::getline (inputFile, teamId);
		clients.insert (std::make_pair (stoi(teamId), teamInfo (teamName, names, teamId)));		
	}
	inputFile.close ();
}

void updateClients ()
{
	std::ifstream inputFile("File.logs");
	int i = 0;
	while (!inputFile.eof ())
	{
		std::string	m;
		std::getline (inputFile, m);
		if (m.empty () || m[0] == ' ' || m[0] == '\n')
			break ;
		int	teamId = stoi (m);
		std::getline (inputFile, m);
		int	level = stoi (m);
		auto it = clients.find (teamId);
		if (it == clients.end ())
			continue ;
		it->second.level = level;
	}
	inputFile.close ();
}

int main(int ac, char **av)
{
	getClients ();
	updateClients ();
	std::ofstream logsFd("File.logs");
	int sockfd = create_socket ("10.11.2.10", "1337");
	if (sockfd < 0)
		fatal_error();
	FD_ZERO(&fds);
	max = sockfd;
	FD_SET(sockfd, &fds);
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	addr.sin_family = AF_INET;
	std::cout << "Server running on 10.11.2.10:1337" << std::endl;
	while (1)
	{
		wfds = rfds = fds;
		if (select(max + 1, &wfds, &rfds, NULL, NULL) < 0)
			continue ;
		if (FD_ISSET(sockfd, &wfds))
		{
			int clientSock = accept(sockfd, (struct sockaddr *)&addr, &addr_len);
			if (clientSock < 0)
				continue ;
			max = (clientSock > max) ? clientSock : max;
			std::map<int, teamInfo>::iterator	it = clients.find(addr.sin_addr.s_addr);
			if (it == clients.end ()) {
				sprintf(bufWrite, "%s\033[0;31mHello Team, sorry but it look like you do not authentificate at time\n\033[0;37m", CLEAR_TERMINAL);
				send_client(clientSock);
				continue ;
			}
			it->second.socket = clientSock;
			FD_SET(clientSock, &fds);
			if (it->second.level < it->second._gameData.size ())
				sprintf(bufWrite, "%sHi %s\nWelcome to the tresor game\nLEVEL %d:\n\033[0;33m%s\n\033[0;37m", CLEAR_TERMINAL, it->second.teamName.c_str(), it->second.level + 1, it->second._gameData[it->second.level].first.c_str());
			else
				sprintf(bufWrite, "%sHi %s\n\033[0;32mCongratulation you win Click on the link to tell everyOne that you success\nLink : https://www.youtube.com/watch?v=dQw4w9WgXcQ&ab_channel=RickAstley\n\033[0;37m", CLEAR_TERMINAL, it->second.teamName.c_str());
			send_client(clientSock);
			continue ;
		}
		for (auto &_it : clients)
		{
			int id = _it.first;
			teamInfo &client = _it.second;
			if (client.socket > 0 && FD_ISSET(client.socket, &wfds) && client.socket != sockfd)
			{
				int res = recv(client.socket, bufRead, 42 * 4096, 0);
				if (res <= 0)
				{
					FD_CLR(client.socket, &fds);
					close(client.socket);
					client.socket = -1;
					break ;
				}
				else
				{
					if (client.level != client._gameData.size ())
					{
						std::string	_ans = std::string (bufRead, res);
						_ans.pop_back ();
						if (client._gameData[client.level].second == _ans)
						{
							client.level += 1;
							logsFd << id << "\n" << client.level << "\n";
							logsFd.flush();
							if (client.level < client._gameData.size ())
								sprintf(bufWrite, "%s\033[0;32mCongratulations\n\033[0;37mLEVEL %d:\n\033[0;33m%s\n\033[0;37m", CLEAR_TERMINAL, client.level + 1, client._gameData[client.level].first.c_str());
						}
						else
						{
							client.wrongAnswer++;
							if (client.wrongAnswer % 3 == 0)
							{
								sprintf(bufWrite, "\033[0;32m%sCongratulation You Win\n\033[0;37m And this is the real password pass it to fgses program '%s'\n", CLEAR_TERMINAL, client.level + 1, client._gameData[client.level].first.c_str(), wrongPassword[++i]);
							}
							sprintf(bufWrite, "%s\033[0;31mOps.. Wrong answer\n\033[0;37mLEVEL %d:\n\033[0;33m%s\n\033[0;37m", CLEAR_TERMINAL, client.level + 1, client._gameData[client.level].first.c_str());
						}
					}
					if (client.level >= client._gameData.size ())
							sprintf(bufWrite, "\033[0;32m%sCongratulation You Win\n\033[0;37m And this is the real password pass it to fgses program 'FGSESX1337'\n", CLEAR_TERMINAL);
					send_client (client.socket);
					break;
				}
			}
		}
	}
	return 0;
}

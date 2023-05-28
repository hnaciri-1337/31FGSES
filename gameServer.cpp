#include "teamInfo.hpp"

const int BUF_SIZE = 42 * 4096;

std::map <int, teamInfo>	clients;
int max = 0;
fd_set fds, wfds, rfds;
char bufRead[BUF_SIZE], bufWrite[BUF_SIZE];

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
	int	membersNumber;

	while (inputFile >> membersNumber)
	{
		std::string					teamName;
		std::string					teamId;
		std::vector <std::string>	names (membersNumber);
		inputFile >> teamName;
		for (int i = 0; i < membersNumber; i++)
			inputFile >> names[i];
		inputFile >> teamId;
		clients.insert (std::make_pair (stoi(teamId), teamInfo (teamName, names)));		
	}
	inputFile.close ();
}

int main(int ac, char **av)
{	
	getClients ();
	int sockfd = create_socket ("10.11.1.7", "1337");
	if (sockfd < 0)
		fatal_error();
	FD_ZERO(&fds);
	max = sockfd;
	FD_SET(sockfd, &fds);
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	addr.sin_family = AF_INET;
	std::cout << "Server running on 10.11.1.7:1337" << std::endl;
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
				std::map<int, teamInfo>::iterator	it = clients.find(addr.sin_addr.s_addr);
				if (it == clients.end ())
				{
					sprintf(bufWrite, "Hello Team, sorry but it look like you do not authentificate at time\n");
					send_client(clientSock);
					break ;
				}
				it = clients.find(addr.sin_addr.s_addr);
				FD_SET(clientSock, &fds);
				sprintf(bufWrite, "Hi %s\nWelcome to the tresor game\nPlease save your id\nYou are in the %d level and this is your task\n%s\n", it->second.teamName.c_str (), it->second.level, it->second._gameData[it->second.level].first.c_str());
				send_client(clientSock);
				break ;
			}
			if (FD_ISSET(s, &wfds) && s != sockfd)
			{
				int res = recv(s, bufRead, 42 * 4096, 0);
				if (res <= 0)
				{
					FD_CLR(s, &fds);
					close(s);
					break ;
				}
				else
				{
					int	id = atoi (bufRead);
					std::map<int, teamInfo>::iterator	it = clients.find(id);
					if (it == clients.end ())
						sprintf(bufWrite, "it seems like you forget your id\nYou should disconnect from the server and connect again to restore it\nAnyway this is the correct way to submit your answer\nid answer\n");
					else
					{
						std::string	_ans = std::string (bufRead, res);
						_ans.pop_back ();
						_ans = _ans.substr (_ans.find_first_of(' ') + 1);
						if (it->second._gameData[it->second.level].second == _ans)
						{
							it->second.level += 1;
							if (it->second.level == 5)
								sprintf (bufWrite, "Congratulation you win Click on the link to tell everyOne that you success\nLink : https://www.youtube.com/watch?v=dQw4w9WgXcQ&ab_channel=RickAstley\n");
							else
								sprintf(bufWrite, "server: Hello id : %d\nCongratulations you pass to the %d level and this is your task\n%s\n", addr.sin_addr.s_addr, it->second.level, it->second._gameData[it->second.level].first.c_str());
						}
						else
							sprintf(bufWrite, "server: Hello id : %d\nOps.. Wrong answer You still in the %d level and this is your task\n%s\n", addr.sin_addr.s_addr, it->second.level, it->second._gameData[it->second.level].first.c_str());
					}
					send_client (s);
					break;
				}
			}
		}
	}
	return 0;
}

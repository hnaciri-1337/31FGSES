#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using	namespace std;

char* rot13(char* str) {
    if (str == NULL) {
        return NULL;
    }
    char* result = (char*)malloc(strlen(str) + 1);
    if (result == NULL) {
        return NULL;
    }

    char* ptr = result;
    while (*str != '\0') {
        if ((*str >= 'A' && *str <= 'M') || (*str >= 'a' && *str <= 'm')) {
            *ptr = *str + 13;
        } else if ((*str >= 'N' && *str <= 'Z') || (*str >= 'n' && *str <= 'z')) {
            *ptr = *str - 13;
        } else {
            *ptr = *str;
        }
        ptr++;
        str++;
    }
    *ptr = '\0';
    return result;
}

void	createPasswords ()
{
	int	fd;
	fd = mkdir ("FollowMe", 0777);
	fd = open ("FollowMe/TeamPassword.txt", O_WRONLY | O_CREAT, 0777);
	write (fd, "There is no password here", strlen ("There is no password here"));
	close (fd);
	mkdir ("FollowMe/FollowMe", 0777);
	fd = open ("FollowMe/FollowMe/TeamPassword.txt", O_WRONLY | O_CREAT, 0777);
	write (fd, "There is no password here", strlen ("There is no password here"));
	close (fd);
	mkdir ("FollowMe/FollowMe/FollowMe", 0777);
	mkdir ("FollowMe/FollowMe/FollowMe/FollowMe", 0777);
	fd = open ("FollowMe/FollowMe/FollowMe/FollowMe/TeamPassword.txt", O_WRONLY | O_CREAT, 0777);
	write (fd, "There is no password here", strlen ("There is no password here"));
	close (fd);
}

void	generateRot13 (char *teamName) {
	int	fd;
	char	*encrypted = rot13 (teamName);
	fd = mkdir ("Agora", 0777);
	fd = open ("Agora/.rot13.txt", O_WRONLY | O_CREAT, 0777);
	write (fd, "Imagine you have a secret code that can make your messages look like a jumbled mess!\nWell, that's what ROT13 does.\nIt's a special code that takes each letter in your message and replaces it with another letter from the alphabet.\nBut it's not just any replacement—it's a secret pattern that goes 13 letters ahead.\nLet's say you team name : '", strlen ("Imagine you have a secret code that can make your messages look like a jumbled mess!\nWell, that's what ROT13 does.\nIt's a special code that takes each letter in your message and replaces it with another letter from the alphabet.\nBut it's not just any replacement—it's a secret pattern that goes 13 letters ahead.\nLet's say you team name : '"));
	write (fd, teamName, strlen (teamName));
	write (fd, "' After applying the rot13, it becomes: '", strlen ("' After applying the rot13, it becomes: '"));
	write (fd, encrypted, strlen (encrypted));
	write (fd, "'.\nCool, right? It's like having your very own secret language to share fun messages with your friends!\n", strlen("'.\nCool, right? It's like having your very own secret language to share fun messages with your friends!\n"));
	write (fd, "The key is : Uryyb YVBAFTRRX. Jrypbzr Gb 1337 <3\n", strlen("The key is : Uryyb YVBAFTRRX. Jrypbzr Gb 1337 <3\n"));
	close (fd);
}

string	fromVector (vector <string> const &_vec, string const &teamName, int socket) {
	sockaddr_in	clientAddress;
    socklen_t	addressLength = sizeof(clientAddress);
    getsockname(socket, reinterpret_cast<struct sockaddr*>(&clientAddress), &addressLength);

	string	s = to_string (_vec.size ()) + "\n" + teamName + "\n";
	for (int i = 0; i < _vec.size(); i++)
		s += _vec[i] + "\n";
	createPasswords ();
	generateRot13 (strdup (teamName.c_str()));
	string	teamId = to_string (clientAddress.sin_addr.s_addr);
	int			fd = open ("FollowMe/FollowMe/FollowMe/TeamPassword.txt", O_WRONLY | O_CREAT, 0777);
	write (fd, "Team Password : ", strlen ("Team Password : "));
	write (fd, teamId.c_str (), teamId.size());
	write (fd, "\n", strlen ("\n"));
	close (fd);
	s += teamId + "\n";
	return (s);
}

bool isNumber(const string& str) {
    if (str.empty())
        return (false);
    for (int i = 0; i < str.size(); i++)
        if (!isdigit(str[i]))
            return (false);
    return (true);
}

int main() {
	const char* host = "10.11.13.2";
	int port = 4000;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		cerr << "Failed to create socket." << endl;
		return (1);
	}
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	if (inet_pton(AF_INET, host, &(serverAddress.sin_addr)) <= 0) {
		cerr << "Failed to convert the host address." << endl;
		close(sock);
		return (1);
	}
	if (connect(sock, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
		cerr << "Failed to connect to the server." << endl;
		close(sock);
		return (1);
	}
	while (1) {
		string	count;
		int		teamCount;
		cout << "How many member you are ? ";
		getline(cin,  count);
		if (!isNumber(count)) {
			cout << "Invalid number try again\n";
			continue ;
		}
		teamCount = stoi (count);
		cout << "Cool now enter a name for you team :";
		getline(cin,  count);
		vector <string>	membersNames (teamCount);
		for (int _i = 0; _i < teamCount; _i++) {
			cout << "Enter member " << _i + 1 << " name: ";
			getline(cin,  membersNames [_i]);
		}
		string	confirmation; 
		cout << "Are you sure from the informations (yes, no) :";
		getline (cin, confirmation);
		if (confirmation != "yes")
			continue ;
		char	*request = strdup (fromVector (membersNames, count, sock).c_str ());
		if (send(sock, request, strlen(request), 0) < 0) {
			cerr << "Failed to send the request." << endl;
			close(sock);
			return (1);
		}
		free (request);
		system ("rm -rf register");
    	return (0);
	}
}

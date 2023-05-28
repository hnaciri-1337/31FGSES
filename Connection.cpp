#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string.h>
#include <vector>

using	namespace std;


string	fromVector (vector <string> _vec, string teamName, int socket) {
	sockaddr_in clientAddress{};
    socklen_t addressLength = sizeof(clientAddress);
    getsockname(socket, reinterpret_cast<struct sockaddr*>(&clientAddress), &addressLength);
	string	s = to_string (_vec.size ()) + "\n" + teamName + "\n";
	for (int i = 0; i < _vec.size(); i++)
		s += _vec[i] + "\n";
	s += to_string (clientAddress.sin_addr.s_addr) + "\n";
	return (s);
}

bool isNumber(const std::string& str) {
    if (str.empty()) {
        return (false);
    }
    for (char c : str) {
        if (!std::isdigit(c)) {
            return (false);
        }
    }
    return (true);
}

int main() {
	const char* host = "10.11.1.7";
	int port = 4000;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		cerr << "Failed to create socket." << endl;
		return (1);
	}
	sockaddr_in serverAddress{};
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
		cout << "Are you sure from the informations (yes, no) :";
		string	confirmation; 
		getline (cin, confirmation);
		if (confirmation != "yes")
			continue ;
		char	*request = strdup (fromVector (membersNames, count, sock).c_str ());
		cout << "|" << request << "|" << endl;
		if (send(sock, request, strlen(request), 0) < 0) {
			cerr << "Failed to send the request." << endl;
			close(sock);
			return (1);
		}
		free (request);
		system ("rm -rf client");
    	return (0);
	}
}

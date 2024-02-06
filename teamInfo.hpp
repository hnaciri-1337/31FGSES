#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <fcntl.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <filesystem>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <fstream>

class teamInfo
{
	public:
		int													level;
		std::vector <std::pair<std::string, std::string> >	_gameData;
		std::vector <std::string>							teamMembers;
		std::string											teamName;
		std::string											teamPassWord;
		int													socket;
		int													wrongAnswer;
		teamInfo(std::string teamName, std::vector <std::string> names, std::string teamPassWord) {
			this->level = 0;
			this->wrongAnswer = 0;
			this->teamName = teamName;
			this->teamMembers = names;
			socket = -1;
			this->teamPassWord = teamPassWord;
			setChallenges();
		}
	~teamInfo() {}
	private:
		void setChallenges() {
			_gameData.push_back (std::make_pair ("1 + 1 = ?\n", "2"));
		
			_gameData.push_back (std::make_pair ("Think like Albert Einstein !\n1 + 1 = ?\n", "3"));
		
			_gameData.push_back (std::make_pair ("In the world of computers, any thing is represented using a system called binary, which consists of only two digits: 0 and 1.\nComputers understand these two digits as the building blocks of all numbers and calculations.\nLet's explore how binary addition works.\n1 + 1 = ?\n", "10"));

			_gameData.push_back (std::make_pair ("So what do you think the sum of (11011001011 + 100011101)/10\n", "2024"));

			_gameData.push_back (std::make_pair ("So what do you think the sum of (1000 + 337)/2\n", "10100111001"));

			_gameData.push_back (std::make_pair ("And now 1 + 1 = ?\n", "2"));

			_gameData.push_back (std::make_pair ("Once upon a time, Sarah was designing her website. She placed a captivating image over a crucial button, but it seemed unclickable. What CSS property could Sarah adjust to ensure the button is accessible despite the image overlay?\n", "z-index"));
		
			_gameData.push_back (std::make_pair ("Then she noticed her content aligned awkwardly at the side. Seeking balance, what CSS technique could she employ to effortlessly center her elements horizontally and vertically without specifying exact dimensions? (We expect answer in this way `property:value`\n", "margin:auto"));
			
			_gameData.push_back (std::make_pair ("She encountered a scenario where she needed to ensure a critical piece of code always executed, regardless of success or failure. Which JavaScript block would Jake use to guarantee execution after try and catch blocks?\n", "finally"));

			_gameData.push_back (std::make_pair ("In the vast world of Linux, there lived a wise librarian named Lily.\nLily was the guardian of countless directories and files, each containing valuable knowledge and tales waiting to be discovered.\nOne day, a curious visitor arrived at Lily's library named Alex, eager to explore the hidden treasures within.\nAlex asked, Oh wise Lily, how can I unlock the secrets held within your vast collection?\nLily smiled warmly and replied, Fear not, dear explorer! There is a command bestowed upon us by the Linux deities.\nIt is a command that allows you to peer into the depths of directories and files, revealing their names and sizes.\n", "ls"));

			_gameData.push_back (std::make_pair ("In the vast world of Linux, Alex embarked on a quest.\nTo navigate the maze of directories, Alex whispered in command like magic.\nCan you recall what enabled Alex to easily change directories and discover hidden treasures?\n", "cd"));

			_gameData.push_back (std::make_pair ("In the vast world of Linux, a mysterious command held the power to unravel the secrets hidden within files.\nIt was whispered among the curious explorers as they ventured through the command line kingdom.\nCan you recall the name of this command that, when called, allowed them to peer into the contents of files, unlocking knowledge and revealing the tales concealed within?\n", "cat"));

			_gameData.push_back (std::make_pair ("Now you have the enough knowledge that allow you to find your team Password, Try to find it\n", this->teamPassWord));

			_gameData.push_back (std::make_pair ("To continue Alex adventure and find the biggest treasure.\nAhmed, a virtuous man, guided Alex to the path of the treasure known as \"Agora\".However, despite Ahmed's sincerity, Alex found nothing at that location. What's the magical command that allow Alex to find the hidden treasure (rot13)?\n", "ls -a"));

			_gameData.push_back (std::make_pair ("Alex treasure is a file called rot13, read it\nAnd show me the key that Alex find !\n", "Uryyb STFRF. Jrypbzr Gb 1337 <3"));
						
			_gameData.push_back (std::make_pair ("Alex is really happy and follows the only path he can find.\nAlex walks until he finds a big door that he can't open with the key he found, maybe he should decrypt the key.\n", "Hello LIONSGEEK. Welcome To 1337 <3"));
		}	
};

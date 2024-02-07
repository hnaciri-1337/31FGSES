all:
	c++ -std=c++17 authServer.cpp -o Authentication
	c++ -std=c++17 Connection.cpp -o Registration
	c++ -std=c++17 gameServer.cpp -o GameServer
	c++ -std=c++17 leetFind.cpp -o LeetFind
	c++ -std=c++17 lionGeeks.cpp -o LionGeeks

fclean:
	rm -rf *.txt *.logs Authentication Registration GameServer LeetFind LionGeeks Agora FollowMe

re: fclean all

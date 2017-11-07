#include <iostream>
#include <thread>
#include <conio.h>
#include <boost\asio.hpp>

using namespace boost::asio;

#define FIELD_SIZE 25

struct key
{
	static bool is_pressed(int key_code,char c)
	{
		if		((key_code == 230 || key_code == 150 || key_code == 119 || key_code == 87) && c == 'W') return true;
		else if ((key_code == 228 || key_code == 65  || key_code == 97) && c == 'A')					return true;
		else if ((key_code == 235 || key_code == 155 || key_code == 83 || key_code == 115) && c == 'S') return true;
		else if ((key_code == 162 || key_code == 130 || key_code == 68 || key_code == 100) && c == 'D') return true;

		return false;
	}
};

void print_positions(int *positions)
{
	std::cout << "u1->x = " << positions[0] << " u1->y = " << positions[1] << " | u2->x = " << positions[2] << " u2->y = " << positions[3] << std::endl;
}

////////////////////////struct user////////////////////////////////////
struct user
{
	int x, y;
	char model;

	user(int _x, int _y) : x(_x), y(_y) {};
	user(const user &_user);
	user(const user *_user);
	user();

	void setPosition(int *);
	void setPosition(int _x, int _y);
	void setPosition(int *, int *);
	void setPosition(int *, user &_user);

	int* getPosition();
	void getPosition(int *);
	void getPosition(int &_x, int &_y);

	void printPosition();
	void printPosition(int *);
	void printPosition(int _x, int _y);

	friend std::ostream& operator <<(std::ostream &os, user &_user);
	friend std::ostream& operator <<(std::ostream &os, user *_user);
};

////////////////////////////// user implementation begin ///////////////////////////////////////////

user::user()
{
	x = y = 0;
}

user::user(const user &_user)
{
	x = _user.x;
	y = _user.y;
}

user::user(const user *_user)
{
	x = _user->x;
	y = _user->y;
}


void user::setPosition(int *position)
{
	x = position[0];
	y = position[1];
}

void user::setPosition(int _x, int _y)
{
	x = _x;
	y = _y;
}

int* user::getPosition()
{
	int *position = new int[2];
	position[0] = x;
	position[1] = y;

	return position;
}

void user::getPosition(int *position)
{
	position[0] = x;
	position[1] = y;
}

void user::getPosition(int &_x, int &_y)
{
	_x = x;
	_y = y;
}

void user::printPosition()
{
	std::cout << " x = " << x << " | y = " << y << std::endl;
}

void user::printPosition(int *position)
{
	std::cout << " x = " << position[0] << " | y = " << position[1] << std::endl;
}

void user::printPosition(int _x, int _y)
{
	std::cout << " x = " << _x << " | y = " << _y << std::endl;
}

std::ostream& operator<<(std::ostream &os, user &_user)
{
	os << " x = " << _user.x << " | y = " << _user.y << std::endl;

	return os;
}

std::ostream& operator <<(std::ostream &os, user *_user)
{
	os << " x = " << _user->x << " | y = " << _user->y << std::endl;

	return os;
}

void user::setPosition(int *pos1, int *pos2)
{
	pos1[0] = pos2[0];
	pos1[1] = pos2[1];

}

void user::setPosition(int *pos, user &_user)
{
	pos[0] = _user.x;
	pos[1] = _user.y;
}

void swap_position(int *p1, int *p2)
{
	p1[0] = p2[0];
	p1[1] = p2[1];
	p1[2] = p2[2];
}

////////////////////////////// user implementation end///////////////////////////////////////////
////////////////////////////// end struct user /////////////////////////////////////////////////

////////////////////////////// Class Socket /////////////////////////////////////////////////////

enum SOCKET_STATUS
{
	SOCKET_CONNECTED = 1,
	SOCKET_DISCONNECTED = 0
};

class Socket
{
private:

	ip::tcp::socket *_socket;

	SOCKET_STATUS status;

public:

	Socket(io_service &service);
	Socket(ip::tcp::socket *_socket);
	Socket(Socket &_sock);
	Socket(Socket *_sock);

	~Socket();

	void read(int &, boost::system::error_code &er);

	void write(int *buff, int size_message, boost::system::error_code &er);
	void write(int *buff, boost::system::error_code &er);

	void set_socket(ip::tcp::socket *_socket);

	void connect(ip::tcp::endpoint &_endpoint);

	void close();

	void set_socket_status(SOCKET_STATUS _status);

	SOCKET_STATUS sock_status();

	ip::tcp::socket* get_socket();
};

////////////////////////////// Socket implementation end ///////////////////////////////////////////

Socket::Socket(io_service &service)
{
	_socket = new ip::tcp::socket(service);
}

Socket::Socket(ip::tcp::socket *_sock)
{
	_socket = _sock;
}

Socket::Socket(Socket &_sock)
{
	_socket = _sock.get_socket();
}

Socket::Socket(Socket *_sock)
{
	_socket = _sock->get_socket();
}

Socket::~Socket()
{
	if (_socket)
	{
		this->close();
		delete _socket;
	}
}

void Socket::read(int &i, boost::system::error_code &er)
{
	if (_socket && status)
	{	
		int key_code[1] = { i };
		_socket->read_some(buffer(key_code), er);
		i = key_code[0];
	}
}

void Socket::write(int *buff, int size_buffer, boost::system::error_code &er)
{
	if (_socket && status == SOCKET_CONNECTED)
	{	
		int position[4] = { buff[0],buff[1],buff[2],buff[3] };
		_socket->write_some(buffer(position, size_buffer), er);
	}
}
void Socket::write(int *buff, boost::system::error_code &er)
{	
	if (_socket && status)
	{
		int position[4] = { buff[0],buff[1],buff[2],buff[3]};
		_socket->write_some(buffer(position), er);
	}
}

void Socket::set_socket(ip::tcp::socket *_sock)
{
	_socket = _sock;
}

void Socket::connect(ip::tcp::endpoint &_endpoint)
{
	boost::system::error_code er;

	if (_socket)
	{
		_socket->connect(_endpoint, er);
		if (er)
		{
			std::cout << "Error connection : ip = and port =  " << std::endl;
			return;
		}
	}

	status = SOCKET_CONNECTED;
}

void Socket::close()
{
	_socket->close();
	status = SOCKET_DISCONNECTED;
}

SOCKET_STATUS Socket::sock_status()
{
	return status;
}

void Socket::set_socket_status(SOCKET_STATUS _status)
{
	status = _status;
}

ip::tcp::socket* Socket::get_socket()
{
	return _socket;
}

////////////////////////////// Socket implementation end ///////////////////////////////////////////
////////////////////////////// End Class Socket         ///////////////////////////////////////////

void set_position(int key_code, user *_user)
{
	if		(key::is_pressed(key_code,'W')) _user->y--;
	else if (key::is_pressed(key_code,'S')) _user->y++;
	else if (key::is_pressed(key_code,'D')) _user->x++;
	else if (key::is_pressed(key_code,'A')) _user->x--;

	_user->printPosition();
}

void listen_user(Socket *_socket,user *_user)
{
	int key_code;
	boost::system::error_code er;

	while (1) 
	{
		_socket->read(key_code, er);

		if (er || !_socket->sock_status())
		{
			std::cout << "User disconneted" << std::endl;
			_socket->set_socket_status(SOCKET_DISCONNECTED);
			break;
		}
		set_position(key_code, _user);
	}
}

void write_user(Socket *_socket1,Socket *_socket2,user *u1,user *u2)
{
	boost::system::error_code er;

	while (1)
	{
		int position[4] = {u1->x,u1->y,u2->x,u2->y};
		
		_socket1->write(position,er);

		if (er || !_socket1->sock_status())
		{
			std::cout << "User disconnected" << std::endl;
			_socket1->set_socket_status(SOCKET_DISCONNECTED);
			break;
		}

		_socket2->write(position,er);

		if (er || !_socket2->sock_status())
		{
			std::cout << "User disconnected" << std::endl;
			_socket2->set_socket_status(SOCKET_DISCONNECTED);
			break;
		}
	}
}

class Server
{
private:

public:

	~Server();

	void start(io_service &service);
};

Server::~Server()
{

}

void Server::start(io_service &service)
{	
	std::cout << "Start server" << std::endl;

	ip::tcp::endpoint _endpoint(ip::address_v4::from_string("127.0.0.1"), 1234);
	ip::tcp::acceptor _acceptor(service, _endpoint);

	boost::system::error_code er;

	Socket *_socket1 = new Socket(service);
	_acceptor.accept(*(_socket1->get_socket()),er);
	
	if (er)
	{
		std::cout << "Bad connected...";
		return;
	}

	std::cout << "New user connected" << std::endl;

	Socket *_socket2 = new Socket(service);
	_acceptor.accept(*(_socket2->get_socket()),er);

	if (er)
	{
		std::cout << "Bad connected...";
		return;
	}

	std::cout << "New user connected" << std::endl;

	user *u1, *u2;

	u1 = new user();
	u2 = new user();

	u1->x = u1->y = 5;
	u2->x = u2->y = 10;

	_socket1->set_socket_status(SOCKET_CONNECTED);
	_socket2->set_socket_status(SOCKET_CONNECTED);

	std::thread t1(write_user,_socket1,_socket2,u1,u2);
	t1.detach();

	std::thread t3(listen_user, _socket1, u1);
	t3.detach();
	std::thread t4(listen_user, _socket2, u2);
	t4.detach();


	while (_socket1->sock_status() || _socket1->sock_status())
	{
		//loop
	}

	delete u1, u2;
}

int main()
{
	io_service service;

	Server server;
	server.start(service);

	_getch();

	return 0;
}
#pragma once
#include <vector>
#include "User.h"
#include "Message.h"
#include <string>
#include <unordered_map>
#include "QtSql/QSqlDatabase"
#include <QSqlQuery>
#include <QDebug>

class Database
{
    QSqlDatabase sdb;
    bool open;

public:
    vector<string> getUserList() const;
    string getUserName(int userId) const;
    int searchUserByName(string);
    Database();
	int addUser(string username, string password);
	int checkPassword(string username, string password);
	void addChatMessage(string sender, string);
	bool addPrivateMessage(string sender, string target, string message);
	vector<string> getChatMessages();//показать все сообщения
  vector<Message> getPrivateMessage(int userID = -1);//показать личные сообщения пользователю username

  // method to get all messages

  vector<Message> getAllMessages();

  // methods for blocking users

  bool userLocked(string username);
  void lockUser(string username);
  void unlockUser(string username);
 };

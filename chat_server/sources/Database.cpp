#include "Database.h"
#include "Parsing.h"
#include "sha1.h"
#include <memory>
#include "User.h"
#include "QtSql/QSqlDatabase"
#include <QSqlQuery>
#include <QDebug>

//=========================================================================
//                           Db Constructor
//=========================================================================

Database::Database()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    // sdb.setDatabaseName("C:\\test.db");
    sdb.setDatabaseName("../chat.db");
    open = sdb.open();
};


//=========================================================================
//                           Main Methods
//=========================================================================

int Database::searchUserByName(string username)     // get user id by name
{
  QSqlQuery query;
  string s = "SELECT id FROM users WHERE name = '";
  s += username.c_str();
  s += "'";
  // qDebug() << s.c_str();
  query.exec(s.c_str());
  if(query.next())
  {
      auto id = query.value(0);
      return id.toInt();
  };
  return -1;
}

//=========================================================================

vector<string> Database::getUserList() const        // get the list of user names
{
  vector<string> userList;
  std::string name;
  QSqlQuery query;
  query.exec("SELECT name FROM users");
  while(query.next())
  {
      name = query.value(0).toString().toStdString();
      userList.push_back(name);
  };
  return userList;
}

//=========================================================================

string Database::getUserName(int userId) const      // get user nаme by user id
{
    QSqlQuery query;
    string s = "SELECT name FROM users WHERE id = ";
    s += to_string(userId);
    // qDebug() << s.c_str();
    query.exec(s.c_str());
    if(query.next())
    {
        auto id = query.value(0);
        return id.toString().toStdString();
    };
    return "";
}

//=========================================================================

char str_digest[42];

void sha_Str(Hash& hash)
{
    int k = 0;
    char s[10];
    for (int i = 0; i < 5; i++)
    {
        sprintf(s, "%08x", hash[i]);
        for (int j = 0; j < 8; j++)
        {
            str_digest[k] = s[j];
            k++;
        };
    };
    str_digest[40] = 0;
};

//=========================================================================

int Database::addUser(string username, string password)
{
    QSqlQuery query;
    if (!correctName(username)) return -1;              // check name
    if( searchUserByName(username) != -1)return -2;     // already existing?

    Hash hash = sha1(password);                         // calculate hash
    sha_Str(hash);

    string s = "INSERT INTO users (name, pass, lock) VALUES ('";
    s += username.c_str();
    s += "', '";
    s += str_digest;
    s += "', ";
    s += to_string(0);
    s += ")";
    // qDebug() << s.c_str();
    query.exec(s.c_str());
    int id = searchUserByName(username);
    // qDebug() << id;
    return id;
}

//=========================================================================

int Database::checkPassword(string username, string password)
{
    QSqlQuery query;
    string s = "SELECT id, pass FROM users WHERE name = '";
    s += username.c_str();
    s += "'";
    // qDebug() << s.c_str();
    query.exec(s.c_str());
    if(query.next())                                // if user found
    {
        int id = query.value(0).toInt();            // get user id
        Hash hash = sha1(password);                 // calculate hash
        sha_Str(hash);
        string pass = query.value(1).toString().toStdString();  // get hash string
        if(strcmp(pass.c_str(), str_digest) == 0)return id;     // compare strings
    };
    return -1;
}

//=========================================================================

void Database::addChatMessage(string sender, string text)
{
       QSqlQuery query;
       int sent = searchUserByName(sender); // sender id
       if(sent == -1)return;
       string s = "INSERT INTO messages (sent, rcvd, text) VALUES (";
       s += to_string(sent);
       s += ", -1, '";
       s += text;
       s += "')";
       // qDebug() << s.c_str();
       query.exec(s.c_str());
}

//=========================================================================

bool Database::addPrivateMessage(string sender, string target, string text)
{
    QSqlQuery query;
    int sent = searchUserByName(sender);    // sender id
    if(sent == -1)return false;
    int rcvd = searchUserByName(target);    // target id
    if(rcvd == -1)return false;
    string s = "INSERT INTO messages (sent, rcvd, text) VALUES (";
    s += to_string(sent);
    s += ", ";
    s += to_string(rcvd);
    s += ", '";
    s += text;
    s += "')";
    // qDebug() << s.c_str();
    query.exec(s.c_str());
	return true;
}

//=========================================================================

vector<string> Database::getChatMessages()
{
	vector<string> strings;
    QSqlQuery query;
    query.exec("SELECT * FROM messages WHERE rcvd = -1");
    while(query.next())
    {
        string sent = query.value(1).toString().toStdString();
        string text = query.value(3).toString().toStdString();
        strings.push_back("<" + sent + ">: " + text);
    };
	return strings;
}

//=========================================================================

vector<Message> Database::getPrivateMessage(int userID)
{
    vector<Message> messages;
    // int sender;
    int target;
    string text;
    string writer;
    QSqlQuery query;
    string s = "SELECT sent, rcvd, text FROM messages WHERE ";
    string user = to_string(userID);
    s += " sent = ";
    s += user;
    s += " OR ";
    s += " rcvd = ";
    s += user;
    // qDebug() << s.c_str();
    query.exec(s.c_str());
    while(query.next())
    {
        // sender = query.value(0).toInt();
        target = query.value(1).toInt();
        text = query.value(2).toString().toStdString();
        writer = getUserName(target);
        messages.push_back(Message(writer, target, text));
    };
    return messages;
}

//=========================================================================
//                          Get all messages
//=========================================================================

vector<Message> Database::getAllMessages()
{
    vector<Message>messages;
    int sender;
    int target;
    string text;
    string writer;
    QSqlQuery query;
    query.exec("SELECT * FROM messages");
    while(query.next())
    {
        sender = query.value(1).toInt();
        target = query.value(2).toInt();
        text = query.value(3).toString().toStdString();
        writer = getUserName(sender);
        messages.push_back(Message(writer, target, text));
    };
    return messages;
};

//=========================================================================
//                      Methods to lock and unlock user
//=========================================================================

bool Database::userLocked(string username)
{
    QSqlQuery query;
    string s = "SELECT lock FROM users WHERE name = '";
    s += username.c_str();
    s += "'";
    // qDebug() << s.c_str();
    query.exec(s.c_str());
    if(query.next())
    {
        auto lock = query.value(0).toInt();
        if(lock) return true;
    };
    return false;
};

//=========================================================================

void Database::lockUser(string username)
{
    QSqlQuery query;
    string s = "UPDATE users SET lock = 1 WHERE name = '";
    s += username.c_str();
    s += "'";
    // qDebug() << s.c_str();
    query.exec(s.c_str());
};

//=========================================================================

void Database::unlockUser(string username)
{
    QSqlQuery query;
    string s = "UPDATE users SET lock = 0 WHERE name = '";
    s += username.c_str();
    s += "'";
    // qDebug() << s.c_str();
    query.exec(s.c_str());
};

//=========================================================================

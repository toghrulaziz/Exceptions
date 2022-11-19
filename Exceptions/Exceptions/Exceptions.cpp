#include <iostream>
#include <windows.h>
#include <vector>


using namespace std;


// Database elave olunsun ++
void DeleteUserById(const int& id);//eger bele bir id istifadechi yoxdursa exception atilsin ++
void HashUserDataById(const int& id);//gonderilen id deki istifadechinin butun melumatlari hash olunsun(string olanlar)digerleri qalsin --


// text, line, source, time. //cari vaxti alinmasini internetden arashdirin ++
class Exception {
	string text;
	int line;
	string source;
	string time;
public:

	Exception(string text, int line, string source, string time) {
		SetText(text);
		SetLine(line);
		SetSource(source);
		SetTime(time);
	}

#pragma region Setters
	void SetText(string text) {
		this->text = text;
	}

	void SetLine(int line) {
		this->line = line;
	}

	void SetSource(string source) {
		this->source = source;
	}

	void SetTime(string time) {
		this->time = time;
	}
#pragma endregion


#pragma region Getters
	string GetText()const { return text; }
	int GetLine()const { return line; }
	string GetSource()const { return source; }
	string GetTime()const { return time; }
#pragma endregion

};



class DatabaseException :public Exception {
public:
	DatabaseException(string text, int line, string source, string time) :Exception(text, line, source, time) {}
};



class InvalidArgumentException :public Exception {
public:
	InvalidArgumentException(string text, int line, string source, string time) :Exception(text, line, source, time) {}
};




class User {
	int id;
	static int staticId;
	string username;
	string password;
	string name;
	string surname;
public:

	User() {
		id = staticId++;
	}

	User(string username, string password, string name, string surname)
	{
		id = staticId++;
		SetUsername(username);
		SetPassword(password);
		SetName(name);
		SetSurname(surname);
	}

#pragma region Setters
	void SetUsername(string username) {
		this->username = username;
	}

	void SetPassword(string password) {
		this->password = password;
	}

	void SetName(string name) {
		this->name = name;
	}

	void SetSurname(string surname) {
		this->surname = surname;
	}
#pragma endregion


#pragma region Getters
	int GetId()const { return id; }
	string GetUsername()const { return username; }
	string GetPassword()const { return password; }
	string GetName()const { return name; }
	string GetSurname()const { return surname; }
#pragma endregion


	void Show() const {
		cout << "Id: " << id << "\nUsername: " << username << "\nPassword: " << password << "\nName: " << name << "\nSurname: " << surname << endl << endl;
	}

	friend ostream& operator<<(ostream& out, const User& o) {
		out << "Id: " << o.id << "\nUsername: " << o.username << "\nPassword: " << o.password << "\nName: " << o.name << "\nSurname: " << o.surname << endl << endl;
		return out;
	}

};
int User::staticId = 1;




class Database {
	vector<User> users;
public:

	// void addUser(const User& user); ++

	void AddUser(const User& user) {
		users.push_back(user);
	}




	//User& getUserByUsername(string username); ++

	User& GetUserByUsername(string username) {
		if (!users.empty()) {
			for (int i = 0; i < users.size(); i++) {
				if (users[i].GetUsername() == username) {
					return users[i];
				}
				else
					cout << "This username not found!\n";
			}
		}
		else
			cout << "Database is empty!\n";
	}




	//void updateUser(User& olduser, const User& newuser); ++

	void UpdateUser(User& olduser, const User& newuser) {
		for (int i = 0; i < users.size(); i++) {
			if (users[i].GetId() == olduser.GetId() && users[i].GetName() == olduser.GetName() && users[i].GetPassword() == olduser.GetPassword() && users[i].GetSurname() == olduser.GetSurname() && users[i].GetUsername() == olduser.GetUsername()) {
				users[i] = newuser;
			}
		}
	}


	void Show() {
		for (int i = 0; i < users.size(); i++) {
			cout << users[i];
		}
	}


	void DeleteUserById(const int& id) {
		for (int i = 0; i < users.size(); i++) {
			if (id == users[i].GetId()) {
				users.erase(users.begin() + i);
				return;
			}
		}
		throw new InvalidArgumentException("Id is not valid.", __LINE__, "DeleteUserById()", __TIME__);
	}


	bool CheckUsername(string username) {
		for (int i = 0; i < users.size(); i++) {
			if (users[i].GetUsername() == username) {
				return true;
			}
		}
		return false;
	}



	bool CheckPassword(string password) {
		for (int i = 0; i < users.size(); i++) {
			if (users[i].GetPassword() == password) {
				return true;
			}
		}
		return false;
	}


};



class Registration {
	Database database;
public:
	Registration(const Database& database) {
		this->database = database;
	}


	//eger bu username istifadechi yoxdursa error atsin ++
	//eger username varsa amma password yanlishdirsa error atsin ++
	void SignIn(string username, string password) {
		if (database.CheckUsername(username) == false) {
			throw new DatabaseException("This username is not in the database", __LINE__, "SignIn()", __TIME__);
		}
		if (database.CheckPassword(password) == false) {
			throw new DatabaseException("This password is not in the database", __LINE__, "SignIn()", __TIME__);
		}
	}



	//Eger istifadechi varsa hemen username de throw DatabaseException ++
	//Eger username xarakter sayi 6 dan kichikdirse InvalidArgumentException ++
	//Eger username ilk herf kichikdirse InvalidArgumentException ++
	//Eger password xarakter sayi 6 dan kichikdirse InvalidArgumentException ++
	//Eger name xarakter sayi 4 dan kichikdirse InvalidArgumentException ++
	//Eger surname xarakter sayi 4 dan kichikdirse InvalidArgumentException ++
	void SignUp(string username, string password, string name, string surname) {
		if (database.CheckUsername(username) == true) {
			throw new DatabaseException("This username exists in the database.", __LINE__, "SignUp()", __TIME__);
		}
		if (username.size() < 6) {
			throw new InvalidArgumentException("This username is not valid.", __LINE__, "SignUp()", __TIME__);
		}
		if (username.at(0) > char(97) && username.at(0) < char(122)) {
			throw new InvalidArgumentException("The first character of the username must be capitalized.", __LINE__, "SignUp()", __TIME__);
		}
		if (password.size() < 6) {
			throw new InvalidArgumentException("This password is not valid.", __LINE__, "SignUp()", __TIME__);
		}
		if (name.size() < 4) {
			throw new InvalidArgumentException("This name is not valid.", __LINE__, "SignUp()", __TIME__);
		}
		if (surname.size() < 4) {
			throw new InvalidArgumentException("This surname is not valid.", __LINE__, "SignUp()", __TIME__);
		}
	}
};


class System {
public:
	static void Control() {

		User u = User("Hakunamatata", "HakunaMatata", "Toghrul", "Azizli");
		Database db;
		db.AddUser(u);
		Registration twitter(db);
		try
		{
			twitter.SignIn("Hakunamatata", "HakunaMatata");
			twitter.SignUp("Toghrulaziz", "Togrul123", "Toghrul", "Azizli");
		}
		catch (Exception* ex)
		{
			cout << "Text: " << ex->GetText() << "\nLine: " << ex->GetLine() << "\nSource: " << ex->GetSource() << "\nTime: " << ex->GetTime() << endl;
		}
	}

};

void main() {
	System::Control();


	system("pause");
}
#ifndef ADDRESSBOOK_HPP_
#define ADDRESSBOOK_HPP_

#include "lib_common2.hpp"

namespace nOT {
class AddressBook {
	AddressBook(const string & nymID);

public:
	static AddressBook Load(const string &nymID);
	size_t getCount() { return contacts.size(); };

	bool add(const string & nymName, const string & nymID);
	bool nymExist(const string & nymID) const;
	bool remove(const string & nymID);

	vector<string> getAllNames();

	void display();
	virtual ~AddressBook();

private:
	class Entry {
		string nymName;
	public:
		Entry(const string & name) :
				nymName(name) {
		};

		string toString() { return nymName; };
		static Entry fromString(string strEntry);
	};

	void createDirectory();
	bool loadFromFile();

	void saveContacts();
	void saveContacts(map<string, string> contacts);

	const string ownerNymID;
	string path;
	map<string, string> contacts;
};

class AddressBookStorage {
public:
	static AddressBook Get(const string & nymID);
private:
	static map <string, shared_ptr<AddressBook>> saved;

};


} // nOT



#endif

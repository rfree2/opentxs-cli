#ifndef ADDRESSBOOK_HPP_
#define ADDRESSBOOK_HPP_

#include "lib_common2.hpp"

namespace nOT {
class AddressBook {

public:
	AddressBook(const string & nymID);

	static shared_ptr<AddressBook> Load(const string &nymID);
	size_t getCount() { return contacts.size(); };

	bool add(const string & nymName, const string & nymID);
	bool nymExist(const string & nymID) const;
	string nymGetID(const string & nymName) const;
	bool nymNameExist(const string & nymName) const;
	string nymGetName(const string & id) const;

	bool remove(const string & nymID);
	void removeAll();

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
	static shared_ptr<AddressBook> Get(const string & nymID); ///< returns pointer to address book for specific nym
	static void ForceClear(); ///< remove pointer to addressBook
	static string GetNymName(const string & nymID, const vector<string> & allNymsID);
private:
	static map <string, shared_ptr<AddressBook>> saved; ///< map with pointers to address book
};


} // nOT



#endif

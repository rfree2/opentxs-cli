#ifndef ADDRESSBOOK_HPP_
#define ADDRESSBOOK_HPP_

#include "lib_common2.hpp"

namespace nOT {
class AddressBook {
	AddressBook(const string & nymID);



public:
	static AddressBook Load(const string &nymID);
	size_t getCount() { return contacts.size(); };

	void add(const string & nymName, const string & nymID);
//	void remove();

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
	bool checkExistance(const string & nymID);

	const string ownerNymID;
	string path;
	map<string, string> contacts;
};



} // nOT



#endif

#include "addressbook.hpp"
#include "lib_common3.hpp"
#include "bprinter/table_printer.h"

namespace nOT {

INJECT_OT_COMMON_USING_NAMESPACE_COMMON_3

AddressBook ::AddressBook(const string & nymID) :
		ownerNymID(nymID) {
	_fact("constructor");
	this->path = string(opentxs::OTPaths::AppDataFolder().Get()) + "client_data/" + "addressbook/" + ownerNymID;
	_dbg2("owner: " << ownerNymID);
	_dbg3(path);

	if (!loadFromFile()) {
		createDirectory();
	}
}

shared_ptr<AddressBook> AddressBook::Load(const string &nymID) {
	_info("loading address book for nym: " << nymID);
	shared_ptr<AddressBook> addressBookPointer(nullptr);
	try {
		addressBookPointer = std::make_shared<AddressBook>(nymID);
	} catch (std::exception &e) {
		cout << zkr::cc::fore::lightred << "Can't load address book" << zkr::cc::console << endl;
		_erro(e.what());
	}
	return addressBookPointer;
}

bool AddressBook::loadFromFile() {
	if (!opentxs::OTPaths::PathExists(opentxs::String(path)))
		return false;

	nOT::nUtils::cConfigManager utils;
	utils.Load(path, contacts);

	if (getCount() == 0)
		_warn("Empty address book");

	return true;
}

void AddressBook::createDirectory() {
	_dbg1("creating directory: " << path);
	bool createFolder = true;
	bool created = opentxs::OTPaths::BuildFilePath(path, createFolder);

	if (!created)
		throw "Can't create file!";
}

bool AddressBook::add(const string & nymName, const string & nymID) {
	if (nymExist(nymID)) {
		_warn("This nym: " << nymName << "(" << nymID << ") already exist in address book, aborting");
		return false;
	}
	_info("adding to address book: " << nymName << " (" << nymID << ")");
	AddressBook::Entry entry(nymName);

	std::pair<string, string> contact(nymID, entry.toString());
	nOT::nUtils::cConfigManager utils;

	try {
		contacts.insert(contact);
		utils.SaveStr(path, contact);
		_dbg3("all ok");
	} catch (...) {
		_erro("Problem with saving to file");
		return false;
	}
	return true;
}

bool AddressBook::nymExist(const string &nymID) const {
	auto count = contacts.count(nymID);
	_dbg3("check existance nym: " << nymID << " ->" << count);
	if (count == 0) {
		return false;
	} else if (count == 1)
		return true;
	else {
		throw "duplicate entry in AddressBook!";
		return false;
	}
}

bool AddressBook::nymNameExist(const string & nymName) const {
	return !nymGetID(nymName).empty();
}

string AddressBook::nymGetID(const string & nymName) const {
	for (auto it = contacts.begin(); it != contacts.end(); ++it)
		if (it->second == nymName) {
			_info("nym " << nymName << " exists in addressBook");
			return it->first;
		}
	_info("nym " << nymName << " DOESN'T exist in addressBook");
	return "";
}

string AddressBook::nymGetName(const string & id) const {
	auto it = contacts.find(id);
	if (it == contacts.end())
		return "";
	return it->second;
}

void AddressBook::display() {
	if (getCount() == 0) {
		cout << "Empty address book" << endl;
		return;
	}
	bprinter::TablePrinter tp(&std::cout);
	tp.AddColumn("Nr", 5);
	tp.AddColumn("Nym", 20);
	tp.AddColumn("ID", 40);

	tp.PrintHeader();

	int i = 0;
	for (auto pair : contacts) {
		tp << i << pair.second << pair.first;
		++i;
	}
	tp.PrintFooter();
}

bool AddressBook::remove(const string & nymID) {
	if (!nymExist(nymID)) {
		cout << zkr::cc::fore::yellow << "This nym doesn't exist" << zkr::cc::console << endl;
		_warn("Can't find nym: " << nymID);
		return false;
	}
	auto copyOfConacts = this->contacts;
	try {
		contacts.erase(nymID);
		saveContacts(contacts);
		_info("removing nym: " << nymID << " successfull");
	} catch (...) {
		contacts.clear();
		contacts = copyOfConacts;
		saveContacts(contacts);
		_erro("can't remove nym: " << nymID << ", aborting");
		return false;
	}

	return true;
}

void AddressBook::removeAll() {
	_warn("deleting all entires from address book");
	for(auto nym : contacts)
		remove(nym.first);
}

void AddressBook::saveContacts(map<string, string> contacts) {
	if (!opentxs::OTPaths::PathExists(opentxs::String(path)))
		createDirectory();

	nOT::nUtils::cConfigManager utils;
	try {
		utils.SaveStr(path, contacts);
	} catch (...) {
		_erro("problem with saving to file");
	}
}

void AddressBook::saveContacts() {
	saveContacts(this->contacts);
}

vector<string> AddressBook::getAllNames() {
	vector<string> nyms;
	nyms.reserve(contacts.size());
	for (auto pair : contacts) {
		nyms.push_back(pair.second);
	}
	return nyms;
}

AddressBook::~AddressBook() {
	_fact("DESTRUCTOR: " << ownerNymID);
}

AddressBook::Entry AddressBook::Entry::fromString(string strEntry) {
	AddressBook::Entry entry(strEntry);
	return entry;
}

map<string, shared_ptr<AddressBook>> AddressBookStorage::saved;
vector <string> AddressBookStorage::names = {};

shared_ptr<AddressBook> AddressBookStorage::Get(const string & nymID) {
	try {
		return saved.at(nymID);
	} catch (const std::out_of_range& e) {
		_note("can't find in map, creating the new one for nym: " << nymID);
		auto addressBookPointer = AddressBook::Load(nymID);
		saved.insert(std::pair<string, shared_ptr<AddressBook>>(nymID, addressBookPointer));
		return addressBookPointer;
	}
}

void AddressBookStorage::ForceClear() {
	saved.clear();
}

string AddressBookStorage::GetNymName(const string & nymID, const vector<string> & allNymsID) {
	string nymName = "";
	for (auto ownerNymID : allNymsID) {
		nymName = Get(ownerNymID)->nymGetName(nymID);
		if (!nymName.empty()) {
			_dbg1("ok found nym");
			ForceClear();
			return nymName;
		}
	}
	_dbg1("Nym not found");
	ForceClear();
	return nymName;
}

vector<string> AddressBookStorage::GetAllNames(const vector<string> & allNymsID) {
	using namespace nOT::nUtils::nOper;
	if(names.empty()) {
		_dbg2("empty vetor, load all nyms from beginning");
		for(auto nymID : allNymsID) {
			names = names + Get(nymID)->getAllNames();
		}
		ForceClear();
	} else {
		_dbg2("vector has already nym names, only saved nyms can be changed");
		for(auto pair : saved) {
			names = names + pair.second->getAllNames();
		}
	}
	// removes duplicates
	sort( names.begin(), names.end() );
	names.erase( unique( names.begin(), names.end() ), names.end() );
	_dbg3("names vector: (" << names.size() << ") -> " <<  DbgVector(names));
	return names;
}

}
// nOT

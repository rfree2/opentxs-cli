#include "addressbook.hpp"
#include "lib_common3.hpp"
#include "bprinter/table_printer.h"

namespace nOT {

INJECT_OT_COMMON_USING_NAMESPACE_COMMON_3

AddressBook ::AddressBook(const string & nymID) :
		ownerNymID(nymID) {
	this->path = string(opentxs::OTPaths::AppDataFolder().Get()) + "client_data/" + "addressbook/" + ownerNymID;
	_dbg2("owner: " << ownerNymID);
	_dbg3(path);

	if (!loadFromFile()) {
		createDirectory();
	}
}

AddressBook AddressBook::Load(const string &nymID) {
	try {
		AddressBook addressBook(nymID);
		return addressBook;
	} catch(std::exception &e) {
		cout << zkr::cc::fore::lightred << "Can't load address book" << zkr::cc::console << endl;
		_erro(e.what());
		//return NULL;
	}
}

bool AddressBook::loadFromFile() {
	if (!opentxs::OTPaths::PathExists(opentxs::String(path)))
		return false;

	nOT::nUtils::cConfigManager utils;
	utils.Load(path, contacts);

	if(getCount() == 0)
		cout << "Empty address book";

	return true;
}

void AddressBook::createDirectory() {
	_dbg1("creating directory: " << path);
	bool createFolder = true;
	bool created = opentxs::OTPaths::BuildFilePath(path, createFolder);

	if (!created)
		throw "Can't create file!";
}

void AddressBook::add(const string & nymName, const string & nymID) {
	if(checkExistance(nymID)) {
		_warn("This nym: " << nymName << "(" << nymID << ") already exist in address book, aborting");
		return;
	}
	_info("adding to address book: " << nymName << " (" << nymID << ")");
	AddressBook::Entry entry(nymName);

	std::pair <string, string> contact = std::make_pair(nymID, entry.toString());
	nOT::nUtils::cConfigManager utils;

	try {
		contacts.insert(contact);
		utils.SaveStr(path, contact);
	} catch(...) {
		_erro("Problem with saving to file");
	}
}

bool AddressBook::checkExistance(const string &nymID) {
	auto count = contacts.count(nymID);
	_dbg3(count);
	if(count == 0) return false;
	else if (count == 1) return true;
	else {
		throw "duplicate entry in AddressBook!";
		return false;
	}
}

void AddressBook::display() {
	if(getCount() == 0) return;
	bprinter::TablePrinter tp(&std::cout);
	tp.AddColumn("Nr", 5);
	tp.AddColumn("Nym", 20);
	tp.AddColumn("ID", 40);

	tp.PrintHeader();

	int i=0;
	for (auto pair : contacts) {
		tp << i << pair.second << pair.first;
		++i;
	}
	tp.PrintFooter();

}

AddressBook::~AddressBook() {
	_info("");
}

AddressBook::Entry AddressBook::Entry::fromString(string strEntry) {
	//auto nymVec = nOT::nUtils::SplitString(strEntry);
	AddressBook::Entry entry(strEntry);
	return entry;
}


}
// nOT

#ifndef EXCHANGERATESTABLE_H
#define EXCHANGERATESTABLE_H

#include <string>
#include <fstream>

#include "tools/logger.h"
#include "tools/Constants.h"
#include "tools/json.hpp"
#include "services/ExchangeRatesTable.h"

using json = nlohmann::json;

class ExchangeRatesTable {

private:
	json table;

public:
	ExchangeRatesTable();
	~ExchangeRatesTable();
	int get(std::string currency);
	void saveChanges();
};

#endif // EXCHANGERATESTABLE_H

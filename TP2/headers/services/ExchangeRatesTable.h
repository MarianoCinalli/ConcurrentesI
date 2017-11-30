#ifndef EXCHANGERATESTABLE_H
#define EXCHANGERATESTABLE_H

#include <string>
#include <fstream>

#include "tools/logger.h"
#include "tools/Constants.h"
#include "tools/json.hpp"

using json = nlohmann::json;

class ExchangeRatesTable {

private:
	json table;

public:
	ExchangeRatesTable();
	~ExchangeRatesTable();
	int get(std::string currency);
	void update(std::string currency, int exchangeRate);
	void erase(std::string currency);
	void saveChanges();
};

#endif // EXCHANGERATESTABLE_H
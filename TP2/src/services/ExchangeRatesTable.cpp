#include "services/ExchangeRatesTable.h"

ExchangeRatesTable::ExchangeRatesTable() {
    // Cargo las configuraciones default.
    std::ifstream i("db/exchangeRates.json");
    this->table = json::parse(i);
};

int ExchangeRatesTable::get(int id) {
	std::string key = std::to_string(id);
	int exchangeRate = -1;
    if (this->table.find(key) == this->table.end()) {
        log("ExchangeRatesTable: **error** No se encontro la key: " + key , ERROR);
    } else {
        exchangeRate = this->table[key];
    }
    return exchangeRate;
};

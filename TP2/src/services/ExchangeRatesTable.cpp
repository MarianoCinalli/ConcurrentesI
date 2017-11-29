#include "services/ExchangeRatesTable.h"

ExchangeRatesTable::ExchangeRatesTable() {
    // Cargo las configuraciones default.
    std::ifstream i(EXCHANGE_RATES_DB_FILE);
    this->table = json::parse(i);
};

int ExchangeRatesTable::get(std::string currency) {
	int exchangeRate = -1;
    if (this->table.find(currency) == this->table.end()) {
        log("ExchangeRatesTable: **error** No se encontro la moneda: " + currency , ERROR);
    } else {
        exchangeRate = this->table[currency];
    }
    return exchangeRate;
};

void ExchangeRatesTable::update(std::string currency, int exchangeRate) {
    if (this->table.find(currency) == this->table.end()) {
        log("ExchangeRatesTable: Creando nueva moneda " + currency + " = ", exchangeRate, INFORMATION);
        this->table[currency] = exchangeRate;
    } else {
        log("ExchangeRatesTable: Actualizando moneda " + currency + " = ", exchangeRate, INFORMATION);
        this->table[currency] = exchangeRate;
    }
}

void ExchangeRatesTable::erase(std::string currency) {
    if (this->table.find(currency) == this->table.end()) {
        log("ExchangeRatesTable: **error** No se encontro la moneda: " + currency , ERROR);
    } else {
        log("ExchangeRatesTable: Eliminando " + currency , INFORMATION);
        this->table.erase(currency);
    }
}

void ExchangeRatesTable::saveChanges() {
    // Write prettified JSON to file. With four spaces
    log("ExchangeRatesTable: Saving JSON db", INFORMATION);
    std::ofstream o(EXCHANGE_RATES_DB_FILE, std::ios_base::trunc);
    o << std::setw(4) << this->table << std::endl;
    log("ExchangeRatesTable: JSON saved!", INFORMATION);
}

#include "tools/Messages.h"

#ifndef EXCHANGERATEDAO_H
#define EXCHANGERATEDAO_H

class ExchangeRateDAO {
public:
    ExchangeRateDAO();
    ~ExchangeRateDAO();
    exchangeRate get(int exchangeRateId);
    int update(exchangeRate exchangeRate);
    int create(exchangeRate exchangeRate);
    int remove(int exchangeRateId);
};
#endif // EXCHANGERATEDAO_H
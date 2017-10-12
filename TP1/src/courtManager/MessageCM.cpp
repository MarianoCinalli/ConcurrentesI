#include "courtManager/MessageCM.h"

MessageCM::MessageCM() {
	this->operation = 4;
};

MessageCM::MessageCM(struct messageCourtManager message) {
	this->operation = message.operation;
	this->operationValue = message.operationValue;
};

MessageCM::~MessageCM() {

};

int MessageCM::getOperation() {
	return this->operation;
};

int MessageCM::getOperationValue() {
	return this->operationValue;
};

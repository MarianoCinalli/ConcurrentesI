#include "courtManager/MessageCM.h"

MessageCM::MessageCM() {
	this->operation = 4;
};

MessageCM::MessageCM(struct messageCourtManager message) {
	this->operation = message.operation;
};

MessageCM::~MessageCM() {

};

int MessageCM::getOperation() {
	return this->operation;
};

#ifndef MESSAGECM_H
#define MESSAGECM_H

#include "tools/Messages.h"

class MessageCM {
	private:
		int operation;
		int operationValue;

	public:
		MessageCM();
		MessageCM(struct messageCourtManager message);
		~MessageCM();
		int getOperation();
		int getOperationValue();
};

#endif // MESSAGECM_H

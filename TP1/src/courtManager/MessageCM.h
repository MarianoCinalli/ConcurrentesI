#include "tools/Messages.h"

#ifndef MESSAGECM_H
#define MESSAGECM_H

class MessageCM {

	private:
		int operation;
		int operationValue;

	public:
		MessageCM();
		MessageCM(struct messageCourtManager message);
		~Message();
		int getOperation();
		int getOperationValue();
};

#endif // MESSAGECM_H

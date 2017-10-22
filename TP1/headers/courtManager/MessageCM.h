#ifndef MESSAGECM_H
#define MESSAGECM_H

#include "tools/Messages.h"

class MessageCM {
	private:
		int operation;

	public:
		MessageCM();
		MessageCM(struct messageCourtManager message);
		~MessageCM();
		int getOperation();
};

#endif // MESSAGECM_H

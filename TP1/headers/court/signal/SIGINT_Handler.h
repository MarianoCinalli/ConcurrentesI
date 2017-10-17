#ifndef SIGINT_HANDLER_H_
#define SIGINT_HANDLER_H_

#include <signal.h>
#include <assert.h>
#include "tools/logger.h"
#include "tools/Messages.h"
#include "tools/Constants.h"
#include "court/Message.h"
#include "court/signal/EventHandler.h"
#include "court/Court.h"

class Court;

class SIGINT_Handler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;
		Court* court;

	public:

		SIGINT_Handler(Court* court);

		~SIGINT_Handler();

		virtual int handleSignal(int signum) {
			assert ( signum == SIGINT );
			this->gracefulQuit = 1;
			this->handler_cleanAndExit();
			return 0;
		};

		sig_atomic_t getGracefulQuit() const;

	private:
		void handler_cleanAndExit();

};

#endif /* SIGINT_HANDLER_H_ */

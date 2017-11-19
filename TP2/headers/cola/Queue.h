#ifndef QUEUE_H_
#define QUEUE_H_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "../../headers/tools/logger.h"


class Queue{

	private:
		key_t key;
		int id;
	public:
		Queue(const std::string &file, const char letter);
		~Queue();
		int write(const void* message, size_t size) const;
		int read(const long type, void* buffer, size_t size);
		int destroy()const;
};

#endif /* QUEUE_H_ */
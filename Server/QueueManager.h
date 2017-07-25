#pragma once
#include <queue>
#include "MessageProtocol.h"

class QueueManager {
public:
	static std::queue<request> requests;   // <- Everything read.
	static std::queue<response> responses; // <- Everything to write.
};

std::queue<request> QueueManager::requests = std::queue<request>();
std::queue<response> QueueManager::responses = std::queue<response>();

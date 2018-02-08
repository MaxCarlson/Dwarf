#pragma once
#include <queue>
#include <mutex>

template<typename MSG>
class MessageQueue
{
public:
	void emplace(MSG &&msg)
	{
		std::lock_guard<std::mutex> lock(mutex);
		queue.emplace(msg);
	}

	template<typename F>
	void processAll(F &&func)
	{
		std::lock_guard<std::mutex> lock(mutex);
		while (!queue.empty())
		{
			MSG msg = queue.front();
			queue.pop();
			func(msg);
		}
	}

private:
	std::mutex mutex;
	std::queue<MSG> queue;
};
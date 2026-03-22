#pragma once
#include <chrono>

class Stopwatch
{
public:
	Stopwatch() = default;
	~Stopwatch() = default;
	
	static Stopwatch StartNew();

	void Start();
	void Stop();
	void Restart();

	float GetElapsedMilliseconds() const;

private:
	std::chrono::high_resolution_clock::time_point myStartTime;
	int64_t myElapsedNanoSeconds = 0;
	bool myIsRunning = false;
};


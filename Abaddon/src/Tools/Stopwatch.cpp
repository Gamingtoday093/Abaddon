#include "pch.h"
#include "Stopwatch.h"

using namespace std::chrono;

Stopwatch Stopwatch::StartNew()
{
	Stopwatch sw;
	sw.Start();
	return sw;
}

void Stopwatch::Start()
{
	if (myIsRunning) return;
	myIsRunning = true;
	
	myStartTime = high_resolution_clock::now();
}

void Stopwatch::Stop()
{
	if (!myIsRunning) return;
	myIsRunning = false;

	myElapsedNanoSeconds += (high_resolution_clock::now() - myStartTime).count();
}

void Stopwatch::Reset()
{
	myIsRunning = false;
	myElapsedNanoSeconds = 0;
}

void Stopwatch::Restart()
{
	Reset();
	Start();
}

float Stopwatch::GetElapsedMilliseconds() const
{
	int64_t totalNanoSeconds = myElapsedNanoSeconds;
	if (myIsRunning)
		totalNanoSeconds += (high_resolution_clock::now() - myStartTime).count();

	constexpr float nanoToMilli = 1.0f / float(std::nano::den / std::milli::den);
	return totalNanoSeconds * nanoToMilli;
}

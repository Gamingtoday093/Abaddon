#include "pch.h"
#include "ImGuiLogger.h"

ImGuiLogger::ImGuiLogger() : ImGuiLogger(std::cout.rdbuf()) {}
ImGuiLogger::ImGuiLogger(std::streambuf* aOriginal) : myMirroredBuf(aOriginal) {}

ImGuiLogger::~ImGuiLogger()
{
	std::cout.rdbuf(myMirroredBuf);
}

const std::deque<ImGuiLogger::LogMessage>& ImGuiLogger::GetOutput() const
{
	return myLogBuffer;
}

void ImGuiLogger::ClearOutput()
{
	myLogBuffer.clear();
}

void ImGuiLogger::Bind()
{
	std::cout.rdbuf(this);
}

int ImGuiLogger::overflow(int c)
{
	if (c == EOF) return c;

	if (myLogBuffer.size() == 0 || myLogBuffer.back().myMessage.back() == '\n')
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		
		if (myLogBuffer.size() == MAXIMUM_LOGS)
			myLogBuffer.pop_front();

		myLogBuffer.emplace_back(c, info.wAttributes);
	}
	else
		myLogBuffer.back().myMessage.push_back(c);

	myMirroredBuf->sputc(c);
	return c;
}

std::streamsize ImGuiLogger::xsputn(const char* s, std::streamsize n)
{
	if (myLogBuffer.size() == 0 || myLogBuffer.back().myMessage.back() == '\n')
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

		if (myLogBuffer.size() == MAXIMUM_LOGS)
			myLogBuffer.pop_front();

		myLogBuffer.emplace_back(s, info.wAttributes);
	}
	else
		myLogBuffer.back().myMessage.append(s);

	return myMirroredBuf->sputn(s, n);
}

int ImGuiLogger::sync()
{
	return myMirroredBuf->pubsync();
}

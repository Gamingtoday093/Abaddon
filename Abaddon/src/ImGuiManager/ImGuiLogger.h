#pragma once
#include "ImGui/ImGui.h"
#include <deque>

class ImGuiLogger : public std::streambuf
{
public:
	ImGuiLogger();
	ImGuiLogger(std::streambuf* aOriginal);
	~ImGuiLogger();

	struct LogMessage
	{
		std::string myMessage;
		ImVec4 myColor;

		LogMessage(std::string aMessage, WORD aColor) : myMessage(aMessage) 
		{
			switch (aColor)
			{
				case RED:
					myColor = { 255, 0, 0, 255 };
					break;
				case GREEN:
					myColor = { 0, 255, 0, 255 };
					break;
				case YELLOW:
					myColor = { 255, 255, 0, 255 };
					break;
				case WHITE:
				default:
					myColor = { 255, 255, 255, 255 };
					break;
			};
		}
		LogMessage(char aChar, WORD aColor) : LogMessage::LogMessage(std::string{aChar}, aColor) {}
	};

	const std::deque<ImGuiLogger::LogMessage>& GetOutput() const;
	void ClearOutput();
	void Bind();

protected:
	int overflow(int c) override;
	std::streamsize xsputn(const char* s, std::streamsize n) override;
	int sync() override;

private:
	static const int MAXIMUM_LOGS = 256;
	std::deque<ImGuiLogger::LogMessage> myLogBuffer;
	std::streambuf* myMirroredBuf;
};


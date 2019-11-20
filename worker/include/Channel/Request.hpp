#ifndef MS_CHANNEL_REQUEST_HPP
#define MS_CHANNEL_REQUEST_HPP

#include "common.hpp"
#include "json.hpp"
#include <string>
#include <unordered_map>

using json = nlohmann::json;

namespace Channel
{
	// Avoid cyclic #include problem by declaring classes instead of including
	// the corresponding header files.
	class UnixStreamSocket;

	class Request
	{
	public:
		enum class MethodId
		{
			WORKER_DUMP = 1,
			WORKER_UPDATE_SETTINGS,
			RECORD_START,
			RECORD_STOP,

		};

	private:
		static std::unordered_map<std::string, MethodId> string2MethodId;

	public:
		Request(Channel::UnixStreamSocket* channel, json& jsonRequest);
		virtual ~Request();

		void Accept();
		void Accept(json& data);
		void Error(const char* reason = nullptr);
		void TypeError(const char* reason = nullptr);

	public:
		// Passed by argument.
		Channel::UnixStreamSocket* channel{ nullptr };
		uint32_t id{ 0u };
		std::string method;
		MethodId methodId;
		json internal;
		json data;
		// Others.
		bool replied{ false };
	};
} // namespace Channel

#endif

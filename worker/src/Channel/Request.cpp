#define MS_CLASS "Channel::Request"
// #define MS_LOG_DEV

#include "Channel/Request.hpp"
#include "Logger.hpp"
#include "MediaSoupErrors.hpp"

namespace Channel
{
	/* Class variables. */

	// clang-format off
	std::unordered_map<std::string, Request::MethodId> Request::string2MethodId =
	{
		{ "worker.dump",                     Request::MethodId::WORKER_DUMP                        },
		{ "worker.updateSettings",           Request::MethodId::WORKER_UPDATE_SETTINGS             },
		{ "record.start",                    Request::MethodId::RECORD_START                       },
		{ "record.stop",                     Request::MethodId::RECORD_STOP                        },
	};
	// clang-format on

	/* Instance methods. */

	Request::Request(Channel::UnixStreamSocket* channel, json& jsonRequest) : channel(channel)
	{
		MS_TRACE();

		auto jsonIdIt = jsonRequest.find("id");

		if (jsonIdIt == jsonRequest.end() || !jsonIdIt->is_number_unsigned())
			MS_THROW_ERROR("missing id");

		this->id = jsonIdIt->get<uint32_t>();

		auto jsonMethodIt = jsonRequest.find("method");

		if (jsonMethodIt == jsonRequest.end() || !jsonMethodIt->is_string())
			MS_THROW_ERROR("missing method");

		this->method = jsonMethodIt->get<std::string>();

		auto methodIdIt = Request::string2MethodId.find(this->method);

		if (methodIdIt == Request::string2MethodId.end())
		{
			Error("unknown method");

			MS_THROW_ERROR("unknown method '%s'", this->method.c_str());
		}

		this->methodId = methodIdIt->second;

		auto jsonInternalIt = jsonRequest.find("internal");

		if (jsonInternalIt != jsonRequest.end() && jsonInternalIt->is_object())
			this->internal = *jsonInternalIt;
		else
			this->internal = json::object();

		auto jsonDataIt = jsonRequest.find("data");

		if (jsonDataIt != jsonRequest.end() && jsonDataIt->is_object())
			this->data = *jsonDataIt;
		else
			this->data = json::object();
	}

	Request::~Request()
	{
		MS_TRACE();
	}

	void Request::Accept()
	{
		MS_TRACE();

		MS_ASSERT(!this->replied, "request already replied");

		this->replied = true;

		json jsonResponse = json::object();

		jsonResponse["id"]       = this->id;
		jsonResponse["accepted"] = true;

		this->channel->Send(jsonResponse);
	}

	void Request::Accept(json& data)
	{
		MS_TRACE();

		MS_ASSERT(!this->replied, "request already replied");

		this->replied = true;

		json jsonResponse = json::object();

		jsonResponse["id"]       = this->id;
		jsonResponse["accepted"] = true;

		if (data.is_structured())
			jsonResponse["data"] = data;

		this->channel->Send(jsonResponse);
	}

	void Request::Error(const char* reason)
	{
		MS_TRACE();

		MS_ASSERT(!this->replied, "request already replied");

		this->replied = true;

		json jsonResponse = json::object();

		jsonResponse["id"]    = this->id;
		jsonResponse["error"] = "Error";

		if (reason != nullptr)
			jsonResponse["reason"] = reason;

		this->channel->Send(jsonResponse);
	}

	void Request::TypeError(const char* reason)
	{
		MS_TRACE();

		MS_ASSERT(!this->replied, "request already replied");

		this->replied = true;

		json jsonResponse = json::object();

		jsonResponse["id"]    = this->id;
		jsonResponse["error"] = "TypeError";

		if (reason != nullptr)
			jsonResponse["reason"] = reason;

		this->channel->Send(jsonResponse);
	}
} // namespace Channel

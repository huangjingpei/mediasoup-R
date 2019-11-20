#define MS_CLASS "Worker"
// #define MS_LOG_DEV

#include "Worker.hpp"
#include "DepLibUV.hpp"
#include "Logger.hpp"
#include "MediaSoupErrors.hpp"
#include "Settings.hpp"
#include "Channel/Notifier.hpp"

/* Instance methods. */

Worker::Worker(Channel::UnixStreamSocket* channel) : channel(channel)
{
	MS_TRACE();

	// Set us as Channel's listener.
	this->channel->SetListener(this);

	// Set the signals handler.
	this->signalsHandler = new SignalsHandler(this);

	// Add signals to handle.
	this->signalsHandler->AddSignal(SIGINT, "INT");
	this->signalsHandler->AddSignal(SIGTERM, "TERM");

	// Tell the Node process that we are running.
	Channel::Notifier::Emit(std::to_string(Logger::pid), "running");

	MS_DEBUG_DEV("starting libuv loop");
	DepLibUV::RunLoop();
	MS_DEBUG_DEV("libuv loop ended");
}

Worker::~Worker()
{
	MS_TRACE();

	if (!this->closed)
		Close();
}

void Worker::Close()
{
	MS_TRACE();

	if (this->closed)
		return;

	this->closed = true;

	// Delete the SignalsHandler.
	delete this->signalsHandler;

	// Close the Channel.
	delete this->channel;
}

void Worker::FillJson(json& jsonObject) const
{
	MS_TRACE();

	// Add pid.
	jsonObject["pid"] = Logger::pid;

	// Add routerIds.
	jsonObject["routerIds"] = json::array();
	auto jsonRouterIdsIt    = jsonObject.find("routerIds");


}

void Worker::SetNewRouterIdFromRequest(Channel::Request* request, std::string& routerId) const
{
	MS_TRACE();

	auto jsonRouterIdIt = request->internal.find("routerId");

	if (jsonRouterIdIt == request->internal.end() || !jsonRouterIdIt->is_string())
		MS_THROW_ERROR("request has no internal.routerId");

	routerId.assign(jsonRouterIdIt->get<std::string>());


}


inline void Worker::OnChannelRequest(Channel::UnixStreamSocket* /*channel*/, Channel::Request* request)
{
	MS_TRACE();

	MS_DEBUG_DEV(
	  "Channel request received [method:%s, id:%" PRIu32 "]", request->method.c_str(), request->id);

	switch (request->methodId)
	{

		case Channel::Request::MethodId::RECORD_START:
		{

			/**
				{
					"route": "0x33434",
					"action": "record",
					"encode": [{
							"audio": {
								"enc": "aac",
								"channels": 2,
								"sampleRate": 48000
							}
						},
						{
							"video": {
								"enc": "h264",
								"width": 1280,
								"height": 720
							}
						}
					]
				}
			 */
			break;
		}
		case Channel::Request::MethodId::RECORD_STOP:
		{
			break;
		}

	}
}

inline void Worker::OnChannelRemotelyClosed(Channel::UnixStreamSocket* /*socket*/)
{
	MS_TRACE_STD();

	// If the pipe is remotely closed it means that mediasoup Node process
	// abruptly died (SIGKILL?) so we must die.
	MS_ERROR_STD("channel remotely closed, closing myself");

	Close();
}

inline void Worker::OnSignal(SignalsHandler* /*signalsHandler*/, int signum)
{
	MS_TRACE();

	if (this->closed)
		return;

	switch (signum)
	{
		case SIGINT:
		{
			if (this->closed)
				return;

			MS_DEBUG_DEV("INT signal received, closing myself");

			Close();

			break;
		}

		case SIGTERM:
		{
			if (this->closed)
				return;

			MS_DEBUG_DEV("TERM signal received, closing myself");

			Close();

			break;
		}

		default:
		{
			MS_WARN_DEV("received a non handled signal [signum:%d]", signum);
		}
	}
}

#ifndef MS_RECORD_HANDLER_HPP
#define MS_RECORD_HANDLER_HPP

#include <uv.h>
#include <string>
#include <vector>
#include "json.hpp"
#include "WebSocketClient.hpp"
using json = nlohmann::json;

class RecordClient
{
public:
	enum {
		RECORD_ERROR_OK =0,
		RECORD_ERROR_REQUEST_INVALID,
		RECORD_ERROR_QUTOA_WARNNING,
		RECORD_ERROR_FILE_TOO_BIGGER,
	};
public:
	class Listener
	{
	public:
		virtual ~Listener() = default;

	public:
		virtual void onRecordEvent(int event) = 0;
	};
public:
	int startRecord(std::string routerId, json request);
	void stopRecord(std::string routerId);

public:
	explicit RecordClient(Listener* listener);
	virtual ~RecordClient();

	Listener* listener{ nullptr };


	WebSocketClient *wssClient{ nullptr };

	//list all the libmediasoupclient objects.

};

#endif //MS_RECORD_HANDLER_HPP

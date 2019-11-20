#ifndef MS_WEBSOCKET_CLIENT_HPP
#define MS_WEBSOCKET_CLIENT_HPP

#include <uv.h>
#include <string>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

class WebSocketClient
{

public:
	explicit WebSocketClient();
	virtual ~WebSocketClient();

};

#endif //MS_WEBSOCKET_CLIENT_HPP

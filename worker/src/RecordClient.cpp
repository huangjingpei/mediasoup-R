#include "RecordClient.hpp"

RecordClient::RecordClient(Listener *listener) :
	listener(listener),
	wssClient(new WebSocketClient()){

}

RecordClient::~RecordClient() {
	if (wssClient) {
		delete wssClient;
		wssClient = nullptr;
	}
}



int RecordClient::startRecord(std::string routerId, json request) {

	return 0;
}

void RecordClient::stopRecord(std::string routerId) {

}




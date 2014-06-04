#include "./Communication/include/WebServer.h"
#include "./Data/include/File.h"
#include "./Core/include/Log.h"

#include <time.h>

#ifdef _WIN32
#define snprintf _snprintf
#endif


static int iterate_callback(struct mg_connection *c, enum mg_event ev, void* server_obj) {
	if (ev == MG_POLL && c->is_websocket) {
		OPWebServerMessagesContainer* messagesContainer = (OPWebServerMessagesContainer*)c->callback_param;
		for (i32 i = 0; i < messagesContainer->messageCount; i++) {
			mg_websocket_write(c, 2, (i8*)messagesContainer->messages[i]->Data, messagesContainer->messages[i]->Length);
		}
	}

	return MG_TRUE;
}

static int send_reply(struct mg_connection *conn, OPWebServer* server) {
	if (conn->is_websocket) {
		OPlog("Web Socket Request... %s", conn->content);
		// This handler is called for each incoming websocket frame, one or more
		// times for connection lifetime.
		// Echo websocket data back to the client.
		//i8* data = (i8*)OPalloc(conn->content_len + 1);
		if (conn->content_len == 0 && conn->wsbits == 138) {
			mg_websocket_write(conn, 10, "", 0);
			return MG_TRUE;
		}

		if (conn->wsbits == 136) {
			return MG_FALSE;
		}

		
		conn->content[conn->content_len] = NULL;

		OPstream* str = OPstreamCreate(conn->content_len);
		OPmemcpy(str->Data, conn->content, conn->content_len);

		i8* key = OPreadstring(str);
		OPlog("KEY: %s", key);

		if (OPhashMapExists(server->WebSocketKeys, key)) {
			void(*handler)(OPstream*);
			OPhashMapGet(server->WebSocketKeys, key, (void**)&handler);
			handler(str);
			mg_websocket_write(conn, 2, conn->content, conn->content_len);
		}

		OPstreamDestroy(str);

		return conn->content_len == 4 && !memcmp(conn->content, "exit", 4) ?
			MG_FALSE : MG_TRUE;
		//return MG_FALSE;
	}
	else {
		OPlog("Requested: %s", conn->uri);
		i32 uriLength = strlen(conn->uri);
		if (conn->uri[0] == '/' && uriLength == 1) {
			OPstream* index = OPreadFile("Web/index.html");
			mg_send_header(conn, "Content-Type", "text/html");
			mg_send_data(conn, index->Data, index->Length);
			return MG_TRUE;
		}
		else {
			i8* path = "";
			i32 urisize = strlen(conn->uri);
			i32 size = urisize + strlen(path);

			if (urisize < 1) return MG_TRUE;
			i32 offset = conn->uri[0] == '/';
			i8* filepath = (i8*)OPalloc(sizeof(i8)* size + 1 - offset);

			OPmemcpy(filepath, path, strlen(path));
			OPmemcpy(filepath + strlen(path), conn->uri + offset, strlen(conn->uri) - offset);
			filepath[size - offset] = NULL;

			OPstream* index = OPreadFile(filepath);
			if (index == NULL) return MG_TRUE;

			OPfree(filepath);

			if (OPmemcmp(filepath + size - 3, ".js", 3)) {
				mg_send_header(conn, "Content-Type", "text/javascript");
			}
			else {
				mg_send_header(conn, "Content-Type", "text/html");
			}

			mg_send_data(conn, index->Data, index->Size);
			OPstreamDestroy(index);
			return MG_TRUE;
		}

		return MG_FALSE;
	}
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev, void* server_obj) {
	if (ev == MG_REQUEST) {
		return send_reply(conn, (OPWebServer*)server_obj);
	}
	else if (ev == MG_AUTH) {
		return MG_TRUE;
	}
	else {
		return MG_FALSE;
	}
}


OPWebServer* OPwebServerCreate(OPchar* port) {
	OPWebServer* server = (OPWebServer*)OPalloc(sizeof(OPWebServer));
	server->WebSocketKeys = OPhashMapCreate(16);
	server->WebSocketMessages = OPlistCreate(16, sizeof(OPstream));
	server->Server = mg_create_server(NULL, ev_handler, server);
	mg_set_option(server->Server, "listening_port", port);
	printf("Started on port %s\n", mg_get_option(server->Server, "listening_port"));
	return server;
}

void OPwebServerSocket(OPWebServer* server, i8* key, void(*handler)(OPstream*)) {
	OPhashMapPut(server->WebSocketKeys, key, handler);
}

void OPwebServerUpdate(OPWebServer* server) {
	mg_poll_server(server->Server, 1);

	OPWebServerMessagesContainer* messagesContainer = (OPWebServerMessagesContainer*)OPalloc(sizeof(OPWebServerMessagesContainer));
	messagesContainer->messageCount = OPlistSize(server->WebSocketMessages);
	messagesContainer->messages = (OPstream**)OPalloc(sizeof(OPstream*) * messagesContainer->messageCount);

	for (i32 i = 0; i < messagesContainer->messageCount; i++) {
		messagesContainer->messages[i] = (OPstream*)OPlistPop(server->WebSocketMessages);;
	}

	mg_iterate_over_connections(server->Server, iterate_callback, messagesContainer);

	//for (i32 i = 0; i < messagesContainer->messageCount; i++) {
	//	OPstreamDestroy(messagesContainer->messages[i]);
	//}

	OPfree(messagesContainer->messages);
	OPfree(messagesContainer);
}

void OPwebServerQueue(OPWebServer* server, i8* key, i8* data, ui32 datalen) {
	ui32 size = sizeof(i8) * strlen(key) + datalen;
	OPstream* d = OPstreamCreate(size);
	
	ui32 len = strlen(key);
	OPwrite(d, (ui8*)&len, sizeof(ui32));
	OPwrite(d, key, sizeof(i8) * len);
	OPwrite(d, data, datalen);
	OPseek(d, 0);
	OPlistPush(server->WebSocketMessages, (ui8*)d);
}

void OPwebServerDestroy(OPWebServer* server) {
	mg_destroy_server(&server->Server);
	OPfree(server);
}
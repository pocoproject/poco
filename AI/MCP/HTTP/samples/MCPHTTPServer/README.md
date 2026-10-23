# MCPHTTPServer sample

A minimal [Model Context Protocol](https://modelcontextprotocol.io) server over the Streamable HTTP transport, built on the POCO AI MCP libraries (`Poco::AI::MCP`, `Poco::AI::MCP::HTTP`), protocol revision 2026-07-28. It exposes two trivial tools, `echo` and `ping`, at `http://127.0.0.1:8080/mcp`, and serves as a working reference and a smoke test for the server transport.

## Build

CMake: configure POCO with `-DENABLE_AI_MCP_HTTP=ON -DENABLE_SAMPLES=ON`; the binary is `bin/MCPHTTPServer` in the build directory.

Make: build the libraries with `make -C AI/MCP` and `make -C AI/MCP/HTTP`, and the sample with `make -C AI/MCP/HTTP/samples`; the binary is `AI/MCP/HTTP/samples/MCPHTTPServer/bin/<OS>/<arch>/MCPHTTPServer`.

With shared POCO libraries the binary needs the library directory on the loader path (`LD_LIBRARY_PATH` or `DYLD_LIBRARY_PATH`), or a static build.

## Tools

| Tool   | Arguments                   | Returns               |
|--------|-----------------------------|-----------------------|
| `echo` | `{ "message": "<string>" }` | the same message back |
| `ping` | none                        | the text `pong`       |

## Running

```bash
MCPHTTPServer                      # http://127.0.0.1:8080/mcp
MCPHTTPServer --port 9000          # another port
MCPHTTPServer --host 0.0.0.0       # every interface: the server signs nobody in
MCPHTTPServer --origin http://localhost:3000   # a page that may call it from a browser
MCPHTTPServer --selftest           # serves on a free port, runs the HTTP client against it, exits
```

The server signs no caller in. Keep it on the loopback interface, or put it behind something that does.

A request with an `Origin` header, which a browser sends, is refused with 403 unless the origin is allowed with `--origin`; this keeps the pages of other sites from calling the server. A page of another origin also needs CORS headers to read a reply, which this sample does not send.

### By hand

Every request is a POST of one JSON-RPC message. The protocol is stateless: every request names the protocol revision and the client's capabilities in its `_meta` object, and the headers `MCP-Protocol-Version` and `Mcp-Method`, and `Mcp-Name` for a tool call, repeat what the body says.

```bash
META='"_meta":{"io.modelcontextprotocol/protocolVersion":"2026-07-28","io.modelcontextprotocol/clientCapabilities":{}}'
curl -s http://127.0.0.1:8080/mcp \
  -H 'Content-Type: application/json' -H 'Accept: application/json, text/event-stream' \
  -H 'MCP-Protocol-Version: 2026-07-28' -H 'Mcp-Method: tools/call' -H 'Mcp-Name: echo' \
  -d '{"jsonrpc":"2.0","id":1,"method":"tools/call","params":{"name":"echo","arguments":{"message":"hi"},'"$META"'}}'
```

A header that does not match the body gets 400 with a Header mismatch error (-32020). A request without the `_meta` object gets 400 with an Invalid params error; one of another revision, or an `initialize` request of an earlier one, gets 400 with an Unsupported protocol version error that names the revision the server implements.

## Registering with an MCP client

> [!NOTE]
> The client must implement MCP revision 2026-07-28: it sends `server/discover` and names the revision in the `_meta` object of every request.

A client that supports the Streamable HTTP transport takes the URL of the endpoint. Most clients take a JSON configuration of this shape:

```json
{
  "mcpServers": {
    "poco-mcp-http-sample": { "type": "http", "url": "http://127.0.0.1:8080/mcp" }
  }
}
```

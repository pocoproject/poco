# MCPServer sample

A minimal [Model Context Protocol](https://modelcontextprotocol.io) server built on the POCO AI MCP library (`Poco::AI::MCP`), protocol revision 2026-07-28. It exposes two trivial tools, `echo` and `ping`, over the stdio transport, and serves as a working reference and a smoke test for the library.

## Build

CMake: configure POCO with `-DENABLE_AI_MCP=ON -DENABLE_SAMPLES=ON`; the binary is `bin/MCPServer` in the build directory.

Make: build the library with `make -C AI/MCP` and the sample with `make -C AI/MCP/samples`; the binary is `AI/MCP/samples/MCPServer/bin/<OS>/<arch>/MCPServer`.

With shared POCO libraries the binary needs the library directory on the loader path (`LD_LIBRARY_PATH` or `DYLD_LIBRARY_PATH`), or a static build.

## Tools

| Tool   | Arguments                   | Returns               |
|--------|-----------------------------|-----------------------|
| `echo` | `{ "message": "<string>" }` | the same message back |
| `ping` | none                        | the text `pong`       |

## Running

### Self-test (no client needed)

Drives an in-process client through discovery, the tool list and a tool call:

```bash
MCPServer --selftest
```

### stdio transport

This is a server, not a REPL. It reads newline-delimited JSON-RPC 2.0 messages from stdin and writes one JSON reply per line to stdout. Logs go to stderr, so stdout stays protocol-clean. Plain text such as `hello` is a parse error.

The protocol is stateless: there is no handshake, and every request names the protocol revision and the client's capabilities in its `_meta` object. Drive it by hand, discovery plus a tool call:

```bash
META='"_meta":{"io.modelcontextprotocol/protocolVersion":"2026-07-28","io.modelcontextprotocol/clientCapabilities":{}}'
printf '%s\n' \
  '{"jsonrpc":"2.0","id":1,"method":"server/discover","params":{'"$META"'}}' \
  '{"jsonrpc":"2.0","id":2,"method":"tools/list","params":{'"$META"'}}' \
  '{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"echo","arguments":{"message":"hi"},'"$META"'}}' \
  | MCPServer
```

A request without the `_meta` object gets an Invalid params error; one that names another revision, or an `initialize` request of an earlier revision, gets an Unsupported protocol version error that names the revision the server implements.

## Registering with an MCP client

> [!NOTE]
> The client must implement MCP revision 2026-07-28: it sends `server/discover` and names the revision in the `_meta` object of every request. A client of an earlier revision starts with `initialize`, which this server answers with an Unsupported protocol version error.

A client that supports the stdio transport launches the server as a subprocess and talks to it over its stdin and stdout; the client owns the process lifecycle. Most clients take a JSON configuration of this shape:

```json
{
  "mcpServers": {
    "poco-mcp-sample": { "command": "/absolute/path/to/MCPServer", "args": [] }
  }
}
```

If the client starts the binary outside the shell that sets the loader path, point `command` at a small wrapper script that sets the path and then runs the binary, or use a static build. The tools then appear to the model under the names the client derives from the server name, typically prefixed with it.

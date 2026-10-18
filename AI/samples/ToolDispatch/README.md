# ToolDispatch sample

Tools for a model that cannot call tools itself. `ToolDispatchProvider` wraps an OpenAI-compatible provider: keyword rules match the user's question to tools, the agent loop runs the matched tools, and the wrapped provider answers from their output. When no rule matches, the question goes to the model as it is.

The rules are application policy, which is why this lives in a sample and not in the library: the matching is a case-insensitive ASCII substring search, so `temp` also matches `attempt`, and it does not see negation (`do not restart` matches `restart`). A matched tool runs without the model's judgment. The tool output reaches the model in a delimited data block of the user message, with an instruction to treat it as data.

## Build

CMake: configure POCO with `-DENABLE_AI=ON -DENABLE_SAMPLES=ON`; the binary is `bin/ToolDispatch`. Make: build the libraries, then `make -C AI/samples`.

## Run

```bash
ToolDispatch http://localhost:11434/v1 llama3.2 "What time is it?"
```

The arguments are the base URL of an OpenAI-compatible API (here ollama), the model, and the question. The sample has one tool, `current_time`, matched by the keywords `time` and `clock`. An API key, if the endpoint needs one, comes from the environment variable `OPENAI_API_KEY`.

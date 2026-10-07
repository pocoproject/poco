# The AI library family: the Model Context Protocol core (MCP), its HTTP
# client transport (MCP/HTTP) and the AI library itself, which depends on
# both and is therefore configured after them.
if(ENABLE_AI_MCP)
	add_subdirectory(MCP)
endif()

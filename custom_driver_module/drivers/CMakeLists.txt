# custom_driver_module/drivers/CMakeLists.txt

# Adiciona o subdiretório do driver SOMENTE SE a Kconfig estiver habilitada
add_subdirectory_ifdef(CONFIG_POT_MCP41XXX mcp41xxx)
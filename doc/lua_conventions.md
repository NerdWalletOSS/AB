Convention Guide:

1. Two spaces for indent

# Imports

1. Always import from the base directory 
2. Requires are always top, alphabetical, two quotes
3. Folders are denoted with a slash
	- Edit external packages
4. No dofile. Use Require
5. (TODO) Must 'require "lua/strict"'

# Variables

1. No globals except in dt.lua
2. Constants are always capitalized
3. Everything else in snake_case


# Tests

1. Tests are named test_*.lua
	- No non-test files can start with test_
2. All tests with busted must have line 1 as:
	require 'busted.runner'()


# Files

1. Typically one function to a file
	- Exception: Object-oriented programming
	- One file per helper function
	- For returning multiple functions:
		local x = {}
		function x.foo() ...
		function x.bar() ...
		return x


# Foreign modules

1. If possible to extract source code from a module:
	- Put the folder in /lua
	- Change the require statements
2. If outside dependencies that do not exist are required:
	- edit aio.sh's -t flag:
		sudo luarocks install penlight

# Checking in to GitHub


# Others

1. When using Lua in C:
	- Good: status = luaL_dostring(g_L, "require 'RTS/ab'");
	https://github.com/NerdWalletOSS/AB/blob/7070eb883956753b3c191645b17b227d5a049253/src/init.c#L52

	Reason: what makes this possible is this line:
	export LUA_PATH="`pwd`/?.lua;;"
	https://github.com/NerdWalletOSS/AB/blob/7070eb883956753b3c191645b17b227d5a049253/to_source

	This has to be executed in AB/

	- Bad: status = luaL_dofile(g_L, "./RTS/ab.lua");
	Reason: depends on Lua and C being deployed *together*


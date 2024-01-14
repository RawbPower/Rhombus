rhombus = require "rhombus"

function PrintMessage()
	return "Hello World from Lua!"
end

function PrintInt(value)
	return "Lua says: "..value
end

function PrintInts(value1, value2)
	return "Lua says: "..value1.." and "..value2
end

function PrintCustomMessage(message)
	return "Lua says: "..message
end

function CallHostFunction(a, b)
	c = HostFunction(a + 10, b * 3)
	c = test.Add(4,3)
	return c
end
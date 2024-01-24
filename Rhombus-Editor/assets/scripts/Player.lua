local rhombus = require "rhombus"

KEY_SPACE = 32
KEY_A = 97
KEY_D = 100
KEY_S = 115
KEY_W = 119

function Init()
	Log("Player Start using Log ")
	return "Player Start "
end

function Update(entityID, dt)

	returnString = "Player Update: ".. entityID

	Log(returnString)

	speed = 0.03
	velX = 0
	velY = 0

	if rhombus.IsKeyDown(KEY_A) then
		returnString = "A"
		velX = -1
	elseif rhombus.IsKeyDown(KEY_D) then
		returnString = "D"
		velX = 1
	end

	velX = velX * speed
	velY = velY * speed

	ApplyLinearImpulse(entityID, velX, velY)

	jump = false
	jumpStrength = 0.1

	if rhombus.IsKeyDown(KEY_SPACE) then
		jump = true
	end

	if jump then
		ApplyLinearImpulse(entityID, 0, jumpStrength)
	end

	return returnString
end
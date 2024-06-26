Player = {}

KEY_SPACE = 32
KEY_A = 97
KEY_D = 100
KEY_S = 115
KEY_W = 119

function Player:Init()
	rhombus.Log("Lua Init")
end

function Player:Update(dt)
	rhombus.Log("Lua Update")

	speed = 0.3
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

	rhombus.ApplyLinearImpulse(self, velX, velY)

	jump = false
	jumpStrength = 1

	if rhombus.IsKeyDown(KEY_SPACE) then
		jump = true
	end

	if jump then
		rhombus.ApplyLinearImpulse(self, 0, jumpStrength)
	end
end
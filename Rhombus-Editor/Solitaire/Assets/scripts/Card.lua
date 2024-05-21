Card = {x = 0, y = 0, z = 0, isCardPickedUp = false}

function Card:PositionToString()
	return "(" .. self.x .. "," .. self.y .. "," .. self.z .. ")"
end

function Card:Init()
	rhombus.Log(rhombus.GetName(self) .. " Init")
	x, y, z = rhombus.GetPosition(self)
	rhombus.Log(rhombus.GetName(self) .. " Position: (" .. x .. "," .. y .. "," .. z .. ")")
end

function Card:Update(dt)
	if isCardPickedUp then 
		mousePosX, mousePosY = rhombus.GetMousePosition()
		rhombus.SetPosition(self, mousePosX, mousePosY, 0)
	end
end

function Card:OnMouseButtonPressed(button)
	rhombus.Log(rhombus.GetName(self) .. ": Mouse button " .. button .. " pressed!")
	if rhombus.IsMouseInArea(self) then
		isCardPickedUp = true
	end
end

function Card:OnMouseButtonReleased(button)
	rhombus.Log(rhombus.GetName(self) .. ": Mouse button " .. button .. " released!")
	if isCardPickedUp then 
		isCardPickedUp = false
	end
end
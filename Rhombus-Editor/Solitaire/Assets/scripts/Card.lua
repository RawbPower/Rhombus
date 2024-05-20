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
	if rhombus.IsMouseInArea(self) then
		mousePosX, mousePosY = rhombus.GetMousePosition()
		rhombus.SetPosition(self, mousePosX, mousePosY, 0)
	end
end

function Card:OnMouseEnterArea()
	rhombus.Log("Mouse entered " .. rhombus.GetName(self) .. " card area!")
	isCardPickedUp = true
end

function Card:OnMouseExitArea()
	rhombus.Log("Mouse exited " .. rhombus.GetName(self) .. " card area!")
end
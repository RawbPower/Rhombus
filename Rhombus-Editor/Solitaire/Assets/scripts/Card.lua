Card = {}

function Card:Init()
	rhombus.Log("Card Init")
end

function Card:OnMouseEnterArea()
	rhombus.Log("Mouse entered " .. rhombus.GetName(self) .. " card area!")
end

function Card:OnMouseExitArea()
	rhombus.Log("Mouse exited " .. rhombus.GetName(self) .. " card area!")
end
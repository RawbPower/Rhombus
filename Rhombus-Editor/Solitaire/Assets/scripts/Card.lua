Card = {}

function Card:Init()
	rhombus.Log("Card Init")
end

function Card:OnMouseEnterArea()
	rhombus.Log("Mouse entered card area!")
end

function Card:OnMouseExitArea()
	rhombus.Log("Mouse exited card area!")
end
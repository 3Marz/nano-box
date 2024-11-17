
function _boot()
	print("Initialized!")
	print(peek(0x3001))
end

local x = 0

function _update()
	x = x + 1

	cls(0)
	for i = 1, 10, 1 do
		for j = 1, 10, 1 do
			pxset(i+x, j, 2)
		end
	end
end

function _draw()
end


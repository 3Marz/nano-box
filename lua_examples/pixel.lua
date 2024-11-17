

cls(0)
function _update()
	for i = 0, 6000 do
		x = math.random(192)
		y = math.random(128)

		local color = (time()//1000*x*y)%60

		pxset(x, y, color)
	end
end

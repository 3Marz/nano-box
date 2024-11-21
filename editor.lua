
-- Helper Functions
function string.insert(str, index, value)
	return str:sub(1, index - 1) .. value .. str:sub(index, -1)
end
function string.remove(str, index)
	return str:sub(1, index - 1) .. str:sub(index + 1, -1)
end
function arrowBtns(i)
	if btn(i-1) then
		Arrows[i].heldFor = Arrows[i].heldFor + 1
		if Arrows[i].prev == false then
			Arrows[i].prev = true
			return true
		end
		local heldTooLong = Arrows[i].heldFor > Keyboard.initialDelay
		if heldTooLong then
			Arrows[i].heldFor = Keyboard.initialDelay - Keyboard.repeatDelay
			return true
		end
	else
		Arrows[i].heldFor = 0
		Arrows[i].prev = false
	end
end

Arrows = {
	{ heldFor = 0, prev = false },
	{ heldFor = 0, prev = false },
	{ heldFor = 0, prev = false },
	{ heldFor = 0, prev = false },
}
Keyboard = {
	initialDelay = 20,
	repeatDelay = 2,
	holds = {},
	prev = {}
}
for i = 0, 255 do
	Keyboard.holds[i] = 0
	Keyboard.prev[i] = false
end

Editor = {
	mode = 0,
	code = {
		prevChar = 0,
		repeated = 0,
		maxRepeat = 15,
		yoff = 3,
		row = 1,
		col = 1,
		data = {
			"",
			"local hi = 'Im the greatest'",
			"",
			"function omar(x, y)",
			" print('hello world')",
			"end"
		}
	}
}

function compseString()
	for i, str in pairs(Editor.code.data) do
		text(0, i*6 + Editor.code.yoff, str, 2)
	end
end

function handleMovingCursor()
	if arrowBtns(1) and Editor.code.row > 1 then
		Editor.code.row = Editor.code.row - 1
		if Editor.code.col > string.len(Editor.code.data[Editor.code.row]) then
			Editor.code.col = string.len(Editor.code.data[Editor.code.row])+1
		end
	elseif arrowBtns(2) and Editor.code.row < #Editor.code.data then
		Editor.code.row = Editor.code.row + 1
		if Editor.code.col > string.len(Editor.code.data[Editor.code.row]) then
			Editor.code.col = string.len(Editor.code.data[Editor.code.row])+1
		end
	elseif arrowBtns(3) then
		if Editor.code.col < string.len(Editor.code.data[Editor.code.row])+1 then
			Editor.code.col = Editor.code.col + 1
		elseif Editor.code.col == string.len(Editor.code.data[Editor.code.row])+1 and Editor.code.row < #Editor.code.data then
			Editor.code.col = 1
			Editor.code.row = Editor.code.row + 1
		end
	elseif arrowBtns(4) then
		if Editor.code.col > 1 then
			Editor.code.col = Editor.code.col - 1
		elseif Editor.code.col == 1 and Editor.code.row > 1 then
			Editor.code.col = string.len(Editor.code.data[Editor.code.row-1])+1
			Editor.code.row = Editor.code.row - 1
		end
	end
end

local function handleEditorOnPress(char_code)
	if char_code == 8 then -- Backspace
		if Editor.code.col > 1 then
			Editor.code.col = Editor.code.col - 1
			Editor.code.data[Editor.code.row] = string.remove(Editor.code.data[Editor.code.row], Editor.code.col)
		elseif Editor.code.row > 1 then
			local removed = table.remove(Editor.code.data, Editor.code.row)
			Editor.code.row = Editor.code.row - 1
			Editor.code.col = string.len(Editor.code.data[Editor.code.row])+1
			Editor.code.data[Editor.code.row] = Editor.code.data[Editor.code.row] .. removed
		end
	elseif char_code == 13 then -- Enter
		Editor.code.row = Editor.code.row + 1
		table.insert(Editor.code.data, Editor.code.row, string.sub(Editor.code.data[Editor.code.row-1], Editor.code.col, Editor.code.data[Editor.code.row-1]:len()))
		Editor.code.data[Editor.code.row-1] = string.sub(Editor.code.data[Editor.code.row-1], 1, Editor.code.col-1)
		Editor.code.col = 1
	else
		Editor.code.data[Editor.code.row] = string.insert(Editor.code.data[Editor.code.row], Editor.code.col, string.char(char_code))
		Editor.code.col = Editor.code.col + 1
	end
end

local function handleKeyBoard()
	local char_code = getkeys()

	if char_code ~= 0 then
		Keyboard.holds[char_code] = Keyboard.holds[char_code] + 1
		if Keyboard.prev[char_code] == false then
			handleEditorOnPress(char_code)
			Keyboard.prev[char_code] = true
		end
		local heldTooLong = Keyboard.holds[char_code] > Keyboard.initialDelay
		if heldTooLong then
			handleEditorOnPress(char_code)
			Keyboard.holds[char_code] = Keyboard.initialDelay - Keyboard.repeatDelay
		end
	else
		for i = 0, 255 do
			Keyboard.holds[i] = 0
			Keyboard.prev[i] = false
		end
	end
end

function CodeEditor()
	rectf(Editor.code.col*5-2, Editor.code.row*6-1 + Editor.code.yoff, 5, 7, 14)
	compseString()
	handleMovingCursor()
	handleKeyBoard()
end



function _update()
	cls(0)
	rectf(0,0, 192, 6, 1)
	if Editor.mode == 0 then
		CodeEditor()
	end
end


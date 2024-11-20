
-- Helper Functions
function string.insert(str, index, value)
	return str:sub(1, index - 1) .. value .. str:sub(index, -1)
end
function string.remove(str, index)
	return str:sub(1, index - 1) .. str:sub(index + 1, -1)
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
		}
	}
}

function compseString()
	for i, string in pairs(Editor.code.data) do
		text(0, i*6 + Editor.code.yoff, string, 2)
	end
end

function CodeEditor()
	rectf(Editor.code.col*5-2, Editor.code.row*6-1 + Editor.code.yoff, 5, 7, 14)
	compseString()
	char_code = getkeys()
	if char_code ~= 0 then
		Editor.code.repeated = Editor.code.repeated + 1
	else
		Editor.code.repeated = Editor.code.maxRepeat
	end

	if char_code ~= Editor.code.prevChar then
		Editor.code.repeated = Editor.code.maxRepeat
	end

	if Editor.code.repeated >= Editor.code.maxRepeat and char_code ~= 0 then
		Editor.code.repeated = 0
		if char_code == 8 then -- Backspace
			if Editor.code.col > 1 then
				Editor.code.col = Editor.code.col - 1
				Editor.code.data[Editor.code.row] = string.remove(Editor.code.data[Editor.code.row], Editor.code.col)
			elseif Editor.code.row > 1 then
				table.remove(Editor.code.data, Editor.code.row)
				Editor.code.row = Editor.code.row - 1
				Editor.code.col = string.len(Editor.code.data[Editor.code.row])+1
				-- Editor.code.data[Editor.code.row] = string.remove(Editor.code.data[Editor.code.row], Editor.code.col)
			end
		elseif char_code == 13 then -- Enter
			Editor.code.row = Editor.code.row + 1
			table.insert(Editor.code.data, Editor.code.row, "")
			Editor.code.col = 1
		else
			Editor.code.data[Editor.code.row] = string.insert(Editor.code.data[Editor.code.row], Editor.code.col, string.char(char_code))
			Editor.code.col = Editor.code.col + 1
		end
		Editor.code.prevChar = char_code
	end
end

function _update()
	cls(0)
	rectf(0,0, 192, 6, 1)
	if Editor.mode == 0 then
		CodeEditor()
	end
end


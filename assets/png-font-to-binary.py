import sys
from PIL import Image

img_path = sys.argv[1]

cell_w = int(sys.argv[2])
cell_h = int(sys.argv[3])

rows = int(sys.argv[4])
cols = int(sys.argv[5])

character_order = [" ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
"@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
"`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~"]

im = Image.open(img_path)

def convert_image(im: Image.Image):
    char_index = 0
    for y in range(rows):
        for x in range(cols):
            char = im.crop((x*cell_w,
                            y*cell_h, 
                            (x*cell_w)+cell_w, 
                            (y*cell_h)+cell_h))
            print(region_to_binary(char, char_index))
            char_index += 1


def region_to_binary(region: Image.Image, char_index) -> str:
    if char_index >= len(character_order):
        return ""

    charString = ""
    for y in range(cell_h):
        binary = ""
        for x in range(cell_w):
            if region.getpixel((x, y)) == (0, 0, 0, 255):
                binary += "1"
            else:
                binary += "0"
        charString += hex(int(binary, 2)) + ", "
    return charString + " // \"" + character_order[char_index] + "\""

convert_image(im)

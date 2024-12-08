def is_in_ucs2le(char):
    codepoint = ord(char)
    return 0x0000 <= codepoint <= 0xFFFF

# 检查“猯”是否在UCS-2LE字符集中
char = '‼'
print(f"'{char}' 是否在UCS-2LE: {is_in_ucs2le(char)}")
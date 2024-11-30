import io
import struct
from enum import Enum
from PIL import Image, ImageDraw, ImageFont
from dataclasses import dataclass
from typing import List, Tuple

class FONT_LINE_SPACE(Enum):
    SPELL_FONT = 0x0215
    TALK_FONT = 0x0018
    TEXTURE_16_600 = 0x0210
    TEXTURE_24 = 0x0318
    TEST = 0x64



class COLOR_CHANNEL(Enum):
    BLUE = 0
    GREEN = 1
    RED = 2
    ALPHA = 3

# 定义字符详情数据结构
@dataclass
class CharaDetail:
    x: int  # 字符在整张图像中的x坐标
    y: int  # 字符在整张图像中的y坐标
    x_width: int  # 字符x方向宽度
    y_width: int  # 字符y方向宽度
    align_offset: int  # 基线对齐偏移量
    color: int  # 存储颜色通道 (0: BLUE, 1: GREEN, 2: RED, 3: ALPHA)

def cartesian_to_pixel(x, y):
    """
    将笛卡尔坐标系的点 (x, y) 转换为像素坐标系的点 (x', y')。
    确保 y 轴翻转，同时保证像素坐标非负。
    """
    x_prime = x + 12
    y_prime = 12 - y

    # 如果映射后的像素坐标包含负值，则作额外处理（例如：限制输入范围）
    if y_prime < 0:
        raise ValueError("笛卡尔坐标的 y 值太大，导致像素坐标的 y' 为负值！")

    return x_prime, y_prime

def convert_to_descartes(x, y):
    x_prime = x - 12
    y_prime = 12 - y
    return x_prime, y_prime

# Load the font file
def load_font(font_path, font_size):
    return ImageFont.truetype(font_path, font_size)

# 将枚举成员转换为小端序字节
def enum_member_to_little_endian_byte(enum) -> bytes:
    return struct.pack('<H', enum.value)

def generate_halfwidth_characters():
    """
    生成所有半角字符的字符列表，包括标点符号、数字、大写字母、小写字母。

    :return: 半角字符列表
    """
    # 半角字符的 ASCII 范围
    ascii_ranges = [
        range(0x20, 0x7E + 1)  # 半角字符的 ASCII 范围：从 0x21 到 0x7E
    ]
    
    # 生成字符列表
    halfwidth_characters = [chr(code_point) for r in ascii_ranges for code_point in r]
    
    return halfwidth_characters

# Generate GB2312 character set
def generate_gb2312_characters():
    gb2312_chars = []
    # GB2312 高字节和低字节范围
    for high in range(0xA1, 0xFE + 1):  # 高字节范围
        for low in range(0xA1, 0xFE + 1):  # 低字节范围
            try:
                # 拼接成字节，并解码为 GB2312 字符
                byte_seq = bytes([high, low])
                char = byte_seq.decode('gb2312')
                gb2312_chars.append(char)
            except UnicodeDecodeError:
                # 如果不是有效字符，跳过
                continue
    return gb2312_chars

def generate_shift_jis_chars():
    """
    生成所有合法的Shift-JIS编码字符。
    返回一个包含所有字符的列表。
    """
    all_chars = []
    for byte1 in range(0x20, 0xFF):  # 遍历第一个字节
        try:
            char = bytes([byte1]).decode('shift_jis')
            all_chars.append(char)
        except UnicodeDecodeError:
            pass  # 非法的单字节字符，跳过

    for byte1 in range(0x81, 0xA0):  # 遍历双字节字符范围
        for byte2 in range(0x40, 0xFF):
            try:
                char = bytes([byte1, byte2]).decode('shift_jis')
                all_chars.append(char)
            except UnicodeDecodeError:
                pass  # 非法的双字节字符，跳过

    for byte1 in range(0xE0, 0xFF):  # 遍历双字节字符的另一部分范围
        for byte2 in range(0x40, 0xFF):
            try:
                char = bytes([byte1, byte2]).decode('shift_jis')
                all_chars.append(char)
            except UnicodeDecodeError:
                pass  # 非法的双字节字符，跳过

    return all_chars

def draw_char(draw_obj, char, font, x, y, rotate, char_size, talkfont=False, cut_offset=0):
    """Helper function to draw a character with optional rotation."""
    img_width, img_height = char_size
    if rotate:
        # Create a separate image for the character to apply rotation
        temp_img = Image.new('L', (img_width, img_height), 0)
        temp_draw = ImageDraw.Draw(temp_img)
        text_bbox = temp_draw.textbbox((0, 0), char, font=font)
        # Make the characters fit tightly to the right border
        char_x = img_width - text_bbox[2]
        temp_draw.text((char_x, 0), char, font=font, fill=255)
        rotated_img = temp_img.rotate(90, expand=True)
        # Paste rotated character into the main draw object
        draw_obj.bitmap((x - cut_offset, y), rotated_img, fill=255)
    else:
        # Draw character directly without rotation
        temp_img = Image.new('L', (img_width, img_height), 0)
        temp_draw = ImageDraw.Draw(temp_img)
        text_bbox = temp_draw.textbbox((0, 0), char, font=font)
        temp_draw.text((0, 0), char, font=font, fill=255)
        cropped = temp_img.crop(text_bbox)
        temp_draw.rectangle(text_bbox, fill=0)
        temp_img.paste(cropped, (0, 0))
        draw_obj.bitmap((x, y), temp_img, fill=255)

def convert_to_gb2312(char_list):
    """
    将字符列表中的数据全部转换为 GB2312 编码，并返回字节流。
    
    :param char_list: List[str] - 包含字符串数据的列表
    :return: bytes - 转换后的 GB2312 编码字节流
    """
    if not isinstance(char_list, list):
        raise ValueError("输入必须是一个字符列表")
    
    try:
        # 将字符列表中的每个字符串转换为 GB2312 编码并拼接到字节流
        gb2312_bytes = b''.join([
    # 如果编码结果长度为 1 字节，补足为 2 字节并调整为小端序
        (b'\x00' + char.encode('gb2312'))[::-1] if len(char.encode('gb2312')) == 1
        else char.encode('gb2312')[::-1]  # 直接反转字节序
        for char in char_list
    ])
        return gb2312_bytes
    except UnicodeEncodeError as e:
        raise ValueError(f"字符列表中包含无法用 GB2312 编码表示的字符: {e}")

def convert_to_shift_jis(char_list):
    """
    将字符列表编码为Shift-JIS字节流，并将每个字符的字节码表示为两字节小端序。
    :param char_list: 包含Unicode字符的列表
    :return: 合并后的字节流
    """
    byte_stream = b''
    for char in char_list:
        try:
            # 使用 Shift-JIS 对字符进行编码
            encoded_bytes = char.encode('shift_jis')
            
            if len(encoded_bytes) == 1:  # 单字节字符
                # 单字节字符补 0x00 在高位，确保为两字节小端序
                byte_stream += encoded_bytes + b'\x00'
            elif len(encoded_bytes) == 2:  # 双字节字符
                # 确保小端序存储（字节顺序交换）
                byte_stream += encoded_bytes[1:2] + encoded_bytes[0:1]
            else:
                raise ValueError(f"字符 '{char}' 编码结果长度异常")
        except UnicodeEncodeError:
            raise ValueError(f"字符 '{char}' 无法编码为Shift-JIS")
    return byte_stream

# Create a grid of characters with RGBA channels
def create_character_grid(font, chars, image_size=(2048, None), char_size=(40, 40), 
                          char_offset=5, extra_height=12, rotate_characters=False, 
                          non_rotatable=[], align_table={}, talkfont=False, cut_offset=0):
    # Fixed width of the image
    fixed_width = image_size[0]
    char_width = char_size[0]
    img_height = char_size[1] + char_offset
    cut = cut_offset

    # Initialize variables for the final image
    final_height = 0
    final_img = None
    chara_info: List[CharaDetail] = []  # 存储字符信息的列表

    i = 0
    while i < len(chars):
        # Create new images for each channel (R, G, B, A) for one row of characters
        r_img = Image.new('L', (fixed_width, img_height), 0)  # Red channel
        g_img = Image.new('L', (fixed_width, img_height), 0)  # Green channel
        b_img = Image.new('L', (fixed_width, img_height), 0)  # Blue channel
        a_img = Image.new('L', (fixed_width, img_height), 0)  # Alpha channel

        # Create draw objects for each channel
        r_draw = ImageDraw.Draw(r_img)
        g_draw = ImageDraw.Draw(g_img)
        b_draw = ImageDraw.Draw(b_img)
        a_draw = ImageDraw.Draw(a_img)

        # Track current x position in the row
        x = 0
        while i < len(chars):

            # Check if there's enough space for a new character
            if fixed_width - x < char_width:
                # Not enough space, end this row and move to the next row
                break

            # Determine whether the current character should be rotated
            rotate = rotate_characters and (chars[i] not in non_rotatable)

            # Determine the color channel for the current character
            color_channel = (i - 1) % 4
            color_enum = COLOR_CHANNEL(color_channel)

            # Draw each character on its respective channel
            draw_func = None
            if color_enum == COLOR_CHANNEL.BLUE:
                draw_func = b_draw
            elif color_enum == COLOR_CHANNEL.GREEN:
                draw_func = g_draw
            elif color_enum == COLOR_CHANNEL.RED:
                draw_func = r_draw
            elif color_enum == COLOR_CHANNEL.ALPHA:
                draw_func = a_draw
            

            actual_size = draw_func.textbbox((0, 0), chars[i], font=font)        # 实际宽高
            text_width = actual_size[2] - actual_size[0]
            text_height = actual_size[3] - actual_size[1]

            # 生成字符渲染数据
            y_width = 0
            x_width = 0
            align = 0
            if (rotate and chars[i] in non_rotatable):
                y_width = text_height
                x_width = text_width

            else:
                if (talkfont):
                    x_width = actual_size[3] - cut_offset
                    y_width = text_width
                else:
                    x_width = actual_size[2] + 4
                    y_width = text_height
            if (talkfont):
                if (rotate and chars[i] in non_rotatable):
                    align = actual_size[3] - cut_offset
                else:
                    align = char_size[1] - y_width
            else:
                align = actual_size[1] - 1

            if (align < 0):
                align = 0

            if (chars[i] in align_table.keys()):
                align = align_table[chars[i]]

            if (chars[i] == ' '):
                chara_info.append(CharaDetail(
                x = 0,
                y = 0,  # 当前字符所在行的起始y坐标
                x_width = x_width,
                y_width = 0,
                align_offset = 0,
                color = COLOR_CHANNEL.BLUE.value  # 当前字符存储的颜色通道
                ))
                i += 1
                continue

            # Draw the character
            draw_char(draw_func, chars[i], font, x, 1, rotate, char_size, cut_offset=cut)

            # Record character details
            chara_info.append(CharaDetail(
                x = x,
                y = final_height,  # 当前字符所在行的起始y坐标
                x_width = x_width,
                y_width = y_width + 1, 
                align_offset = align,
                color = color_channel  # 当前字符存储的颜色通道
            ))

            # Move x position only after all 4 channels are written
            if color_enum == COLOR_CHANNEL.ALPHA:
                x += char_width + char_offset

            i += 1

        # Merge the channels into a single RGBA image for this row
        row_img = Image.merge('RGBA', (r_img, g_img, b_img, a_img))

        # Append this row to the final image
        if final_img is None:
            final_img = row_img
        else:
            # Create a new image with extended height
            new_height = final_height + img_height
            temp_img = Image.new('RGBA', (fixed_width, new_height))
            temp_img.paste(final_img, (0, 0))
            temp_img.paste(row_img, (0, final_height))
            final_img = temp_img

        # Update final height
        final_height += img_height

    # Add extra height to the final image
    final_height += extra_height
    extended_img = Image.new('RGBA', (fixed_width, final_height), (0, 0, 0, 0))
    extended_img.paste(final_img, (0, 0))

    # 将字符信息转换为字节流
    chara_info_bytes = chara_info_to_bytes(chara_info)

    return extended_img, chara_info_bytes


def chara_info_to_bytes(chara_info: List[CharaDetail]) -> bytes:
    """
    将字符信息列表转换为字节流，并以小端序排列。
    每个字符的信息包括：x, y (int16)，width, height, y_offset, color (uint8)。
    """
    byte_stream = b""
    for char in chara_info:
        # 使用 struct 打包数据为字节流
        # 格式 '<hhBBBB' 表示小端序，两个 int16，四个 uint8
        byte_stream += struct.pack(
            '<hhBBBB',  # 小端序（<），两个 int16 (h)，四个 uint8 (B)
            char.x,     # x 坐标
            char.y,     # y 坐标
            char.x_width, # 图像宽度
            char.y_width, # 图像高度
            char.align_offset, # y 偏移
            char.color  # 颜色通道
        )
    return byte_stream

def main():
    font_path = 'siyuanheitiCNMedium.ttf'  # Replace with your font file path
    font_size = 24  # Font size to use
    cut_offset = 6

    # Generate GB2312 characters
    gb2312_characters = generate_gb2312_characters()
    halfwidth_characters = generate_halfwidth_characters()
    shift_jis_chars = generate_shift_jis_chars()

    lc = halfwidth_characters + gb2312_characters
    lsc = shift_jis_chars
    # Load font
    font = load_font(font_path, font_size)
    talk_font = True

    non_rotation = ['…', '—', '（', '）', '《', '》', '【', '】', '『', '』', '「', '」', '——']

    manually_align_adjust = {
        '…':10
    }

    non_rotation = non_rotation + halfwidth_characters

    # Create character grid
    # img, chara_info = create_character_grid(font, lc, char_offset=2, extra_height=16, rotate_characters=True, non_rotatable=non_rotation)

    img, chara_info = create_character_grid(font, lsc, char_offset=2, extra_height=16, 
                                            rotate_characters=True, non_rotatable=non_rotation, 
                                            char_size=(30, 30), align_table=manually_align_adjust, 
                                            talkfont=talk_font, cut_offset=cut_offset)

    # Save the image

    with io.BytesIO() as b:
        img.save(b, format='BMP')
        img_buff = b.getvalue()

    line_space = enum_member_to_little_endian_byte(FONT_LINE_SPACE.TALK_FONT)
    # chara_num = len(lc).to_bytes(2, byteorder='little')
    chara_num = len(lsc).to_bytes(2, byteorder='little')

    # font_data = img_buff + unk + chara_num + convert_to_gb2312(lc) + chara_info
    font_data = img_buff + line_space + chara_num + convert_to_shift_jis(lsc) + chara_info

    with open('output2.bmp', 'wb') as f:
        f.write(font_data)

if __name__ == "__main__":
    main()
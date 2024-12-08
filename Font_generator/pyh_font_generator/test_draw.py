from PIL import Image, ImageDraw, ImageFont
from fontTools.ttLib import TTFont
import sys

class FontCharacterChecker:
    def __init__(self, font_path):
        """
        初始化字体检测器，并缓存字体的字符映射表。
        
        参数：
        - font_path: 字体文件路径（.ttf 或 .otf 文件）
        """
        self.font_path = font_path
        self.character_map = self._build_character_map()

    def _build_character_map(self):
        """
        构建并缓存字体的字符映射表。
        
        返回值：
        - 一个集合，包含字体支持的所有 Unicode 码点。
        """
        try:
            font = TTFont(self.font_path)
            character_set = set()
            for table in font['cmap'].tables:
                character_set.update(table.cmap.keys())
            return character_set
        except Exception as e:
            print(f"加载字体文件时发生错误：{e}")
            return set()

    def is_character_supported(self, character):
        """
        检测单个字符是否被字体支持。
        
        参数：
        - character: 要检测的字符（单个字符）
        
        返回值：
        - True: 如果字体支持该字符
        - False: 如果字体不支持该字符
        """
        return ord(character) in self.character_map

    def are_characters_supported(self, characters):
        """
        检测多个字符是否被字体支持。
        
        参数：
        - characters: 要检测的字符列表或字符串
        
        返回值：
        - 一个字典，键为字符，值为 True/False，表示是否支持
        """
        return {char: self.is_character_supported(char) for char in characters}

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

# 创建字符图像
char = "{|}　「梦想封印」！＊，？啊哀"  # 要绘制的字符
font_size = 22  # 字体大小
font = ImageFont.truetype("msyh_2.ttf", font_size)  # 使用系统中的字体
font_check = FontCharacterChecker("msyh_2.ttf")

# is_supported = font_check.is_character_supported(char)
# print(f"字符 '{char}' 是否被支持：{is_supported}")
# if not is_supported:
#     sys.exit(1)

# 创建图像并绘制字符
image = Image.new("L", (font_size * 20, font_size * 3), color=0)  # 创建灰度图像，背景为黑色
draw = ImageDraw.Draw(image)

# 获取文本边界框并绘制文本在中心
text_bbox = draw.textbbox((0, 0), char, font=font)
draw.text((0, 2), char, fill=255, font=font)


base_bbox = draw.textbbox((0, 0), ' ', font=font)
key_point = convert_to_descartes(base_bbox[2], base_bbox[3])
convert_point = cartesian_to_pixel(-key_point[1],key_point[0])
baseline = convert_point[1] + base_bbox[2] // 2

# 输出原始文本边界框
print("原始边界框:", text_bbox)

# 将图像逆时针旋转 90 度
rotated_image = image.rotate(90, expand=True)

# 获取旋转后的边界框
rotated_bbox = rotated_image.getbbox()
print("旋转后边界框:", rotated_bbox)
print(f"基线位置：{baseline}")
# 显示原始和旋转后的图像（可选）
image.show()
# rotated_image.show()
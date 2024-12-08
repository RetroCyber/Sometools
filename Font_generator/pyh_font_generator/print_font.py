from fontTools.ttLib import TTFont
from PIL import ImageFont

class PrintFont:
    def __init__(self, font_path: str, size: int, char_offset: int, align_offset: int):
        """
        初始化字体信息结构。
        :param font_path: 字体文件的路径。
        :param size: 字体的字号。
        :param cut_offset: 字符上移偏移值。
        """
        self.font_name = font_path.split("/")[-1]  # 从路径中提取字体文件名
        self.font_object = ImageFont.truetype(font_path, size)
        self.char_table = self.get_supported_characters(font_path)
        self.size = size
        self.cut_offset = char_offset
        self.align_offset = align_offset

    def get_supported_characters(self, font_path):
        """
        使用 FontTools 获取字体支持的字符码表。
        :param font_path: 字体文件路径。
        :return: 字符码表的列表，包含支持的字符。
        """
        supported_characters = []
        try:
            font = TTFont(font_path)  # 加载字体文件
            cmap = font['cmap']  # 获取 cmap 表
            for table in cmap.tables:
                supported_characters.extend(table.cmap.keys())  # 提取支持的字符码点
        except Exception as e:
            print(f"Error reading font file: {e}")
        return [chr(code) for code in sorted(set(supported_characters))]

    def __repr__(self):
        """
        返回字体结构的字符串表示。
        """
        return f"font_name='{self.font_name}', size={self.size}, char_offset={self.cut_offset}"


# 示例用法
if __name__ == "__main__":
    font_path = "path/to/your/font.ttf"  # 替换为字体文件的路径
    font_size = 16  # 字号
    char_offset = 2  # 字符上移偏移值（可根据需要调整）

    font_info = PrintFont(font_path, font_size, char_offset)
    print(font_info)

    # 打印支持的字符码表
    print("Supported Characters:", font_info.char_table)
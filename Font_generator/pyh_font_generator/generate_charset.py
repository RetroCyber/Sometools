import json

def read_spell_and_sort(filename):
    try:
        # 读取 JSON 文件
        with open(filename, 'r', encoding='utf-8') as file:
            data = json.load(file)

        # 提取所有的 value 并生成字符集合
        char_set = set()
        for value in data.values():
            char_set.update(value)

        # 按 GBK 编码值对字符进行排序
        sorted_chars = sorted(char_set, key=lambda char: char.encode('gbk'))

        # 输出结果
        return ''.join(sorted_chars)

    except FileNotFoundError:
        print(f"文件 {filename} 未找到，请检查文件路径。")
    except json.JSONDecodeError:
        print(f"文件 {filename} 不是有效的 JSON 格式。")
    except Exception as e:
        print(f"发生错误：{e}")

# 调用函数
if __name__ == "__main__":
    filename = "E:\\thcrap\\repos\\thpatch\\lang_zh-hans\\th155\\spells.js"  # 文件名
    sorted_characters = read_spell_and_sort(filename)
    if sorted_characters:
        print("排序后的字符集合：")
        print(sorted_characters)
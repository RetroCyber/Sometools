from PIL import Image, ImageDraw, ImageFont
import cv2
import numpy as np

def draw_glowing_text(image_size, text, font_path, font_size, text_position, text_color, glow_color, glow_radius, blur_radius):
    # 创建灰度图像
    image = Image.new("L", image_size, 0)
    draw = ImageDraw.Draw(image)
    
    # 加载字体
    font = ImageFont.truetype(font_path, font_size)
    
    # 绘制文字
    draw.text(text_position, text, font=font, fill=text_color)
    
    # 转换为NumPy数组以便处理
    image_np = np.array(image)
    
    # 创建扩散效果
    glow_layer = np.zeros_like(image_np)
    for r in range(1, glow_radius + 1):
        layer = Image.new("L", image_size, 0)
        layer_draw = ImageDraw.Draw(layer)
        layer_draw.text(text_position, text, font=font, fill=glow_color)
        layer_np = np.array(layer)
        
        # 用OpenCV膨胀生成扩散效果
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (r * 2 + 1, r * 2 + 1))
        dilated = cv2.dilate(layer_np, kernel)
        
        # 叠加扩散效果
        glow_layer = np.maximum(glow_layer, dilated)
    
    # 将文字从扩散层中扣除，确保文字部分不被模糊
    glow_layer = np.where(image_np == text_color, 0, glow_layer)
    
    # 对扩散层的最外围进行高斯模糊
    blurred_glow = cv2.GaussianBlur(glow_layer, (blur_radius * 2 + 1, blur_radius * 2 + 1), 0)
    
    # 合并文字和模糊后的扩散效果
    final_np = np.maximum(image_np, blurred_glow)
    
    # 转换为Pillow图像
    final_image = Image.fromarray(final_np)
    
    return final_image

# 参数设置
image_size = (500, 300)  # 图像大小 (宽, 高)
text = "阇"  # 文字内容
font_path = "仓耳灵动黑 简 Heavy.ttf"  # 字体路径（确保字体文件存在）
font_size = 32  # 字体大小
text_position = (50, 100)  # 文字位置 (x, y)
text_color = 255  # 文字颜色（灰度值）
glow_color = 200  # 发光区域颜色（略低于文字颜色的灰度值）
glow_radius = 2  # 扩散半径
blur_radius = 2  # 高斯模糊半径（针对最外围像素）

# 生成发光文字图像
glowing_text_image = draw_glowing_text(image_size, text, font_path, font_size, text_position, text_color, glow_color, glow_radius, blur_radius)

# 保存结果
# glowing_text_image.save("glowing_text.png")
glowing_text_image.show()
// 4.14.0 0xa78dac8b
// Generated by imageconverter. Please, do not edit!

#include <BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_blue_buttons_round_edge_small[]; // BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID = 0, Size: 170x60 pixels
extern const unsigned char image_blue_buttons_round_edge_small_pressed[]; // BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID = 1, Size: 170x60 pixels
extern const unsigned char image_lightbulb_1[]; // BITMAP_LIGHTBULB_1_ID = 2, Size: 200x200 pixels
extern const unsigned char image_lightbulb_2[]; // BITMAP_LIGHTBULB_2_ID = 3, Size: 200x200 pixels
extern const unsigned char image_shou[]; // BITMAP_SHOU_ID = 4, Size: 200x200 pixels
extern const unsigned char image_xuanzhong[]; // BITMAP_XUANZHONG_ID = 5, Size: 200x200 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] =
{
    { image_blue_buttons_round_edge_small, 0, 170, 60, 11, 4, 148, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_blue_buttons_round_edge_small_pressed, 0, 170, 60, 11, 4, 148, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 50, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_lightbulb_1, 0, 200, 200, 0, 0, 200, (uint8_t)(touchgfx::Bitmap::RGB565) >> 3, 200, (uint8_t)(touchgfx::Bitmap::RGB565) & 0x7 },
    { image_lightbulb_2, 0, 200, 200, 0, 0, 200, (uint8_t)(touchgfx::Bitmap::RGB565) >> 3, 200, (uint8_t)(touchgfx::Bitmap::RGB565) & 0x7 },
    { image_shou, 0, 200, 200, 155, 52, 5, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 117, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 },
    { image_xuanzhong, 0, 200, 200, 57, 49, 53, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 41, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
}

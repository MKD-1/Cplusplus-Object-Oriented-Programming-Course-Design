#pragma once
#include "Auto.h"

namespace MyConsole {
	extern std::wstring LOG_s;
	extern HANDLE hOut;
	// 清屏|清除整个控制台屏幕并将光标移到左上角
	void clearScreen();

	// 获取当前光标位置|返回值x横坐标,y纵坐标,不是行列
	COORD getCursorPosition();

	// 去你想去的地方呐|x横坐标,y纵坐标,不是行列
	void gotoXY(short x, short y);

	// 更新字符串实际函数
	void _update(const std::wstring& str, int width = -1);

	// 处理字符串vector
	std::pair<size_t, std::vector<std::wstring>> processString(const std::wstring& str);

	// 带边框的输出
	void updateBorder(int x, int y, const std::wstring& str, const char c = '-', int width = -1);
	void updateBorder(int x, int y, const std::vector< std::wstring >& strs, const char c = '-', int width = 2);

	// 简单输出
	void MyPrint(const std::wstring& str);

	// x横坐标,y纵坐标,不是行列
	void update(short x, short y, const std::wstring& str, int width = -1);
	void update(const std::wstring& str, int width = -1);

	// 光标位置不变,x横坐标,y纵坐标,不是行列
	void updateKeepCursor(short x, short y, const  std::wstring& str, int width = -1);
	void updateKeepCursor(const std::wstring& str, int width = -1);

	// 开新行|输出endl
	void newLine();
	
	// 错误信息
	void LOG(int x, int y);

	// 清除指定行
	void clearLine(short y);

	// 清除指定矩形区域
	void clearRect(short x, short y, short width, short height);

	// 清除缓冲区输入|清除控制台输入缓冲区中的所有字符
	void ClearInputLine();

	// 初始输出函数
	void init(const std::vector<std::wstring>& options, int& preY, int& logY);
}
namespace utf8_width {

	static bool in(uint32_t x, uint32_t l, uint32_t r);

	// 单个 Unicode 码点在控制台中的大致显示宽度
	static int charWidth(uint32_t cp);

	// 计算 wstring 在 CMD 中大致占用的列宽
	int displayWidth(const std::wstring& s);
}




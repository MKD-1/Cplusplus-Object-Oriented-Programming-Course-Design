#include "Console.h"



namespace MyConsole {
	std::wstring LOG_s = L"";
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	void clearScreen() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD count;
		DWORD cellCount;
		COORD homeCoords = { 0, 0 };
		if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return; // TODO: log failure here instead of just returning
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;
		FillConsoleOutputCharacter(
			hOut,
			' ',
			cellCount,
			homeCoords,
			&count
		);
		FillConsoleOutputAttribute(
			hOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		);
		SetConsoleCursorPosition(hOut, homeCoords);
	}
	COORD getCursorPosition() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return { 0, 0 }; // TODO: log failure here instead of just returning { 0, 0 }
		return csbi.dwCursorPosition;
	}
	void gotoXY(short x, short y) {
		COORD pos = { x,y };
		SetConsoleCursorPosition(hOut, pos);
	}

	void _update(const std::wstring& str, int width) {
		if (width == -1) {
			std::wcout << str;
		} else {
			std::wstring s = str;
			int currentWidth = utf8_width::displayWidth(s);
			assert(currentWidth >= s.size());
			if (currentWidth < width)s += std::wstring(width - currentWidth, ' ');
			std::wcout << s.substr(0, width);
		}
	}
	std::pair<size_t, std::vector<std::wstring>> processString(const std::wstring& str) {
		std::vector<std::wstring> lines;
		size_t len_max = 0;
		size_t start = 0;
		size_t pos = 0;
		while ((pos = str.find('\n', start)) != std::wstring::npos) {
			std::wstring line = str.substr(start, pos - start);
			if (!line.empty() && line.back() == '\r') {
				line.pop_back();
			}
			lines.push_back(line);
			len_max = (std::max)((int)len_max, utf8_width::displayWidth(line));
			start = pos + 1;
		}
		if (start <= str.size()) {
			std::wstring lastLine = str.substr(start);
			if (!lastLine.empty() && lastLine.back() == '\r') {
				lastLine.pop_back();
			}
			lines.push_back(lastLine);
			len_max = (std::max)((int)len_max, utf8_width::displayWidth(lastLine));
		}
		return  { len_max + 2,lines };
	}
	void updateBorder(int x, int y, const std::wstring& str, const char c, int width) {
		std::pair<size_t, std::vector<std::wstring>> res = processString(str);
		updateBorder(x, y, res.second, c, (int)res.first);
	}
	void updateBorder(int x, int y, const std::vector< std::wstring >& strs, const char c, int width) {
		for (auto& it : strs)width = (std::max)(width, utf8_width::displayWidth(it) + 2);
		gotoXY(x, y);
		_update(std::wstring(width, c));
		int n = (int)strs.size();
		for (int i = 1; i <= n; i++) {
			gotoXY(x, y + i);
			std::wcout << c;
			_update(strs[i - 1], width - 2);
			std::wcout << c;
		}
		gotoXY(x, y + n + 1);
		_update(std::wstring(width, c));
		gotoXY(x, y + n + 2);
	}
	void MyPrint(const std::wstring& str) {
		_update(str);
		newLine();
	}
	void update(short x, short y, const std::wstring& str, int width) {
		gotoXY(x, y);
		_update(str, width);
	}
	void update(const std::wstring& str, int width) {
		_update(str, width);
	}
	void updateKeepCursor(short x, short y, const  std::wstring& str, int width) {
		COORD oldPos = getCursorPosition();
		gotoXY(x, y);
		_update(str, width);
		SetConsoleCursorPosition(hOut, oldPos);
	}
	void updateKeepCursor(const std::wstring& str, int width) {
		COORD oldPos = getCursorPosition();
		_update(str, width);
		SetConsoleCursorPosition(hOut, oldPos);
	}
	void newLine() {
		std::wcout << std::endl;
	}
	void LOG(int x, int y) {
		int width = utf8_width::displayWidth(LOG_s);
		gotoXY(x, y);
		clearRect(x, y + 1, 100, 2);
		updateBorder(x, y, LOG_s, '-', width + 2);
	}
	void clearLine(short y) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hOut, &csbi);

		DWORD written;
		COORD pos = { 0, y };

		FillConsoleOutputCharacter(
			hOut,
			' ',
			csbi.dwSize.X,
			pos,
			&written
		);

		FillConsoleOutputAttribute(
			hOut,
			csbi.wAttributes,
			csbi.dwSize.X,
			pos,
			&written
		);
	}
	void clearRect(short x, short y, short width, short height) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hOut, &csbi);

		DWORD written;

		for (short row = 0; row < height; row++) {
			COORD pos = { x, short(y + row) };

			FillConsoleOutputCharacter(
				hOut,
				' ',
				width,
				pos,
				&written
			);

			FillConsoleOutputAttribute(
				hOut,
				csbi.wAttributes,
				width,
				pos,
				&written
			);
		}
	}
	void ClearInputLine() {
		std::wcin.clear();
		std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
	}
	void init(const std::vector<std::wstring>& options, int& preY, int& logY) {
		MyConsole::clearScreen();
		MyConsole::updateBorder(0, 0, options);
		preY = static_cast<int>(options.size() + 2), logY = 0;
		if (utf8_width::displayWidth(MyConsole::LOG_s) > 0) {
			MyConsole::LOG(0, preY - 1);
			logY = 2;
		}
		MyConsole::gotoXY(0, preY + logY);
	}
}
namespace utf8_width {
	static bool in(uint32_t x, uint32_t l, uint32_t r) {
		return l <= x && x <= r;
	}
	// 单个 Unicode 码点在控制台中的大致显示宽度
	static int charWidth(uint32_t cp) {
		// 控制字符
		if (cp < 32 || in(cp, 0x7F, 0x9F)) return 0;
		// 常见零宽字符：组合符、变体选择符、零宽连接符
		if (in(cp, 0x0300, 0x036F) ||
			in(cp, 0x1AB0, 0x1AFF) ||
			in(cp, 0x1DC0, 0x1DFF) ||
			in(cp, 0x20D0, 0x20FF) ||
			in(cp, 0xFE00, 0xFE0F) ||
			in(cp, 0xFE20, 0xFE2F) ||
			cp == 0x200D)
			return 0;
		// 东亚宽字符、全角字符、常见 emoji
		if (in(cp, 0x1100, 0x115F) ||
			cp == 0x2329 || cp == 0x232A ||
			in(cp, 0x2E80, 0xA4CF) ||
			in(cp, 0xAC00, 0xD7A3) ||
			in(cp, 0xF900, 0xFAFF) ||
			in(cp, 0xFE10, 0xFE19) ||
			in(cp, 0xFE30, 0xFE6F) ||
			in(cp, 0xFF00, 0xFF60) ||
			in(cp, 0xFFE0, 0xFFE6) ||
			in(cp, 0x1F300, 0x1FAFF) ||
			in(cp, 0x20000, 0x3FFFD))
			return 2;
		return 1;
	}

	// 计算 wstring 在 CMD 中大致占用的列宽
	int displayWidth(const std::wstring& s) {
		int w = 0;
		for (size_t i = 0; i < s.size(); ++i) {
			uint32_t cp = s[i];
			// Windows 下 wchar_t 是 UTF-16，需要处理代理对
			if (0xD800 <= cp && cp <= 0xDBFF && i + 1 < s.size()) {
				uint32_t lo = s[i + 1];
				if (0xDC00 <= lo && lo <= 0xDFFF) {
					cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
					++i;
				}
			}
			w += charWidth(cp);
		}
		return w;
	}
}




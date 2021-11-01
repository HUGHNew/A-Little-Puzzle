#include "FallingChar.h"
namespace GameComponents{
	enum CharType{};
	enum EventType{
		Match,
		Exit,
	};
	static int MatchCode;
	static auto MapToEvent=[](int code)->EventType{
		switch(code) {
		default:
			return Exit;
		}
	};
	bool FallingInit(){
		if (not DetechGraph()) { return false; }
		settextstyle(16, 8, _T("Courier"));	// 设置字体

		// 设置颜色
		settextcolor(GREEN);
		setbkcolor(BLACK);
		return true;
	}
	bool FallingShow() { return true; }
	void FallingEvents(int keycode){
		switch(MapToEvent(keycode)){
			case Match:
			case Exit:
			default:
				break;
		}
	}
}
namespace GameCollection{
	/**
	 * @brief 第一个小游戏
	 * 
	*/
	bool FallingChar() {
		using namespace GameComponents;
		/**
		 * FallingInit
		 * FallingShow(Type:CharType)
		 * FallingEvents(Event:EventType)
		*/
		if (FallingInit() && FallingShow()) { return true; }
		return false;
	}
}
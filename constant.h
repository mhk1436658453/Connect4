#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <iostream>
using namespace std::string_view_literals;

namespace constants
{

	constexpr int b_h{ 6 };		// board height
	constexpr int b_w{ 7 };		// board width

	// constant for displaying the board

	// chess size
	constexpr int chessX{ 6 };
	constexpr int chessY{ 3 };

	// square size, plus 2 for some space between grid lines and the chess piece
	constexpr int squareX = chessX + 2;
	constexpr int squareY = chessY + 2;
	// size of the displayed board on screen
	constexpr int display_width = (b_w + 1) + b_w * squareX;		// vertical lines + 7 for each grid
	constexpr int display_height = (b_h + 1) + b_h * squareY;		// horizonal lines + 7 for each grid

	// color for printing in console
	constexpr int default_color{ 15 };								// black background, white text
	constexpr int red = 4;
	constexpr int yellow = 6;

	#pragma region game_msg
	constexpr std::string_view welcome_msg = R""""(
   ___                                   _       ___                     
  / __\  ___   _ __   _ __    ___   ___ | |_    / __\  ___   _   _  _ __ 
 / /    / _ \ | '_ \ | '_ \  / _ \ / __|| __|  / _\   / _ \ | | | || '__|
/ /___ | (_) || | | || | | ||  __/| (__ | |_  / /    | (_) || |_| || |   
\____/  \___/ |_| |_||_| |_| \___| \___| \__| \/      \___/  \__,_||_|   

)"""";

	constexpr std::string_view win_msg = R""""(
           _           _    _    _    _ 
__      __(_) _ __    / \  / \  / \  / \
\ \ /\ / /| || '_ \  /  / /  / /  / /  /
 \ V  V / | || | | |/\_/ /\_/ /\_/ /\_/ 
  \_/\_/  |_||_| |_|\/   \/   \/   \/   
                                       
)"""";

	constexpr std::string_view draw_msg = R""""(
     _                        
  __| | _ __   __ _ __      __
 / _` || '__| / _` |\ \ /\ / /
| (_| || |   | (_| | \ V  V / 
 \__,_||_|    \__,_|  \_/\_/  

)"""";
	#pragma endregion game_msg
}
#endif

#include <iostream>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include <Windows.h>
  
HANDLE CON_HWND = GetStdHandle(STD_OUTPUT_HANDLE);

using err_t = uint8_t;
enum class Result : err_t {
     GOOD_JOB       = 0x0000
   , ARGUMENTS_LOW  = 0x0001
   , IMAGE_NOT_OPEN = 0x0010
};

//
//enum class ConsoleColor {
//     Black = 0
//   , Blue 
//   , Green
//   , Cyan
//   , Red
//   , Magenta
//   , Brown
//   , LightGray
//   , DarkGray
//   , LightBlue
//   , LightGreen
//   , LightCyan
//   , LightRed
//   , LightMagenta
//   , Yellow
//   , White
//};

auto GetConsoleColor(uint8_t r, uint8_t g, uint8_t b) {
   int step = 255 / 15;

   int tmp = (r + b + g) / 3 / step;

   return tmp;//(int)ConsoleColor(tmp);
}

auto ResizeConsole(COORD size) {
   SMALL_RECT src = { 0, 0, size.X, size.Y };
   SetConsoleWindowInfo(CON_HWND, true, &src);
   SetConsoleScreenBufferSize(CON_HWND, size);
   //SetConsoleTitle(L"test");
   //MoveWindow(FindWindow(NULL, L"test"), 0, 0, 1366, 768, TRUE);
   
   return std::pair(size.X, size.Y);
}

void ShowImage(sf::Image const& img) {
   // рисуем в консоль
   auto [w, h] = ResizeConsole( { (SHORT)img.getSize().y, (SHORT)img.getSize().x } );

   for (size_t i = 0; i < h; ++i) {
      for (size_t j = 0; j < w; ++j) {
         auto r = img.getPixel(j, i);
         
         SetConsoleTextAttribute(CON_HWND, (WORD)(GetConsoleColor(r.r, r.g, r.b) << 4 | 0));
         std::cout << ' ';
      }
      std::cout << '\n';
   }
}

auto OpenImage(std::string path) {
   sf::Image im;

   im.loadFromFile(path);
   
   return im;
}

int main(int argc, char* argv[])
{
   setlocale(LC_ALL, "Ru");
   //Log<std::ostream&> console(std::cout);
   std::string IMG_FILE;

   if (argc < 2) {
#ifndef  _DEBUG
      std::cout << "Недостаточно аргументов. Необходим путь к картинке\n";
      getchar();
      return (err_t)Result::ARGUMENTS_LOW;
#else
      IMG_FILE = "IMAGE.png";
#endif
   } else { 
      IMG_FILE = argv[1];
   }

   auto a = OpenImage(IMG_FILE);

   ShowImage(a);

   getchar();
   return (err_t)Result::GOOD_JOB;
}

#ifndef SDDS_UTILITIES_H
#define SDDS_UTILITIES_H
#include <iostream>
#include <string>

namespace sdds {

   class Utilities {
      size_t m_widthField{ 1u };
      static char m_delimiter;
   public:
      Utilities();
      void setFieldWidth(size_t newWidth);
      size_t getFieldWidth() const;
      std::string extractToken(const std::string& str, size_t& next_pos, bool& more);
      static void setDelimiter(char newDelimiter);
      static char getDelimiter();
      bool isInt(const std::string& str) const;
      size_t& setWidthField(size_t& widField);
      std::string truncate(std::string& src, size_t& next_pos);
   };
}
#endif // !SDDS_UTILITIES_H


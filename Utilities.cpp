#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "Utilities.h"

using namespace std;


namespace sdds {
   bool Udebug = false;
   char Utilities::m_delimiter = '|';

   void Utilities::setFieldWidth(size_t newWidth) {
      if (newWidth) m_widthField = newWidth;
   }

   Utilities::Utilities() {}

   size_t Utilities::getFieldWidth() const {
      return m_widthField;
   }

   std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {

      //Lamda expression
      auto newString = [&next_pos](const std::string& src) -> std::string {
         std::string str = src.substr(next_pos);
         if (Udebug) cout << "Substring from position " << setw(2) << next_pos << " in Utilites : " << str << endl;
         return str;
      };

      std::string buffer{};
      more = false;
      if (str.length() > next_pos) {
         //uses the delimiter to extract the next token from str starting at position next_pos
         std::stringstream lineRead(newString(str));
         getline(lineRead, buffer, m_delimiter);


         if (!buffer.empty()) {
            if (str.find(m_delimiter, next_pos) != std::string::npos) more = true;
            
            next_pos += buffer.length() + 1;

            if (str.find(m_delimiter, next_pos) == next_pos + 1) //eports an exception if a delimiter is found at next_pos;
               throw std::string("Invalid input!");
            

            if (Udebug) cout << "widthField: " << m_widthField << endl;
            if (buffer.length() > m_widthField) m_widthField = buffer.length();


            buffer = buffer.substr(buffer.find_first_not_of(' '),
               (buffer.find_last_not_of(' ') - buffer.find_first_not_of(' ')) + 1);
            if (Udebug) cout << buffer << endl;

         }else throw std::string("Invalid input!");
      }
      else throw std::string("Invalid input!");
 
      return buffer;
   }

   void Utilities::setDelimiter(char newDelimiter) {
      if (newDelimiter) m_delimiter = newDelimiter;
   }

   char Utilities::getDelimiter() {
      return m_delimiter;
   }

   bool Utilities::isInt(const std::string& str) const{
      bool isInt = (str.find_first_not_of("0123456789.") == std::string::npos);
       return isInt;
   }

   size_t& Utilities::setWidthField(size_t& widField){
      if (widField < this->m_widthField)  widField = this->m_widthField;
      return widField;
   }

   std::string Utilities::truncate(std::string& src, size_t& next_pos){
      std::string str = src.substr(next_pos);
      if (Udebug) cout << "Substring from position " << setw(2) << next_pos << " in Utilites : " << str << endl;
      return str;
   }

   
}
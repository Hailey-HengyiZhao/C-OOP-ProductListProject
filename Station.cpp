#include <iostream>
#include <string>
#include <iomanip>
#include "Station.h"
#include "Utilities.h"

using namespace std;


namespace sdds {

   bool Sdebug = false;

   size_t Station::m_widthField = 0;
   size_t Station::id_generator = 0;

   Station::Station(){}

   Station::Station(const std::string src){

      Utilities utils{};
      bool moreInput{ true };
      std::string m_serialS{}, m_qS{};
      size_t posistion{};

      if (Sdebug) cout << "Station -> Read line: " << src << endl;


      m_name = utils.extractToken(src, posistion, moreInput);
      if (Sdebug) cout << m_name << endl;
      


      m_serialS = utils.extractToken(src, posistion, moreInput);
      if (utils.isInt(m_serialS))m_serialNumber = stoi(m_serialS);
      else throw std::string("Invalid record!");
      if (Sdebug) cout << m_serialNumber << endl;


      m_qS = utils.extractToken(src, posistion, moreInput);
      if (utils.isInt(m_qS))m_quantity = stoi(m_qS);
      else throw std::string("Invalid record!");
      if (Sdebug) cout << m_qS << endl;

      m_widthField = utils.setWidthField(m_widthField);

      m_description = utils.extractToken(src, posistion, moreInput);
      if(Sdebug) cout << m_description << endl;

      m_id = ++id_generator;
   }

   const std::string& Station::getItemName() const{
        return m_name;
   }

   size_t Station::getNextSerialNumber(){
      return m_serialNumber++;
   }

   size_t Station::getQuantity() const{
      return m_quantity;
   }

   void Station::updateQuantity(){
      if (m_quantity > 0) --m_quantity;
   }

   void Station::display(std::ostream& os, bool full) const{
      if (full) {
         os << setw(3) << setfill('0') << right << m_id << " | "
            << setw(m_widthField) << setfill(' ') << left << m_name << " | "
            << setw(6) << setfill('0') << right << m_serialNumber << " | "
            << setw(4) << setfill(' ') << m_quantity << " | "
            << m_description << endl;
      } 
      else {
         os << setw(3) << setfill('0') << right << m_id << " | "
            << setw(m_widthField) << setfill(' ') << left << m_name << " | "
            <<  setw(6) << setfill('0') << right << m_serialNumber << " | " << endl;
      }
   }



}
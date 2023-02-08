//*****************************************************************************
//Workshop - MS3
//Full Name : Hengyi Zhao
//Student ID# : 173484213
//Email : hzhao94@myseneca.ca
//Section : OOP345NDD
//Date : 
//Authenticity Declaration :
//I declare this submission is the result of my own work and has not been
//shared with any other student or 3rd party content provider.This submitted
//piece of work is entirely of my own creation.
//* ****************************************************************************
#ifndef SDDS_STATION_H
#define SDDS_STATION_H
#include <iostream>
#include <string>

namespace sdds {
   class Station{
      int m_id{};
      std::string m_name{};
      std::string m_description{};
      size_t m_serialNumber{};
      size_t m_quantity{};
      static size_t m_widthField;
      static size_t id_generator;
   public:
      Station();
      Station(const std::string src);
      const std::string& getItemName() const;
      size_t getNextSerialNumber();
      size_t getQuantity() const;
      void updateQuantity();
      void display(std::ostream& os, bool full) const;
   };

}
#endif // !SDDS_STATION_H



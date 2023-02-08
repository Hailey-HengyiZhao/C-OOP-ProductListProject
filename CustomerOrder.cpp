#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "Utilities.h"
#include "CustomerOrder.h"


using namespace std;
namespace sdds {
   bool Cdebug = false;
   size_t CustomerOrder::m_widthField = 0;

   CustomerOrder::CustomerOrder() {}

   CustomerOrder::CustomerOrder(const std::string src) {

      Utilities utils{};
      bool moreInput{ true };
      size_t posistion{};
      std::string buffer{};
      vector<Item*> tempList{};

      if (Cdebug) cout << "Customer Order -> Read line: \n" << src <<
         "\n=========Now Start Reading=========" << endl;

      //Read customer name
      m_name = utils.extractToken(src, posistion, moreInput);
      if (Cdebug) cout << m_name << endl;

      //Read product name
      m_product = utils.extractToken(src, posistion, moreInput);
      if (Cdebug) cout << m_product << endl;

      //Read list item
      {
         if (Cdebug) cout << "\n\n=========List Item=========" << endl;
         while (posistion < src.length()) {

            buffer = utils.extractToken(src, posistion, moreInput);
            if (Cdebug) cout << buffer << endl;

            if (!buffer.empty()) {
               tempList.push_back(new Item(buffer));
               ++m_cntItem;
            }
         }
         if (Cdebug) cout << "=======Reading PartsEnd, Temporary list has " << m_cntItem << " items\n" << endl;
      }


      //Create Item** list
      m_lstItem = new Item * [m_cntItem + 1];
      for (auto i = 0u; i < m_cntItem; ++i) {
         if (Cdebug) cout << "The address[" << i + 1 << "]: " << tempList.at(i) << endl;
         m_lstItem[i] = tempList.at(i);
      }

      if (Cdebug) {
         for (auto i = 0u; i < m_cntItem; ++i) {
            cout << "Item[" << i + 1 << "]: " << m_lstItem[i]->m_itemName << endl;
         }
      }

      //Update m_widthField
      m_widthField = utils.setWidthField(m_widthField);
   }

   CustomerOrder::CustomerOrder(const CustomerOrder& src) {
      throw std::string("ERROR: Cannot make copies.");
   }

   CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept {
      operator=(std::move(src));
   }

   CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src) noexcept {
      if (this != &src) {
         for (auto i = 0u; i < m_cntItem; i++) {
            delete m_lstItem[i];
         }
         delete[] m_lstItem;

         m_name = src.m_name;
         m_product = src.m_product;
         m_cntItem = src.m_cntItem;
         m_lstItem = src.m_lstItem;
         m_widthField = src.m_widthField;

         src.m_cntItem = 0;
         src.m_lstItem = nullptr;
      }
      return *this;
   }

   CustomerOrder::~CustomerOrder() {
      for (auto i = 0u; i < m_cntItem; i++) {
         delete m_lstItem[i];
      }
      delete[] m_lstItem;
   }

   bool CustomerOrder::isOrderFilled() const {
      bool allFilled = true;
      for (auto i = 0u; i < m_cntItem && allFilled; ++i) {
         if (m_lstItem[i]->m_isFilled == false) allFilled = false;
      }
      return allFilled;
   }

   bool CustomerOrder::isItemFilled(const std::string& itemName) const{
      Item* item{ nullptr };
      for (auto i = 0u; i < m_cntItem; ++i) {
         if (m_lstItem[i]->m_itemName == itemName && m_lstItem[i]->m_isFilled == false) {
            item = m_lstItem[i];
            break;
         }
      }
      if (item == nullptr) return true;
      else return item->m_isFilled;
   }

   void CustomerOrder::fillItem(Station& station, std::ostream& os){
      
      bool found = false;

      for (auto i = 0u; i < m_cntItem && !found; ++i) {
         if (m_lstItem[i]->m_itemName == station.getItemName()) {
            if (Cdebug) cout << "FOUND!! Found the item: " << station.getItemName() << endl;

            if (m_lstItem[i]->m_isFilled == false && station.getQuantity() > 0) {

               station.updateQuantity();
               m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
               m_lstItem[i]->m_isFilled = true;

               os << "    Filled " << m_name << ", " << m_product <<
                  " [" << m_lstItem[i]->m_itemName << "]" << endl;
               found = true;
            }
            else if (m_lstItem[i]->m_isFilled == false && station.getQuantity() == 0) {
               os << "    Unable to fill " << m_name << ", " << m_product <<
                  " [" << m_lstItem[i]->m_itemName << "]" << endl;
            }
         }
      }
   }

   void CustomerOrder::display(std::ostream& os) const {
      cout << m_name << " - " << m_product << endl;

      for (auto i = 0u; i < m_cntItem; ++i) {
         os << "[" << setw(6) << setfill('0') << right << m_lstItem[i]->m_serialNumber << "] "
            << setw(m_widthField) << setfill(' ') << left << m_lstItem[i]->m_itemName;
         if (m_lstItem[i]->m_isFilled) os << " - FILLED";
         else os << " - TO BE FILLED";
         os << endl;
      }

   }

}

#include <iostream>
#include <string>
#include <iomanip>
#include <deque>
#include "Workstation.h"
#include "Utilities.h"

using namespace std;
namespace sdds {

   bool Wdebug = false;

   std::deque<CustomerOrder> g_pending{};
   std::deque<CustomerOrder> g_completed{};
   std::deque<CustomerOrder> g_incomplete{};

   Workstation::Workstation() {}

   Workstation::Workstation(const std::string& filename) : Station(filename) {};

   void Workstation::fill(std::ostream& os) {

      if (m_orders.size() > 0) {
         if (!m_orders.front().isItemFilled(getItemName()))
            m_orders.front().fillItem(*this, os);
      }
   }

   bool Workstation::attemptToMoveOrder() {
      bool result = false;
      if (Wdebug) {
         cout << "m_orders' size is: " << m_orders.size() << endl;
         cout << "Complete queqe size is: " << g_completed.size() << endl;
         cout << "Incomplete queqe size is: " << g_incomplete.size() << endl;
      }

      if (m_orders.size() > 0) {
         if (m_orders.front().isItemFilled(Station::getItemName())) {
            if (m_pNextStation != nullptr) {
               *m_pNextStation += std::move(m_orders.front());
               m_orders.pop_front();
               result = true;
            }
            else {
               if (m_orders.front().isOrderFilled()) {
                  g_completed.push_back(std::move(m_orders.front()));
                  m_orders.pop_front();
                  result = true;
               }
               else {
                  g_incomplete.push_back(std::move(m_orders.front()));
                  m_orders.pop_front();
                  result = true;
               }
            }
         }
         else if (Station::getQuantity() <= 0) {
            if (m_pNextStation) {
               *m_pNextStation += std::move(m_orders.front());
               m_orders.pop_front();
               result = true;
            }
            else {
               g_incomplete.push_back(std::move(m_orders.front()));
               m_orders.pop_front();
               result = true;

            }
            
         }
      }
      return result;
   }

   void Workstation::setNextStation(Workstation* station) {
      m_pNextStation = station;
   }

   Workstation* Workstation::getNextStation() const {
      return m_pNextStation;
   }

   void Workstation::display(std::ostream& os) const {
      os << Station::getItemName() << " --> ";
      if (m_pNextStation == nullptr) os << "End of Line";
      else os << m_pNextStation->getItemName();
      os << endl;
   }

   Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
      m_orders.push_back(std::move(newOrder));
      return *this;
   }

}
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <deque>
#include "Utilities.h"
#include "LineManager.h"

using namespace std;

namespace sdds {
   bool Ldebug = false;

   LineManager::LineManager() {}

   LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {

      Utilities utils{};
      bool moreInput{ true };
      size_t posistion{};

      std::vector<std::string> str2Arr{};
      std::string buffer{};
      std::string str1{}, str2{};

      // read the file and save to tempStation
      ifstream fin(file);
      if (fin.fail()) {
         throw string("Unable to open [") + file + "] file.";
      }


      while (!fin.eof()) {

         moreInput = false;
         posistion = 0;

         // read the line
         getline(fin, buffer);


         // save the first name of station
         str1 = utils.extractToken(buffer, posistion, moreInput);
         auto wSta1 = std::find_if(stations.begin(), stations.end(), [str1](Workstation* src) {
            Workstation* empty{};
            if (src->getItemName() == str1) return src;
            else return empty;
         });
         m_activeLine.push_back(*wSta1);
         /*if (Ldebug) cout << "First: " << setw(30) << left << (*wSta1)->getItemName();*/



         // save the second name of station
         if (posistion != buffer.length() + 1) {
            str2 = utils.extractToken(buffer, posistion, moreInput);
            str2Arr.push_back(str2);

            auto wSta2 = std::find_if(stations.begin(), stations.end(), [str2](Workstation* src) {
               Workstation* empty{};
               if (src->getItemName() == str2) return src;
               else return empty;
               });

            (*wSta1)->setNextStation((*wSta2));

         }
         else (*wSta1)->setNextStation(nullptr);


         /*if (Ldebug) cout << "Second: " << (*wSta1)->getNextStation() << endl;*/

         str1.clear();
         str2.clear();

      }

      if (Ldebug) {
         for (auto& station : m_activeLine) {
            cout << "First: " << setw(20) << left << station->getItemName();
            if (station->getNextStation() != nullptr) {
               cout << "Next: " << setw(20) << left << station->getNextStation()->getItemName()
                  << "Address: " << station->getNextStation() << endl;
            }
            else cout << "End of line" << endl;
         }
      }

      //Find the firstStation
      //if (Ldebug) {
      //   for (auto i = 0u; i < str2Arr.size(); ++i) cout << str2Arr[i] << " | ";
      //   cout << endl;
      //}

      bool found = false;
      size_t i{};
      for (; i < m_activeLine.size() && !found; ) {
         std::string catches = m_activeLine[i]->getItemName();
         if (none_of(str2Arr.begin(), str2Arr.end(), [catches](std::string& str) {
            return catches == str;
            })) found = true;
         i++;
      }
      m_firstStation = m_activeLine[i - 1];
      if (Ldebug) cout << "Name of first Station is " << m_activeLine[i - 1]->getItemName() << " And it's address is " << m_firstStation << endl;


      //Updates g_pending queue.
      m_cntCustomerOrder = g_pending.size();
      if (Ldebug) cout << "m_cntCustomerOrder : " << m_cntCustomerOrder << endl;
   }

   void LineManager::reorderStations() {

      std::vector<Workstation*> newActiveLine{};
      Workstation* tempNextStation{};

      //push the first station
      newActiveLine.push_back(m_firstStation);

      if (Ldebug)
         cout << "Name of This Station is " << setw(20) << left << m_firstStation->getItemName()
         << " And it's address is " << setw(20) << left << m_firstStation
         << "Item: " << newActiveLine.size() << endl;

      tempNextStation = m_firstStation->getNextStation();


      //push the second...then station
      for (auto i = 1u; i < m_activeLine.size(); ++i) {

         if (Ldebug)
            cout << "Name of This Station is " << setw(20) << left << tempNextStation->getItemName()
            << " And it's address is " << setw(20) << left << tempNextStation;

         newActiveLine.push_back(tempNextStation);
         tempNextStation = tempNextStation->getNextStation();

         if (Ldebug) cout << "Item: " << newActiveLine.size() << endl;
      }

      std::move(newActiveLine.begin(), newActiveLine.end(), m_activeLine.begin());
   }

   bool LineManager::run(std::ostream& os) {

      static size_t iteration{ 1 };

      os << "Line Manager Iteration: " << iteration << endl;

      if (g_pending.size() > 0) {
         *m_firstStation += std::move(g_pending.front());
         g_pending.pop_front();

      }

      for (auto& station : m_activeLine) {
         station->fill(os);
      }

      for (auto& station : m_activeLine) {
         station->attemptToMoveOrder();
      }

      ++iteration;
      return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
   }

   void LineManager::display(std::ostream& os) const {
      std::for_each(m_activeLine.begin(), m_activeLine.end(), [&os](const Workstation* stations) {
         stations->display(os);
         });
   }



}
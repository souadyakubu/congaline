/* CongaLine.h declares class CongaLine,
 *  a Conga line dance simulation.
 * Based on an exercise by Marc LeBlanc from Wheaton College (MS).
 */

#ifndef CONGALINE_H_
#define CONGALINE_H_

#include <string>

#include "List.h"
using namespace std;

class CongaLine {
public:
    CongaLine(const string& person1, const string& person2);
    void run();

private:
    List<string> myLine;

    string MY_MENU;
    string myFirstPerson;
    string mySecondPerson;
};

#endif /* CONGALINE_H_ */

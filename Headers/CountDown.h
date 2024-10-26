/*
 * CountDown.h
 *
 *  Created on: Oct 19, 2024
 *      Author: isaia
 */

#ifndef HEADERS_COUNTDOWN_H_
#define HEADERS_COUNTDOWN_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/calctrl.h>
#include "wx/datectrl.h"




// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
    #include "../sample.xpm"
#endif

//Global Enums



// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnCalendarChange(wxCalendarEvent& event);
    void OnStartDateButtonClick(wxCommandEvent& event);
    void OnCountDowntoButtonClick(wxCommandEvent& event);
    void CalculateCountDown();

    // binding events to event handler functions

private:
    //Custom Types
    enum UIObjects {STARTDATEBUTTON, COUNTDOWNTOBUTTON}uiObj;

    //UI elements
    wxButton* startDateButton;
    wxButton* countDownToButton;
    wxStaticText* startDateStaticText;
    wxStaticText* countDownToStaticText;
    wxStaticText* remainingDaysStaticText;
    wxGauge* countDownGauge;
    //variables
    wxDateTime startDate;
    wxDateTime countDownToDate;



};

#endif /* HEADERS_COUNTDOWN_H_ */

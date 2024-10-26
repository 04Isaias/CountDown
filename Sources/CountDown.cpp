/////////////////////////////////////////////////////////////////////////////
// Name:        CountDown.cpp
// Purpose:     Simple application to determine the remaining days before
//				a user specified date.
// Author:      Julian Smart - built minimal sample
// Modified by:	Isaias M Ramirez - built CountDown on top of minimal sample
// Created:     10/18/24
// Copyright:   none
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#include <CountDown.h>



// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit() wxOVERRIDE;
};



// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------


// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
wxIMPLEMENT_APP(MyApp);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

    // create the main application window
    MyFrame *frame = new MyFrame("CountDown");

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(nullptr, wxID_ANY, title)
{

#if wxUSE_MENUBAR
    // create a menu bar
    wxMenu *fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ANY, "&About\tF1", "Show about dialog");
    helpMenu->Bind(wxEVT_MENU, &MyFrame::OnAbout, this);

    fileMenu->Append(wxID_ANY, "E&xit\tAlt-X", "Quit this program");
    fileMenu->Bind(wxEVT_MENU, &MyFrame::OnQuit, this);

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#else // !wxUSE_MENUBAR
    // If menus are not available add a button to access the about box
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
    aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
    sizer->Add(aboutBtn, wxSizerFlags().Center());
    SetSizer(sizer);
#endif // wxUSE_MENUBAR/!wxUSE_MENUBAR

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to CountDown!");
#endif // wxUSE_STATUSBAR
    //setting defaults
    uiObj = STARTDATEBUTTON;
    startDate = wxDateTime::Today();
    countDownToDate = wxDateTime::Today();


    // set the frame icon
    SetIcon(wxICON(sample));

    //initial MyFrame Size
    SetClientSize(1000,600);

    //top parent panel for sizers.
    wxPanel* parentPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    parentPanel->SetBackgroundColour(wxColor(100,100,200));

    //parent Sizer sizer holds dateButtonSizer and progressbar
    wxBoxSizer* dateCalSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* barAndButtonSizer = new wxBoxSizer(wxVERTICAL);

    wxGridSizer* dateButtonSizer = new wxGridSizer(2, 2, wxSize(0,0));


    // Buttons, button bindings, and static Text
    startDateButton = new wxButton(parentPanel, wxID_OK, wxT("Start Date: "));
    startDateButton->Bind(wxEVT_BUTTON, &MyFrame::OnStartDateButtonClick, this);
    countDownToButton = new wxButton(parentPanel, wxID_OK, wxT("Count Down to: "));
    countDownToButton->Bind(wxEVT_BUTTON, &MyFrame::OnCountDowntoButtonClick, this);
    startDateStaticText = new wxStaticText(parentPanel, wxID_OK, wxDateTime::Today().FormatDate());
    countDownToStaticText = new wxStaticText(parentPanel, wxID_OK, wxT("Select Date"));
    remainingDaysStaticText = new wxStaticText(parentPanel, wxID_OK, wxT("Remaining Days"));

    //progress bar and Binding
    countDownGauge = new wxGauge(parentPanel, wxID_OK, 0, wxDefaultPosition, wxDefaultSize, wxCENTER);

    //calendar
    wxCalendarCtrl* m_calendar = new wxCalendarCtrl(parentPanel,wxID_OK,
    		wxDefaultDateTime,wxDefaultPosition,wxDefaultSize,wxCAL_SHOW_HOLIDAYS| wxBORDER_THEME);
    m_calendar->Bind(wxEVT_CALENDAR_SEL_CHANGED, &MyFrame::OnCalendarChange, this);

    //all buttons and labels go in dateButtonSizer
    dateButtonSizer->Add(startDateButton, 1, wxALIGN_LEFT|wxALL, 5);
    dateButtonSizer->Add(startDateStaticText, 1, wxALIGN_LEFT|wxBOTTOM|wxTOP|wxRIGHT, 5);
    dateButtonSizer->Add(countDownToButton, 1, wxALIGN_LEFT|wxBOTTOM|wxLEFT|wxRIGHT, 5);
    dateButtonSizer->Add(countDownToStaticText, 1, wxALIGN_LEFT|wxBOTTOM|wxRIGHT, 5);

    //remainingDaysStaticText, progress bar (countDown) and dateButtonSizer go into bar and Button Sizer
    barAndButtonSizer->Add(dateButtonSizer, 1, wxALIGN_CENTER|wxALL, 5);
    barAndButtonSizer->Add(countDownGauge,0, wxALIGN_CENTER|wxBottom|wxRIGHT|wxLEFT, 5);
    barAndButtonSizer->Add(remainingDaysStaticText,1,wxALIGN_CENTER|wxBottom|wxRIGHT|wxLEFT, 5);

    //barAndButton Sizer and calendar go in dateCalSizer
    dateCalSizer->Add(barAndButtonSizer, 0, wxALIGN_CENTER | wxALL, 5);
    dateCalSizer->Add(m_calendar,0,wxFIXED_MINSIZE|wxTop|wxRIGHT|wxBOTTOM, 5);
    parentPanel->SetSizer(dateCalSizer);
    dateCalSizer->SetSizeHints(this);

}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)){
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)){
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is Isaias' first UI application created: 20241018\n"
                    "running under %s.",
                    wxGetLibraryVersionInfo().GetVersionString(),
                    wxGetOsDescription()
                 ),
                 "About CountDown",
                 wxOK | wxICON_INFORMATION,
                 this);
}

void MyFrame::OnCalendarChange(wxCalendarEvent& event){
	wxString s;
	s.Printf("%s", event.GetDate().FormatDate());

	switch(uiObj){
	case STARTDATEBUTTON:
		startDateStaticText->SetLabel(s);
		startDate = event.GetDate();
		break;
	case COUNTDOWNTOBUTTON:
		countDownToStaticText->SetLabel(s);
		countDownToDate = event.GetDate();
		break;
	default:
		std::cout << "Error: unknown selected object!";
		break;
	}
	CalculateCountDown();

	event.Skip();
}

void MyFrame::OnStartDateButtonClick(wxCommandEvent& event){
	uiObj = STARTDATEBUTTON;
}

void MyFrame::OnCountDowntoButtonClick(wxCommandEvent& event){
	uiObj = COUNTDOWNTOBUTTON;
}

void MyFrame::CalculateCountDown(){
	//subtract startDate from countDownToDate = Range
	wxTimeSpan range = countDownToDate.Subtract(startDate);

	//subtract startDate from currentDate = value
	wxTimeSpan value = wxDateTime::Today().Subtract(startDate);

	//remaining days = range - value
	wxTimeSpan remDays = range;
	remDays.Subtract(value);

	countDownGauge->SetRange(range.GetDays());
	countDownGauge->SetValue(value.GetDays());

	wxString s;
	s.Printf("There are %d Days remaining!", remDays.GetDays());
	remainingDaysStaticText->SetLabel(s);

}

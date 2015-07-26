#if !defined ( __MAIN_H )
#define __MAIN_H

//#define _CRTDBG_MAP_ALLOC
//#define _CRT_SECURE_NO_WARNINGS

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/print.h>
#include <wx/printdlg.h>
#include "printout.h"
#include "tablature.h"

enum {
    MENU_NEW,
    MENU_OPEN,
    MENU_SAVE,
    MENU_SAVE_AS,
    MENU_PRINT,
    MENU_ABOUT,
    MENU_QUIT
};

/**************************** App **********************************/
class App : public wxApp {
public:
    virtual bool OnInit();
};

/************************** Frame **********************************/
class Frame: public wxFrame {
    public:
        Frame();
		~Frame();
        void Setup_Menu();

        void Menu_New(wxCommandEvent &event);
        void Menu_Open(wxCommandEvent &event);
        void Menu_Save(wxCommandEvent &event);
        void Menu_Save_As(wxCommandEvent &event);
        void Menu_Print(wxCommandEvent &event);
        void Menu_About(wxCommandEvent &event);
        void Menu_Quit(wxCommandEvent &event);

        void Paint_Event(wxPaintEvent &event);
        void Char_Event(wxKeyEvent &event);

    private:
        wxBitmap *buffer_bmp;
        wxString current_tab_filename;

        Tablature *tab;

        void New_Tab();
        void Save_Tab(bool new_file);

    DECLARE_EVENT_TABLE()
};

#endif
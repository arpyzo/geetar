#include "debug.h"
#include "main.h"

IMPLEMENT_APP(App)

/*************************** App_Class ****************************/
bool App::OnInit() {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Frame *main_frame = new Frame();

    main_frame->Show(TRUE);
    return TRUE;
}

/***************************** Frame ******************************/
BEGIN_EVENT_TABLE(Frame,wxFrame)
    EVT_MENU(MENU_NEW, Frame::Menu_New)
    EVT_MENU(MENU_OPEN, Frame::Menu_Open)
    EVT_MENU(MENU_SAVE, Frame::Menu_Save)
    EVT_MENU(MENU_SAVE_AS, Frame::Menu_Save_As)
    EVT_MENU(MENU_PRINT, Frame::Menu_Print)
    EVT_MENU(MENU_ABOUT, Frame::Menu_About)
    EVT_MENU(MENU_QUIT,  Frame::Menu_Quit)
    EVT_PAINT(Frame::Paint_Event)
    EVT_CHAR(Frame::Char_Event)
END_EVENT_TABLE()

Frame::Frame() 
: wxFrame((wxFrame *)NULL, -1, "Geetar", wxPoint(100, 100), wxSize(866, 1158)) { // wxSize(864, 1154)
    SetMaxSize(wxSize(866, 1158));

    SetIcon(wxIcon("PROGRAM_ICON"));
    Setup_Menu();

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    buffer_bmp = new wxBitmap(850, 1100);

    tab = new Tablature();

    current_tab_filename = "";
}

Frame::~Frame() {
	delete tab;
	delete buffer_bmp;
}

void Frame::Setup_Menu() {
    wxMenuBar *menu_bar = new wxMenuBar();
    wxMenu *file_menu = new wxMenu();
    wxMenu *help_menu = new wxMenu();

    file_menu->Append(MENU_NEW, "New");
    file_menu->Append(MENU_OPEN, "Open");
    file_menu->Append(MENU_SAVE, "Save");
    file_menu->Append(MENU_SAVE_AS, "Save As");
    file_menu->Append(MENU_PRINT, "Print");
    file_menu->Append(MENU_QUIT, "E&xit");
    menu_bar->Append(file_menu, "&File");

    help_menu->Append(MENU_ABOUT, "&About..");
    menu_bar->Append(help_menu, "&Help");

    SetMenuBar(menu_bar);
}

void Frame::Menu_New(wxCommandEvent &WXUNUSED(event)) {
    New_Tab();
}

void Frame::Menu_Open(wxCommandEvent &WXUNUSED(event)) {
    wxString filename = "";

    filename = wxFileSelector("Select Tab File...", "", "*.tab", "*.tab", "Tab Files (*.tab)|*.tab|All Files (*.*)|*.*");

    //current_input_dir = filename.BeforeLast('/');
    current_tab_filename = filename;

    if (filename.IsEmpty()) {
        return;
    }

    wxTextFile tab_file(filename);
    if (!tab_file.Open()) {
        wxLogError("Couldn't open \"" + filename + "\"!");
        return;
    }

    if (tab_file.Eof()) {
        wxLogError("The tab file is empty!");
        tab_file.Close();
        return;
    }

    New_Tab();
    tab->Load_Tab(&tab_file);
    tab_file.Close();
    this->Refresh();
}

void Frame::Menu_Save(wxCommandEvent &WXUNUSED(event)) {
    Save_Tab(false);
}

void Frame::Menu_Save_As(wxCommandEvent &WXUNUSED(event)) {
    Save_Tab(true);
}

void Frame::Menu_Print(wxCommandEvent &WXUNUSED(event)) {
    //wxPrintData *g_printData = new wxPrintData;
    //wxPrintDialogData printDialogData(*g_printData);

    //wxPrinter printer(&printDialogData);
    //wxPrinter *printer = new wxPrinter();
    wxPrinter printer;
    Printout print_doc(wxT("My printout"), tab->Get_DC());
    printer.Print(this, &print_doc, true);
/*    if (!printer.Print(this, &printout, true)) {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR) {
            wxLogError(wxT("There was a problem printing. Perhaps your current printer is not set correctly?"));
        } else {
            wxLogMessage(wxT("You canceled printing"));
        }
    } else {
        (*g_printData) = printer.GetPrintDialogData().GetPrintData();
    }*/
}

void Frame::Menu_About(wxCommandEvent &WXUNUSED(event)) {
    wxMessageBox("Geetar\nRobert Pyzalski 2014", "About..", wxICON_INFORMATION);
}

void Frame::Menu_Quit(wxCommandEvent &WXUNUSED(event)) {
    Close(TRUE);
}

void Frame::Paint_Event(wxPaintEvent &WXUNUSED(event)) {
    wxBufferedPaintDC paint_win(this, *buffer_bmp);

    paint_win.Blit(0, 0, 850, 1100, tab->Get_DC(), 0, 0);
}

void Frame::Char_Event(wxKeyEvent &event) {
    switch (event.GetKeyCode()) {
        case(WXK_LEFT):
            tab->Move_Cursor(-1, 0);
            this->Refresh();
            return;
        case(WXK_RIGHT):
            tab->Move_Cursor(1, 0);
            this->Refresh();
            return;
        case(WXK_UP):
            tab->Move_Cursor(0, -1);
            this->Refresh();
            return;
        case(WXK_DOWN):
            tab->Move_Cursor(0, 1);
            this->Refresh();
            return;
        case(WXK_DELETE):
            tab->Change_Note(event.GetKeyCode());
            this->Refresh();
            return;
    }

    if (event.GetKeyCode() >= '0' && event.GetKeyCode() <= '9') {
            tab->Change_Note(event.GetKeyCode());
            this->Refresh();
    }
}

void Frame::New_Tab() {
    delete tab;
    tab = new Tablature();
}

void Frame::Save_Tab(bool new_file) {
    wxString filename = "";

    if (new_file)
    { filename = wxFileSelector("Select Tab Save File...", "", "Song.tab", "tab", "Tab Files (*.tab)|*.tab|All Files (*.*)|*.*", wxFD_SAVE);
        if (!filename.IsEmpty())
        { //current_tab_dir = filename.BeforeLast('/');
            current_tab_filename = filename;
        }
    } else {
        filename = current_tab_filename;
    }

    if (filename.IsEmpty()) {
        return;
    }

    wxTextFile save_file(filename);
    if (save_file.Exists()) {
        if (new_file) {
            if (wxMessageBox("File exists. Overwrite?", "Warning", wxOK | wxCANCEL) == wxCANCEL) {
                return;
            }
        }
        if (!save_file.Open()) {
            wxLogError("Couldn't open \"" + filename + "\"!");
            return;
        }
        save_file.Clear();
    } else {
        if (!save_file.Create()) {
            wxLogError("Couldn't create \"" + filename + "\"!");
            return;
        }
    }
    
    tab->Save_Tab(&save_file);
    if (!save_file.Write()) {
        wxLogError("Failed to write \"" + filename + "\"!");
    }
    save_file.Close();
}

#include "debug.h"
#include "tablature.h"

/***************************** Tablature ******************************/
Tablature::Tablature() {
    Init_Notes();

    graphics_bmp = new wxBitmap(850, 1100);
    graphics_mem = new wxMemoryDC(*graphics_bmp);

    Draw_Fresh_Tab();
}

Tablature::~Tablature() {
	delete graphics_mem;
	delete graphics_bmp;
}

wxDC* Tablature::Get_DC() {
    return graphics_mem;
}

void Tablature::Draw_Fresh_Tab() {
    graphics_mem->SetPen(wxPen(*wxWHITE));
    graphics_mem->SetBrush(wxBrush(*wxWHITE));
    graphics_mem->DrawRectangle(0, 0, 850, 1100);

    graphics_mem->SetPen(wxPen(*wxLIGHT_GREY));
    graphics_mem->SetBrush(wxBrush(*wxLIGHT_GREY));
    for (int set = 0 ; set < NUM_TABS ; set++) {
        for (int line = 0 ; line < 6 ; line++) {
            graphics_mem->DrawLine(TAB_TOP_X, TAB_TOP_Y + set*TAB_SPACING + line*TAB_LINE_HEIGHT, 
                                   TAB_TOP_X + TAB_LENGTH, TAB_TOP_Y + set*TAB_SPACING + line*TAB_LINE_HEIGHT);
        }
        graphics_mem->DrawRectangle(TAB_TOP_X, TAB_TOP_Y + set*TAB_SPACING, 3, TAB_LINE_HEIGHT*5 + 1);
        graphics_mem->DrawRectangle(TAB_TOP_X + TAB_LENGTH - 3, TAB_TOP_Y + set*TAB_SPACING, 3, TAB_LINE_HEIGHT*5 + 1);
    }

    cursor_x = 0;
    cursor_y = 0;
    Draw_Cursor();
}

/***************************** Cursor ******************************/
wxPoint Tablature::Get_Cursor_Corner() {
    return Get_Cursor_Corner(cursor_x, cursor_y);
}

wxPoint Tablature::Get_Cursor_Corner(int x, int y) {
    wxPoint corner;

    corner.x = TAB_TOP_X + TAB_INDENT + (CURSOR_SIZE + NOTE_SPACING) * x;
    corner.y = TAB_TOP_Y - CURSOR_SIZE/2 + y/NUM_TABS * TAB_SPACING + y%NUM_TABS * TAB_LINE_HEIGHT;

    return corner;
}

void Tablature::Set_Cursor(int x, int y) {
    Erase_Cursor();
    cursor_x = Check_X_Range(x);
    cursor_y = Check_Y_Range(y);
    Draw_Cursor();
}

void Tablature::Move_Cursor(int x_delta, int y_delta) {
    Erase_Cursor();
    cursor_x = Check_X_Range(cursor_x + x_delta);
    cursor_y = Check_Y_Range(cursor_y + y_delta);
    Draw_Cursor();
}

int Tablature::Check_X_Range(int x) {
    if (x < 0) {
        return 0;
    }
    if (x >= NUM_NOTES) {
        return NUM_NOTES - 1;
    }
    return x;
}

int Tablature::Check_Y_Range(int y) {
    if (y < 0) {
        return 0;
    }
    if (y >= NUM_TABS*6) {
        return NUM_TABS*6 - 1;
    }
    return y;
}

void Tablature::Draw_Cursor() {
    graphics_mem->SetPen(wxPen(*wxLIGHT_GREY));
    graphics_mem->SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
    graphics_mem->DrawRectangle(Get_Cursor_Corner(), wxSize(CURSOR_SIZE, CURSOR_SIZE));
}

void Tablature::Erase_Cursor() {
    graphics_mem->SetPen(wxPen(*wxWHITE));
    graphics_mem->SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
    graphics_mem->DrawRectangle(Get_Cursor_Corner(), wxSize(CURSOR_SIZE, CURSOR_SIZE));

    graphics_mem->SetPen(wxPen(*wxBLACK));
    graphics_mem->DrawPoint(Get_Cursor_Corner() + wxPoint(0, CURSOR_SIZE/2));
    graphics_mem->DrawPoint(Get_Cursor_Corner() + wxPoint(CURSOR_SIZE - 1, CURSOR_SIZE/2));
}

/***************************** Notes ******************************/
void Tablature::Init_Notes() {
    for (int i = 0 ; i < NUM_TABS*6 ; i++) {
        for (int j = 0 ; j < NUM_NOTES ; j++) {
            notes[i][j] = -1;
        }
    }
}

int Tablature::Get_Note() {
    return Get_Note(cursor_x, cursor_y);
}

int Tablature::Get_Note(int x, int y) {
    return notes[y][x];
}

void Tablature::Set_Note(int fret) {
    notes[cursor_y][cursor_x] = fret;
}

void Tablature::Change_Note(char input_char) {
    if (input_char == 127) {
        Set_Note(-1);
    }

    if (input_char > '9' || input_char < '0') {
        return;
    }

    int input_num = input_char - 48;

    if (Get_Note() == 1 || Get_Note() == 2) {
        Set_Note(Get_Note()*10 + input_num);
    } else {
        Set_Note(input_num);
    }

    Draw_Note();
}

void Tablature::Draw_Note() {
    Draw_Note(cursor_x, cursor_y);
}

void Tablature::Draw_Note(int x, int y) {
    if (notes[y][x] < 0) {
        return;
    }

    Erase_Note(x, y);

    int fret_margin = (Get_Note(x, y) >= 10) ? 2 : 5;

    graphics_mem->SetTextForeground(wxColor(*wxBLACK));
    graphics_mem->SetFont(wxFont(*wxNORMAL_FONT));    
    graphics_mem->DrawText(wxString::Format("%d",Get_Note(x, y)), Get_Cursor_Corner(x, y) + wxPoint(fret_margin, 0));
}

void Tablature::Erase_Note() {
    Erase_Note(cursor_x, cursor_y);
}

void Tablature::Erase_Note(int x, int y) {
    graphics_mem->SetPen(wxPen(*wxWHITE));
    graphics_mem->SetBrush(wxBrush(*wxWHITE));
    graphics_mem->DrawRectangle(Get_Cursor_Corner(x, y) + wxPoint(1, 1), wxSize(CURSOR_SIZE - 2, CURSOR_SIZE - 2));

    graphics_mem->SetPen(wxPen(*wxBLACK));
    graphics_mem->DrawLine(Get_Cursor_Corner(x, y) + wxPoint(1, CURSOR_SIZE/2), Get_Cursor_Corner(x, y) + wxPoint(CURSOR_SIZE - 1, CURSOR_SIZE/2));
}

void Tablature::Draw_All_Notes() {
    for (int i = 0 ; i < NUM_TABS*6 ; i++) {
        for (int j = 0 ; j < NUM_NOTES ; j++) {
            Draw_Note(j, i);
        }
    }
}

/***************************** File I/O ******************************/
void Tablature::Load_Tab(wxTextFile *tab_file) {
    wxRegEx tab_match = "\\[Tab ([0-9]+), Line ([0-9])\\]";
    wxRegEx note_match = "Note ([0-9]+)=([0-9][0-9]?)";

    Init_Notes();

    int tab_line = 0;
    for (wxString file_line = tab_file->GetFirstLine() ; !tab_file->Eof() ; file_line = tab_file->GetNextLine()) {
        if (tab_match.Matches(file_line)) {
            tab_line = (Str_To_Int(tab_match.GetMatch(file_line, 1)) - 1)*6 + Str_To_Int(tab_match.GetMatch(file_line, 2)) - 1;
            if (tab_line >= NUM_TABS*6) {
                wxLogError(wxString::Format("Found out of range tab line: %d!", tab_line));
                break;
            }
        } else if (note_match.Matches(file_line)) {
            int position = Str_To_Int(note_match.GetMatch(file_line, 1)) - 1;
            if (position >= NUM_NOTES) {
                continue;
            }
            int fret = Str_To_Int(note_match.GetMatch(file_line, 2));
            notes[tab_line][position] = fret;
        }
    }

    Draw_Fresh_Tab();
    Draw_All_Notes();
}

void Tablature::Save_Tab(wxTextFile *tab_file) {
    for (int set = 0 ; set < NUM_TABS ; set++) {
        for (int line = 0 ; line < 6 ; line++) {
            tab_file->AddLine(wxString::Format("[Tab %d, Line %d]", set + 1, line + 1));
            for (int note = 0 ; note < NUM_NOTES ; note++) {
                if (notes[set*6 + line][note] != -1) {
                    tab_file->AddLine(wxString::Format("Note %d=%d", note + 1, notes[set*6 + line][note]));
                }
            }
            tab_file->AddLine("");
        }
    }
}

/***************************** Utility ******************************/
int Tablature::Str_To_Int(wxString num_string) {
    long result;

    if (num_string.ToLong(&result)) {
        return (int)result;
    }

    return -1;
}
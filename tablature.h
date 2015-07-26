#if !defined ( __TABLATURE_H )
#define __TABLATURE_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/regex.h>

#define TAB_TOP_X 82
#define TAB_TOP_Y 100
#define TAB_LENGTH 686
#define TAB_INDENT 10
#define TAB_SPACING 165
#define TAB_LINE_HEIGHT 15
#define NUM_TABS 6
#define NOTE_SPACING 6
#define NUM_NOTES 32
#define CURSOR_SIZE 15

/************************** Tablature **********************************/
class Tablature {
    public:
        Tablature();
		~Tablature();

        wxDC* Get_DC();

        void Set_Cursor(int x, int y);
        void Move_Cursor(int x_delat, int y_delta);

        void Change_Note(char input);

        void Load_Tab(wxTextFile *tab_file);
        void Save_Tab(wxTextFile *save_file);

    private:
        wxMemoryDC *graphics_mem;
        wxBitmap *graphics_bmp;

        int cursor_x, cursor_y;
        int notes[NUM_TABS * 6][NUM_NOTES];

        int Check_X_Range(int x);
        int Check_Y_Range(int y);
        wxPoint Get_Cursor_Corner();
        wxPoint Get_Cursor_Corner(int x, int y);

        void Draw_Fresh_Tab();
        void Draw_Cursor();
        void Erase_Cursor();

        void Init_Notes();
        void Set_Note(int fret);
        int Get_Note();
        int Get_Note(int x, int y);
        void Draw_Note();
        void Draw_Note(int x, int y);
        void Erase_Note();
        void Erase_Note(int x, int y);
        void Draw_All_Notes();

        wxString Get_Fret_String(char fret);
        int Str_To_Int(wxString num);
};

#endif
#include "debug.h"
#include "printout.h"

/***************************** Printout ******************************/
Printout::Printout(const wxString &title, wxDC *tab_dc)
: wxPrintout(title) {
    this->tab_dc = tab_dc;
}

bool Printout::OnPrintPage(int page) {
    wxDC *printout_dc = GetDC();
    if (printout_dc) {
        FitThisSizeToPaper(wxSize(850, 1100));
        
        wxRect r = GetLogicalPaperRect();
        wxMessageBox(wxString::Format("X: %d  Y: %d  Width: %d  Height: %d", r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight()));

        r = GetPaperRectPixels();
        wxMessageBox(wxString::Format("X: %d  Y: %d  Width: %d  Height: %d", r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight()));

        
        r = GetLogicalPageRect();
        wxMessageBox(wxString::Format("X: %d  Y: %d  Width: %d  Height: %d", r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight()));
        
        //r = GetLogicalPageMarginsRect();
        //wxMessageBox(wxString::Format("X: %d  Y: %d  Width: %d  Height: %d", r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight()));

        //printout_dc->DrawText(("MY TEXT"), 100, 100);
        printout_dc->Blit(8, 0, 850, 1100, tab_dc, 0, 0);
        return true;
    } else {
        return false;
    }
}

bool Printout::HasPage(int page) {
    return true;
}

bool Printout::OnBeginDocument(int startPage, int endPage) {
    if (!wxPrintout::OnBeginDocument(startPage, endPage)) {
        return false;
    }
    return true;
}

void Printout::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo) {

}

/*
HP LaserJet 4100

PS Driver
	PaperRect : -17, -17, 888, 1149
	PageRect : 0, 0, 850, 1113

PCL6 Driver
	PaperRect : -25, -18, 900, 1165
	PageRect : 0, 0, 850, 1130
*/

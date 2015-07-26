#if !defined ( __PRINTOUT_H )
#define __PRINTOUT_H

#include <wx/wx.h>
#include <wx/print.h>
//#include "stdwx.h"

/************************** Printout **********************************/
class Printout: public wxPrintout {
    public:
        Printout(const wxString &title = wxT("My printout"), wxDC *tab_dc = NULL);

        virtual bool OnPrintPage(int page);
        virtual bool HasPage(int page);
        virtual bool OnBeginDocument(int startPage, int endPage);
        virtual void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);

    private:
        wxDC *tab_dc;
};

#endif
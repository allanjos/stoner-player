/**
 * Headers for graphical user interface definitions.
 *
 * \file gui.hpp
 * \author: Allann Jones
 */

/** \defgroup GUI Graphical User Interface */
/**@{*/

#ifndef GUI_HPP
#define GUI_HPP

#include "ui/gui_lib.hpp"

#define GUI_COLOR_LIST_ITEM_ODD wxColour(0xff, 0xff, 0xff)

#define GUI_COLOR_LIST_ITEM_EVEN wxColour(0xf5, 0xf5, 0xf5)

#define GUI_COLOR_LIST_ITEM_WARNING wxColour(0xff, 0xd5, 0xd5)

#define GUI_COLOR_LIST_ITEM_ERROR wxColour(0xff, 0xd5, 0xd5)

#define GUI_COLOR_LIST_ITEM_DISABLED wxColour(0xdd, 0xdd, 0xdd)

#define GUI_COLOR_FONT_DISABLED wxColour(0x99, 0x99, 0x99)

class GUI
{
    public:

        static int ListSetRowBgColor(wxGrid *grid, int row);

        static int ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex);

        static int ListSetRowBgColor(wxGrid *grid, int row, wxColour color);

        static int ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, wxColour color);

        static int ListSetRowBgColor(wxGrid *grid, int row, int state);

        static int ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, int state);
};

class ContainerIntData: public wxClientData
{
    private:
        int value;

    public:
        ContainerIntData(int value);

        int GetData();
};

#endif /* _GUI_HPP_ */

/**@}*/

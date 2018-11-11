/**
 * Implement Graphical User Interface high level functions.
 *
 * \file  gui.cpp
 * \author Allann Jones
 */

#include "ui/gui.hpp"

int GUI::ListSetRowBgColor(wxGrid *grid, int row) {
  if (row % 2 == 0) {
    for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
      grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_EVEN);
    }
  }
  else {
    for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
      grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_ODD);
    }
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex) {
  if (itemIndex % 2 == 0) {
    listCtrl->SetItemBackgroundColour(itemIndex, GUI_COLOR_LIST_ITEM_EVEN);
  }
  else {
    listCtrl->SetItemBackgroundColour(itemIndex, GUI_COLOR_LIST_ITEM_ODD);
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxGrid *grid, int row, wxColour color) {
  for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
    grid->SetCellBackgroundColour(row, colIndex, color);
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, wxColour color) {
  listCtrl->SetItemBackgroundColour(itemIndex, color);

  return 0;
}

int GUI::ListSetRowBgColor(wxGrid *grid, int row, int state) {
  if (state > 0) {
    if (row % 2 == 0) {
      for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
        grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_EVEN);
      }
    }
    else {
      for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
        grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_ODD);
      }
    }
  }
  else {
    for (int colIndex = 0; colIndex < grid->GetNumberCols(); colIndex++) {
      grid->SetCellBackgroundColour(row, colIndex, GUI_COLOR_LIST_ITEM_DISABLED);
      grid->SetCellTextColour(row, colIndex, GUI_COLOR_FONT_DISABLED);
    }
  }

  return 0;
}

int GUI::ListSetRowBgColor(wxListCtrl *listCtrl, long itemIndex, int state)
{
  if (state >= 0) {
    GUI::ListSetRowBgColor(listCtrl, itemIndex);
  }
  else {
    GUI::ListSetRowBgColor(listCtrl, itemIndex, GUI_COLOR_LIST_ITEM_DISABLED);
  }

  return 0;
}

ContainerIntData::ContainerIntData(int value) {
    this->value = value;
}

int ContainerIntData::GetData() {
    return this->value;
}

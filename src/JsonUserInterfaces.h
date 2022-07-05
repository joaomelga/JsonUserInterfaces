/**
 * @file Pages.h
 * @author João Melga
 * @brief
 * @version 0.1
 * @date 2022-06-01
 *
 * @copyright Copyright (c) 2022
 *
 * Contexts: visualization, navigation, editting
 *
 */

#ifndef JSONUSERINTERFACES_H
#define JSONUSERINTERFACES_H

#define MAX_PAGE_OPTIONS 10
#define DISPLAY_X_DIVISIONS 20
#define DISPLAY_Y_DIVISIONS 4

#include <Arduino.h>
#include <Wire.h>

// enum { simpleClick } events;

class Pages {
   private:
    uint8_t _pageIndex = 0;
    uint8_t _pageShift = 0;
    uint8_t _pageDivisions = 1;
    uint8_t _cursorPosition = 0;
    uint8_t _cursorSuperiorLimit = 1;
    uint8_t _displayableDivisions = 1;
    uint8_t _selectedDivision = 0;
    uint8_t _updateFlag = 1;

    uint16_t _pageTimeout = 0;

    unsigned long int _timeoutRef = 0;

    String _pageId;
    String _pageContext;
    String _pagesJson;
    String _pageTitle;
    String _pageOptions[MAX_PAGE_OPTIONS];

   public:
    Pages();

    void setPage(String pageId);
    void redirect(uint8_t redirectionIndex);
    void decreaseCursorPosition();
    void increaseCursorPosition();
    void setCursorSuperiorLimit(uint8_t upLimit) { _cursorSuperiorLimit = upLimit; };
    void setPageDivisions(uint8_t pageDivisions) { _pageDivisions = pageDivisions; };
    void setDisplayabeDivisions(uint8_t displayableDivisions) { _displayableDivisions = displayableDivisions; };
    void setUpdateFlag();
    void setCursorPosition(uint8_t position) { _cursorPosition = position; };
    
    uint8_t getPageShift() { return _pageShift; };
    uint8_t getCursorPosition() { return _cursorPosition; };
    uint8_t getCursorSuperiorLimit() { return _cursorSuperiorLimit; };
    uint8_t getDisplayableDivisions() { return _displayableDivisions; };
    uint8_t getPageDivisions() { return _pageDivisions; };
    uint8_t getUpdateFlag();
    uint8_t pageIsTimedout() { return _pageTimeout && (millis() - _timeoutRef > _pageTimeout); };

    String getPageId() { return String(_pageId); };
    String getPageContext() { return _pageContext; };
    String getPageTitle() { return _pageTitle; };
    String* getPageOptions() { return _pageOptions; };
    String getSerialized();
};

#endif

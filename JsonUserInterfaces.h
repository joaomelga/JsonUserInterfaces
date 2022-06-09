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
#include <ArduinoJson.h>
#include <Wire.h>

// enum { simpleClick } events;

class Pages {
   private:
    uint8_t _pageIndex;
    uint8_t _pageShift;
    uint8_t _pageDivisions;
    uint8_t _cursorPosition;
    uint8_t _cursorSuperiorLimit = 1;
    uint8_t _displayableDivisions;
    uint8_t _selectedDivision;
    uint8_t _updateFlag = 1;

    uint16_t _pageTimeout = 0;

    unsigned long int _timeoutRef = 0;

    String _pageId;
    String _pageContext;
    String _pagesJson = "[{\"id\":\"prod\",\"context\":\"custom\"},{\"id\":\"menu\",\"context\":\"navigation\",\"title\":\"Menu\",\"options\":[{\"title\":\"Preset\",\"redirects\":[\"preset\"]},{\"title\":\"Ajustes\",\"redirects\":[\"adjusts\"]},{\"title\":\"Calibracoes\"},{\"title\":\"Historico\"}]},{\"id\":\"adjusts\",\"context\":\"navigation\",\"title\":\"Ajustes\",\"options\":[{\"title\":\"Teste1\",\"redirects\":[\"menu\"]},{\"title\":\"Teste2\"},{\"title\":\"Teste3\"},{\"title\":\"Teste4\"}]},{\"id\":\"preset\",\"context\":\"editing\",\"title\":\"AjustaPreset\",\"parameterId\":\"preset\",\"significativeValues\":4,\"redirects\":[\"menu\"]}]";
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

    uint8_t getPageShift() { return _pageShift; };
    uint8_t getCursorPosition() { return _cursorPosition; };
    uint8_t getCursorSuperiorLimit() { return _cursorSuperiorLimit; };
    uint8_t getDisplayableDivisions() { return _displayableDivisions; };
    uint8_t getPageDivisions() { return _pageDivisions; };
    uint8_t getUpdateFlag();
    uint8_t pageIsTimedout() { return millis() - _timeoutRef > _pageTimeout; };

    String getPageId() { return String(_pageId); };
    String getPageContext() { return _pageContext; };
    String getPageTitle() { return _pageTitle; };
    String* getPageOptions() { return _pageOptions; };
};

#endif
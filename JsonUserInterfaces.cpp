#include "Pages.h"

#include <SPIFFS.h>

Pages::Pages() {
    // screenSize = 8; // sizeof(pageLines) / sizeof(pageLines[0]);
    _pageShift = 0;
    _cursorPosition = _cursorSuperiorLimit;
    _selectedDivision = 0;
}

void Pages::setPage(String pageId) {
    File file = SPIFFS.open("/pages.json");

    char fileContent[file.size()];
    for (uint16_t i = 0; file.available(); i++)
        fileContent[i] = file.read();

    _pagesJson = String(fileContent);

    DynamicJsonDocument pages(1024);
    deserializeJson(pages, _pagesJson);  //, DeserializationOption::Filter(filter));

    for (uint8_t pageIndex = 0; pageIndex < pages.size(); pageIndex++) {
        if (pages[pageIndex]["id"].as<String>() == String(pageId)) {
            _pageContext = pages[pageIndex]["context"].as<String>();
            _pageId = pageId;
            _pageIndex = pageIndex;
            _pageShift = 0;
            _pageTitle = pages[pageIndex]["title"].as<String>();

            if (_pageContext == "navigation") {
                JsonArray pageOptionsJson = pages[pageIndex]["options"].as<JsonArray>();

                _pageDivisions = pageOptionsJson.size();
                _selectedDivision = 0;
                _displayableDivisions = DISPLAY_Y_DIVISIONS;
                _cursorSuperiorLimit = 1;

                uint8_t optionsIndex = 0;
                for (JsonVariant option : pageOptionsJson) {
                    _pageOptions[optionsIndex] = option["title"].as<String>();
                    optionsIndex++;
                }

            } else if (_pageContext == "editing") {
                _pageDivisions = 20;
                _displayableDivisions = _pageDivisions;
                _cursorSuperiorLimit = 0;
            } else if (_pageContext == "messaging") {
                _timeoutRef = millis();
                _pageTimeout = pages[pageIndex]["timeout"].as<uint16_t>();
            }

            _cursorPosition = _cursorSuperiorLimit;
            return;
        }
    }
}

void Pages::redirect(uint8_t redirectionIndex) {
    DynamicJsonDocument pages(1024);
    deserializeJson(pages, _pagesJson);  //, DeserializationOption::Filter(filter));

    _pageContext = pages[_pageIndex]["context"].as<String>();

    if (_pageContext == "navigation") {
        JsonArray jsonRedirects = pages[_pageIndex]["options"][_selectedDivision]["redirects"].as<JsonArray>();

        if (jsonRedirects != NULL) {
            setPage(jsonRedirects[redirectionIndex].as<String>());
            _updateFlag = 1;
        }
    } else if (_pageContext == "editing" || _pageContext == "messaging") {
        JsonArray jsonRedirects = pages[_pageIndex]["redirects"];

        if (jsonRedirects != NULL) {
            setPage(jsonRedirects[redirectionIndex].as<String>());
            _updateFlag = 1;
        }
    }
}

void Pages::decreaseCursorPosition() {
    if (_cursorPosition < _displayableDivisions - 1) {
        _cursorPosition++;
        _selectedDivision++;
    } else if (_pageShift + _displayableDivisions < _pageDivisions + _cursorSuperiorLimit) {
        _pageShift++;
        _selectedDivision++;
    } else {
        _cursorPosition = _cursorSuperiorLimit;
        _pageShift = 0;
        _selectedDivision = 0;
    }

    _updateFlag = 1;
}

void Pages::increaseCursorPosition() {
    if (_cursorPosition > _cursorSuperiorLimit) {
        _cursorPosition--;
        _selectedDivision--;
    } else if (_pageShift > 0) {
        _pageShift--;
        _selectedDivision--;
    } else {
        _cursorPosition = _displayableDivisions - 1;
        _pageShift = _pageDivisions + _cursorSuperiorLimit - _displayableDivisions;
        _selectedDivision = _pageDivisions - 1;
    }

    _updateFlag = 1;
}

uint8_t Pages::getUpdateFlag() {
    if (_updateFlag) {
        _updateFlag = 0;
        return 1;
    } else
        return 0;
}

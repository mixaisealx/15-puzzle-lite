# 15 puzzle (game of fifteen) - Lite
[![License](https://img.shields.io/badge/LICENSE-Apache%202.0-green?style=flat)](/LICENSE)  [![Version](https://img.shields.io/badge/VERSION-RELEASE%20--%201.0-green?style=flat)](https://github.com/mixaisealx/15-puzzle-lite/releases)
### :capital_abcd: [Версия на русском языке](/README-ru.md)

This demo program represents the implementation of the "15 puzzle" in the C++ programming language in a function-oriented style with a division into functional layers.

The program interface is made minimalistic, but understandable:
![Screenshot of the program window](https://github.com/mixaisealx/15-puzzle-lite/raw/master/screenshot.png)

### Features
1. Using separate template classes (modules) to separate the areas of responsibility of modules.
2. Event-driven approach through the use of CallBack-functions.
3. Using pure *WINAPI* (no *ALT* and *MFC*) when working with graphics.
4. Using exclusively *WINGDI* (instead of *GDI+*) for rendering any primitives, as well as text.
5. Exclusively custom rendering (the buttons provided by the system were not used).
6. Implementation of saving history in such a way that data on 4 moves are placed in one byte.
7. Unicode functions are used to display text, which simplifies localization.
8. Two win check algorithms: classic and snake. The first is active by default, and the second can be activated by uncommenting the lines of code marked *SnakeAlg*. This algorithm determines the payoff if it is possible to get from cell "1" to "15" by passing through neighboring cells (for example, cell "2" is searched around cell "1", then cell "3" is searched around cell "2", and so on). The *SnakeAlg* algorithm is disabled by default due to the fact that it violates the rules of the classic game.
9. A good structural template for the implementation of any simple "passive" game (i.e. a game where all events are generated exclusively by the user, there are no in-game events).

To make it easier to make changes, a UML diagram was drawn:
![UML diagram (SVG)](https://github.com/mixaisealx/15-puzzle-lite/raw/master/uml.svg)
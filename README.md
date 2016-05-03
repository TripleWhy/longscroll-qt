# longscroll-qt
[![Build Status](https://travis-ci.org/TripleWhy/longscroll-qt.svg?branch=master)](https://travis-ci.org/TripleWhy/longscroll-qt) [![Build status](https://ci.appveyor.com/api/projects/status/wpkj3f4oca4umh4t/branch/master?svg=true)](https://ci.appveyor.com/project/TripleWhy/longscroll-qt/branch/master)

longscroll-qt is a library to create very long, fast and responsive scrollable widgets in Qt.

The longscroll widget can visualize a large list of items, similar to a QAbstractItemView.
The main differences are that longscroll-qt is responsive, has much more flexible layout options and uses real widgets to display items.
This allows easy user interaction and simple customization as you can for example use the Qt designer to create an item widget.
longscroll-qt also offers a fully customizable navigator widget wich is shown between two rows, creating a complete google-images-like view.

The library was tested and works with millions of image items.

## API Docs
Online documentation for the current release can be found at https://triplewhy.gitlab.io/longscroll-qt/doc/.

You can also [download](../../releases) a html or qhp version to integrate into Qt Assistant and Qt Creator.

## Examples
The library comes with two [examples](examples). To understand the behavior best, keep resizing the window.
- [examples/simple](examples/simple) sets up a simple program using only a few lines of code to get you up running fast.
- [examples/demo](examples/demo) deomstrates the power of longscroll-qt by providing a gui that lets you dynamically change nearly every setting to see what it does.

The documentation of [longscroll::ContentWidget](https://triplewhy.gitlab.io/longscroll-qt/doc/classlongscroll_1_1_content_widget.html) can give a good overview of what longscroll-qt can do, as most of its functionality is provided by that class.

## Screenshots
This library can best be experienced by running the demo program.
The following are some layout possibilities from the documentation (they are actual screen shots):

![layout 1](doxygen/img/layout1.png)
![layout 2](doxygen/img/layout2.png)
![layout 3](doxygen/img/layout3.png)
![layout 4](doxygen/img/layout4.png)
![layout 5](doxygen/img/layout5.png)
![layout 6](doxygen/img/layout6.png)

Alright here are some real screenshots anyway:

![screenshot 1](doxygen/img/screen01.jpg)

![screenshot 2](doxygen/img/screen02.jpg)

![screenshot 3](doxygen/img/screen03.jpg)

![screenshot 4](doxygen/img/screen04.jpg)

![screenshot 5](doxygen/img/screen05.jpg)

## License
longscroll-qt is licensed under AGPL v3. (Contact me if you need a commercial license.)

// xbel.cpp
//
// Copyright (C) 2008, the Celestia Development Team
// Original version by Chris Laurel <claurel@gmail.com>
//
// XBEL bookmarks reader and writer.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "xbel.h"
#include "qtbookmark.h"

#include <iostream>
XbelReader::XbelReader(QIODevice* device) :
    QXmlStreamReader(device)
{
}


// This code is based on QXmlStreamReader example from Qt 4.3.3

BookmarkItem*
XbelReader::read()
{
    BookmarkItem* rootItem = new BookmarkItem(BookmarkItem::Folder, NULL);

    std::clog << "XbelReader::read()\n";
    while (!atEnd())
    {
        readNext();

        if (isStartElement())
        {
            QStringRef version = attributes().value("version");
            if (name() == "xbel" && (version == "1.0" || version.isEmpty()))
                readXbel(rootItem);
            else
                raiseError(QObject::tr("Not an XBEL version 1.0 file."));
        }
    }

    if (error() != NoError)
    {
        delete rootItem;
        rootItem = NULL;
    }

    return rootItem;
}


void
XbelReader::readXbel(BookmarkItem* root)
{
    while (!atEnd())
    {
        readNext();
        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == QLatin1String("folder"))
                readFolder(root);
            else if (name() == QLatin1String("bookmark"))
                readBookmark(root);
            else if (name() == QLatin1String("separator"))
                readSeparator(root);
            else
                skipUnknownElement();
        }
    }
}


void
XbelReader::readFolder(BookmarkItem* parent)
{
    BookmarkItem* folder = new BookmarkItem(BookmarkItem::Folder, parent);
    folder->setFolded(attributes().value(QLatin1String("folded")) == QLatin1String("yes"));

    std::clog << "readFolder\n";
    while (!atEnd())
    {
        readNext();
        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == QLatin1String("folder"))
                readFolder(folder);
            else if (name() == QLatin1String("bookmark"))
                readBookmark(folder);
            else if (name() == QLatin1String("separator"))
                readSeparator(folder);
            else if (name() == QLatin1String("title"))
                readTitle(folder);
            else if (name() == QLatin1String("desc"))
                readDescription(folder);
            else
                skipUnknownElement();
        }
    }

    parent->append(folder);
}


void
XbelReader::readBookmark(BookmarkItem* parent)
{
    BookmarkItem* item = new BookmarkItem(BookmarkItem::Bookmark, parent);
    item->setUrl(attributes().value(QLatin1String("href")).toString());

    while (!atEnd())
    {
        readNext();
        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == QLatin1String("title"))
                readTitle(item);
            else if (name() == QLatin1String("desc"))
                readDescription(item);
            else
                skipUnknownElement();
        }
    }

    if (item->title().isEmpty())
        item->setTitle("Unknown");

    parent->append(item);
}


void
XbelReader::readSeparator(BookmarkItem* parent)
{
    BookmarkItem* separator = new BookmarkItem(BookmarkItem::Separator, parent);
    parent->append(separator);
    readNext();
}


void
XbelReader::readTitle(BookmarkItem* item)
{
    item->setTitle(readElementText());
}


void
XbelReader::readDescription(BookmarkItem* item)
{
    item->setDescription(readElementText());
}


void
XbelReader::skipUnknownElement()
{
    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;
        if (isStartElement())
            skipUnknownElement();
    }
}
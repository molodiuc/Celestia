// fsutils.h
//
// Copyright (C) 2001-present, the Celestia Development Team
// Original version by Chris Laurel <claurel@shatters.net>
//
// Miscellaneous useful filesystem-related functions.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#pragma once

#include <string_view>

#include <celcompat/filesystem.h>
#include <celutil/array_view.h>

namespace celestia::util
{

fs::path LocaleFilename(const fs::path& filename);
fs::path PathExp(const fs::path& filename);
fs::path ResolveWildcard(const fs::path& wildcard,
                         array_view<std::string_view> extensions);
bool IsValidDirectory(const fs::path &dir);
#ifndef PORTABLE_BUILD
fs::path HomeDir();
fs::path WriteableDataPath();
#endif

} // end namespace celestia::util

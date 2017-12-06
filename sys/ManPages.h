#ifndef _ManPages_h_
#define _ManPages_h_
/* ManPages.h
 *
 * Copyright (C) 1996-2011,2015,2017 Paul Boersma
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ManPage.h"
#include "Collection.h"

Thing_define (ManPages, Daata) {
	OrderedOf<structManPage> pages;
	const char32 **titles;
	bool ground, dynamic, executable;
	structMelderDir rootDirectory;

	void v_destroy () noexcept
		override;
	void v_readText (MelderReadText text, int formatVersion)
		override;
};

autoManPages ManPages_create ();

void ManPages_addPage (ManPages me, const char32 *title, const char32 *author, integer date,
	struct structManPage_Paragraph paragraphs []);
/*
	All string and struct arguments should be statically allocated
	and not change after adding them to the ManPages.
*/

integer ManPages_lookUp (ManPages me, const char32 *title);

void ManPages_writeOneToHtmlFile (ManPages me, integer ipage, MelderFile file);
void ManPages_writeAllToHtmlDir (ManPages me, const char32 *dirPath);

integer ManPages_uniqueLinksHither (ManPages me, integer ipage);
const char32 **ManPages_getTitles (ManPages me, integer *numberOfTitles);

/* End of file ManPages.h */
#endif

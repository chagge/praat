/* Strings_extensions.cpp
 *
 * Copyright (C) 1993-2012, 2015-2016 David Weenink
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 djmw 20011003
 djmw 20020813 GPL header
 djmw 20030107 Added Strings_setString
 djmw 20031212 Added Strings_extractPart
 djmw 20040301 Added Strings_createFixedLength.
 djmw 20040308 Corrected bug in strings_to_Strings.
 djmw 20040427 Strings_append added.
 djmw 20040629 Strings_append  now accepts an Ordered of Strings.
 djmw 20050714 New: Strings_to_Permutation, Strings_and_Permutation_permuteStrings.
 djmw 20050721 Extra argument in Strings_to_Permutation.
 djmw 20101007 StringsIndex Stringses_to_StringsIndex (Strings me, Strings classes)
 djmw 20120407 + Strings_createFromCharacters
 djmw 20120813 -Strings_setString
*/

#include "Strings_extensions.h"
#include "NUM2.h"

autoStrings Strings_createFixedLength (long numberOfStrings) {
	try {
		if (numberOfStrings <= 0) {
			Melder_throw (U"The number of strings must be positive.");
		}
		autoStrings me = Thing_new (Strings);
		my strings = NUMvector<char32 *> (1, numberOfStrings);
		my numberOfStrings = numberOfStrings;
		return me;
	} catch (MelderError) {
		Melder_throw (U"Strings not created.");
	}
}

autoStrings Strings_createAsCharacters (const char32 *string) {
	try {
		autoStrings me = Thing_new (Strings);
		my numberOfStrings = str32len (string);
		my strings = NUMvector<char32 *> (1, my numberOfStrings);
		for (long i = 1; i <= my numberOfStrings; i++) {
			my strings[i] = Melder_dup (Melder_character (*string++));
		}
		return me;
	} catch (MelderError) {
		Melder_throw (U"Strings from characters not created.");
	}
}

autoStrings Strings_createAsTokens (const char32 *string) {
	try {
		autoStrings me = Thing_new (Strings);
		my numberOfStrings =  Melder_countTokens (string);
		my strings = NUMvector<char32 *> (1, my numberOfStrings);
		long i = 1;
		for (char32 *token = Melder_firstToken (string); token != 0; token = Melder_nextToken ()) {
			my strings[i++] = Melder_dup (token);
		}
		return me;
	} catch (MelderError) {
		Melder_throw (U"Strings from characters not created.");
	}
}

long Strings_findString (Strings me, const char32 *string) {
	for (long i = 1; i <= my numberOfStrings; i++) {
		if (Melder_equ (my strings[i], string)) {
			return i;
		}
	}
	return 0;
}

autoStrings Strings_append (OrderedOf<structStrings>* me) {
	try {
		long index = 1, numberOfStrings = 0;

		for (long i = 1; i <= my size; i ++) {
			Strings s = my at [i];
			numberOfStrings += s -> numberOfStrings;
		}

		autoStrings thee = Strings_createFixedLength (numberOfStrings);

		for (long i = 1; i <= my size; i ++) {
			Strings s = my at [i];
			for (long j = 1; j <= s -> numberOfStrings; j ++, index ++) {
				thy strings [index] = Melder_dup (s -> strings [j]);
			}
		}
		return thee;
	} catch (MelderError) {
		Melder_throw (me, U": not appended.");
	}
}

autoStrings Strings_change (Strings me, const char32 *search, const char32 *replace, int maximumNumberOfReplaces, long *nmatches, long *nstringmatches, int use_regexp) {
	try {
		autoStrings thee = Thing_new (Strings);
		char32 **strings = strs_replace (my strings, 1, my numberOfStrings, search, replace, maximumNumberOfReplaces, nmatches, nstringmatches, use_regexp);
		thy numberOfStrings = my numberOfStrings;
		thy strings = strings;
		return thee;
	} catch (MelderError) {
		Melder_throw (me, U": not changed.");
	}
}

autoStrings strings_to_Strings (char32 **strings, long from, long to) {
	try {
		autoStrings thee = Strings_createFixedLength (to - from + 1);
		for (long i = from; i <= to; i++) {
			thy strings[i - from + 1]  = Melder_dup (strings[i]);
		}
		return thee;
	} catch (MelderError) {
		Melder_throw (U"Strings not created.");
	}
}

autoStrings Strings_extractPart (Strings me, long from, long to) {
	try {
		if (from < 1 || to > my numberOfStrings || from > to) Melder_throw
			(U"Strings_extractPart: begin and end must be in interval [1, ", my numberOfStrings, U"].");
		return strings_to_Strings (my strings, from, to);
	} catch (MelderError) {
		Melder_throw (me, U": no part extracted.");
	}
}

autoStrings strings_to_Strings_link (char32 **strings, long n) {
	try {
		autoStrings me = Strings_createFixedLength (n);
		for (long i = 1; i <= n; i++) {
			my strings[i] = strings[i];
		}
		return me;
	} catch (MelderError) {
		Melder_throw (U"Strings not linked.");
	}
}

void Strings_unlink (Strings me) {
	for (long i = 1; i <= my numberOfStrings; i++) {
		my strings[i] = nullptr;
	}
}

autoPermutation Strings_to_Permutation (Strings me, int sort) {
	try {
		autoPermutation thee = Permutation_create (my numberOfStrings);
		if (sort != 0) {
			NUMindexx_s (my strings, my numberOfStrings, thy p);
		}
		return thee;
	} catch (MelderError) {
		Melder_throw (me, U": no Permutation created.");
	}
}

autoStrings Strings_and_Permutation_permuteStrings (Strings me, Permutation thee) {
	try {
		if (my numberOfStrings != thy numberOfElements) Melder_throw (U"Strings_and_Permutation_permuteStrings: "
			        U"The number of strings and the number of elements in the Permutation must be equal.");
		autoStrings him = Strings_createFixedLength (my numberOfStrings);
		for (long i = 1; i <= thy numberOfElements; i++) {
			long index = thy p[i];
			his strings[i] = Melder_dup (my strings[index]);
		}
		return him;
	} catch (MelderError) {
		Melder_throw (me, U": no permuted Strings created.");
	}
}

autoStringsIndex Stringses_to_StringsIndex (Strings me, Strings classes) {
	try {
		autoStringsIndex tmp = Strings_to_StringsIndex (classes);
		long numberOfClasses = tmp -> classes->size;

		autoStringsIndex him = StringsIndex_create (my numberOfStrings);
		for (long i = 1; i <= numberOfClasses; i ++) {
			SimpleString t = (SimpleString) tmp -> classes->at [i];   // FIXME cast
			autoSimpleString t2 = Data_copy (t);
			his classes -> addItem_move (t2.move());
		}
		for (long j = 1; j <= my numberOfStrings; j ++) {
			long index = 0;
			char32 *stringsj = my strings [j];
			for (long i = 1; i <= numberOfClasses; i ++) {
				SimpleString ss = (SimpleString) his classes->at [i];   // FIXME cast
				if (Melder_equ (stringsj, ss -> string)) {
					index = i;
					break;
				}
			}
			his classIndex [j] = index;
		}
		return him;
	} catch (MelderError) {
		Melder_throw (me, U": no StringsIndex created.");
	}
}

autoStringsIndex Strings_to_StringsIndex (Strings me) {
	try {
		autoStringsIndex thee = StringsIndex_create (my numberOfStrings);
		autoPermutation sorted = Strings_to_Permutation (me, 1);
		long numberOfClasses = 0;
		char32 *strings = nullptr;
		for (long i = 1; i <= sorted -> numberOfElements; i ++) {
			long index = sorted -> p [i];
			char32 *stringsi = my strings [index];
			if (i == 1 || Melder_cmp (strings, stringsi) != 0) {
				numberOfClasses ++;
				autoSimpleString him = SimpleString_create (stringsi);
				thy classes -> addItem_move (him.move());
				strings = stringsi;
			}
			thy classIndex [index] = numberOfClasses;
		}
		return thee;
	} catch (MelderError) {
		Melder_throw (me, U": no StringsIndex created.");
	}
}

autoStrings StringsIndex_to_Strings (StringsIndex me) {
	try {
		autoStrings thee = Strings_createFixedLength (my numberOfItems);
		for (long i = 1; i <= thy numberOfStrings; i ++) {
			SimpleString s = (SimpleString) my classes->at [my classIndex [i]];   // FIXME cast, FIXME classIndex
			thy strings [i] = Melder_dup (s -> string);
		}
		return thee;
	} catch (MelderError) {
		Melder_throw (me, U": no Strings created.");
	}
}

autoStringsIndex Table_to_StringsIndex_column (Table me, long column) {
	try {
		if (column < 1 || column > my numberOfColumns) {
			Melder_throw (U"Invalid column number.");
		}
		long numberOfRows = my rows.size;
		Table_numericize_Assert (me, column);
		autoNUMvector<char32 *> groupLabels (1, numberOfRows);
		for (long irow = 1; irow <= numberOfRows; irow ++) {
			groupLabels [irow] = my rows.at [irow] -> cells [column] .string;
		}
		autoStrings thee = strings_to_Strings (groupLabels.peek(), 1, numberOfRows);
		autoStringsIndex him = Strings_to_StringsIndex (thee.get());
		return him;
	} catch (MelderError) {
		Melder_throw (me, U"No StringsIndex created from column ", column, U".");
	}
}

/* End of file Strings_extensions.cpp */

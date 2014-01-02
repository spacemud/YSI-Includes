Test:INI_AtStartOfLine1()
{
	ASSERT(INI_AtStartOfLine("hi", 0));
	ASSERT(INI_AtStartOfLine("  hi", 0));
	ASSERT(INI_AtStartOfLine("	hi", 0));
	ASSERT(INI_AtStartOfLine("  hi", 2));
	ASSERT(INI_AtStartOfLine("\thi", 1));
	ASSERT(INI_AtStartOfLine("\nhi", 1));
	ASSERT(!INI_AtStartOfLine("x\nhi", 1));
	ASSERT(INI_AtStartOfLine("\rhi", 1));
	ASSERT(!INI_AtStartOfLine("x\rhi", 1));
	ASSERT(!INI_AtStartOfLine("hhi", 1));
	ASSERT(!INI_AtStartOfLine("c hi", 2));
}

Test:INI_IsEscapeSequence1()
{
	new
		str[16];
	str = "\\;";
	ASSERT(INI_IsEscapeSequence(str, 1));
	str = "\\\\";
	ASSERT(INI_IsEscapeSequence(str, 1));
	str = "\\\\\\;";
	ASSERT(INI_IsEscapeSequence(str, 3));
	str = "\\\\\\\\";
	ASSERT(INI_IsEscapeSequence(str, 3));
	str = "\\\\\\n";
	ASSERT(INI_IsEscapeSequence(str, 3));
	str = "v\\\\\\\\";
	ASSERT(INI_IsEscapeSequence(str, 4));
	str = "hi\\n";
	ASSERT(INI_IsEscapeSequence(str, 3));
	str = ";";
	ASSERT(!INI_IsEscapeSequence(str, 0));
	str = "\\";
	ASSERT(!INI_IsEscapeSequence(str, 0));
	str = "hi\\\\n";
	ASSERT(!INI_IsEscapeSequence(str, 4));
	str = "\\\\";
	ASSERT(!INI_IsEscapeSequence(str, 0));
	// TODO: What should happen in this case?
	//ASSERT(INI_IsEscapeSequence("\\\\\\", 1));
	
}

Test:INI_GetCommentPos1()
{
	new
		val = cellmax;
	ASSERT(val >= cellmax);
	ASSERT(cellmax >= val);
	ASSERT(0 < val);
	ASSERT(cellmin < val);
	ASSERT(!(100000 >= val));
}

Test:INI_GetCommentPos2()
{
	ASSERT(INI_FindString("; ", ";") == 0);
	ASSERT(INI_FindString("hello ; hi", ";") == 6);
	ASSERT(INI_FindString("woop", ";") == cellmax);
	ASSERT(INI_FindString("there \\; you", ";") == cellmax);
	ASSERT(INI_FindString("maybe works? \\\\; nope", ";") == 15);
	ASSERT(INI_FindString("[bad;tag]", ";") == 4);
}

Test:INI_ReverseWhitespace1()
{
	ASSERT(INI_ReverseWhitespace("a b", 2) == 1);
	ASSERT(INI_ReverseWhitespace("abb", 2) == 2);
	ASSERT(INI_ReverseWhitespace("  b", 2) == 0);
	ASSERT(INI_ReverseWhitespace("b", 0) == 0);
}

Test:INI_IdentifyLineType1()
{
	// Comment lines and blank lines may as well be treated as the same thing,
	// except when rewriting the file.
	new p0s, p0e, p1s, p1e, p2s, p2e, bool:cont;
	// Comment lines.
	ASSERT(INI_IdentifyLineType("; Hello", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType("   ; There", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType("	; Hi", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType(";", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType(";\r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType("\\;\r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont) != e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType("hi ; there", p0s, p0e, p1s, p1e, p2s, p2e, cont) != e_INI_LINE_TYPE_BLANK);
	// Blank lines.
	ASSERT(INI_IdentifyLineType("", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType("    ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType("	\r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
	ASSERT(INI_IdentifyLineType("\r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_BLANK);
}

Test:INI_IdentifyLineType2()
{
	new p0s, p0e, p1s, p1e, p2s, p2e, bool:cont;
	// Invalid lines.
	new
		e_INI_LINE_TYPE:result;
	result = INI_IdentifyLineType("[badtag", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("   [tag", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("   [   ]", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("[tag;end]", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("[tag\\;end]", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result != e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType(" [tag\\]", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType(" [tag\\\\]", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result != e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("key", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("key\\=", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("key\\\\\\=value", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("\\;\r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("key =  ", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType(" = value", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("thing", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
	result = INI_IdentifyLineType("thing ; comment", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_INVALID);
}

Test:INI_IdentifyLineType3()
{
	new p0s, p0e, p1s, p1e, p2s, p2e, bool:cont;
	// Entry lines.
	ASSERT(INI_IdentifyLineType("key = value", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p0e == 3 && p1s == 6 && p1e == 11 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("key=value", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p0e == 3 && p1s == 4 && p1e == 9 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("key = value ; comment", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	//printf("%d %d %d %d %d %d", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(p0s == 0 && p0e == 3 && p1s == 6 && p1e == 11 && p2s == 12 && p2e == 21);
	ASSERT(INI_IdentifyLineType("key = value ; comment\r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p0e == 3 && p1s == 6 && p1e == 11 && p2s == 12 && p2e == 21);
	ASSERT(INI_IdentifyLineType("key =  value   ; com ment  \r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p0e == 3 && p1s == 7 && p1e == 12 && p2s == 15 && p2e == 25);
	ASSERT(INI_IdentifyLineType("key = value\r\n", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p0e == 3 && p1s == 6 && p1e == 11 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("key	=value", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p0e == 3 && p1s == 5 && p1e == 10 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("   key=	value", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 3 && p0e == 6 && p1s == 8 && p1e == 13 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("   key=	value    ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 3 && p0e == 6 && p1s == 8 && p1e == 13 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("	key   =value  ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 1 && p0e == 4 && p1s == 8 && p1e == 13 && p2s == p2e);
}

Test:INI_IdentifyLineType4()
{
	new p0s, p0e, p1s, p1e, p2s, p2e, bool:cont;
	// Tag lines.
	ASSERT(INI_IdentifyLineType("[hello]", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType(" [hello] ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 2 && p0e == 7 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType(" [ hello] ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 3 && p0e == 8 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType(" [hello ] ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 2 && p0e == 7 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[ hello] ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 2 && p0e == 7 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[hello ]", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[hello   ]", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[   hello ]", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	// printf("%d %d %d %d %d %d", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(p0s == 4 && p0e == 9 && p1s == p1e && p2s == p2e);
}

Test:INI_IdentifyLineType5()
{
	new p0s, p0e, p1s, p1e, p2s, p2e, bool:cont;
	// Inheritance.
	ASSERT(INI_IdentifyLineType("[hello] : ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == p1e && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[hello] : first", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == 10 && p1e == 15 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[hello] : first, second", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == 10 && p1e == 23 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[hello]   : first, second     ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == 12 && p1e == 25 && p2s == p2e);
	ASSERT(INI_IdentifyLineType("[hello] :    first, second   ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == 13 && p1e == 26 && p2s == p2e);
	// Commented out.
	ASSERT(INI_IdentifyLineType("[hello] ;: ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == p1e && p2s == 8 && p2e == 10);
	ASSERT(INI_IdentifyLineType("[hello] ; : first, second ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 1 && p0e == 6 && p1s == p1e && p2s == 8 && p2e == 25);
	// printf("%d %d %d %d %d %d", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	// Both.
	ASSERT(INI_IdentifyLineType(" [ hello ]  :  inheritance  ;  : first, second ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_TAG);
	ASSERT(p0s == 3 && p0e == 8 && p1s == 15 && p1e == 26 && p2s == 28 && p2e == 46);
	// printf("%d %d %d %d %d %d", p0s, p0e, p1s, p1e, p2s, p2e, cont);
}

Test:INI_IdentifyLineType6()
{
	new p0s, p0e, p1s, p1e, p2s, p2e, bool:cont;
	// Continuation.
	new
		e_INI_LINE_TYPE:result;
	cont = false;
	result = INI_IdentifyLineType("hi = there\\ ", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_ENTRY);
	//printf("%d %d %d %d %d %d", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(p0s == 0 && p1e == 10);
	ASSERT(cont);
	cont = false;
	ASSERT(INI_IdentifyLineType("hi = there  \\   ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p1e == 10);
	ASSERT(cont);
	cont = false;
	ASSERT(INI_IdentifyLineType("hi =    \\   ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_ENTRY);
	ASSERT(p0s == 0 && p0e == 2);
	ASSERT(p1s == p1e);
	ASSERT(cont);
	cont = true;
	ASSERT(INI_IdentifyLineType("hello there ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_CONT);
	ASSERT(p0s == p0e && p1s == 0 && p1e == 11);
	ASSERT(!cont);
	cont = true;
	ASSERT(INI_IdentifyLineType("hello there \\  ", p0s, p0e, p1s, p1e, p2s, p2e, cont) == e_INI_LINE_TYPE_CONT);
	ASSERT(p0s == p0e && p1s == 0 && p1e == 11);
	ASSERT(cont);
	cont = true;
	result = INI_IdentifyLineType("   \\", p0s, p0e, p1s, p1e, p2s, p2e, cont);
	ASSERT(result == e_INI_LINE_TYPE_CONT);
	ASSERT(cont);
	ASSERT(p0s == p0e && p1s == p1e);
}

Test:Trim1()
{
	new start, end;
	start = 0, end = 9;
	Trim("  hello  ", start, end);
	ASSERT(start == 2);
	ASSERT(end == 7);
	start = 0, end = 9;
	Trim("         ", start, end);
	ASSERT(start == 9);
	ASSERT(end == 9);
	start = 6, end = 13;
	Trim(" hello there ", start, end);
	ASSERT(start == 7);
	ASSERT(end == 12);
	start = 0, end = 13;
	Trim("hello there ", start, end);
	ASSERT(start == 0);
	ASSERT(end == 11);
}

Test:INI_SetupCallbackName0()
{
	new
		dest[32];
	INI_SetupCallbackName(dest, "%s", "", false);
	ASSERT(!strcmp(dest, "%s"));
	INI_SetupCallbackName(dest, "%s", "", true);
	ASSERT(dest[0] == '\0');
	INI_SetupCallbackName(dest, "%s", "fname", true);
	ASSERT(!strcmp(dest, "fname"));
	INI_SetupCallbackName(dest, "hello%sthere", "", false);
	ASSERT(!strcmp(dest, "hello%sthere"));
	INI_SetupCallbackName(dest, "hello%sthere", "", true);
	ASSERT(!strcmp(dest, "hellothere"));
	INI_SetupCallbackName(dest, "how%sare%syou", "", false);
	ASSERT(!strcmp(dest, "how%sareyou"));
	INI_SetupCallbackName(dest, "how%sare%syou", "", true);
	ASSERT(!strcmp(dest, "howare%syou"));
	INI_SetupCallbackName(dest, "hello%sthere", "fname", false);
	ASSERT(!strcmp(dest, "hello%sthere"));
	INI_SetupCallbackName(dest, "hello%sthere", "fname", true);
	ASSERT(!strcmp(dest, "hellofnamethere"));
	INI_SetupCallbackName(dest, "how%sare%syou", "fname", false);
	ASSERT(!strcmp(dest, "how%sarefnameyou"));
	INI_SetupCallbackName(dest, "how%sare%syou", "fname", true);
	ASSERT(!strcmp(dest, "howfnameare%syou"));
	INI_SetupCallbackName(dest, "foobar", "fname", true);
	ASSERT(!strcmp(dest, "foobar"));
	INI_SetupCallbackName(dest, "foobar", "fname", false);
	ASSERT(!strcmp(dest, "foobar"));
	INI_SetupCallbackName(dest, "%s%s", "fname", true);
	ASSERT(!strcmp(dest, "fname%s"));
	INI_SetupCallbackName(dest, "%s%s", "fname", false);
	ASSERT(!strcmp(dest, "%sfname"));
}

Test:INI_SetupCallbackName1()
{
	new
		dest[32];
	INI_SetupCallbackName(dest, "%s", "hi/there", false);
	ASSERT(!strcmp(dest, "%s"));
	INI_SetupCallbackName(dest, "%s", "hi/there", true);
	ASSERT(!strcmp(dest, "there"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there", false);
	ASSERT(!strcmp(dest, "%sthere"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there.txt", false);
	ASSERT(!strcmp(dest, "%sthere"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there", true);
	ASSERT(!strcmp(dest, "there%s"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there.txt", true);
	ASSERT(!strcmp(dest, "there%s"));
}

Test:INI_SetupCallbackName2()
{
	new
		dest[32];
	INI_SetupCallbackName(dest, "%s", "hi/there you", false);
	ASSERT(!strcmp(dest, "%s"));
	INI_SetupCallbackName(dest, "%s", "hi/there you", true);
	ASSERT(!strcmp(dest, "there_you"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there you", false);
	ASSERT(!strcmp(dest, "%sthere_you"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there you.txt", false);
	ASSERT(!strcmp(dest, "%sthere_you"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there you", true);
	ASSERT(!strcmp(dest, "there_you%s"));
	INI_SetupCallbackName(dest, "%s%s", "hi/there you.txt", true);
	ASSERT(!strcmp(dest, "there_you%s"));
}

Test:INI_SetupCallbackName3()
{
	new
		dest[32];
	INI_SetupCallbackName(dest, "%s%s", "one/two/three/hello_there you", false);
	ASSERT(!strcmp(dest, "%shello_there_you"));
}

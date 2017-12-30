#include "UrlApi.h"

String UrlApi::urlEncode(String url) {
	String encodedUrl = "";
	char c, code0, code1, code2;
	for (int i =0; i < url.length(); i++) {
		c = url.charAt(i);
		if (c == ' ') encodedUrl += '+';
		else if (isalnum(c)) encodedUrl += c;
		else {
			code1 = (c & 0xf) + '0';
			if ((c & 0xf) >9) code1 = (c & 0xf) - 10 + 'A';
			c = (c >> 4) & 0xf;
			code0 = c + '0';
			if (c > 9) code0 = c - 10 + 'A';
			code2 = '\0';
			encodedUrl += '%';
			encodedUrl += code0;
			encodedUrl += code1;
		}
		yield();
	}
	return encodedUrl;	
}
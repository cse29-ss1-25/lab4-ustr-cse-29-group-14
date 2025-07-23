#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ustr.h"
#include "pset1.h"

/*
Initializes a new UStr with contents
*/
UStr new_ustr(char* contents) {
	int32_t bytes = strlen(contents);
	int32_t codepoints = utf8_strlen(contents);
	uint8_t all_ascii = bytes == codepoints;
	char* contents_copy = malloc(bytes + 1);
	strcpy(contents_copy, contents);
	UStr s = {
		codepoints,
		bytes,
		all_ascii,
		contents_copy
	};
	return s;
}

/*
Returns length of string
*/
int32_t len(UStr s) {
	return s.codepoints;
}

/*
Returns a substring of str, starting at index start (inclusive) 
and ending at index end (exclusive).

Returns an empty string on invalid range.
*/

UStr substring(UStr s, int32_t start, int32_t end) {
        // TODO: implement this
        //sets up the ranges 
        if (start < 0) start = 0;
        if (end > s.codepoints) end = s.codepoints;
        if (start >= end){
                return new_ustr("");
        }

        //finding starting point from offset
int32_t cp = 0;
        char* p = s.contents;
        while (cp < start)  {
            int len = utf8_codepoint_size(*p);

                p += len;
                cp++;
        }
        int byte_start = p - s.contents;

        //finding ending point
        while (cp < end) {
            int len = utf8_codepoint_size(*p);

                p += len;
                cp++;
        }
        int byte_end = p - s.contents;

        //copy bytes
        int n = byte_end - byte_start;
        char* buf = malloc(n + 1);
        memcpy(buf, s.contents + byte_start, n);
        buf[n] = '\0';

  UStr result = new_ustr(buf);
        free(buf);
        return result;
}      
/*
Given 2 strings s1 and s2, returns a string that is the result of 
concatenating s1 and s2. 
*/
UStr concat(UStr s1, UStr s2) {
	UStr result;

	result.bytes = s1.bytes + s2.bytes;
	result.codepoints =  s1.codepoints + s2.codepoints;

	if (s1.is_ascii == 1 && s2.is_ascii == 1) {
		result.is_ascii = 1;
	} else {
		result.is_ascii = 0;
	}

	result.contents = malloc(result.bytes + 1);

	if (result.contents == NULL) {
		result.bytes = 0;
		result.codepoints = 0;
		result.is_ascii = 1;
		result.contents = NULL;
		return result;
	}

	strcpy(result.contents, s1.contents);
	strcat(result.contents, s2.contents);
	return result;
}

/*
Given a string s and an index, return a string with the character at index 
removed from the original string. 

Returns the original string if index is out of bounds.
*/
UStr removeAt(UStr s, int32_t index) {
	
	//Check if index is valid.
	if(index<0||index>=s.codepoints){
		return s;
	}
	
	//Find which index (in terms of bytes) corresponds with the inputted index.
	int char_count = 0;
	int byte_index;
	for(int i =0; i<=s.bytes;i++){
		if(char_count == index){
			byte_index=i;
			break;
		}
		char_count++;
		if((s.contents[i]&0b10000000)==0b00000000){
			continue;
		}
		else if((s.contents[i]&0b11100000)==0b11000000){
			i++;
		}
		else if((s.contents[i]&0b11110000)==0b11100000){
			i+=2;
		}
		else{
			i+=3;
		}
	}

	//Calculating the number of bytes to remove/shift.
	int bytes_removed=0;
	if((s.contents[byte_index]&0b10000000)==0b00000000){
		bytes_removed=1;
	}
	else if((s.contents[byte_index]&0b11100000)==0b11000000){
		bytes_removed=2;
	}
	else if((s.contents[byte_index]&0b11110000)==0b11100000){
		bytes_removed=3;
	}
	else{
		bytes_removed = 4;
	}
	
	//Removing/shifting characters by bytes.
	char* new_contents = malloc(s.bytes-bytes_removed+1);
	for(int i =0; i<byte_index; i++){
		new_contents[i]=s.contents[i];
	}
	for(int i = byte_index; i<=s.bytes-bytes_removed;i++){
		new_contents[i]=s.contents[i+bytes_removed];
	}
	
	//Final product.
	UStr toReturn = new_ustr(new_contents);
	free(new_contents);
	return toReturn;
}

/*
Given a string s, return s reversed. 

Example: reverse("apples🍎 and bananas🍌") = "🍌sananab dna 🍎selppa")
*/

UStr reverse(UStr s) {
    char* reversed = malloc(s.bytes + 1);
    int write_index = 0;
    int i = s.bytes - 1;

    while (i >= 0) {
        int start = i;
        while (start > 0 && (s.contents[start] & 0xC0) == 0x80) {
            start--;
        }
        int len = i - start + 1;
        for (int j = 0; j < len; j++) {
            reversed[write_index++] = s.contents[start + j];
        }
        i = start - 1;
    }

    reversed[write_index] = '\0';
    UStr result = new_ustr(reversed);
    free(reversed);
    return result;
}

void print_ustr(UStr s) {
	printf("%s [codepoints: %d | bytes: %d]", s.contents, s.codepoints, s.bytes);
}

void free_ustr(UStr s) {
	if (s.contents != NULL) {
		free(s.contents);
		s.contents = NULL;
	}
}

